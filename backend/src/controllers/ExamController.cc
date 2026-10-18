#include <drogon/drogon.h>
#include "controllers/ExamController.h"

#include <cstdint>
#include <memory>
#include <string>
#include <utility>

using namespace drogon;

namespace universe::controllers
{

namespace
{

using Callback = std::function<void(const HttpResponsePtr &)>;

std::shared_ptr<orm::DbClient> database()
{
    return app().getDbClient("default");
}

HttpResponsePtr jsonResponse(
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
    return jsonResponse(std::move(body), status);
}

bool isStaff(const HttpRequestPtr &request)
{
    auto session = request->session();

    if (!session)
        return false;

    const auto role = session->get<std::string>("role");
    return role == "admin" || role == "faculty";
}

long long sessionUserId(const HttpRequestPtr &request)
{
    auto session = request->session();

    if (!session)
        return 0;

    return session->get<long long>("user_id");
}

bool validText(
    const Json::Value &json,
    const char *field,
    std::size_t maxLength
)
{
    return json.isMember(field) &&
        json[field].isString() &&
        !json[field].asString().empty() &&
        json[field].asString().size() <= maxLength;
}

void databaseError(
    const std::shared_ptr<Callback> &callback,
    const orm::DrogonDbException &error
)
{
    LOG_ERROR << "Examination database error: "
              << error.base().what();

    (*callback)(
        errorResponse(
            k500InternalServerError,
            "Database operation failed."
        )
    );
}

// GET /api/v1/admin/exams
void listExams(const HttpRequestPtr &request, Callback &&callback)
{
    auto cb = std::make_shared<Callback>(std::move(callback));

    if (!isStaff(request))
    {
        (*cb)(errorResponse(k403Forbidden, "Staff access required."));
        return;
    }

    database()->execSqlAsync(
        R"SQL(
            SELECT
                id,
                course_code,
                title,
                exam_type,
                TO_CHAR(exam_date, 'YYYY-MM-DD') AS exam_date,
                total_marks,
                passing_marks
            FROM examinations
            ORDER BY exam_date DESC, id DESC
        )SQL",

        [cb](const orm::Result &result)
        {
            Json::Value body;
            body["status"] = "success";
            body["exams"] = Json::arrayValue;

            for (const auto &row : result)
            {
                Json::Value item;
                item["id"] = Json::Int64(
                    row["id"].as<std::int64_t>()
                );
                item["courseCode"] =
                    row["course_code"].as<std::string>();
                item["title"] = row["title"].as<std::string>();
                item["examType"] =
                    row["exam_type"].as<std::string>();
                item["examDate"] =
                    row["exam_date"].as<std::string>();
                item["totalMarks"] =
                    row["total_marks"].as<double>();
                item["passingMarks"] =
                    row["passing_marks"].as<double>();

                body["exams"].append(item);
            }

            (*cb)(jsonResponse(std::move(body)));
        },

        [cb](const orm::DrogonDbException &error)
        {
            databaseError(cb, error);
        }
    );
}

// POST /api/v1/admin/exams
void createExam(const HttpRequestPtr &request, Callback &&callback)
{
    auto cb = std::make_shared<Callback>(std::move(callback));

    if (!isStaff(request))
    {
        (*cb)(errorResponse(k403Forbidden, "Staff access required."));
        return;
    }

    auto json = request->getJsonObject();

    if (!json || !json->isObject() ||
        !validText(*json, "title", 180) ||
        !validText(*json, "courseCode", 30) ||
        !validText(*json, "examType", 20) ||
        !validText(*json, "examDate", 10) ||
        !(*json)["totalMarks"].isNumeric() ||
        !(*json)["passingMarks"].isNumeric())
    {
        (*cb)(errorResponse(k400BadRequest, "Invalid examination details."));
        return;
    }

    const std::string examType =
        (*json)["examType"].asString();

    if (examType != "quiz" &&
        examType != "internal" &&
        examType != "midterm" &&
        examType != "practical" &&
        examType != "assignment" &&
        examType != "final")
    {
        (*cb)(errorResponse(k400BadRequest, "Invalid examination type."));
        return;
    }

    const double totalMarks = (*json)["totalMarks"].asDouble();
    const double passingMarks = (*json)["passingMarks"].asDouble();

    if (totalMarks <= 0 ||
        passingMarks < 0 ||
        passingMarks > totalMarks)
    {
        (*cb)(errorResponse(k400BadRequest, "Invalid marks range."));
        return;
    }

    const long long userId = sessionUserId(request);

    const std::string title = (*json)["title"].asString();
    const std::string courseCode = (*json)["courseCode"].asString();
    const std::string examDate = (*json)["examDate"].asString();

    database()->execSqlAsync(
        R"SQL(
            INSERT INTO examinations (
                course_code, title, exam_type, exam_date,
                total_marks, passing_marks, created_by
            )
            VALUES ($1, $2, $3, $4::date, $5, $6, $7)
            RETURNING id
        )SQL",

        [cb](const orm::Result &result)
        {
            Json::Value body;
            body["status"] = "success";
            body["message"] = "Examination created successfully.";
            body["id"] = Json::Int64(
                result[0]["id"].as<std::int64_t>()
            );

            (*cb)(jsonResponse(std::move(body), k201Created));
        },

        [cb](const orm::DrogonDbException &error)
        {
            databaseError(cb, error);
        },

        courseCode,
        title,
        examType,
        examDate,
        totalMarks,
        passingMarks,
        userId
    );
}

