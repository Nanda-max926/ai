
#include "controllers/AuthController.h"

#include <drogon/drogon.h>

using namespace drogon;

namespace universe::controllers
{

// ============================================
// REGISTER AUTHENTICATION ROUTES
// ============================================

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

// ============================================
// TEMPORARY LOGIN HANDLER
// ============================================

void AuthController::login(
    const HttpRequestPtr &,
    ResponseCallback &&callback
)
{
    Json::Value json;

    json["status"] = "pending";
    json["message"] =
        "Login implementation will be connected next";

    auto response = HttpResponse::newHttpJsonResponse(json);

    response->setStatusCode(k501NotImplemented);

    callback(response);
}

// ============================================
// CURRENT USER HANDLER
// ============================================

void AuthController::me(
    const HttpRequestPtr &,
    ResponseCallback &&callback
)
{
    Json::Value json;

    json["status"] = "error";
    json["message"] = "Authentication required";

    auto response = HttpResponse::newHttpJsonResponse(json);

    response->setStatusCode(k401Unauthorized);

    callback(response);
}

// ============================================
// LOGOUT HANDLER
// ============================================

void AuthController::logout(
    const HttpRequestPtr &,
    ResponseCallback &&callback
)
{
    Json::Value json;

    json["status"] = "success";
    json["message"] = "Logout endpoint is working";

    auto response = HttpResponse::newHttpJsonResponse(json);

    callback(response);
}

}
