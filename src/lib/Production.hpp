#ifndef PRODUCTION_HPP
#define PRODUCTION_HPP

#include "Token.hpp"

class Production;
extern std::ostream &operator<<(std::ostream &os, const Production &p);

class Production {
public:
    Production(const std::string &left, const std::vector<std::string> &rights) :
            left(left), rights(rights) { }

    const std::string left;
    const std::vector<std::string> rights;
};

#endif /* PRODUCTION_HPP */
