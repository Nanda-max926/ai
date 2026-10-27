
#include "controllers/AuthController.h"

#include "repositories/UserRepository.h"
#include "services/AuthService.h"

#include <sodium.h>

#include <memory>
#include <string>
#include <utility>

using namespace drogon;

namespace universe::controllers
{

namespace
{

HttpResponsePtr jsonResponse(
    HttpStatusCode status,
    const Json::Value &body
)
{
    auto response =
        HttpResponse::newHttpJsonResponse(body);

    response->setStatusCode(status);

    response->addHeader(
        "Cache-Control",
        "no-store"
    );

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

void wipePassword(std::string &password)
{
    if (!password.empty())
    {
        sodium_memzero(
            password.data(),
            password.size()
        );

        password.clear();
    }
}

}

// =============================================
// REGISTER ROUTES
// =============================================

void AuthController::registerRoutes()
{
    app().registerHandler(
        "/api/v1/auth/login",
        &AuthController::login,
        {Post}
    );

    app().registerHandler(
        "/api/v1/auth/me",
        &AuthController::me,
        {Get}
    );

    app().registerHandler(
        "/api/v1/auth/logout",
        &AuthController::logout,
        {Post}
    );

    LOG_INFO << "Authentication routes registered";
}

// =============================================
// LOGIN
// =============================================

void AuthController::login(
    const HttpRequestPtr &request,
    ResponseCallback &&callback
)
{
    auto callbackPtr =
        std::make_shared<ResponseCallback>(
            std::move(callback)
        );

    auto json = request->getJsonObject();

    if (!json || !json->isObject())
    {
        (*callbackPtr)(
            errorResponse(
                k400BadRequest,
                "Invalid JSON request."
            )
        );

        return;
    }

    if (!(*json)["loginId"].isString() ||
        !(*json)["password"].isString() ||
        !(*json)["role"].isString())
    {
        (*callbackPtr)(
            errorResponse(
                k400BadRequest,
                "Login ID, password and role are required."
            )
        );

        return;
    }

    const std::string loginId =
        (*json)["loginId"].asString();

    const std::string requestedRole =
        (*json)["role"].asString();

    auto password =
        std::make_shared<std::string>(
            (*json)["password"].asString()
        );

    if (loginId.empty() ||
        loginId.size() > 254 ||
        password->empty() ||
        password->size() > 1024 ||
        !services::AuthService::isValidRole(requestedRole))
    {
        wipePassword(*password);

        (*callbackPtr)(
            errorResponse(
                k400BadRequest,
                "Invalid login details."
            )
        );

        return;
    }

    // Find account in PostgreSQL.
    repositories::UserRepository::findByLoginId(

        loginId,

        [request, callbackPtr, password, requestedRole]
        (std::optional<domain::User> user)
        {
            if (!user.has_value())
            {
                wipePassword(*password);

                (*callbackPtr)(
                    errorResponse(
                        k401Unauthorized,
                        "Invalid login ID or password."
                    )
                );

                return;
            }

            std::string errorMessage;

            const bool authenticated =
                services::AuthService::authenticate(
                    *user,
                    *password,
                    requestedRole,
                    errorMessage
                );

            // Clear plaintext password after verification.
            wipePassword(*password);

            if (!authenticated)
            {
                (*callbackPtr)(
                    errorResponse(
                        k401Unauthorized,
                        errorMessage
                    )
                );

                return;
            }

            // Create server-managed session.
            auto session = request->session();

            if (!session)
            {
                (*callbackPtr)(
                    errorResponse(
                        k500InternalServerError,
                        "Session service unavailable."
                    )
                );

                return;
            }

            // Clear old session information.
            session->clear();

            // Ask Drogon to rotate session ID.
            session->changeSessionIdToClient();

            // Store authenticated identity.
            session->insert("user_id", user->id);
            session->insert("login_id", user->loginId);
            session->insert("email", user->email);
            session->insert("role", user->role);

            Json::Value response;
            Json::Value userDetails;

            userDetails["id"] =
                static_cast<Json::Int64>(user->id);

            userDetails["loginId"] = user->loginId;
            userDetails["email"] = user->email;
            userDetails["role"] = user->role;

            response["status"] = "success";
            response["message"] = "Login successful";
            response["mustChangePassword"] =
                user->mustChangePassword;

            response["user"] = userDetails;

            (*callbackPtr)(
                jsonResponse(k200OK, response)
            );
        },

        [callbackPtr, password]
        (const orm::DrogonDbException &error)
        {
            wipePassword(*password);

            LOG_ERROR
                << "Authentication database error: "
                << error.base().what();

            (*callbackPtr)(
                errorResponse(
                    k503ServiceUnavailable,
                    "Authentication service unavailable."
                )
            );
        }
    );
}

// =============================================
// CURRENT LOGGED-IN USER
// =============================================

void AuthController::me(
    const HttpRequestPtr &request,
    ResponseCallback &&callback
)
{
    auto session = request->session();

    if (!session || !session->find("user_id"))
    {
        callback(
            errorResponse(
                k401Unauthorized,
                "Authentication required."
            )
        );

        return;
    }

    Json::Value user;

    user["id"] = static_cast<Json::Int64>(
        session->get<long long>("user_id")
    );

    user["loginId"] =
        session->get<std::string>("login_id");

    user["email"] =
        session->get<std::string>("email");

    user["role"] =
        session->get<std::string>("role");

    Json::Value response;

    response["status"] = "success";
    response["user"] = user;

    callback(
        jsonResponse(k200OK, response)
    );
}

// =============================================
// LOGOUT
// =============================================

void AuthController::logout(
    const HttpRequestPtr &request,
    ResponseCallback &&callback
)
{
    auto session = request->session();

    if (session)
    {
        session->clear();

        session->changeSessionIdToClient();
    }

    Json::Value response;

    response["status"] = "success";
    response["message"] = "Logged out successfully";

    callback(
        jsonResponse(k200OK, response)
    );
}

}

