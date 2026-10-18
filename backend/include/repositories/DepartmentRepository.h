#pragma once

#include "repositories/IDepartmentRepository.h"

namespace universe::repositories
{

class DepartmentRepository final : public IDepartmentRepository
{
public:
    void listAll(
        ListCallback success,
        ErrorCallback error
    ) override;

    void create(
        const std::string &code,
        const std::string &name,
        const std::string &description,
        const std::string &location,
        const std::string &email,
        const std::string &phone,
        MutationCallback success,
        ErrorCallback error
    ) override;

    void update(
        long long id,
        const std::string &code,
        const std::string &name,
        const std::string &description,
        const std::string &location,
        const std::string &email,
        const std::string &phone,
        MutationCallback success,
        ErrorCallback error
    ) override;

    void setActive(
        long long id,
        bool active,
        MutationCallback success,
        ErrorCallback error
    ) override;

    void assignHod(
        long long departmentId,
        long long facultyUserId,
        MutationCallback success,
        ErrorCallback error
    ) override;
};

}