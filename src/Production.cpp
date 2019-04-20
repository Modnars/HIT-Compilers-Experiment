#include "lib/Production.hpp"

const std::string &Production::get_left() const {
    return left;
}

const std::vector<std::string> &Production::get_right() const {
    return right;
}

std::ostream &operator<<(std::ostream &os, Production &p) {
    os << p.get_left() << " -> ";
    for (auto str : p.get_right()) 
        os << str << " ";
    return os;
}
