#include <cstdint>
#include "repositories/TimetableRepository.h"

#include <stdexcept>
#include <utility>

namespace universe::repositories
{

TimetableRepository::TimetableRepository(
    std::shared_ptr<drogon::orm::DbClient> database
)
    : database_(std::move(database))
{
    if (!database_)
    {
        throw std::invalid_argument(
            "TimetableRepository requires a database client"
        );
    }
}

void TimetableRepository::listRooms(
    RoomCallback success,
    ErrorCallback error
)
{
    database_->execSqlAsync(
        "SELECT id, building, room_number, capacity, room_type, is_active "
        "FROM academic_rooms ORDER BY building, room_number",

        [success = std::move(success)](
            const drogon::orm::Result &result
        )
        {
            std::vector<domain::AcademicRoom> rooms;

            for (const auto &row : result)
            {
                domain::AcademicRoom item;

                item.id = row["id"].as<std::int64_t>();
                item.building = row["building"].as<std::string>();
                item.roomNumber = row["room_number"].as<std::string>();
                item.capacity = row["capacity"].as<int>();
                item.roomType = row["room_type"].as<std::string>();
                item.isActive = row["is_active"].as<bool>();

                rooms.push_back(std::move(item));
            }

            success(std::move(rooms));
        },

        [error = std::move(error)](
            const drogon::orm::DrogonDbException &exception
        )
        {
            error(exception);
        }
    );
}

void TimetableRepository::listFaculty(
    FacultyCallback success,
    ErrorCallback error
)
{
    database_->execSqlAsync(
        "SELECT id, login_id, email "
        "FROM users "
        "WHERE role = 'faculty' AND status = 'active' "
        "ORDER BY login_id",

        [success = std::move(success)](
            const drogon::orm::Result &result
        )
        {
            std::vector<domain::FacultyOption> faculty;

            for (const auto &row : result)
            {
                domain::FacultyOption item;

                item.id = row["id"].as<std::int64_t>();
                item.loginId = row["login_id"].as<std::string>();
                item.email = row["email"].as<std::string>();

                faculty.push_back(std::move(item));
            }

            success(std::move(faculty));
        },

        [error = std::move(error)](
            const drogon::orm::DrogonDbException &exception
        )
        {
            error(exception);
        }
    );
}

void TimetableRepository::listEntries(
    EntryCallback success,
    ErrorCallback error
)
{
    database_->execSqlAsync(
        R"SQL(
            SELECT
                t.id,
                t.section_id,
                t.course_id,
                t.faculty_user_id,
                t.room_id,
                t.day_of_week,
                TO_CHAR(t.start_time, 'HH24:MI') AS start_time,
                TO_CHAR(t.end_time, 'HH24:MI') AS end_time,
                s.section_code,
                s.batch_year,
                c.code AS course_code,
                c.title AS course_title,
                u.login_id AS faculty_login_id,
                CONCAT(r.building, ' / ', r.room_number) AS room_label
            FROM academic_timetable_entries t
            JOIN academic_sections s ON s.id = t.section_id
            JOIN academic_courses c ON c.id = t.course_id
            JOIN users u ON u.id = t.faculty_user_id
            JOIN academic_rooms r ON r.id = t.room_id
            WHERE t.is_active = TRUE
            ORDER BY t.day_of_week, t.start_time
        )SQL",

        [success = std::move(success)](
            const drogon::orm::Result &result
        )
        {
            std::vector<domain::TimetableEntry> entries;

            for (const auto &row : result)
            {
                domain::TimetableEntry item;

                item.id = row["id"].as<std::int64_t>();
                item.sectionId = row["section_id"].as<std::int64_t>();
                item.courseId = row["course_id"].as<std::int64_t>();
                item.facultyUserId =
                    row["faculty_user_id"].as<std::int64_t>();
                item.roomId = row["room_id"].as<std::int64_t>();

                item.dayOfWeek = row["day_of_week"].as<int>();
                item.startTime = row["start_time"].as<std::string>();
                item.endTime = row["end_time"].as<std::string>();

                item.sectionCode =
                    row["section_code"].as<std::string>();
                item.batchYear = row["batch_year"].as<int>();

                item.courseCode = row["course_code"].as<std::string>();
                item.courseTitle = row["course_title"].as<std::string>();

                item.facultyLoginId =
                    row["faculty_login_id"].as<std::string>();
                item.roomLabel = row["room_label"].as<std::string>();

                entries.push_back(std::move(item));
            }

            success(std::move(entries));
        },

        [error = std::move(error)](
            const drogon::orm::DrogonDbException &exception
        )
        {
            error(exception);
        }
    );
}

void TimetableRepository::createRoom(
    const std::string &building,
    const std::string &roomNumber,
    int capacity,
    const std::string &roomType,
    IdCallback success,
    ErrorCallback error
)
{
    database_->execSqlAsync(
        "INSERT INTO academic_rooms "
        "(building, room_number, capacity, room_type) "
        "VALUES ($1, $2, $3, $4) RETURNING id",

        [success = std::move(success)](
            const drogon::orm::Result &result
        )
        {
            success(result[0]["id"].as<std::int64_t>());
        },

        [error = std::move(error)](
            const drogon::orm::DrogonDbException &exception
        )
        {
            error(exception);
        },

        building, roomNumber, static_cast<std::int16_t>(capacity), roomType
    );
}

void TimetableRepository::createEntry(
    std::int64_t sectionId,
    std::int64_t courseId,
    std::int64_t facultyUserId,
    std::int64_t roomId,
    int dayOfWeek,
    const std::string &startTime,
    const std::string &endTime,
    IdCallback success,
    ErrorCallback error
)
{
    database_->execSqlAsync(
        "INSERT INTO academic_timetable_entries "
        "(section_id, course_id, faculty_user_id, room_id, "
        "day_of_week, start_time, end_time) "
        "VALUES ($1, $2, $3, $4, $5, $6::time, $7::time) "
        "RETURNING id",

        [success = std::move(success)](
            const drogon::orm::Result &result
        )
        {
            success(result[0]["id"].as<std::int64_t>());
        },

        [error = std::move(error)](
            const drogon::orm::DrogonDbException &exception
        )
        {
            error(exception);
        },

        sectionId, courseId, facultyUserId, roomId,
        static_cast<std::int16_t>(dayOfWeek),
        startTime, endTime
    );
}

}