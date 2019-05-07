#ifndef TRANSLATOR_HPP
#define TRANSLATOR_HPP

#include "ASTNode.hpp"

extern std::shared_ptr<Node> Root;

extern void semantic(int);
extern void translate(bool format = true, std::ostream &os = std::cout);

#endif /* TRANSLATOR_HPP */
