#include "repositories/DepartmentRepository.h"

#include <drogon/drogon.h>

#include <utility>

namespace universe::repositories
{

void DepartmentRepository::listAll(
    ListCallback success,
    ErrorCallback error
)
{
    auto database = drogon::app().getDbClient("default");

    database->execSqlAsync(
        "SELECT id, code, name, is_active, "
        "COALESCE(description, '') AS description, "
        "COALESCE(office_location, '') AS office_location, "
        "COALESCE(contact_email, '') AS contact_email, "
        "COALESCE(contact_phone, '') AS contact_phone, "
        "COALESCE(hod_user_id, 0) AS hod_user_id, "
        "TO_CHAR(updated_at, 'YYYY-MM-DD HH24:MI:SS') AS updated_at "
        "FROM departments ORDER BY name",

        [success = std::move(success)]
        (const drogon::orm::Result &result) mutable
        {
            std::vector<domain::Department> departments;

            for (const auto &row : result)
            {
                departments.emplace_back(
                    row["id"].as<long long>(),
                    row["code"].as<std::string>(),
                    row["name"].as<std::string>(),
                    row["is_active"].as<bool>(),
                    row["description"].as<std::string>(),
                    row["office_location"].as<std::string>(),
                    row["contact_email"].as<std::string>(),
                    row["contact_phone"].as<std::string>(),
                    row["hod_user_id"].as<long long>(),
                    row["updated_at"].as<std::string>()
                );
            }

            success(std::move(departments));
        },

        [error = std::move(error)]
        (const drogon::orm::DrogonDbException &exception) mutable
        {
            error(exception.base().what());
        }
    );
}

void DepartmentRepository::create(
    const std::string &code,
    const std::string &name,
    const std::string &description,
    const std::string &location,
    const std::string &email,
    const std::string &phone,
    MutationCallback success,
    ErrorCallback error
)
{
    auto database = drogon::app().getDbClient("default");

    database->execSqlAsync(
        "INSERT INTO departments "
        "(code, name, description, office_location, contact_email, contact_phone) "
        "VALUES ($1, $2, NULLIF($3, ''), NULLIF($4, ''), "
        "NULLIF($5, ''), NULLIF($6, '')) RETURNING id",

        [success = std::move(success)]
        (const drogon::orm::Result &result) mutable
        {
            success(
                result.empty()
                    ? 0
                    : result[0]["id"].as<long long>()
            );
        },

        [error = std::move(error)]
        (const drogon::orm::DrogonDbException &exception) mutable
        {
            error(exception.base().what());
        },

        code, name, description, location, email, phone
    );
}

void DepartmentRepository::update(
    long long id,
    const std::string &code,
    const std::string &name,
    const std::string &description,
    const std::string &location,
    const std::string &email,
    const std::string &phone,
    MutationCallback success,
    ErrorCallback error
)
{
    auto database = drogon::app().getDbClient("default");

    database->execSqlAsync(
        "UPDATE departments SET "
        "code = $2, name = $3, "
        "description = NULLIF($4, ''), "
        "office_location = NULLIF($5, ''), "
        "contact_email = NULLIF($6, ''), "
        "contact_phone = NULLIF($7, ''), "
        "updated_at = CURRENT_TIMESTAMP "
        "WHERE id = $1 RETURNING id",

        [success = std::move(success)]
        (const drogon::orm::Result &result) mutable
        {
            success(
                result.empty()
                    ? 0
                    : result[0]["id"].as<long long>()
            );
        },

        [error = std::move(error)]
        (const drogon::orm::DrogonDbException &exception) mutable
        {
            error(exception.base().what());
        },

        id, code, name, description, location, email, phone
    );
}

void DepartmentRepository::setActive(
    long long id,
    bool active,
    MutationCallback success,
    ErrorCallback error
)
{
    auto database = drogon::app().getDbClient("default");

    database->execSqlAsync(
        "UPDATE departments SET "
        "is_active = $2, updated_at = CURRENT_TIMESTAMP "
        "WHERE id = $1 RETURNING id",

        [success = std::move(success)]
        (const drogon::orm::Result &result) mutable
        {
            success(
                result.empty()
                    ? 0
                    : result[0]["id"].as<long long>()
            );
        },

        [error = std::move(error)]
        (const drogon::orm::DrogonDbException &exception) mutable
        {
            error(exception.base().what());
        },

        id, active
    );
}

void DepartmentRepository::assignHod(
    long long departmentId,
    long long facultyUserId,
    MutationCallback success,
    ErrorCallback error
)
{
    auto database = drogon::app().getDbClient("default");

    database->execSqlAsync(
        "UPDATE departments d SET "
        "hod_user_id = $2, updated_at = CURRENT_TIMESTAMP "
        "WHERE d.id = $1 "
        "AND EXISTS ("
        "SELECT 1 FROM faculty_profiles f "
        "WHERE f.user_id = $2 AND f.department_id = $1"
        ") RETURNING d.id",

        [success = std::move(success)]
        (const drogon::orm::Result &result) mutable
        {
            success(
                result.empty()
                    ? 0
                    : result[0]["id"].as<long long>()
            );
        },

        [error = std::move(error)]
        (const drogon::orm::DrogonDbException &exception) mutable
        {
            error(exception.base().what());
        },

        departmentId, facultyUserId
    );
}

}