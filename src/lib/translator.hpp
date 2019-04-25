#ifndef TRANSLATOR_HPP
#define TRANSLATOR_HPP

#include <stack>

#include "ASTNode.hpp"

extern std::shared_ptr<Node> Root;

extern void semantic(int);
extern void translate(std::ostream &os = std::cout);

#endif /* TRANSLATOR_HPP */
