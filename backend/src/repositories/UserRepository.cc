
#include "repositories/UserRepository.h"

#include <drogon/drogon.h>
#include <utility>

namespace universe::repositories
{

void UserRepository::findByLoginId(
    const std::string &loginId,
    SuccessCallback onSuccess,
    ErrorCallback onError
)
{
    // Access the configured PostgreSQL client.
    auto database = drogon::app().getDbClient("default");

    // Find the account using a parameterized SQL query.
    database->execSqlAsync(

        "SELECT "
        "id, "
        "login_id, "
        "email, "
        "password_hash, "
        "role::text AS role, "
        "status::text AS status, "
        "must_change_password "
        "FROM users "
        "WHERE LOWER(login_id) = LOWER($1) "
        "OR LOWER(email) = LOWER($1) "
        "LIMIT 1",

        // SUCCESS CALLBACK
        [onSuccess = std::move(onSuccess)]
        (const drogon::orm::Result &result) mutable
        {
            if (result.empty())
            {
                onSuccess(std::nullopt);
                return;
            }

            const auto &row = result[0];

            domain::User user;

            user.id = row["id"].as<long long>();

            user.loginId =
                row["login_id"].as<std::string>();

            user.email =
                row["email"].as<std::string>();

            user.passwordHash =
                row["password_hash"].as<std::string>();

            user.role =
                row["role"].as<std::string>();

            user.status =
                row["status"].as<std::string>();

            user.mustChangePassword =
                row["must_change_password"].as<bool>();

            onSuccess(std::move(user));
        },

        // ERROR CALLBACK
        [onError = std::move(onError)]
        (const drogon::orm::DrogonDbException &error) mutable
        {
            onError(error);
        },

        loginId
    );
}

}
