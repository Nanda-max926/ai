#include "controllers/AcademicController.h"

#include "domain/AcademicModels.h"
#include "repositories/AcademicRepository.h"
#include "services/AcademicService.h"
#include <cstdint>
#include <stdexcept>
#include <string>
#include <memory>
#include <string>
#include <utility>

using namespace drogon;

namespace universe::controllers
{

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

std::string optionalText(
    const Json::Value &json,
    const char *field
)
{
    if (json.isMember(field) && json[field].isString())
        return json[field].asString();

    return "";
}

HttpResponsePtr createdResponse(
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
namespace
{

using ResponseCallback =
    AcademicController::ResponseCallback;

services::AcademicService &academicService()
{
    static services::AcademicService service(
        std::make_shared<repositories::AcademicRepository>(
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

HttpResponsePtr errorResponse(
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

HttpResponsePtr unauthorizedResponse()
{
    return errorResponse(
        k403Forbidden,
        "Administrator access required."
    );
}

void databaseError(
    const std::shared_ptr<ResponseCallback> &callback,
    const drogon::orm::DrogonDbException &error
)
{
    LOG_ERROR << "Academic database error: "
              << error.base().what();

    (*callback)(
        errorResponse(
            k500InternalServerError,
            "Unable to load academic records."
        )
    );
}

} // anonymous namespace

void AcademicController::registerRoutes()
{
    app().registerHandler(
        "/api/v1/admin/academic/programs",
        [](const HttpRequestPtr &request,
           ResponseCallback &&callback)
        {
            listPrograms(request, std::move(callback));
        },
        {Get}
    );

    app().registerHandler(
        "/api/v1/admin/academic/courses",
        [](const HttpRequestPtr &request,
           ResponseCallback &&callback)
        {
            listCourses(request, std::move(callback));
        },
        {Get}
    );

    app().registerHandler(
        "/api/v1/admin/academic/sections",
        [](const HttpRequestPtr &request,
           ResponseCallback &&callback)
        {
            listSections(request, std::move(callback));
        },
        {Get}
    );
    app().registerHandler(
    "/api/v1/admin/academic/programs",
    [](const HttpRequestPtr &request,
       ResponseCallback &&callback)
    {
        createProgram(request, std::move(callback));
    },
    {Post}
);

app().registerHandler(
    "/api/v1/admin/academic/courses",
    [](const HttpRequestPtr &request,
       ResponseCallback &&callback)
    {
        createCourse(request, std::move(callback));
    },
    {Post}
);

app().registerHandler(
    "/api/v1/admin/academic/sections",
    [](const HttpRequestPtr &request,
       ResponseCallback &&callback)
    {
        createSection(request, std::move(callback));
    },
    {Post}
);

    LOG_INFO << "Academic management routes registered.";
}

void AcademicController::listPrograms(
    const HttpRequestPtr &request,
    ResponseCallback &&callback
)
{
    auto callbackPtr =
        std::make_shared<ResponseCallback>(
            std::move(callback)
        );

    if (!isAdmin(request))
    {
        (*callbackPtr)(unauthorizedResponse());
        return;
    }

    academicService().listPrograms(
        [callbackPtr](
            std::vector<domain::AcademicProgram> programs
        )
        {
            Json::Value body;
            body["status"] = "success";
            body["programs"] = Json::arrayValue;

            for (const auto &item : programs)
            {
                Json::Value record;

                record["id"] = Json::Int64(item.id);
                record["departmentId"] =
                    Json::Int64(item.departmentId);
                record["code"] = item.code;
                record["name"] = item.name;
                record["degreeLevel"] = item.degreeLevel;
                record["durationYears"] = item.durationYears;
                record["totalSemesters"] = item.totalSemesters;
                record["description"] = item.description;
                record["isActive"] = item.isActive;

                body["programs"].append(record);
            }

            (*callbackPtr)(
                HttpResponse::newHttpJsonResponse(body)
            );
        },
        [callbackPtr](
            const drogon::orm::DrogonDbException &error
        )
        {
            databaseError(callbackPtr, error);
        }
    );
}

void AcademicController::listCourses(
    const HttpRequestPtr &request,
    ResponseCallback &&callback
)
{
    auto callbackPtr =
        std::make_shared<ResponseCallback>(
            std::move(callback)
        );

    if (!isAdmin(request))
    {
        (*callbackPtr)(unauthorizedResponse());
        return;
    }

    academicService().listCourses(
        [callbackPtr](
            std::vector<domain::AcademicCourse> courses
        )
        {
            Json::Value body;
            body["status"] = "success";
            body["courses"] = Json::arrayValue;

            for (const auto &item : courses)
            {
                Json::Value record;

                record["id"] = Json::Int64(item.id);
                record["departmentId"] =
                    Json::Int64(item.departmentId);
                record["code"] = item.code;
                record["title"] = item.title;
                record["description"] = item.description;
                record["credits"] = item.credits;
                record["courseType"] = item.courseType;
                record["isActive"] = item.isActive;

                body["courses"].append(record);
            }

            (*callbackPtr)(
                HttpResponse::newHttpJsonResponse(body)
            );
        },
        [callbackPtr](
            const drogon::orm::DrogonDbException &error
        )
        {
            databaseError(callbackPtr, error);
        }
    );
}

void AcademicController::listSections(
    const HttpRequestPtr &request,
    ResponseCallback &&callback
)
{
    auto callbackPtr =
        std::make_shared<ResponseCallback>(
            std::move(callback)
        );

    if (!isAdmin(request))
    {
        (*callbackPtr)(unauthorizedResponse());
        return;
    }

    academicService().listSections(
        [callbackPtr](
            std::vector<domain::AcademicSection> sections
        )
        {
            Json::Value body;
            body["status"] = "success";
            body["sections"] = Json::arrayValue;

            for (const auto &item : sections)
            {
                Json::Value record;

                record["id"] = Json::Int64(item.id);
                record["programId"] = Json::Int64(item.programId);
                record["batchYear"] = item.batchYear;
                record["academicYearStart"] =
                    item.academicYearStart;
                record["semesterNumber"] = item.semesterNumber;
                record["sectionCode"] = item.sectionCode;
                record["capacity"] = item.capacity;
                record["isActive"] = item.isActive;

                body["sections"].append(record);
            }

            (*callbackPtr)(
                HttpResponse::newHttpJsonResponse(body)
            );
        },
        [callbackPtr](
            const drogon::orm::DrogonDbException &error
        )
        {
            databaseError(callbackPtr, error);
        }
    );
}
       void AcademicController::createProgram(
    const HttpRequestPtr &request,
    ResponseCallback &&callback
)
{
    auto callbackPtr =
        std::make_shared<ResponseCallback>(std::move(callback));

    if (!isAdmin(request))
    {
        (*callbackPtr)(unauthorizedResponse());
        return;
    }

    auto json = request->getJsonObject();

    if (!json || !json->isObject() ||
        !(*json)["departmentId"].isInt64() ||
        !validText(*json, "code", 20) ||
        !validText(*json, "name", 150) ||
        !validText(*json, "degreeLevel", 30) ||
        !(*json)["durationYears"].isInt() ||
        !(*json)["totalSemesters"].isInt())
    {
        (*callbackPtr)(
            errorResponse(k400BadRequest, "Invalid program details.")
        );
        return;
    }

    try
    {
        academicService().createProgram(
            (*json)["departmentId"].asInt64(),
            (*json)["code"].asString(),
            (*json)["name"].asString(),
            (*json)["degreeLevel"].asString(),
            (*json)["durationYears"].asInt(),
            (*json)["totalSemesters"].asInt(),
            optionalText(*json, "description"),

            [callbackPtr](std::int64_t id)
            {
                (*callbackPtr)(
                    createdResponse("Program created successfully.", id)
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
        (*callbackPtr)(
            errorResponse(k400BadRequest, error.what())
        );
    }
}

void AcademicController::createCourse(
    const HttpRequestPtr &request,
    ResponseCallback &&callback
)
{
    auto callbackPtr =
        std::make_shared<ResponseCallback>(std::move(callback));

    if (!isAdmin(request))
    {
        (*callbackPtr)(unauthorizedResponse());
        return;
    }

    auto json = request->getJsonObject();

    if (!json || !json->isObject() ||
        !(*json)["departmentId"].isInt64() ||
        !validText(*json, "code", 30) ||
        !validText(*json, "title", 150) ||
        !(*json)["credits"].isNumeric() ||
        !validText(*json, "courseType", 20))
    {
        (*callbackPtr)(
            errorResponse(k400BadRequest, "Invalid course details.")
        );
        return;
    }

    try
    {
        academicService().createCourse(
            (*json)["departmentId"].asInt64(),
            (*json)["code"].asString(),
            (*json)["title"].asString(),
            optionalText(*json, "description"),
            (*json)["credits"].asDouble(),
            (*json)["courseType"].asString(),

            [callbackPtr](std::int64_t id)
            {
                (*callbackPtr)(
                    createdResponse("Course created successfully.", id)
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
        (*callbackPtr)(
            errorResponse(k400BadRequest, error.what())
        );
    }
}

void AcademicController::createSection(
    const HttpRequestPtr &request,
    ResponseCallback &&callback
)
{
    auto callbackPtr =
        std::make_shared<ResponseCallback>(std::move(callback));

    if (!isAdmin(request))
    {
        (*callbackPtr)(unauthorizedResponse());
        return;
    }

    auto json = request->getJsonObject();

    if (!json || !json->isObject() ||
        !(*json)["programId"].isInt64() ||
        !(*json)["batchYear"].isInt() ||
        !(*json)["academicYearStart"].isInt() ||
        !(*json)["semesterNumber"].isInt() ||
        !validText(*json, "sectionCode", 10) ||
        !(*json)["capacity"].isInt())
    {
        (*callbackPtr)(
            errorResponse(k400BadRequest, "Invalid section details.")
        );
        return;
    }

    try
    {
        academicService().createSection(
            (*json)["programId"].asInt64(),
            (*json)["batchYear"].asInt(),
            (*json)["academicYearStart"].asInt(),
            (*json)["semesterNumber"].asInt(),
            (*json)["sectionCode"].asString(),
            (*json)["capacity"].asInt(),

            [callbackPtr](std::int64_t id)
            {
                (*callbackPtr)(
                    createdResponse("Section created successfully.", id)
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
        (*callbackPtr)(
            errorResponse(k400BadRequest, error.what())
        );
    }
}
} // namespace universe::controllers