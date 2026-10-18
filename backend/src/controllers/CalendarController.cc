#include "controllers/CalendarController.h"
#include <drogon/drogon.h>
#include <memory>
#include <string>
#include <utility>
#include <cstdint>

using namespace drogon;

namespace universe::controllers
{

namespace
{

using Callback = CalendarController::ResponseCallback;

HttpResponsePtr makeJson(
    Json::Value body,
    HttpStatusCode status = k200OK
)
{
    auto response = HttpResponse::newHttpJsonResponse(body);
    response->setStatusCode(status);
    response->addHeader("Cache-Control", "no-store");
    return response;
}

HttpResponsePtr makeError(
    HttpStatusCode status,
    const std::string &message
)
{
    Json::Value body;
    body["status"] = "error";
    body["message"] = message;

    return makeJson(std::move(body), status);
}

bool isAdmin(const HttpRequestPtr &request)
{
    auto session = request->session();

    return session &&
        session->get<std::string>("role") == "admin";
}

bool validText(
    const Json::Value &json,
    const char *field,
    std::size_t maxLength
)
{
    return json.isMember(field) &&
        json[field].isString() &&
        !json[field].asString().empty() &&
        json[field].asString().size() <= maxLength;
}

 std::shared_ptr<orm::DbClient> database()
{
    return app().getDbClient("default");
}

void sendDatabaseError(
    const std::shared_ptr<Callback> &callback,
    const orm::DrogonDbException &error
)
{
    const std::string detail = error.base().what();
    LOG_ERROR << "Calendar/announcement database error: " << detail;

    (*callback)(
        makeError(
            k500InternalServerError,
            "The database could not complete this operation."
        )
    );
}

void registerEventRoutes()
{
    // PUBLIC: List published calendar events.
    app().registerHandler(
        "/api/v1/calendar/events",

        [](const HttpRequestPtr &, Callback &&callback)
        {
            auto cb = std::make_shared<Callback>(std::move(callback));

            database()->execSqlAsync(
                R"SQL(
                    SELECT
                        id,
                        title,
                        description,
                        event_type,
                        location,
                        TO_CHAR(start_date, 'YYYY-MM-DD') AS start_date,
                        TO_CHAR(end_date, 'YYYY-MM-DD') AS end_date,
                        COALESCE(TO_CHAR(start_time, 'HH24:MI'), '') AS start_time,
                        COALESCE(TO_CHAR(end_time, 'HH24:MI'), '') AS end_time
                    FROM campus_events
                    WHERE is_published = TRUE
                    ORDER BY start_date, start_time
                )SQL",

                [cb](const orm::Result &result)
                {
                    Json::Value body;
                    body["status"] = "success";
                    body["events"] = Json::arrayValue;

                    for (const auto &row : result)
                    {
                        Json::Value item;
                        item["id"] = Json::Int64(
                            row["id"].as<std::int64_t>()
                        );
                        item["title"] = row["title"].as<std::string>();
                        item["description"] =
                            row["description"].as<std::string>();
                        item["eventType"] =
                            row["event_type"].as<std::string>();
                        item["location"] =
                            row["location"].as<std::string>();
                        item["startDate"] =
                            row["start_date"].as<std::string>();
                        item["endDate"] =
                            row["end_date"].as<std::string>();
                        item["startTime"] =
                            row["start_time"].as<std::string>();
                        item["endTime"] =
                            row["end_time"].as<std::string>();

                        body["events"].append(item);
                    }

                    (*cb)(makeJson(std::move(body)));
                },

                [cb](const orm::DrogonDbException &error)
                {
                    sendDatabaseError(cb, error);
                }
            );
        },

        {Get}
    );

    // ADMIN: Create a calendar event.
    app().registerHandler(
        "/api/v1/admin/calendar/events",

        [](const HttpRequestPtr &request, Callback &&callback)
        {
            auto cb = std::make_shared<Callback>(std::move(callback));

            if (!isAdmin(request))
            {
                (*cb)(makeError(k403Forbidden, "Administrator access required."));
                return;
            }

            auto json = request->getJsonObject();

            if (!json || !json->isObject() ||
                !validText(*json, "title", 180) ||
                !validText(*json, "eventType", 30) ||
                !validText(*json, "startDate", 10) ||
                !validText(*json, "endDate", 10))
            {
                (*cb)(makeError(k400BadRequest, "Invalid event details."));
                return;
            }

            const std::string type = (*json)["eventType"].asString();

            if (type != "academic" &&
                type != "examination" &&
                type != "seminar" &&
                type != "workshop" &&
                type != "holiday" &&
                type != "university_closure" &&
                type != "other")
            {
                (*cb)(makeError(k400BadRequest, "Invalid event type."));
                return;
            }

            const std::string title = (*json)["title"].asString();
            const std::string description =
                json->isMember("description") &&
                (*json)["description"].isString()
                    ? (*json)["description"].asString() : "";

            const std::string location =
                json->isMember("location") &&
                (*json)["location"].isString()
                    ? (*json)["location"].asString() : "";

            const std::string startDate =
                (*json)["startDate"].asString();
            const std::string endDate =
                (*json)["endDate"].asString();

            const std::string startTime =
                json->isMember("startTime") &&
                (*json)["startTime"].isString()
                    ? (*json)["startTime"].asString() : "";

            const std::string endTime =
                json->isMember("endTime") &&
                (*json)["endTime"].isString()
                    ? (*json)["endTime"].asString() : "";

            database()->execSqlAsync(
                R"SQL(
                    INSERT INTO campus_events (
                        title, description, event_type, location,
                        start_date, end_date, start_time, end_time
                    )
                    VALUES (
                        $1, $2, $3, $4,
                        $5::date, $6::date,
                        NULLIF($7, '')::time,
                        NULLIF($8, '')::time
                    )
                    RETURNING id
                )SQL",

                [cb](const orm::Result &result)
                {
                    Json::Value body;
                    body["status"] = "success";
                    body["message"] = "Calendar event created.";
                    body["id"] = Json::Int64(
                        result[0]["id"].as<std::int64_t>()
                    );

                    (*cb)(makeJson(std::move(body), k201Created));
                },

                [cb](const orm::DrogonDbException &error)
                {
                    sendDatabaseError(cb, error);
                },

                title, description, type, location,
                startDate, endDate, startTime, endTime
            );
        },

