#ifndef PRODUCTION_HPP
#define PRODUCTION_HPP

#include "Token.hpp"

extern std::vector<std::string> split(const std::string &, const std::string &); 
extern std::string& trim(std::string &);

class Production {
public:
    Production(const std::string &left, const std::vector<std::string> &right) :
            left(left), right(right) { }
    const std::string &get_left() const ;
    const std::vector<std::string> &get_right() const ;

private:
    std::string left;
    std::vector<std::string> right;
};

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

#endif /* PRODUCTION_HPP */
