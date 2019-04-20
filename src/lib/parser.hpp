#ifndef PARSER_HPP
#define PARSER_HPP

using std::string;
using std::vector;

extern std::vector<std::shared_ptr<Production>> ProdVec; // Store the Production sequence.
extern std::set<std::string> NonTerminalSet;             // Store the non-terminal symbols.
extern std::set<std::string> TerminalSet;                // Store the terminal symbols.

extern std::map<string, std::shared_ptr<vector<string>>> FirstSet;  // Store the First Set.
extern std::map<string, std::shared_ptr<vector<string>>> FollowSet; // Store the Follow Set.

extern std::vector<std::vector<Item>> ClosureSet; // Store the Closures' set.

extern std::vector<ReduceItem> ReduceTable; // Store the Ruduce Action information.
extern std::vector<GotoItem> ShiftTable;    // Store the Shift Action information.
extern std::vector<GotoItem> GotoTable;     // Store the Goto Action information.

extern std::stack<int> StateStack;  // Store the State Stack information.
extern std::stack<std::string> SymbolStack; // Store the Symbol Stack information.

// Read the grammar from file.
extern int read_grammar(const std::string &filename);

// Analysis the input string sequence.
extern void analysis(const vector<string> &seq);

#endif /* PARSER_HPP */
