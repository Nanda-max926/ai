#include "services/TimetableService.h"
#include <cctype>
#include <stdexcept>
#include <utility>

namespace universe::services
{

namespace
{

bool validTime(const std::string &time)
{
    if (time.size() != 5 || time[2] != ':')
        return false;

    if (!std::isdigit(static_cast<unsigned char>(time[0])) ||
        !std::isdigit(static_cast<unsigned char>(time[1])) ||
        !std::isdigit(static_cast<unsigned char>(time[3])) ||
        !std::isdigit(static_cast<unsigned char>(time[4])))
        return false;

    const int hour = std::stoi(time.substr(0, 2));
    const int minute = std::stoi(time.substr(3, 2));

    return hour >= 0 && hour < 24 &&
           minute >= 0 && minute < 60;
}

}

TimetableService::TimetableService(
    std::shared_ptr<repositories::TimetableRepository> repository
)
    : repository_(std::move(repository))
{
    if (!repository_)
        throw std::invalid_argument("TimetableRepository is required.");
}

void TimetableService::listRooms(
    repositories::TimetableRepository::RoomCallback success,
    repositories::TimetableRepository::ErrorCallback error
)
{
    repository_->listRooms(
        std::move(success), std::move(error)
    );
}

void TimetableService::listFaculty(
    repositories::TimetableRepository::FacultyCallback success,
    repositories::TimetableRepository::ErrorCallback error
)
{
    repository_->listFaculty(
        std::move(success), std::move(error)
    );
}

void TimetableService::listEntries(
    repositories::TimetableRepository::EntryCallback success,
    repositories::TimetableRepository::ErrorCallback error
)
{
    repository_->listEntries(
        std::move(success), std::move(error)
    );
}

void TimetableService::createRoom(
    const std::string &building,
    const std::string &roomNumber,
    int capacity,
    const std::string &roomType,
    repositories::TimetableRepository::IdCallback success,
    repositories::TimetableRepository::ErrorCallback error
)
{
    const bool validType =
        roomType == "classroom" ||
        roomType == "laboratory" ||
        roomType == "auditorium" ||
        roomType == "other";

    if (building.empty() || building.size() > 80 ||
        roomNumber.empty() || roomNumber.size() > 30 ||
        capacity < 1 || capacity > 500 ||
        !validType)
    {
        throw std::invalid_argument("Invalid room details.");
    }

    repository_->createRoom(
        building,
        roomNumber,
        capacity,
        roomType,
        std::move(success),
        std::move(error)
    );
}

void TimetableService::createEntry(
    std::int64_t sectionId,
    std::int64_t courseId,
    std::int64_t facultyUserId,
    std::int64_t roomId,
    int dayOfWeek,
    const std::string &startTime,
    const std::string &endTime,
    repositories::TimetableRepository::IdCallback success,
    repositories::TimetableRepository::ErrorCallback error
)
{
    if (sectionId <= 0 || courseId <= 0 ||
        facultyUserId <= 0 || roomId <= 0 ||
        dayOfWeek < 1 || dayOfWeek > 7 ||
        !validTime(startTime) || !validTime(endTime) ||
        startTime >= endTime)
    {
        throw std::invalid_argument("Invalid timetable entry details.");
    }

    repository_->createEntry(
        sectionId,
        courseId,
        facultyUserId,
        roomId,
        dayOfWeek,
        startTime,
        endTime,
        std::move(success),
        std::move(error)
    );
}

}