
#pragma once

#include <drogon/orm/DbClient.h>

#include <cstdint>
#include <functional>
#include <string>

namespace universe::services
{

class AccountProvisioningService
{
public:

    using SuccessCallback =
        std::function<void(long long, std::string)>;

    using ErrorCallback =
        std::function<void(
            const drogon::orm::DrogonDbException &
        )>;

    static void createStudent(
        const std::string &email,
        const std::string &passwordHash,
        const std::string &fullName,
        std::int64_t departmentId,
        int semester,
        int batchYear,
        const std::string &section,
        SuccessCallback onSuccess,
        ErrorCallback onError
    );

    static void createFaculty(
        const std::string &email,
        const std::string &passwordHash,
        const std::string &fullName,
        std::int64_t departmentId,
        const std::string &designation,
        SuccessCallback onSuccess,
        ErrorCallback onError
    );
};

}
