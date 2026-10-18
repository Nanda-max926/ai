#pragma once

#include "domain/TimetableModels.h"

#include <drogon/drogon.h>

#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace universe::repositories
{

class TimetableRepository
{
public:
    using RoomCallback =
        std::function<void(std::vector<domain::AcademicRoom>)>;

    using FacultyCallback =
        std::function<void(std::vector<domain::FacultyOption>)>;

    using EntryCallback =
        std::function<void(std::vector<domain::TimetableEntry>)>;

    using IdCallback =
        std::function<void(std::int64_t)>;

    using ErrorCallback =
        std::function<void(const drogon::orm::DrogonDbException &)>;

    explicit TimetableRepository(
        std::shared_ptr<drogon::orm::DbClient> database
    );

    void listRooms(RoomCallback success, ErrorCallback error);

    void listFaculty(FacultyCallback success, ErrorCallback error);

    void listEntries(EntryCallback success, ErrorCallback error);

    void createRoom(
        const std::string &building,
        const std::string &roomNumber,
        int capacity,
        const std::string &roomType,
        IdCallback success,
        ErrorCallback error
    );

    void createEntry(
        std::int64_t sectionId,
        std::int64_t courseId,
        std::int64_t facultyUserId,
        std::int64_t roomId,
        int dayOfWeek,
        const std::string &startTime,
        const std::string &endTime,
        IdCallback success,
        ErrorCallback error
    );

private:
    std::shared_ptr<drogon::orm::DbClient> database_;
};

}