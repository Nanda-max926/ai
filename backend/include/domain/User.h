
#pragma once

#include <string>

namespace universe::domain
{

struct User
{
    long long id = 0;

    std::string loginId;
    std::string email;

    std::string passwordHash;

    std::string role;
    std::string status;

    bool mustChangePassword = true;
};

}