// GET /api/v1/admin/exam-results?examId=1
void listResults(const HttpRequestPtr &request, Callback &&callback)
{
    auto cb = std::make_shared<Callback>(std::move(callback));

    if (!isStaff(request))
    {
        (*cb)(errorResponse(k403Forbidden, "Staff access required."));
        return;
    }

    long long examId = 0;

    try
    {
        examId = std::stoll(request->getParameter("examId"));
    }
    catch (...)
    {
        (*cb)(errorResponse(k400BadRequest, "A valid examId is required."));
        return;
    }

    if (examId <= 0)
    {
        (*cb)(errorResponse(k400BadRequest, "Invalid examId."));
        return;
    }

    database()->execSqlAsync(
        R"SQL(
            SELECT
                r.id,
                r.student_user_id,
                u.login_id,
                r.marks_obtained,
                e.total_marks,
                r.grade,
                r.remarks
            FROM exam_results r
            JOIN users u ON u.id = r.student_user_id
            JOIN examinations e ON e.id = r.exam_id
            WHERE r.exam_id = $1
            ORDER BY u.login_id
        )SQL",

        [cb](const orm::Result &result)
        {
            Json::Value body;
            body["status"] = "success";
            body["results"] = Json::arrayValue;

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
                item["marksObtained"] =
                    row["marks_obtained"].as<double>();
                item["totalMarks"] =
                    row["total_marks"].as<double>();
                item["grade"] = row["grade"].as<std::string>();
                item["remarks"] = row["remarks"].as<std::string>();

                body["results"].append(item);
            }

            (*cb)(jsonResponse(std::move(body)));
        },

        [cb](const orm::DrogonDbException &error)
        {
            databaseError(cb, error);
        },

        examId
    );
}

// POST /api/v1/admin/exam-results
void saveResult(const HttpRequestPtr &request, Callback &&callback)
{
    auto cb = std::make_shared<Callback>(std::move(callback));

    if (!isStaff(request))
    {
        (*cb)(errorResponse(k403Forbidden, "Staff access required."));
        return;
    }

    auto json = request->getJsonObject();

    if (!json || !json->isObject() ||
        !( (*json)["examId"].isInt64() ||
           (*json)["examId"].isInt() ) ||
        !( (*json)["studentUserId"].isInt64() ||
           (*json)["studentUserId"].isInt() ) ||
        !(*json)["marksObtained"].isNumeric())
    {
        (*cb)(errorResponse(k400BadRequest, "Invalid result details."));
        return;
    }

    const long long examId = (*json)["examId"].asInt64();
    const long long studentId = (*json)["studentUserId"].asInt64();
    const double marks = (*json)["marksObtained"].asDouble();

    const std::string grade =
        json->isMember("grade") && (*json)["grade"].isString()
            ? (*json)["grade"].asString()
            : "";

    const std::string remarks =
        json->isMember("remarks") && (*json)["remarks"].isString()
            ? (*json)["remarks"].asString()
            : "";

    if (examId <= 0 || studentId <= 0 ||
        marks < 0 || grade.size() > 10 || remarks.size() > 2000)
    {
        (*cb)(errorResponse(k400BadRequest, "Invalid result values."));
        return;
    }

    const long long enteredBy = sessionUserId(request);

    database()->execSqlAsync(
        R"SQL(
            INSERT INTO exam_results (
                exam_id, student_user_id, marks_obtained,
                grade, remarks, entered_by
            )
            SELECT
                e.id, u.id, $3::numeric, $4, $5, $6
            FROM examinations e
            JOIN users u
                ON u.id = $2
               AND u.role = 'student'
               AND u.status = 'active'
            WHERE e.id = $1
              AND $3::numeric <= e.total_marks

            ON CONFLICT (exam_id, student_user_id)
            DO UPDATE SET
                marks_obtained = EXCLUDED.marks_obtained,
                grade = EXCLUDED.grade,
                remarks = EXCLUDED.remarks,
                entered_by = EXCLUDED.entered_by,
                updated_at = CURRENT_TIMESTAMP

            RETURNING id
        )SQL",

        [cb](const orm::Result &result)
        {
            if (result.empty())
            {
                (*cb)(errorResponse(
                    k400BadRequest,
                    "Check the exam, active student and maximum marks."
                ));
                return;
            }

            Json::Value body;
            body["status"] = "success";
            body["message"] = "Student result saved successfully.";
            body["id"] = Json::Int64(
                result[0]["id"].as<std::int64_t>()
            );

            (*cb)(jsonResponse(std::move(body), k201Created));
        },

        [cb](const orm::DrogonDbException &error)
        {
            databaseError(cb, error);
        },

        examId,
        studentId,
        marks,
        grade,
        remarks,
        enteredBy
    );
}

