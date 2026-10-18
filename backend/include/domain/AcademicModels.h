#pragma once

#include <cstdint>
#include <string>

namespace universe::domain
{

struct AcademicProgram
{
    std::int64_t id = 0;
    std::int64_t departmentId = 0;
    std::string code;
    std::string name;
    std::string degreeLevel;
    int durationYears = 4;
    int totalSemesters = 8;
    std::string description;
    bool isActive = true;
};

struct AcademicCourse
{
    std::int64_t id = 0;
    std::int64_t departmentId = 0;
    std::string code;
    std::string title;
    std::string description;
    double credits = 0;
    std::string courseType;
    bool isActive = true;
};

struct AcademicSection
{
    std::int64_t id = 0;
    std::int64_t programId = 0;
    int batchYear = 0;
    int academicYearStart = 0;
    int semesterNumber = 1;
    std::string sectionCode;
    int capacity = 60;
    bool isActive = true;
};

}