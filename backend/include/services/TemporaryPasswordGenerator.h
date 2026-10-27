#pragma once

#include <string>

namespace universe::services
{

class TemporaryPasswordGenerator final
{
public:
    bool generate(std::string &output) const;
};

}