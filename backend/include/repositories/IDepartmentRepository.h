#pragma once

#include "domain/Department.h"

#include <functional>
#include <string>
#include <vector>

namespace universe::repositories
{

class IDepartmentRepository
{
public:
    using ListCallback =
        std::function<void(std::vector<domain::Department>)>;

    using MutationCallback =
        std::function<void(long long)>;

    using ErrorCallback =
        std::function<void(const std::string &)>;

    virtual ~IDepartmentRepository() = default;

    virtual void listAll(
        ListCallback success,
        ErrorCallback error
    ) = 0;

    virtual void create(
        const std::string &code,
        const std::string &name,
        const std::string &description,
        const std::string &location,
        const std::string &email,
        const std::string &phone,
        MutationCallback success,
        ErrorCallback error
    ) = 0;

    virtual void update(
        long long id,
        const std::string &code,
        const std::string &name,
        const std::string &description,
        const std::string &location,
        const std::string &email,
        const std::string &phone,
        MutationCallback success,
        ErrorCallback error
    ) = 0;

    virtual void setActive(
        long long id,
        bool active,
        MutationCallback success,
        ErrorCallback error
    ) = 0;

    virtual void assignHod(
        long long departmentId,
        long long facultyUserId,
        MutationCallback success,
        ErrorCallback error
    ) = 0;
};

}