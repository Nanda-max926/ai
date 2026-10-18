
#include "controllers/AdminController.h"
#include "services/TemporaryPasswordGenerator.h"

#include "services/AccountProvisioningService.h"
#include "services/PasswordService.h"

#include <drogon/drogon.h>
#include <sodium.h>

#include <memory>
#include <string>
#include <utility>
#include <cstdint>

using namespace drogon;

namespace universe::controllers
{

namespace
{

using CallbackPtr =
    std::shared_ptr<AdminController::ResponseCallback>;

HttpResponsePtr sendJson(
    HttpStatusCode status,
    const Json::Value &body
)
{
    auto response =
        HttpResponse::newHttpJsonResponse(body);

    response->setStatusCode(status);

    response->addHeader("Cache-Control", "no-store");

    return response;
}

HttpResponsePtr sendError(
    HttpStatusCode status,
    const std::string &message
)
{
    Json::Value body;

    body["status"] = "error";
    body["message"] = message;

    return sendJson(status, body);
}

bool isAdmin(const HttpRequestPtr &request)
{
    auto session = request->session();

    if (!session || !session->find("role"))
        return false;

    return session->get<std::string>("role") == "admin";
}

bool validEmail(const std::string &email)
{
    if (email.empty() || email.size() > 254)
        return false;

    const auto at = email.find('@');

    if (at == std::string::npos || at == 0)
        return false;

    const auto dot = email.find('.', at);

    return dot != std::string::npos &&
           dot > at + 1 &&
           dot + 1 < email.size();
}

bool validStringField(
    const Json::Value &json,
    const std::string &key,
    std::size_t maximumLength
)
{
    if (!json[key].isString())
        return false;

    const auto &value = json[key].asString();

    return !value.empty() &&
           value.size() <= maximumLength;
}

void wipeSecret(std::string &value)
{
    if (!value.empty())
    {
        sodium_memzero(value.data(), value.size());
        value.clear();
    }
}

void sendDatabaseError(
    const CallbackPtr &callback,
    const orm::DrogonDbException &error
)
{
    LOG_ERROR << "Account management database error: "
              << error.base().what();

    (*callback)(
        sendError(
            k500InternalServerError,
            "Account creation failed. Check the email and department."
        )
    );
}

}

// ============================================
// REGISTER ADMIN ROUTES
// ============================================

void AdminController::registerRoutes()
{
    app().registerHandler(
        "/api/v1/admin/departments",
        &AdminController::listDepartments,
        {Get}
    );

    app().registerHandler(
        "/api/v1/admin/students",
        &AdminController::createStudent,
        {Post}
    );

    app().registerHandler(
        "/api/v1/admin/faculty",
        &AdminController::createFaculty,
        {Post}
    );

    LOG_INFO << "Admin management routes registered";
}

// ============================================
// LIST DEPARTMENTS
// ============================================

void AdminController::listDepartments(
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
        (*callbackPtr)(
            sendError(
                k403Forbidden,
                "Administrator access required."
            )
        );
        return;
    }

    auto database = app().getDbClient("default");

    database->execSqlAsync(

        "SELECT id, code, name "
        "FROM departments "
        "WHERE is_active = TRUE "
        "ORDER BY name",

        [callbackPtr](const orm::Result &result)
        {
            Json::Value departments(Json::arrayValue);

            for (const auto &row : result)
            {
                Json::Value item;

                item["id"] =
                    static_cast<Json::Int64>(
                        row["id"].as<long long>()
                    );

                item["code"] =
                    row["code"].as<std::string>();

                item["name"] =
                    row["name"].as<std::string>();

                departments.append(item);
            }

            Json::Value body;

            body["status"] = "success";
            body["departments"] = departments;

            (*callbackPtr)(sendJson(k200OK, body));
        },

        [callbackPtr](const orm::DrogonDbException &error)
        {
            sendDatabaseError(callbackPtr, error);
        }
    );
}

// ============================================
// CREATE STUDENT
// ============================================

