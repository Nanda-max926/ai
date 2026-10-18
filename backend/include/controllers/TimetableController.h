#pragma once

#include <drogon/drogon.h>
#include <functional>

namespace universe::controllers
{

class TimetableController
{
public:
    using ResponseCallback = std::function<void(
        const drogon::HttpResponsePtr &
    )>;

    static void registerRoutes();

    static void listRooms(
        const drogon::HttpRequestPtr &request,
        ResponseCallback &&callback
    );

    static void listFaculty(
        const drogon::HttpRequestPtr &request,
        ResponseCallback &&callback
    );

    static void listEntries(
        const drogon::HttpRequestPtr &request,
        ResponseCallback &&callback
    );

    static void createRoom(
        const drogon::HttpRequestPtr &request,
        ResponseCallback &&callback
    );

    static void createEntry(
        const drogon::HttpRequestPtr &request,
        ResponseCallback &&callback
    );
};

}