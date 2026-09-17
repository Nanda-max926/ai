
#pragma once

#include <drogon/orm/DbClient.h>

#include <functional>
#include <string>

namespace universe::repositories
{

class UserRepository
{
public:

    using SuccessCallback =
        std::function<void(const drogon::orm::Result &)>;

    using ErrorCallback =
        std::function<void(const drogon::orm::DrogonDbException &)>;

    static void findByLoginId(
        const std::string &loginId,
        SuccessCallback onSuccess,
        ErrorCallback onError
    );
};

}
