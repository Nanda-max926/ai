#include "services/AcademicService.h"
#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <utility>

namespace universe::services
{

AcademicService::AcademicService(
    std::shared_ptr<repositories::AcademicRepository> repository
)
    : repository_(std::move(repository))
{
    if (!repository_)
    {
        throw std::invalid_argument(
            "AcademicService requires an AcademicRepository"
        );
    }
}

void AcademicService::listPrograms(
    repositories::AcademicRepository::ProgramCallback success,
    repositories::AcademicRepository::ErrorCallback error
)
{
    repository_->listPrograms(
        std::move(success),
        std::move(error)
    );
}

void AcademicService::listCourses(
    repositories::AcademicRepository::CourseCallback success,
    repositories::AcademicRepository::ErrorCallback error
)
{
    repository_->listCourses(
        std::move(success),
        std::move(error)
    );
}

void AcademicService::listSections(
    repositories::AcademicRepository::SectionCallback success,
    repositories::AcademicRepository::ErrorCallback error
)
{
    repository_->listSections(
        std::move(success),
        std::move(error)
    );
}
 void AcademicService::createProgram(
    std::int64_t departmentId,
    const std::string &code,
    const std::string &name,
    const std::string &degreeLevel,
    int durationYears,
    int totalSemesters,
    const std::string &description,
    repositories::AcademicRepository::IdCallback success,
    repositories::AcademicRepository::ErrorCallback error
)
{
    const bool validDegree =
        degreeLevel == "diploma" ||
        degreeLevel == "undergraduate" ||
        degreeLevel == "postgraduate" ||
        degreeLevel == "doctoral";

    if (departmentId <= 0 ||
        code.empty() || code.size() > 20 ||
        name.empty() || name.size() > 150 ||
        !validDegree ||
        durationYears < 1 || durationYears > 8 ||
        totalSemesters < 1 || totalSemesters > 16)
    {
        throw std::invalid_argument("Invalid academic program details.");
    }

    repository_->createProgram(
        departmentId, code, name, degreeLevel,
        durationYears, totalSemesters, description,
        std::move(success), std::move(error)
    );
}

void AcademicService::createCourse(
    std::int64_t departmentId,
    const std::string &code,
    const std::string &title,
    const std::string &description,
    double credits,
    const std::string &courseType,
    repositories::AcademicRepository::IdCallback success,
    repositories::AcademicRepository::ErrorCallback error
)
{
    const bool validType =
        courseType == "core" ||
        courseType == "elective" ||
        courseType == "laboratory" ||
        courseType == "project" ||
        courseType == "other";

    if (departmentId <= 0 ||
        code.empty() || code.size() > 30 ||
        title.empty() || title.size() > 150 ||
        credits < 0.5 || credits > 20 ||
        !validType)
    {
        throw std::invalid_argument("Invalid academic course details.");
    }

    repository_->createCourse(
        departmentId, code, title, description,
        credits, courseType,
        std::move(success), std::move(error)
    );
}

void AcademicService::createSection(
    std::int64_t programId,
    int batchYear,
    int academicYearStart,
    int semesterNumber,
    const std::string &sectionCode,
    int capacity,
    repositories::AcademicRepository::IdCallback success,
    repositories::AcademicRepository::ErrorCallback error
)
{
    if (programId <= 0 ||
        batchYear < 2000 || batchYear > 2100 ||
        academicYearStart < 2000 || academicYearStart > 2100 ||
        semesterNumber < 1 || semesterNumber > 16 ||
        sectionCode.empty() || sectionCode.size() > 10 ||
        capacity < 1 || capacity > 500)
    {
        throw std::invalid_argument("Invalid academic section details.");
    }

    repository_->createSection(
        programId, batchYear, academicYearStart,
        semesterNumber, sectionCode, capacity,
        std::move(success), std::move(error)
    );
}
}