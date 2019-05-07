#ifndef PARSER_HPP
#define PARSER_HPP

#include <stack>

#include "Token.hpp"
#include "Production.hpp"

// Store the Production sequence.
extern std::vector<std::shared_ptr<Production>> ProdVec; 
// Store the 'id' specific token information. Which is used for semantic analysis.
extern std::stack<std::shared_ptr<Token>> TokenStack;

// Read the grammar from file.
extern int read_grammar(const std::string &filename);

// Analysis the input string sequence.
extern void analysis(const std::vector<std::string> &seq, 
        std::ostream &os = std::cout);
// Parse the token sequence.
extern void parse(const std::vector<std::shared_ptr<Token>> &token_seq, 
        std::ostream &os = std::cout);

// Check whether the grammar is ambigious.
extern void checkGrammar(std::ostream &os = std::cout);

// Print the information about Reduce, Shift and GOTO.
extern void print_ReduceTable(std::ostream &os = std::cout);
extern void print_ShiftTable(std::ostream &os = std::cout);
extern void print_GotoTable(std::ostream &os = std::cout);

#endif /* PARSER_HPP */
