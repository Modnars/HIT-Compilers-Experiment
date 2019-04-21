#include "lib/Production.hpp"

std::ostream &operator<<(std::ostream &os, const Production &p) {
    os << p.left << " -> ";
    for (auto str : p.rights) 
        os << str << " ";
    return os;
}