        {Post}
    );
}

void registerAnnouncementRoutes()
{
    // PUBLIC: List announcements visible to the current audience.
    app().registerHandler(
        "/api/v1/announcements",

        [](const HttpRequestPtr &request, Callback &&callback)
        {
            auto cb = std::make_shared<Callback>(std::move(callback));

            std::string role = "guest";
            auto session = request->session();

            if (session)
            {
                const std::string sessionRole =
                    session->get<std::string>("role");

                if (!sessionRole.empty())
                    role = sessionRole;
            }

            database()->execSqlAsync(
                R"SQL(
                    SELECT
                        id,
                        title,
                        content,
                        priority,
                        audience,
                        TO_CHAR(publish_at, 'YYYY-MM-DD HH24:MI') AS publish_at
                    FROM announcements
                    WHERE is_published = TRUE
                      AND publish_at <= CURRENT_TIMESTAMP
                      AND (expires_at IS NULL OR expires_at > CURRENT_TIMESTAMP)
                      AND (audience = 'everyone' OR audience = $1)
                    ORDER BY
                        CASE priority
                            WHEN 'urgent' THEN 1
                            WHEN 'high' THEN 2
                            WHEN 'normal' THEN 3
                            ELSE 4
                        END,
                        publish_at DESC
                )SQL",

                [cb](const orm::Result &result)
                {
                    Json::Value body;
                    body["status"] = "success";
                    body["announcements"] = Json::arrayValue;

                    for (const auto &row : result)
                    {
                        Json::Value item;
                        item["id"] = Json::Int64(
                            row["id"].as<std::int64_t>()
                        );
                        item["title"] = row["title"].as<std::string>();
                        item["content"] = row["content"].as<std::string>();
                        item["priority"] = row["priority"].as<std::string>();
                        item["audience"] = row["audience"].as<std::string>();
                        item["publishAt"] =
                            row["publish_at"].as<std::string>();

                        body["announcements"].append(item);
                    }

                    (*cb)(makeJson(std::move(body)));
                },

                [cb](const orm::DrogonDbException &error)
                {
                    sendDatabaseError(cb, error);
                },

                role
            );
        },

        {Get}
    );

    // ADMIN: Publish an announcement.
    app().registerHandler(
        "/api/v1/admin/announcements",

        [](const HttpRequestPtr &request, Callback &&callback)
        {
            auto cb = std::make_shared<Callback>(std::move(callback));

            if (!isAdmin(request))
            {
                (*cb)(makeError(k403Forbidden, "Administrator access required."));
                return;
            }

            auto json = request->getJsonObject();

            if (!json || !json->isObject() ||
                !validText(*json, "title", 180) ||
                !validText(*json, "content", 10000) ||
                !validText(*json, "priority", 20) ||
                !validText(*json, "audience", 20))
            {
                (*cb)(makeError(k400BadRequest, "Invalid announcement details."));
                return;
            }

            const std::string priority =
                (*json)["priority"].asString();

            const std::string audience =
                (*json)["audience"].asString();

            if (priority != "low" && priority != "normal" &&
                priority != "high" && priority != "urgent")
            {
                (*cb)(makeError(k400BadRequest, "Invalid priority."));
                return;
            }

            if (audience != "everyone" && audience != "student" &&
                audience != "faculty" && audience != "admin")
            {
                (*cb)(makeError(k400BadRequest, "Invalid audience."));
                return;
            }

            const std::string title =
                (*json)["title"].asString();

            const std::string content =
                (*json)["content"].asString();

            const std::string expiresAt =
                json->isMember("expiresAt") &&
                (*json)["expiresAt"].isString()
                    ? (*json)["expiresAt"].asString() : "";

            database()->execSqlAsync(
                R"SQL(
                    INSERT INTO announcements (
                        title, content, priority, audience, expires_at
                    )
                    VALUES (
                        $1, $2, $3, $4,
                        NULLIF($5, '')::timestamptz
                    )
                    RETURNING id
                )SQL",

                [cb](const orm::Result &result)
                {
                    Json::Value body;
                    body["status"] = "success";
                    body["message"] = "Announcement published.";
                    body["id"] = Json::Int64(
                        result[0]["id"].as<std::int64_t>()
                    );

                    (*cb)(makeJson(std::move(body), k201Created));
                },

                [cb](const orm::DrogonDbException &error)
                {
                    sendDatabaseError(cb, error);
                },

                title, content, priority, audience, expiresAt
            );
        },

        {Post}
    );
}

} // anonymous namespace

void CalendarController::registerRoutes()
{
    registerEventRoutes();
    registerAnnouncementRoutes();

    LOG_INFO << "Calendar and announcement routes registered.";
}

}