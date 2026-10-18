#include <cstdint>
#include "repositories/AcademicRepository.h"

#include <stdexcept>
#include <utility>

namespace universe::repositories
{

AcademicRepository::AcademicRepository(
    std::shared_ptr<drogon::orm::DbClient> database
)
    : database_(std::move(database))
{
    if (!database_)
    {
        throw std::invalid_argument(
            "AcademicRepository requires a database client"
        );
    }
}

void AcademicRepository::listPrograms(
    ProgramCallback success,
    ErrorCallback error
)
{
    database_->execSqlAsync(
        "SELECT id, department_id, code, name, degree_level, "
        "duration_years, total_semesters, description, is_active "
        "FROM academic_programs ORDER BY name",

        [success = std::move(success)](
            const drogon::orm::Result &result
        )
        {
            std::vector<domain::AcademicProgram> programs;

            for (const auto &row : result)
            {
                domain::AcademicProgram item;

                item.id = row["id"].as<std::int64_t>();
                item.departmentId =
                    row["department_id"].as<std::int64_t>();
                item.code = row["code"].as<std::string>();
                item.name = row["name"].as<std::string>();
                item.degreeLevel =
                    row["degree_level"].as<std::string>();
                item.durationYears =
                    row["duration_years"].as<int>();
                item.totalSemesters =
                    row["total_semesters"].as<int>();
                item.description =
                    row["description"].as<std::string>();
                item.isActive = row["is_active"].as<bool>();

                programs.push_back(std::move(item));
            }

            success(std::move(programs));
        },

        [error = std::move(error)](
            const drogon::orm::DrogonDbException &exception
        )
        {
            error(exception);
        }
    );
}

void AcademicRepository::listCourses(
    CourseCallback success,
    ErrorCallback error
)
{
    database_->execSqlAsync(
        "SELECT id, department_id, code, title, description, "
        "credits, course_type, is_active "
        "FROM academic_courses ORDER BY title",

        [success = std::move(success)](
            const drogon::orm::Result &result
        )
        {
            std::vector<domain::AcademicCourse> courses;

            for (const auto &row : result)
            {
                domain::AcademicCourse item;

                item.id = row["id"].as<std::int64_t>();
                item.departmentId =
                    row["department_id"].as<std::int64_t>();
                item.code = row["code"].as<std::string>();
                item.title = row["title"].as<std::string>();
                item.description =
                    row["description"].as<std::string>();
                item.credits = row["credits"].as<double>();
                item.courseType =
                    row["course_type"].as<std::string>();
                item.isActive = row["is_active"].as<bool>();

                courses.push_back(std::move(item));
            }

            success(std::move(courses));
        },

        [error = std::move(error)](
            const drogon::orm::DrogonDbException &exception
        )
        {
            error(exception);
        }
    );
}

void AcademicRepository::listSections(
    SectionCallback success,
    ErrorCallback error
)
{
    database_->execSqlAsync(
        "SELECT id, program_id, batch_year, academic_year_start, "
        "semester_number, section_code, capacity, is_active "
        "FROM academic_sections ORDER BY batch_year DESC, "
        "semester_number, section_code",

        [success = std::move(success)](
            const drogon::orm::Result &result
        )
        {
            std::vector<domain::AcademicSection> sections;

            for (const auto &row : result)
            {
                domain::AcademicSection item;

                item.id = row["id"].as<std::int64_t>();
                item.programId =
                    row["program_id"].as<std::int64_t>();
                item.batchYear = row["batch_year"].as<int>();
                item.academicYearStart =
                    row["academic_year_start"].as<int>();
                item.semesterNumber =
                    row["semester_number"].as<int>();
                item.sectionCode =
                    row["section_code"].as<std::string>();
                item.capacity = row["capacity"].as<int>();
                item.isActive = row["is_active"].as<bool>();

                sections.push_back(std::move(item));
            }

            success(std::move(sections));
        },

        [error = std::move(error)](
            const drogon::orm::DrogonDbException &exception
        )
        {
            error(exception);
        }
    );
}
        void AcademicRepository::createProgram(
    std::int64_t departmentId,
    const std::string &code,
    const std::string &name,
    const std::string &degreeLevel,
    int durationYears,
    int totalSemesters,
    const std::string &description,
    IdCallback success,
    ErrorCallback error
)
{
    database_->execSqlAsync(
        "INSERT INTO academic_programs "
        "(department_id, code, name, degree_level, duration_years, "
        "total_semesters, description) "
        "VALUES ($1, $2, $3, $4, $5, $6, $7) RETURNING id",

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

        departmentId, code, name, degreeLevel,
        static_cast<std::int16_t>(durationYears),
        static_cast<std::int16_t>(totalSemesters),
        description
    );
}

void AcademicRepository::createCourse(
    std::int64_t departmentId,
    const std::string &code,
    const std::string &title,
    const std::string &description,
    double credits,
    const std::string &courseType,
    IdCallback success,
    ErrorCallback error
)
{
    const std::string creditsText = std::to_string(credits);

    database_->execSqlAsync(
        "INSERT INTO academic_courses "
        "(department_id, code, title, description, credits, course_type) "
        "VALUES ($1, $2, $3, $4, $5::numeric, $6) RETURNING id",

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

        departmentId, code, title, description, creditsText, courseType
    );
}

void AcademicRepository::createSection(
    std::int64_t programId,
    int batchYear,
    int academicYearStart,
    int semesterNumber,
    const std::string &sectionCode,
    int capacity,
    IdCallback success,
    ErrorCallback error
)
{
    database_->execSqlAsync(
        "INSERT INTO academic_sections "
        "(program_id, batch_year, academic_year_start, "
        "semester_number, section_code, capacity) "
        "VALUES ($1, $2, $3, $4, $5, $6) RETURNING id",

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

        programId, batchYear, academicYearStart,
        static_cast<std::int16_t>(semesterNumber),
        sectionCode,
        static_cast<std::int16_t>(capacity)
    );
}
}