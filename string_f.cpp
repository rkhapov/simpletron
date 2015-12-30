#include <sstream>

#include "string_f.h"

std::string smpl::funcs::intTo16String(int n)
{
    std::stringstream ss;
    ss << std::hex << std::showbase << n;
    return ss.str();
}


int smpl::funcs::string16ToInt(const std::string &string)
{
    std::stringstream ss;
    ss << string;
    int n;
    ss >> std::hex >> n;
    return n;
}

