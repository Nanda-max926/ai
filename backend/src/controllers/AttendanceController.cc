#include <drogon/drogon.h>
#include "controllers/AttendanceController.h"

#include <cstdint>
#include <memory>
#include <string>
#include <utility>

using namespace drogon;

namespace universe::controllers
{

namespace
{

using Callback = AttendanceController::ResponseCallback;

std::shared_ptr<orm::DbClient> database()
{
    return app().getDbClient("default");
}

HttpResponsePtr makeResponse(
    Json::Value body,
    HttpStatusCode status = k200OK
)
{
    auto response = HttpResponse::newHttpJsonResponse(body);
    response->setStatusCode(status);
    response->addHeader("Cache-Control", "no-store");
    return response;
}

HttpResponsePtr errorResponse(
    HttpStatusCode status,
    const std::string &message
)
{
    Json::Value body;
    body["status"] = "error";
    body["message"] = message;

    return makeResponse(std::move(body), status);
}

bool canManageAttendance(const HttpRequestPtr &request)
{
    auto session = request->session();

    if (!session)
        return false;

    const auto role = session->get<std::string>("role");

    return role == "admin" || role == "faculty";
}

void databaseError(
    const std::shared_ptr<Callback> &callback,
    const orm::DrogonDbException &error
)
{
    LOG_ERROR << "Attendance database error: "
              << error.base().what();

    (*callback)(
        errorResponse(
            k500InternalServerError,
            "Unable to complete the attendance operation."
        )
    );
}

bool textField(
    const Json::Value &json,
    const char *name,
    std::size_t maxLength
)
{
    return json.isMember(name) &&
        json[name].isString() &&
        !json[name].asString().empty() &&
        json[name].asString().size() <= maxLength;
}

void listStudents(
    const HttpRequestPtr &request,
    Callback &&callback
)
{
    auto cb = std::make_shared<Callback>(std::move(callback));

    if (!canManageAttendance(request))
    {
        (*cb)(errorResponse(
            k403Forbidden,
            "Administrator or faculty access required."
        ));
        return;
    }

    database()->execSqlAsync(
        R"SQL(
            SELECT
                u.id,
                u.login_id,
                u.email
            FROM users u
            WHERE u.role = 'student'
              AND u.status = 'active'
            ORDER BY u.login_id
        )SQL",

        [cb](const orm::Result &result)
        {
            Json::Value body;
            body["status"] = "success";
            body["students"] = Json::arrayValue;

            for (const auto &row : result)
            {
                Json::Value item;
                item["id"] = Json::Int64(
                    row["id"].as<std::int64_t>()
                );
                item["loginId"] = row["login_id"].as<std::string>();
                item["email"] = row["email"].as<std::string>();

                body["students"].append(item);
            }

            (*cb)(makeResponse(std::move(body)));
        },

        [cb](const orm::DrogonDbException &error)
        {
            databaseError(cb, error);
        }
    );
}

void listRecords(
    const HttpRequestPtr &request,
    Callback &&callback
)
{
    auto cb = std::make_shared<Callback>(std::move(callback));

    if (!canManageAttendance(request))
    {
        (*cb)(errorResponse(
            k403Forbidden,
            "Administrator or faculty access required."
        ));
        return;
    }

    const std::string date = request->getParameter("date");
    const std::string courseCode =
        request->getParameter("courseCode");

    database()->execSqlAsync(
        R"SQL(
            SELECT
                r.id,
                r.student_user_id,
                u.login_id,
                r.course_code,
                TO_CHAR(r.attendance_date, 'YYYY-MM-DD') AS attendance_date,
                r.status,
                r.remarks
            FROM attendance_records r
            JOIN users u ON u.id = r.student_user_id
            WHERE r.attendance_date =
                COALESCE(NULLIF($1, '')::date, CURRENT_DATE)
              AND ($2 = '' OR r.course_code = $2)
            ORDER BY u.login_id
        )SQL",

        [cb](const orm::Result &result)
        {
            Json::Value body;
            body["status"] = "success";
            body["records"] = Json::arrayValue;

            for (const auto &row : result)
            {
                Json::Value item;
                item["id"] = Json::Int64(
                    row["id"].as<std::int64_t>()
                );
                item["studentUserId"] = Json::Int64(
                    row["student_user_id"].as<std::int64_t>()
                );
                item["loginId"] = row["login_id"].as<std::string>();
                item["courseCode"] = row["course_code"].as<std::string>();
                item["attendanceDate"] =
                    row["attendance_date"].as<std::string>();
                item["status"] = row["status"].as<std::string>();
                item["remarks"] = row["remarks"].as<std::string>();

                body["records"].append(item);
            }

            (*cb)(makeResponse(std::move(body)));
        },

        [cb](const orm::DrogonDbException &error)
        {
            databaseError(cb, error);
        },

        date,
        courseCode
    );
}

