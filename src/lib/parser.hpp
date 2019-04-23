#ifndef PARSER_HPP
#define PARSER_HPP

using std::string;
using std::vector;

// Read the grammar from file.
extern int read_grammar(const std::string &filename);

// Analysis the input string sequence.
extern void analysis(const vector<string> &seq, std::ostream &os = std::cout);

// Check whether the grammar is ambigious.
extern void checkGrammar(std::ostream &os = std::cout);

extern void print_ReduceTable(std::ostream &os = std::cout);
extern void print_ShiftTable(std::ostream &os = std::cout);
extern void print_GotoTable(std::ostream &os = std::cout);

#endif /* PARSER_HPP */
