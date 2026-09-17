
#pragma once

#include <string>

namespace universe::services
{

class PasswordService
{
public:

    static bool initialize();

    static bool hashPassword(
        const std::string &password,
        std::string &outputHash
    );

    static bool verifyPassword(
        const std::string &password,
        const std::string &storedHash
    );
};

}
