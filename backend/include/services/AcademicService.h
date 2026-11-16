#pragma once

#include "repositories/AcademicRepository.h"
#include <cstdint>
#include <string>
#include <memory>

namespace universe::services
{

class AcademicService
{
public:
    explicit AcademicService(
        std::shared_ptr<repositories::AcademicRepository> repository
    );

    void listPrograms(
        repositories::AcademicRepository::ProgramCallback success,
        repositories::AcademicRepository::ErrorCallback error
    );

    void listCourses(
        repositories::AcademicRepository::CourseCallback success,
        repositories::AcademicRepository::ErrorCallback error
    );

    void listSections(
        repositories::AcademicRepository::SectionCallback success,
        repositories::AcademicRepository::ErrorCallback error
    );

    void createProgram(
    std::int64_t departmentId,
    const std::string &code,
    const std::string &name,
    const std::string &degreeLevel,
    int durationYears,
    int totalSemesters,
    const std::string &description,
    repositories::AcademicRepository::IdCallback success,
    repositories::AcademicRepository::ErrorCallback error
);

void createCourse(
    std::int64_t departmentId,
    const std::string &code,
    const std::string &title,
    const std::string &description,
    double credits,
    const std::string &courseType,
    repositories::AcademicRepository::IdCallback success,
    repositories::AcademicRepository::ErrorCallback error
);

void createSection(
    std::int64_t programId,
    int batchYear,
    int academicYearStart,
    int semesterNumber,
    const std::string &sectionCode,
    int capacity,
    repositories::AcademicRepository::IdCallback success,
    repositories::AcademicRepository::ErrorCallback error
);

private:
    std::shared_ptr<repositories::AcademicRepository> repository_;
};

}