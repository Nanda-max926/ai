
#pragma once

#include <drogon/drogon.h>

#include <functional>

namespace universe::controllers
{

class AdminController
{
public:

    using ResponseCallback =
        std::function<void(
            const drogon::HttpResponsePtr &
        )>;

    static void registerRoutes();

    static void listDepartments(
        const drogon::HttpRequestPtr &request,
        ResponseCallback &&callback
    );

    static void createStudent(
        const drogon::HttpRequestPtr &request,
        ResponseCallback &&callback
    );

    static void createFaculty(
        const drogon::HttpRequestPtr &request,
        ResponseCallback &&callback
    );
};

}