// GET /api/v1/student/results
void studentResults(const HttpRequestPtr &request, Callback &&callback)
{
    auto cb = std::make_shared<Callback>(std::move(callback));
    auto session = request->session();

    if (!session ||
        session->get<std::string>("role") != "student")
    {
        (*cb)(errorResponse(k403Forbidden, "Student access required."));
        return;
    }

    const long long studentId =
        session->get<long long>("user_id");

    database()->execSqlAsync(
        R"SQL(
            SELECT
                e.course_code,
                e.title,
                e.exam_type,
                TO_CHAR(e.exam_date, 'YYYY-MM-DD') AS exam_date,
                r.marks_obtained,
                e.total_marks,
                e.passing_marks,
                r.grade,
                r.remarks
            FROM exam_results r
            JOIN examinations e ON e.id = r.exam_id
            WHERE r.student_user_id = $1
            ORDER BY e.exam_date DESC
        )SQL",

        [cb](const orm::Result &result)
        {
            Json::Value body;
            body["status"] = "success";
            body["results"] = Json::arrayValue;

            for (const auto &row : result)
            {
                Json::Value item;
                item["courseCode"] =
                    row["course_code"].as<std::string>();
                item["title"] = row["title"].as<std::string>();
                item["examType"] =
                    row["exam_type"].as<std::string>();
                item["examDate"] =
                    row["exam_date"].as<std::string>();
                item["marksObtained"] =
                    row["marks_obtained"].as<double>();
                item["totalMarks"] =
                    row["total_marks"].as<double>();
                item["passingMarks"] =
                    row["passing_marks"].as<double>();
                item["grade"] = row["grade"].as<std::string>();
                item["remarks"] = row["remarks"].as<std::string>();

                body["results"].append(item);
            }

            (*cb)(jsonResponse(std::move(body)));
        },

        [cb](const orm::DrogonDbException &error)
        {
            databaseError(cb, error);
        },

        studentId
    );
}

} // anonymous namespace

void ExamController::registerRoutes()
{
    app().registerHandler(
        "/api/v1/admin/exams",
        [](const HttpRequestPtr &request, Callback &&callback)
        {
            listExams(request, std::move(callback));
        },
        {Get}
    );

    app().registerHandler(
        "/api/v1/admin/exams",
        [](const HttpRequestPtr &request, Callback &&callback)
        {
            createExam(request, std::move(callback));
        },
        {Post}
    );

    app().registerHandler(
        "/api/v1/admin/exam-results",
        [](const HttpRequestPtr &request, Callback &&callback)
        {
            listResults(request, std::move(callback));
        },
        {Get}
    );

    app().registerHandler(
        "/api/v1/admin/exam-results",
        [](const HttpRequestPtr &request, Callback &&callback)
        {
            saveResult(request, std::move(callback));
        },
        {Post}
    );

    app().registerHandler(
        "/api/v1/student/results",
        [](const HttpRequestPtr &request, Callback &&callback)
        {
            studentResults(request, std::move(callback));
        },
        {Get}
    );

    LOG_INFO << "Examination and results routes registered.";
}

}