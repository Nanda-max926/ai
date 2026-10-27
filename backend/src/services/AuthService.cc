
#include "services/AuthService.h"
#include "services/PasswordService.h"

namespace universe::services
{

bool AuthService::isValidRole(
    const std::string &role
)
{
    return role == "student" ||
           role == "faculty" ||
           role == "admin" ||
           role == "coordinator";
}

bool AuthService::authenticate(
    const domain::User &user,
    const std::string &password,
    const std::string &requestedRole,
    std::string &errorMessage
)
{
    // Check whether account is active.
    if (user.status != "active")
    {
        errorMessage =
            "Account is not active. Contact administration.";

        return false;
    }

    // Check role against the database.
    if (user.role != requestedRole)
    {
        errorMessage =
            "Invalid login ID or password.";

        return false;
    }

    // Verify password hash.
    if (!PasswordService::verifyPassword(
            password,
            user.passwordHash
        ))
    {
        errorMessage =
            "Invalid login ID or password.";

        return false;
    }

    errorMessage.clear();

    return true;
}

}
