#include "services/DepartmentService.h"

#include <algorithm>
#include <cctype>
#include <utility>

namespace universe::services
{

DepartmentService::DepartmentService(
    std::shared_ptr<repositories::IDepartmentRepository> repository
)
    : repository_(std::move(repository))
{
}

void DepartmentService::list(
    repositories::IDepartmentRepository::ListCallback success,
    repositories::IDepartmentRepository::ErrorCallback error
)
{
    repository_->listAll(
        std::move(success),
        std::move(error)
    );
}

void DepartmentService::create(
    std::string code,
    std::string name,
    std::string description,
    std::string location,
    std::string email,
    std::string phone,
    repositories::IDepartmentRepository::MutationCallback success,
    repositories::IDepartmentRepository::ErrorCallback error
)
{
    if (code.empty() || code.size() > 20 ||
        name.empty() || name.size() > 150 ||
        description.size() > 2000 ||
        location.size() > 160 ||
        email.size() > 254 ||
        phone.size() > 30)
    {
        error("Invalid department details.");
        return;
    }

    std::transform(
        code.begin(),
        code.end(),
        code.begin(),
        [](unsigned char c)
        {
            return static_cast<char>(std::toupper(c));
        }
    );

    repository_->create(
        code, name, description, location, email, phone,
        std::move(success), std::move(error)
    );
}

void DepartmentService::update(
    long long id,
    std::string code,
    std::string name,
    std::string description,
    std::string location,
    std::string email,
    std::string phone,
    repositories::IDepartmentRepository::MutationCallback success,
    repositories::IDepartmentRepository::ErrorCallback error
)
{
    if (id <= 0 || code.empty() || code.size() > 20 ||
        name.empty() || name.size() > 150 ||
        description.size() > 2000 ||
        location.size() > 160 ||
        email.size() > 254 ||
        phone.size() > 30)
    {
        error("Invalid department details.");
        return;
    }

    std::transform(
        code.begin(),
        code.end(),
        code.begin(),
        [](unsigned char c)
        {
            return static_cast<char>(std::toupper(c));
        }
    );

    repository_->update(
        id, code, name, description, location, email, phone,
        std::move(success), std::move(error)
    );
}

void DepartmentService::setActive(
    long long id,
    bool active,
    repositories::IDepartmentRepository::MutationCallback success,
    repositories::IDepartmentRepository::ErrorCallback error
)
{
    if (id <= 0)
    {
        error("Invalid department ID.");
        return;
    }

    repository_->setActive(
        id, active, std::move(success), std::move(error)
    );
}

void DepartmentService::assignHod(
    long long departmentId,
    long long facultyUserId,
    repositories::IDepartmentRepository::MutationCallback success,
    repositories::IDepartmentRepository::ErrorCallback error
)
{
    if (departmentId <= 0 || facultyUserId <= 0)
    {
        error("Invalid department or faculty ID.");
        return;
    }

    repository_->assignHod(
        departmentId, facultyUserId,
        std::move(success), std::move(error)
    );
}

}