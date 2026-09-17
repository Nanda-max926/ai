
#include "repositories/UserRepository.h"

#include <drogon/drogon.h>

#include <utility>

using namespace drogon;

namespace universe::repositories
{

void UserRepository::findByLoginId(
    const std::string &loginId,
    SuccessCallback onSuccess,
    ErrorCallback onError
)
{
    auto database = app().getDbClient("default");

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
        "WHERE login_id = $1 OR email = $2 "
        "LIMIT 1",

        std::move(onSuccess),
        std::move(onError),

        loginId,
        loginId
    );
}

}
