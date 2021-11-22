#include "utils.hpp"

bool str_is_digit(const std::string &tmp)
{
    for (int i = 0; i < tmp.size(); i++)
        if (!std::isdigit(tmp[i]))
            return false;
    return true;
}