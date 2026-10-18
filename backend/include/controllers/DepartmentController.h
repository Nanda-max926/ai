#pragma once

#include <drogon/drogon.h>

#include <functional>

namespace universe::controllers
{

class DepartmentController
{
public:
    using ResponseCallback =
        std::function<void(const drogon::HttpResponsePtr &)>;

    static void registerRoutes();

    static void list(
        const drogon::HttpRequestPtr &,
        ResponseCallback &&
    );

    static void create(
        const drogon::HttpRequestPtr &,
        ResponseCallback &&
    );

    static void update(
        const drogon::HttpRequestPtr &,
        ResponseCallback &&
    );

    static void setStatus(
        const drogon::HttpRequestPtr &,
        ResponseCallback &&
    );

    static void assignHod(
        const drogon::HttpRequestPtr &,
        ResponseCallback &&
    );
};

}