#pragma once

#include <string>
#include <utility>

namespace universe::domain
{

class Department final
{
public:
    Department(
        long long id,
        std::string code,
        std::string name,
        bool active,
        std::string description,
        std::string officeLocation,
        std::string contactEmail,
        std::string contactPhone,
        long long hodUserId,
        std::string updatedAt
    )
        : id_(id),
          code_(std::move(code)),
          name_(std::move(name)),
          active_(active),
          description_(std::move(description)),
          officeLocation_(std::move(officeLocation)),
          contactEmail_(std::move(contactEmail)),
          contactPhone_(std::move(contactPhone)),
          hodUserId_(hodUserId),
          updatedAt_(std::move(updatedAt))
    {
    }

    long long id() const { return id_; }
    const std::string &code() const { return code_; }
    const std::string &name() const { return name_; }
    bool isActive() const { return active_; }
    const std::string &description() const { return description_; }
    const std::string &officeLocation() const { return officeLocation_; }
    const std::string &contactEmail() const { return contactEmail_; }
    const std::string &contactPhone() const { return contactPhone_; }
    long long hodUserId() const { return hodUserId_; }
    const std::string &updatedAt() const { return updatedAt_; }

private:
    long long id_;
    std::string code_;
    std::string name_;
    bool active_;
    std::string description_;
    std::string officeLocation_;
    std::string contactEmail_;
    std::string contactPhone_;
    long long hodUserId_;
    std::string updatedAt_;
};

}