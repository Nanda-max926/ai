#include "controllers/DepartmentController.h"

#include "repositories/DepartmentRepository.h"
#include "services/DepartmentService.h"

#include <memory>
#include <string>

using namespace drogon;

namespace universe::controllers
{

namespace
{    
      using ResponseCallback =DepartmentController::ResponseCallback;


services::DepartmentService &departmentService()
{
    static services::DepartmentService service(
        std::make_shared<repositories::DepartmentRepository>()
    );

    return service;
}

bool isAdmin(const HttpRequestPtr &request)
{
    auto session = request->session();

    return session &&
           session->find("role") &&
           session->get<std::string>("role") == "admin";
}

HttpResponsePtr jsonResponse(
    HttpStatusCode status,
    const Json::Value &body
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
    return jsonResponse(status, body);
}

void unauthorized(ResponseCallback &callback)
{
    callback(errorResponse(
        k403Forbidden,
        "Administrator access required."
    ));
}

void databaseError(
    ResponseCallback &callback,
    const std::string &error
)
{
    LOG_ERROR << "Department database error: " << error;

    callback(errorResponse(
        k500InternalServerError,
        "Department operation failed."
    ));
}

std::string stringField(
    const Json::Value &json,
    const char *field
)
{
    return json[field].isString()
        ? json[field].asString()
        : "";
}

void mutationResponse(
    const std::shared_ptr<ResponseCallback> &callback,
    long long id,
    const std::string &message
)
{
    if (id <= 0)
    {
        (*callback)(errorResponse(
            k404NotFound,
            "Department or matching faculty member was not found."
        ));
        return;
    }

    Json::Value body;
    body["status"] = "success";
    body["message"] = message;
    body["id"] = static_cast<Json::Int64>(id);

    (*callback)(jsonResponse(k200OK, body));
}

}

void DepartmentController::registerRoutes()
{
    app().registerHandler(
        "/api/v1/admin/departments/full",
        &DepartmentController::list,
        {Get}
    );

    app().registerHandler(
        "/api/v1/admin/departments",
        &DepartmentController::create,
        {Post}
    );

    app().registerHandler(
        "/api/v1/admin/departments",
        &DepartmentController::update,
        {Put}
    );

    app().registerHandler(
        "/api/v1/admin/departments/status",
        &DepartmentController::setStatus,
        {Patch}
    );

    app().registerHandler(
        "/api/v1/admin/departments/hod",
        &DepartmentController::assignHod,
        {Put}
    );

    LOG_INFO << "Department management routes registered";
}

void DepartmentController::list(
    const HttpRequestPtr &request,
    ResponseCallback &&callback
)
{
    auto callbackPtr =
        std::make_shared<ResponseCallback>(std::move(callback));

    if (!isAdmin(request))
    {
        unauthorized(*callbackPtr);
        return;
    }

    departmentService().list(
        [callbackPtr](std::vector<domain::Department> departments)
        {
            Json::Value body;
            Json::Value items(Json::arrayValue);

            for (const auto &department : departments)
            {
                Json::Value item;

                item["id"] =
                    static_cast<Json::Int64>(department.id());

                item["code"] = department.code();
                item["name"] = department.name();
                item["isActive"] = department.isActive();
                item["description"] = department.description();
                item["officeLocation"] = department.officeLocation();
                item["contactEmail"] = department.contactEmail();
                item["contactPhone"] = department.contactPhone();

                item["hodUserId"] =
                    static_cast<Json::Int64>(department.hodUserId());

                item["updatedAt"] = department.updatedAt();

                items.append(item);
            }

            body["status"] = "success";
            body["departments"] = items;

            (*callbackPtr)(jsonResponse(k200OK, body));
        },

        [callbackPtr](const std::string &error)
        {
            databaseError(*callbackPtr, error);
        }
    );
}

void DepartmentController::create(
    const HttpRequestPtr &request,
    ResponseCallback &&callback
)
{
    auto callbackPtr =
        std::make_shared<ResponseCallback>(std::move(callback));

    if (!isAdmin(request))
    {
        unauthorized(*callbackPtr);
        return;
    }

    auto json = request->getJsonObject();

    if (!json || !json->isObject())
    {
        (*callbackPtr)(errorResponse(k400BadRequest, "Invalid JSON."));
        return;
    }

    const auto code = stringField(*json, "code");
    const auto name = stringField(*json, "name");

    if (code.empty() || name.empty())
    {
        (*callbackPtr)(errorResponse(
            k400BadRequest,
            "Department code and name are required."
        ));
        return;
    }

    departmentService().create(
        code,
        name,
        stringField(*json, "description"),
        stringField(*json, "officeLocation"),
        stringField(*json, "contactEmail"),
        stringField(*json, "contactPhone"),

        [callbackPtr](long long id)
        {
            mutationResponse(
                callbackPtr, id, "Department created successfully."
            );
        },

        [callbackPtr](const std::string &error)
        {
            databaseError(*callbackPtr, error);
        }
    );
}

void DepartmentController::update(
    const HttpRequestPtr &request,
    ResponseCallback &&callback
)
{
    auto callbackPtr =
        std::make_shared<ResponseCallback>(std::move(callback));

    if (!isAdmin(request))
    {
        unauthorized(*callbackPtr);
        return;
    }

    auto json = request->getJsonObject();

    if (!json || !json->isObject() ||
        !(*json)["id"].isInt64())
    {
        (*callbackPtr)(errorResponse(
            k400BadRequest,
            "A valid department ID is required."
        ));
        return;
    }

    departmentService().update(
        (*json)["id"].asInt64(),
        stringField(*json, "code"),
        stringField(*json, "name"),
        stringField(*json, "description"),
        stringField(*json, "officeLocation"),
        stringField(*json, "contactEmail"),
        stringField(*json, "contactPhone"),

        [callbackPtr](long long id)
        {
            mutationResponse(
                callbackPtr, id, "Department updated successfully."
            );
        },

        [callbackPtr](const std::string &error)
        {
            databaseError(*callbackPtr, error);
        }
    );
}

void DepartmentController::setStatus(
    const HttpRequestPtr &request,
    ResponseCallback &&callback
)
{
    auto callbackPtr =
        std::make_shared<ResponseCallback>(std::move(callback));

    if (!isAdmin(request))
    {
        unauthorized(*callbackPtr);
        return;
    }

    auto json = request->getJsonObject();

    if (!json || !json->isObject() ||
        !(*json)["id"].isInt64() ||
        !(*json)["isActive"].isBool())
    {
        (*callbackPtr)(errorResponse(
            k400BadRequest,
            "Department ID and active status are required."
        ));
        return;
    }

    departmentService().setActive(
        (*json)["id"].asInt64(),
        (*json)["isActive"].asBool(),

        [callbackPtr](long long id)
        {
            mutationResponse(
                callbackPtr, id, "Department status updated."
            );
        },

        [callbackPtr](const std::string &error)
        {
            databaseError(*callbackPtr, error);
        }
    );
}

void DepartmentController::assignHod(
    const HttpRequestPtr &request,
    ResponseCallback &&callback
)
{
    auto callbackPtr =
        std::make_shared<ResponseCallback>(std::move(callback));

    if (!isAdmin(request))
    {
        unauthorized(*callbackPtr);
        return;
    }

    auto json = request->getJsonObject();

    if (!json || !json->isObject() ||
        !(*json)["departmentId"].isInt64() ||
        !(*json)["facultyUserId"].isInt64())
    {
        (*callbackPtr)(errorResponse(
            k400BadRequest,
            "Department ID and faculty user ID are required."
        ));
        return;
    }

    departmentService().assignHod(
        (*json)["departmentId"].asInt64(),
        (*json)["facultyUserId"].asInt64(),

        [callbackPtr](long long id)
        {
            mutationResponse(
                callbackPtr, id, "HOD assigned successfully."
            );
        },

        [callbackPtr](const std::string &error)
        {
            databaseError(*callbackPtr, error);
        }
    );
}

}