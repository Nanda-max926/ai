#pragma once

#include "repositories/IDepartmentRepository.h"

#include <memory>

namespace universe::services
{

class DepartmentService final
{
public:
    explicit DepartmentService(
        std::shared_ptr<repositories::IDepartmentRepository> repository
    );

    void list(
        repositories::IDepartmentRepository::ListCallback success,
        repositories::IDepartmentRepository::ErrorCallback error
    );

    void create(
        std::string code,
        std::string name,
        std::string description,
        std::string location,
        std::string email,
        std::string phone,
        repositories::IDepartmentRepository::MutationCallback success,
        repositories::IDepartmentRepository::ErrorCallback error
    );

    void update(
        long long id,
        std::string code,
        std::string name,
        std::string description,
        std::string location,
        std::string email,
        std::string phone,
        repositories::IDepartmentRepository::MutationCallback success,
        repositories::IDepartmentRepository::ErrorCallback error
    );

    void setActive(
        long long id,
        bool active,
        repositories::IDepartmentRepository::MutationCallback success,
        repositories::IDepartmentRepository::ErrorCallback error
    );

    void assignHod(
        long long departmentId,
        long long facultyUserId,
        repositories::IDepartmentRepository::MutationCallback success,
        repositories::IDepartmentRepository::ErrorCallback error
    );

private:
    std::shared_ptr<repositories::IDepartmentRepository> repository_;
};

}