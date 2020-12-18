#pragma once

#include <algorithm>
#include <string>

inline bool str_is_digit(const std::string& str)
{
    if (str.empty() || !std::all_of(str.begin(), str.end(), ::isdigit))
    {
        return false;
    }
    return true;
}
