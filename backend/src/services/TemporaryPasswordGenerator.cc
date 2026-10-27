#include "services/TemporaryPasswordGenerator.h"

#include <sodium.h>

#include <cstddef>
#include <cstdint>
#include <string>

namespace universe::services
{

bool TemporaryPasswordGenerator::generate(
    std::string &output
) const
{
    constexpr char alphabet[] =
        "ABCDEFGHJKLMNPQRSTUVWXYZ"
        "abcdefghijkmnopqrstuvwxyz"
        "23456789";

    constexpr std::size_t passwordLength = 6;
    constexpr std::uint32_t alphabetSize =
        sizeof(alphabet) - 1;

    output.clear();

    if (sodium_init() < 0)
        return false;

    output.reserve(passwordLength);

    for (std::size_t i = 0; i < passwordLength; ++i)
    {
        const auto index =
            randombytes_uniform(alphabetSize);

        output.push_back(alphabet[index]);
    }

    return true;
}

}