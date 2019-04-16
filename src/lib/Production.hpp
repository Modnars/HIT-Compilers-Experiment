#ifndef PRODUCTION_HPP
#define PRODUCTION_HPP

#include "Token.hpp"

extern std::vector<std::string> split(const std::string &, const std::string &); 
extern std::string& trim(std::string &);

class Production {
public:
    Production(const std::string &left, const std::vector<std::string> &right) :
            left(left), right(right) { }
    const std::string &get_left();
    const std::vector<std::string> &get_right();

private:
    std::string left;
    std::vector<std::string> right;
};

const std::string &Production::get_left() {
    return left;
}

const std::vector<std::string> &Production::get_right() {
    return right;
}

std::ostream &operator<<(std::ostream &os, Production &p) {
    os << "Left: " << p.get_left() << std::endl << "Right: ";
    os << p.get_right().size() << " ";
    for (auto str : p.get_right()) 
        os << ":" << str << ": ";
    os << std::endl;
    return os;
}

#endif /* PRODUCTION_HPP */
