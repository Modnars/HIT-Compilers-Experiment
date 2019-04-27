#include <iomanip>

#include "lib/Code.hpp"

std::ostream &operator<<(std::ostream &os, const Code &item) {
    os << std::right << "[" << std::setw(10) << (item.action + ",") << std::setw(10) 
       << (item.arg1 + ",") << std::setw(10) << (item.arg2 + ",") << std::setw(10) 
       << item.result << "]";
    return os;
}
