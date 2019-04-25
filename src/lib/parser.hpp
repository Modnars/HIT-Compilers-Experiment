#ifndef PARSER_HPP
#define PARSER_HPP

#include "Token.hpp"
#include "Production.hpp"

using std::string;
using std::vector;

// Store the Production sequence.
extern std::vector<std::shared_ptr<Production>> ProdVec; 
// Store the current analysis symbol position, which is also the TokenVec's index.
// Which is used in parser(s) for mark sequences!
extern int idx;
// Store the 'id' specific token information. Which is used for semantic analysis.
extern std::stack<std::shared_ptr<Token>> TokenStack;

// Read the grammar from file.
extern int read_grammar(const std::string &filename);

// Analysis the input string sequence.
extern void analysis(const vector<string> &seq, std::ostream &os = std::cout);
// Parse the token sequences.
extern void parse(const vector<std::shared_ptr<Token>> &token_seq, 
        std::ostream &os = std::cout);

// Check whether the grammar is ambigious.
extern void checkGrammar(std::ostream &os = std::cout);

extern void print_ReduceTable(std::ostream &os = std::cout);
extern void print_ShiftTable(std::ostream &os = std::cout);
extern void print_GotoTable(std::ostream &os = std::cout);

#endif /* PARSER_HPP */
