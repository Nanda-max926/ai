
#include "services/AccountProvisioningService.h"

#include <drogon/drogon.h>

#include <utility>

using namespace drogon;

namespace universe::services
{

void AccountProvisioningService::createStudent(
    const std::string &email,
    const std::string &passwordHash,
    const std::string &fullName,
    std::int64_t departmentId,
    int semester,
    int batchYear,
    const std::string &section,
    SuccessCallback onSuccess,
    ErrorCallback onError
)
{
    auto database = app().getDbClient("default");

    const std::string query = R"SQL(
        WITH generated AS (
            SELECT
               'AP' ||
               RIGHT($6::integer::text, 2) ||
                  LPAD(
                    NEXTVAL('student_registration_seq')::text,
                        7,
                      '0'
                    ) AS login_id
        ),
        new_user AS (
            INSERT INTO users (
                login_id,
                email,
                password_hash,
                role,
                status,
                must_change_password,
                temporary_password_expires_at
            )
                  SELECT
            generated.login_id,
            $1,
            $2,
            'student'::app_role,
            'active'::account_status,
            TRUE,
            CURRENT_TIMESTAMP + INTERVAL '24 hours'
        FROM generated
            RETURNING id, login_id
        ),
        new_profile AS (
            INSERT INTO student_profiles (
                user_id,
                full_name,
                department_id,
                semester,
                batch_year,
                section
            )
            SELECT
                id,
                $3,
                $4,
                $5::integer,
                $6,
                $7
            FROM new_user
            RETURNING user_id
        )
        SELECT
            new_user.id AS user_id,
            new_user.login_id
        FROM new_user
        JOIN new_profile
            ON new_profile.user_id = new_user.id
    )SQL";

    database->execSqlAsync(
        query,

        [onSuccess = std::move(onSuccess)]
        (const orm::Result &result) mutable
        {
            if (result.empty())
                return;

            long long userId =
                result[0]["user_id"].as<long long>();

            std::string loginId =
                result[0]["login_id"].as<std::string>();

            onSuccess(userId, std::move(loginId));
        },

        [onError = std::move(onError)]
        (const orm::DrogonDbException &error) mutable
        {
            onError(error);
        },

        email,
        passwordHash,
        fullName,
        departmentId,
        semester,
        batchYear,
        section
    );
}

void AccountProvisioningService::createFaculty(
    const std::string &email,
    const std::string &passwordHash,
    const std::string &fullName,
    std::int64_t departmentId,
    const std::string &designation,
    SuccessCallback onSuccess,
    ErrorCallback onError
)
{
    auto database = app().getDbClient("default");

    const std::string query = R"SQL(
        WITH generated AS (
            SELECT
                'AF' ||
                    TO_CHAR(CURRENT_DATE, 'YY') ||
                    LPAD(
                         NEXTVAL('faculty_employee_seq')::text,
                             7,
                                  '0'
                        ) AS login_id 
        ),
        new_user AS (
            INSERT INTO users (
                login_id,
                email,
                password_hash,
                role,
                status,
                must_change_password
            )
            SELECT
                generated.login_id,
                $1,
                $2,
                'faculty'::app_role,
                'active'::account_status,
                TRUE
            FROM generated
            RETURNING id, login_id
        ),
        new_profile AS (
            INSERT INTO faculty_profiles (
                user_id,
                full_name,
                department_id,
                designation
            )
            SELECT
                id,
                $3,
                $4,
                $5
            FROM new_user
            RETURNING user_id
        )
        SELECT
            new_user.id AS user_id,
            new_user.login_id
        FROM new_user
        JOIN new_profile
            ON new_profile.user_id = new_user.id
    )SQL";

    database->execSqlAsync(
        query,

        [onSuccess = std::move(onSuccess)]
        (const orm::Result &result) mutable
        {
            if (result.empty())
                return;

            long long userId =
                result[0]["user_id"].as<long long>();

            std::string loginId =
                result[0]["login_id"].as<std::string>();

            onSuccess(userId, std::move(loginId));
        },

        [onError = std::move(onError)]
        (const orm::DrogonDbException &error) mutable
        {
            onError(error);
        },

        email,
        passwordHash,
        fullName,
        departmentId,
        designation
    );
}

}
