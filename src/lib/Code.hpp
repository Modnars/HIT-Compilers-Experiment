#ifndef CODE_HPP
#define CODE_HPP

#include <iostream>
#include <string>

class Code {
public:
    std::string action;
    std::string arg1;
    std::string arg2;
    std::string result;
};

extern std::ostream &operator<<(std::ostream &os, const Code &item);

#endif /* CODE_HPP */
