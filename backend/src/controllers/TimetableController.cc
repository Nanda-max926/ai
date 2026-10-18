#include "controllers/TimetableController.h"

#include "domain/TimetableModels.h"
#include "repositories/TimetableRepository.h"
#include "services/TimetableService.h"
#include <stdexcept>
#include <cstdint>
#include <memory>
#include <string>
#include <utility>

using namespace drogon;

namespace universe::controllers
{

namespace
{

using ResponseCallback = TimetableController::ResponseCallback;

services::TimetableService &timetableService()
{
    static services::TimetableService service(
        std::make_shared<repositories::TimetableRepository>(
            app().getDbClient("default")
        )
    );

    return service;
}

bool isAdmin(const HttpRequestPtr &request)
{
    auto session = request->session();

    return session &&
        session->get<std::string>("role") == "admin";
}

HttpResponsePtr jsonError(
    HttpStatusCode status,
    const std::string &message
)
{
    Json::Value body;
    body["status"] = "error";
    body["message"] = message;

    auto response = HttpResponse::newHttpJsonResponse(body);
    response->setStatusCode(status);
    response->addHeader("Cache-Control", "no-store");

    return response;
}

HttpResponsePtr forbidden()
{
    return jsonError(k403Forbidden, "Administrator access required.");
}

bool validString(
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

HttpResponsePtr created(
    const std::string &message,
    std::int64_t id
)
{
    Json::Value body;
    body["status"] = "success";
    body["message"] = message;
    body["id"] = Json::Int64(id);

    auto response = HttpResponse::newHttpJsonResponse(body);
    response->setStatusCode(k201Created);

    return response;
}

void databaseError(
    const std::shared_ptr<ResponseCallback> &callback,
    const drogon::orm::DrogonDbException &error
)
{
    const std::string detail = error.base().what();

    LOG_ERROR << "Timetable database error: " << detail;

    // PostgreSQL exclusion violations mean a timetable clash.
    if (detail.find("exclusion constraint") != std::string::npos)
    {
        (*callback)(
            jsonError(
                k409Conflict,
                "Schedule conflict: this section, faculty member, or room is already occupied."
            )
        );
        return;
    }

    if (detail.find("duplicate key") != std::string::npos)
    {
        (*callback)(
            jsonError(
                k409Conflict,
                "This room already exists."
            )
        );
        return;
    }

    (*callback)(
        jsonError(
            k500InternalServerError,
            "Unable to complete the timetable operation."
        )
    );
}

} // anonymous namespace

void TimetableController::registerRoutes()
{
    app().registerHandler(
        "/api/v1/admin/timetable/rooms",
        [](const HttpRequestPtr &request, ResponseCallback &&callback)
        {
            listRooms(request, std::move(callback));
        },
        {Get}
    );

    app().registerHandler(
        "/api/v1/admin/timetable/faculty",
        [](const HttpRequestPtr &request, ResponseCallback &&callback)
        {
            listFaculty(request, std::move(callback));
        },
        {Get}
    );

    app().registerHandler(
        "/api/v1/admin/timetable/entries",
        [](const HttpRequestPtr &request, ResponseCallback &&callback)
        {
            listEntries(request, std::move(callback));
        },
        {Get}
    );

    app().registerHandler(
        "/api/v1/admin/timetable/rooms",
        [](const HttpRequestPtr &request, ResponseCallback &&callback)
        {
            createRoom(request, std::move(callback));
        },
        {Post}
    );

    app().registerHandler(
        "/api/v1/admin/timetable/entries",
        [](const HttpRequestPtr &request, ResponseCallback &&callback)
        {
            createEntry(request, std::move(callback));
        },
        {Post}
    );

    LOG_INFO << "Timetable management routes registered.";
}

void TimetableController::listRooms(
    const HttpRequestPtr &request,
    ResponseCallback &&callback
)
{
    auto callbackPtr =
        std::make_shared<ResponseCallback>(std::move(callback));

    if (!isAdmin(request))
    {
        (*callbackPtr)(forbidden());
        return;
    }

    timetableService().listRooms(
        [callbackPtr](std::vector<domain::AcademicRoom> rooms)
        {
            Json::Value body;
            body["status"] = "success";
            body["rooms"] = Json::arrayValue;

            for (const auto &room : rooms)
            {
                Json::Value item;
                item["id"] = Json::Int64(room.id);
                item["building"] = room.building;
                item["roomNumber"] = room.roomNumber;
                item["capacity"] = room.capacity;
                item["roomType"] = room.roomType;
                item["isActive"] = room.isActive;
                body["rooms"].append(item);
            }

            (*callbackPtr)(HttpResponse::newHttpJsonResponse(body));
        },
        [callbackPtr](const drogon::orm::DrogonDbException &error)
        {
            databaseError(callbackPtr, error);
        }
    );
}

void TimetableController::listFaculty(
    const HttpRequestPtr &request,
    ResponseCallback &&callback
)
{
    auto callbackPtr =
        std::make_shared<ResponseCallback>(std::move(callback));

    if (!isAdmin(request))
    {
        (*callbackPtr)(forbidden());
        return;
    }

    timetableService().listFaculty(
        [callbackPtr](std::vector<domain::FacultyOption> faculty)
        {
            Json::Value body;
            body["status"] = "success";
            body["faculty"] = Json::arrayValue;

            for (const auto &person : faculty)
            {
                Json::Value item;
                item["id"] = Json::Int64(person.id);
                item["loginId"] = person.loginId;
                item["email"] = person.email;
                body["faculty"].append(item);
            }

            (*callbackPtr)(HttpResponse::newHttpJsonResponse(body));
        },
        [callbackPtr](const drogon::orm::DrogonDbException &error)
        {
            databaseError(callbackPtr, error);
        }
    );
}

void TimetableController::listEntries(
    const HttpRequestPtr &request,
    ResponseCallback &&callback
)
{
    auto callbackPtr =
        std::make_shared<ResponseCallback>(std::move(callback));

    if (!isAdmin(request))
    {
        (*callbackPtr)(forbidden());
        return;
    }

    timetableService().listEntries(
        [callbackPtr](std::vector<domain::TimetableEntry> entries)
        {
            Json::Value body;
            body["status"] = "success";
            body["entries"] = Json::arrayValue;

            for (const auto &entry : entries)
            {
                Json::Value item;

                item["id"] = Json::Int64(entry.id);
                item["sectionId"] = Json::Int64(entry.sectionId);
                item["courseId"] = Json::Int64(entry.courseId);
                item["facultyUserId"] =
                    Json::Int64(entry.facultyUserId);
                item["roomId"] = Json::Int64(entry.roomId);

                item["dayOfWeek"] = entry.dayOfWeek;
                item["startTime"] = entry.startTime;
                item["endTime"] = entry.endTime;

                item["sectionCode"] = entry.sectionCode;
                item["batchYear"] = entry.batchYear;
                item["courseCode"] = entry.courseCode;
                item["courseTitle"] = entry.courseTitle;
                item["facultyLoginId"] = entry.facultyLoginId;
                item["roomLabel"] = entry.roomLabel;

                body["entries"].append(item);
            }

            (*callbackPtr)(HttpResponse::newHttpJsonResponse(body));
        },
        [callbackPtr](const drogon::orm::DrogonDbException &error)
        {
            databaseError(callbackPtr, error);
        }
    );
}

void TimetableController::createRoom(
    const HttpRequestPtr &request,
    ResponseCallback &&callback
)
{
    auto callbackPtr =
        std::make_shared<ResponseCallback>(std::move(callback));

    if (!isAdmin(request))
    {
        (*callbackPtr)(forbidden());
        return;
    }

    auto json = request->getJsonObject();

    if (!json || !json->isObject() ||
        !validString(*json, "building", 80) ||
        !validString(*json, "roomNumber", 30) ||
        !(*json)["capacity"].isInt() ||
        !validString(*json, "roomType", 20))
    {
        (*callbackPtr)(
            jsonError(k400BadRequest, "Invalid room details.")
        );
        return;
    }

    try
    {
        timetableService().createRoom(
            (*json)["building"].asString(),
            (*json)["roomNumber"].asString(),
            (*json)["capacity"].asInt(),
            (*json)["roomType"].asString(),

            [callbackPtr](std::int64_t id)
            {
                (*callbackPtr)(
                    created("Room created successfully.", id)
                );
            },

            [callbackPtr](const drogon::orm::DrogonDbException &error)
            {
                databaseError(callbackPtr, error);
            }
        );
    }
    catch (const std::invalid_argument &error)
    {
        (*callbackPtr)(jsonError(k400BadRequest, error.what()));
    }
}

void TimetableController::createEntry(
    const HttpRequestPtr &request,
    ResponseCallback &&callback
)
{
    auto callbackPtr =
        std::make_shared<ResponseCallback>(std::move(callback));

    if (!isAdmin(request))
    {
        (*callbackPtr)(forbidden());
        return;
    }

    auto json = request->getJsonObject();

    if (!json || !json->isObject() ||
        !(*json)["sectionId"].isInt64() ||
        !(*json)["courseId"].isInt64() ||
        !(*json)["facultyUserId"].isInt64() ||
        !(*json)["roomId"].isInt64() ||
        !(*json)["dayOfWeek"].isInt() ||
        !validString(*json, "startTime", 5) ||
        !validString(*json, "endTime", 5))
    {
        (*callbackPtr)(
            jsonError(k400BadRequest, "Invalid timetable entry details.")
        );
        return;
    }

    try
    {
        timetableService().createEntry(
            (*json)["sectionId"].asInt64(),
            (*json)["courseId"].asInt64(),
            (*json)["facultyUserId"].asInt64(),
            (*json)["roomId"].asInt64(),
            (*json)["dayOfWeek"].asInt(),
            (*json)["startTime"].asString(),
            (*json)["endTime"].asString(),

            [callbackPtr](std::int64_t id)
            {
                (*callbackPtr)(
                    created("Timetable entry created successfully.", id)
                );
            },

            [callbackPtr](const drogon::orm::DrogonDbException &error)
            {
                databaseError(callbackPtr, error);
            }
        );
    }
    catch (const std::invalid_argument &error)
    {
        (*callbackPtr)(jsonError(k400BadRequest, error.what()));
    }
}

} // namespace universe::controllers