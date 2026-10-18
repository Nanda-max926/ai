#pragma once

#include <cstdint>
#include <string>

namespace universe::domain
{

struct AcademicRoom
{
    std::int64_t id = 0;
    std::string building;
    std::string roomNumber;
    int capacity = 60;
    std::string roomType;
    bool isActive = true;
};

struct FacultyOption
{
    std::int64_t id = 0;
    std::string loginId;
    std::string email;
};

struct TimetableEntry
{
    std::int64_t id = 0;
    std::int64_t sectionId = 0;
    std::int64_t courseId = 0;
    std::int64_t facultyUserId = 0;
    std::int64_t roomId = 0;

    int dayOfWeek = 1;
    std::string startTime;
    std::string endTime;

    std::string sectionCode;
    int batchYear = 0;

    std::string courseCode;
    std::string courseTitle;

    std::string facultyLoginId;
    std::string roomLabel;
};

}