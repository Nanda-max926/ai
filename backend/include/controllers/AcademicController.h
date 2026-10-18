#pragma once

#include <drogon/drogon.h>
#include <functional>

namespace universe::controllers
{

class AcademicController
{
public:
    using ResponseCallback = std::function<void(
        const drogon::HttpResponsePtr &
    )>;

    static void registerRoutes();

    static void listPrograms(
        const drogon::HttpRequestPtr &request,
        ResponseCallback &&callback
    );

    static void listCourses(
        const drogon::HttpRequestPtr &request,
        ResponseCallback &&callback
    );

    static void listSections(
        const drogon::HttpRequestPtr &request,
        ResponseCallback &&callback
    );

    static void createProgram(
    const drogon::HttpRequestPtr &request,
    ResponseCallback &&callback
);

static void createCourse(
    const drogon::HttpRequestPtr &request,
    ResponseCallback &&callback
);

static void createSection(
    const drogon::HttpRequestPtr &request,
    ResponseCallback &&callback
);

};

}