#pragma once

#include "repositories/TimetableRepository.h"

#include <memory>
#include <string>

namespace universe::services
{

class TimetableService
{
public:
    explicit TimetableService(
        std::shared_ptr<repositories::TimetableRepository> repository
    );

    void listRooms(
        repositories::TimetableRepository::RoomCallback success,
        repositories::TimetableRepository::ErrorCallback error
    );

    void listFaculty(
        repositories::TimetableRepository::FacultyCallback success,
        repositories::TimetableRepository::ErrorCallback error
    );

    void listEntries(
        repositories::TimetableRepository::EntryCallback success,
        repositories::TimetableRepository::ErrorCallback error
    );

    void createRoom(
        const std::string &building,
        const std::string &roomNumber,
        int capacity,
        const std::string &roomType,
        repositories::TimetableRepository::IdCallback success,
        repositories::TimetableRepository::ErrorCallback error
    );

    void createEntry(
        std::int64_t sectionId,
        std::int64_t courseId,
        std::int64_t facultyUserId,
        std::int64_t roomId,
        int dayOfWeek,
        const std::string &startTime,
        const std::string &endTime,
        repositories::TimetableRepository::IdCallback success,
        repositories::TimetableRepository::ErrorCallback error
    );

private:
    std::shared_ptr<repositories::TimetableRepository> repository_;
};

}