void createRecord(
    const HttpRequestPtr &request,
    Callback &&callback
)
{
    auto cb = std::make_shared<Callback>(std::move(callback));

    if (!canManageAttendance(request))
    {
        (*cb)(errorResponse(
            k403Forbidden,
            "Administrator or faculty access required."
        ));
        return;
    }

    auto json = request->getJsonObject();

    if (!json || !json->isObject() ||
        !( (*json)["studentUserId"].isInt64() ||
           (*json)["studentUserId"].isInt() ) ||
        !textField(*json, "courseCode", 30) ||
        !textField(*json, "attendanceDate", 10) ||
        !textField(*json, "status", 12))
    {
        (*cb)(errorResponse(
            k400BadRequest,
            "Invalid attendance details."
        ));
        return;
    }

    const std::string status = (*json)["status"].asString();

    if (status != "present" &&
        status != "absent" &&
        status != "late" &&
        status != "excused")
    {
        (*cb)(errorResponse(
            k400BadRequest,
            "Status must be present, absent, late or excused."
        ));
        return;
    }

    auto session = request->session();
    const auto markedBy = session->get<long long>("user_id");

    if (markedBy <= 0)
    {
        (*cb)(errorResponse(
            k401Unauthorized,
            "Please sign in again."
        ));
        return;
    }

    const auto studentId =
        (*json)["studentUserId"].asInt64();

    const std::string courseCode =
        (*json)["courseCode"].asString();

    const std::string attendanceDate =
        (*json)["attendanceDate"].asString();

    const std::string remarks =
        json->isMember("remarks") && (*json)["remarks"].isString()
            ? (*json)["remarks"].asString()
            : "";

    database()->execSqlAsync(
        R"SQL(
            INSERT INTO attendance_records (
                student_user_id,
                marked_by,
                course_code,
                attendance_date,
                status,
                remarks,
                updated_at
            )
            VALUES ($1, $2, $3, $4::date, $5, $6, CURRENT_TIMESTAMP)

            ON CONFLICT (
                student_user_id,
                course_code,
                attendance_date
            )
            DO UPDATE SET
                marked_by = EXCLUDED.marked_by,
                status = EXCLUDED.status,
                remarks = EXCLUDED.remarks,
                updated_at = CURRENT_TIMESTAMP

            RETURNING id
        )SQL",

        [cb](const orm::Result &result)
        {
            Json::Value body;
            body["status"] = "success";
            body["message"] = "Attendance saved successfully.";
            body["id"] = Json::Int64(
                result[0]["id"].as<std::int64_t>()
            );

            (*cb)(makeResponse(std::move(body), k201Created));
        },

        [cb](const orm::DrogonDbException &error)
        {
            databaseError(cb, error);
        },

        studentId,
        markedBy,
        courseCode,
        attendanceDate,
        status,
        remarks
    );
}

} // anonymous namespace

void AttendanceController::registerRoutes()
{
    app().registerHandler(
        "/api/v1/attendance/students",
        [](const HttpRequestPtr &request, Callback &&callback)
        {
            listStudents(request, std::move(callback));
        },
        {Get}
    );

    app().registerHandler(
        "/api/v1/attendance/records",
        [](const HttpRequestPtr &request, Callback &&callback)
        {
            listRecords(request, std::move(callback));
        },
        {Get}
    );

    app().registerHandler(
        "/api/v1/attendance/records",
        [](const HttpRequestPtr &request, Callback &&callback)
        {
            createRecord(request, std::move(callback));
        },
        {Post}
    );

    LOG_INFO << "Attendance management routes registered.";
}

}