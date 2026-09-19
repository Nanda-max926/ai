
#include <drogon/drogon.h>
#include "controllers/AuthController.h"
#include "services/PasswordService.h"
#include <functional>
#include <memory>
#include <utility>

using namespace drogon;

using ResponseCallback =
    std::function<void(const HttpResponsePtr &)>;

int main()
{
    // Load configuration.
    app().loadConfigFile("config/config.json");
        
if (!universe::services::PasswordService::initialize())
{
    LOG_FATAL << "Failed to initialize password hashing";
    return 1;
}

// Enable server-side sessions for 8 hours.
app().enableSession(
    8 * 60 * 60,
    Cookie::SameSite::kLax
);

    // =========================================
    // TEST 1: BACKEND HEALTH
    // =========================================

    app().registerHandler(
        "/api/v1/health",

        [](const HttpRequestPtr &, ResponseCallback &&callback)
        {
            Json::Value json;

            json["status"] = "success";
            json["project"] = "UniVerse";
            json["backend"] = "online";
            json["message"] = "C++ API is responding";

            auto response =
                HttpResponse::newHttpJsonResponse(json);

            callback(response);
        },

        {Get}
    );

    // =========================================
    // TEST 2: DATABASE HEALTH
    // =========================================

    app().registerHandler(
        "/api/v1/health/database",

        [](const HttpRequestPtr &, ResponseCallback &&callback)
        {
            auto callbackPtr =
                std::make_shared<ResponseCallback>(
                    std::move(callback)
                );

            try
            {
                auto database = app().getDbClient("default");

                if (!database)
                {
                    Json::Value json;
                    json["status"] = "error";
                    json["message"] =
                        "Database client is unavailable";

                    auto response =
                        HttpResponse::newHttpJsonResponse(json);

                    response->setStatusCode(
                        k503ServiceUnavailable
                    );

                    (*callbackPtr)(response);
                    return;
                }

                auto onSuccess =
                    std::function<void(const drogon::orm::Result &)>(
                        [callbackPtr](const drogon::orm::Result &)
                        {
                            Json::Value json;

                            json["status"] = "success";
                            json["project"] = "UniVerse";
                            json["database"] = "connected";
                            json["message"] =
                                "PostgreSQL connection successful";

                            auto response =
                                HttpResponse::newHttpJsonResponse(json);

                            (*callbackPtr)(response);
                        }
                    );

                auto onError =
                    std::function<void(
                        const drogon::orm::DrogonDbException &
                    )>(
                        [callbackPtr](
                            const drogon::orm::DrogonDbException &error
                        )
                        {
                            LOG_ERROR
                                << "Database error: "
                                << error.base().what();

                            Json::Value json;

                            json["status"] = "error";
                            json["project"] = "UniVerse";
                            json["database"] = "disconnected";
                            json["message"] =
                                "PostgreSQL query failed";

                            auto response =
                                HttpResponse::newHttpJsonResponse(json);

                            response->setStatusCode(
                                k503ServiceUnavailable
                            );

                            (*callbackPtr)(response);
                        }
                    );

                database->execSqlAsync(
                    "SELECT 1",
                    onSuccess,
                    onError
                );
            }
            catch (const std::exception &error)
            {
                LOG_ERROR << "Health handler error: "
                          << error.what();

                Json::Value json;

                json["status"] = "error";
                json["message"] =
                    "Unable to execute database health check";

                auto response =
                    HttpResponse::newHttpJsonResponse(json);

                response->setStatusCode(
                    k500InternalServerError
                );

                (*callbackPtr)(response);
            }
        },

        {Get}
    );

    // =========================================
    // START SERVER
    // =========================================
      universe::controllers::AuthController::registerRoutes();
    LOG_INFO << "====================================";
    LOG_INFO << "       UniVerse University Portal";
    LOG_INFO << "       C++ Backend Started";
    LOG_INFO << "       Port: 8080";
    LOG_INFO << "====================================";

    app().run();

    return 0;
}
