
#include "services/PasswordService.h"

#include <sodium.h>

namespace universe::services
{

bool PasswordService::initialize()
{
    return sodium_init() >= 0;
}

bool PasswordService::hashPassword(
    const std::string &password,
    std::string &outputHash
)
{
    if (password.size() < 12 ||
        password.size() > 1024)
    {
        return false;
    }

    char hash[crypto_pwhash_STRBYTES] = {};

    const int result = crypto_pwhash_str(
        hash,
        password.c_str(),
        static_cast<unsigned long long>(password.size()),
        crypto_pwhash_OPSLIMIT_INTERACTIVE,
        crypto_pwhash_MEMLIMIT_INTERACTIVE
    );

    if (result != 0)
    {
        sodium_memzero(hash, sizeof(hash));
        return false;
    }

    outputHash = hash;

    sodium_memzero(hash, sizeof(hash));

    return true;
}

bool PasswordService::verifyPassword(
    const std::string &password,
    const std::string &storedHash
)
{
    if (password.empty() ||
        password.size() > 1024 ||
        storedHash.empty() ||
        storedHash.size() >= crypto_pwhash_STRBYTES)
    {
        return false;
    }

    return crypto_pwhash_str_verify(
        storedHash.c_str(),
        password.c_str(),
        static_cast<unsigned long long>(password.size())
    ) == 0;
}

}
