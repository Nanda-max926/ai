#pragma once

#include "domain/AcademicModels.h"

#include <drogon/drogon.h>

#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace universe::repositories
{

class AcademicRepository
{
public:
    using ProgramCallback =
        std::function<void(std::vector<domain::AcademicProgram>)>;

    using CourseCallback =
        std::function<void(std::vector<domain::AcademicCourse>)>;

    using SectionCallback =
        std::function<void(std::vector<domain::AcademicSection>)>;

    using IdCallback =
        std::function<void(std::int64_t)>;

    using ErrorCallback =
        std::function<void(const drogon::orm::DrogonDbException &)>;

    explicit AcademicRepository(
        std::shared_ptr<drogon::orm::DbClient> database
    );

    // READ
    void listPrograms(ProgramCallback success, ErrorCallback error);
    void listCourses(CourseCallback success, ErrorCallback error);
    void listSections(SectionCallback success, ErrorCallback error);

    // CREATE
    void createProgram(
        std::int64_t departmentId,
        const std::string &code,
        const std::string &name,
        const std::string &degreeLevel,
        int durationYears,
        int totalSemesters,
        const std::string &description,
        IdCallback success,
        ErrorCallback error
    );

    void createCourse(
        std::int64_t departmentId,
        const std::string &code,
        const std::string &title,
        const std::string &description,
        double credits,
        const std::string &courseType,
        IdCallback success,
        ErrorCallback error
    );

    void createSection(
        std::int64_t programId,
        int batchYear,
        int academicYearStart,
        int semesterNumber,
        const std::string &sectionCode,
        int capacity,
        IdCallback success,
        ErrorCallback error
    );

private:
    // Only internal database connection is private.
    std::shared_ptr<drogon::orm::DbClient> database_;
};

}