void AdminController::createStudent(
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
        (*callbackPtr)(
            sendError(k403Forbidden, "Administrator access required.")
        );
        return;
    }

    auto json = request->getJsonObject();

    if (!json || !json->isObject())
    {
        (*callbackPtr)(
            sendError(k400BadRequest, "Invalid JSON request.")
        );
        return;
    }

    if (!validStringField(*json, "fullName", 150) ||
        !validStringField(*json, "email", 254) ||
        !validStringField(*json, "section", 20) ||
        !(*json)["departmentId"].isInt64() ||
        !(*json)["semester"].isInt() ||
        !(*json)["batchYear"].isInt())
    {
        (*callbackPtr)(
            sendError(k400BadRequest, "Invalid student details.")
        );
        return;
    }

    const std::string fullName =
        (*json)["fullName"].asString();

    const std::string email =
        (*json)["email"].asString();

    const std::string section =
        (*json)["section"].asString();

    const std::int64_t departmentId =
        (*json)["departmentId"].asInt64();

    const int semester =
        (*json)["semester"].asInt();

    const int batchYear =
        (*json)["batchYear"].asInt();

    if (!validEmail(email) ||
        departmentId <= 0 ||
        semester < 1 || semester > 8 ||
        batchYear < 2000 || batchYear > 2100)
    {
        (*callbackPtr)(
            sendError(k400BadRequest, "Invalid student details.")
        );
        return;
    }

    services::TemporaryPasswordGenerator generator;

    std::string temporaryPassword;

    if (!generator.generate(temporaryPassword))
    {
        (*callbackPtr)(
            sendError(
                k500InternalServerError,
                "Temporary password generation failed."
            )
        );
        return;
    }

    std::string passwordHash;

    if (!services::PasswordService::hashTemporaryPassword(
            temporaryPassword,
            passwordHash
        ))
    {
        wipeSecret(temporaryPassword);

        (*callbackPtr)(
            sendError(
                k500InternalServerError,
                "Temporary password hashing failed."
            )
        );
        return;
    }

    auto temporaryPasswordPtr =
        std::make_shared<std::string>(
            std::move(temporaryPassword)
        );

    auto passwordHashPtr =
        std::make_shared<std::string>(
            std::move(passwordHash)
        );

    services::AccountProvisioningService::createStudent(
        email,
        *passwordHashPtr,
        fullName,
        departmentId,
        semester,
        batchYear,
        section,

        [callbackPtr, temporaryPasswordPtr, passwordHashPtr]
        (long long userId, std::string loginId)
        {
            Json::Value account;

            account["id"] =
                static_cast<Json::Int64>(userId);

            account["loginId"] = loginId;
            account["role"] = "student";
            account["status"] = "active";
            account["mustChangePassword"] = true;

            account["temporaryPassword"] =
                *temporaryPasswordPtr;

            account["temporaryPasswordExpiresInHours"] = 24;

            wipeSecret(*temporaryPasswordPtr);
            wipeSecret(*passwordHashPtr);

            Json::Value body;

            body["status"] = "success";
            body["message"] = "Student account created.";
            body["account"] = account;

            (*callbackPtr)(sendJson(k201Created, body));
        },

        [callbackPtr, temporaryPasswordPtr, passwordHashPtr]
        (const orm::DrogonDbException &error)
        {
            wipeSecret(*temporaryPasswordPtr);
            wipeSecret(*passwordHashPtr);

            sendDatabaseError(callbackPtr, error);
        }
    );
}

// ============================================
// CREATE FACULTY
// ============================================

void AdminController::createFaculty(
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
        (*callbackPtr)(
            sendError(k403Forbidden, "Administrator access required.")
        );
        return;
    }

    auto json = request->getJsonObject();

    if (!json || !json->isObject())
    {
        (*callbackPtr)(
            sendError(k400BadRequest, "Invalid JSON request.")
        );
        return;
    }

    if (!validStringField(*json, "fullName", 150) ||
        !validStringField(*json, "email", 254) ||
        !validStringField(*json, "designation", 100) ||
        !(*json)["departmentId"].isInt64())
    {
        (*callbackPtr)(
            sendError(k400BadRequest, "Invalid faculty details.")
        );
        return;
    }

    const std::string fullName =
        (*json)["fullName"].asString();

    const std::string email =
        (*json)["email"].asString();

    const std::string designation =
        (*json)["designation"].asString();

    const std::int64_t departmentId =
        (*json)["departmentId"].asInt64();

    if (!validEmail(email) || departmentId <= 0)
    {
        (*callbackPtr)(
            sendError(k400BadRequest, "Invalid faculty details.")
        );
        return;
    }

    services::TemporaryPasswordGenerator generator;

    std::string temporaryPassword;

    if (!generator.generate(temporaryPassword))
    {
        (*callbackPtr)(
            sendError(
                k500InternalServerError,
                "Temporary password generation failed."
            )
        );
        return;
    }

    std::string passwordHash;

    if (!services::PasswordService::hashTemporaryPassword(
            temporaryPassword,
            passwordHash
        ))
    {
        wipeSecret(temporaryPassword);

        (*callbackPtr)(
            sendError(
                k500InternalServerError,
                "Temporary password hashing failed."
            )
        );
        return;
    }

    auto temporaryPasswordPtr =
        std::make_shared<std::string>(
            std::move(temporaryPassword)
        );

    auto passwordHashPtr =
        std::make_shared<std::string>(
            std::move(passwordHash)
        );

    services::AccountProvisioningService::createFaculty(
        email,
        *passwordHashPtr,
        fullName,
        departmentId,
        designation,

        [callbackPtr, temporaryPasswordPtr, passwordHashPtr]
        (long long userId, std::string loginId)
        {
            Json::Value account;

            account["id"] =
                static_cast<Json::Int64>(userId);

            account["loginId"] = loginId;
            account["role"] = "faculty";
            account["status"] = "active";
            account["mustChangePassword"] = true;

            account["temporaryPassword"] =
                *temporaryPasswordPtr;

            account["temporaryPasswordExpiresInHours"] = 24;

            wipeSecret(*temporaryPasswordPtr);
            wipeSecret(*passwordHashPtr);

            Json::Value body;

            body["status"] = "success";
            body["message"] = "Faculty account created.";
            body["account"] = account;

            (*callbackPtr)(sendJson(k201Created, body));
        },

        [callbackPtr, temporaryPasswordPtr, passwordHashPtr]
        (const orm::DrogonDbException &error)
        {
            wipeSecret(*temporaryPasswordPtr);
            wipeSecret(*passwordHashPtr);

            sendDatabaseError(callbackPtr, error);
        }
    );
}

}
