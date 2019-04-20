#ifndef PRODUCTION_HPP
#define PRODUCTION_HPP

#include "Token.hpp"

class Production;
extern std::ostream &operator<<(std::ostream &os, Production &p);

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

#endif /* PRODUCTION_HPP */
