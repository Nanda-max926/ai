
#pragma once

#include "domain/User.h"

#include <string>

namespace universe::services
{

class AuthService
{
public:

    static bool authenticate(
        const domain::User &user,
        const std::string &password,
        const std::string &requestedRole,
        std::string &errorMessage
    );

    static bool isValidRole(
        const std::string &role
    );
};

}
