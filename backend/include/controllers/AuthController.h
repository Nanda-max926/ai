
#pragma once

#include <drogon/drogon.h>
#include <functional>

namespace universe::controllers
{

class AuthController
{
public:

    using ResponseCallback =
        std::function<void(
            const drogon::HttpResponsePtr &
        )>;

    static void registerRoutes();

    static void login(
        const drogon::HttpRequestPtr &request,
        ResponseCallback &&callback
    );

    static void me(
        const drogon::HttpRequestPtr &request,
        ResponseCallback &&callback
    );

    static void logout(
        const drogon::HttpRequestPtr &request,
        ResponseCallback &&callback
    );
};

}
