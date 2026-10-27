
#pragma once

#include "domain/User.h"

#include <drogon/orm/DbClient.h>

#include <functional>
#include <optional>
#include <string>

namespace universe::repositories
{

class UserRepository
{
public:

    using SuccessCallback =
        std::function<void(
            std::optional<domain::User>
        )>;

    using ErrorCallback =
        std::function<void(
            const drogon::orm::DrogonDbException &
        )>;

    static void findByLoginId(
        const std::string &loginId,
        SuccessCallback onSuccess,
        ErrorCallback onError
    );
};

}
