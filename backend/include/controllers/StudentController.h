#pragma once

#include <drogon/drogon.h>
#include <functional>

namespace universe::controllers
{
class StudentController
{
public:
    using Callback = std::function<void(
        const drogon::HttpResponsePtr &
    )>;

    static void registerRoutes();

    static void getPortal(
        const drogon::HttpRequestPtr &request,
        Callback &&callback
    );
};
}