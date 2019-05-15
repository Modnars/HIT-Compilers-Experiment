#ifndef SCANNER_HPP
#define SCANNER_HPP

#include "Token.hpp"
#include "SymbolTable.hpp"

// Store the recognized token sequence.
extern std::vector<std::shared_ptr<Token>> TokenVec;
// Store the global symbol table.
extern std::shared_ptr<Symbol> SymbolTable;

/**
 * Scan the source file.
 * 
 * @param filename The source file's path.
 * @param os The program's output stream.
 * @return EXIT_FAILURE when meet any error, otherwise return EXIT_SUCCESS.
 */
extern int scan_file(const std::string &filename, std::ostream &os = std::cout); 

#endif /* SCANNER_HPP */
