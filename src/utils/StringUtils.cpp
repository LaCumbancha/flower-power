#include "StringUtils.h"

#include <algorithm>

string StringUtils::uppercase(string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}
