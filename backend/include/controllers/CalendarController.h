#pragma once

#include <drogon/drogon.h>
#include <functional>

namespace universe::controllers
{

class CalendarController
{
public:
    using ResponseCallback =
        std::function<void(const drogon::HttpResponsePtr &)>;

    static void registerRoutes();
};

}