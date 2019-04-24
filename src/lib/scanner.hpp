#ifndef SCANNER_HPP
#define SCANNER_HPP

#include "Token.hpp"
#include "SymbolTable.hpp"

extern std::vector<std::shared_ptr<Token>> TokenVec;
extern std::shared_ptr<Symbol> SymbolTable;

extern int scan_file(const std::string &filename, std::ostream &os = std::cout); 

#endif /* SCANNER_HPP */
