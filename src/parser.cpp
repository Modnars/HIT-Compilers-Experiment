#include <iostream>
#include <fstream>
#include <stack>
#include <cstdlib>

#include "lib/Production.hpp"
#include "lib/BasicFunc.hpp"
#include "lib/Item.hpp"

#include "lib/scanner.hpp"
#include "lib/parser.hpp"
#include "lib/translator.hpp"

using std::string;
using std::vector;

std::vector<std::shared_ptr<Production>> ProdVec; // Store the Production sequence.
std::set<std::string> NonTerminalSet;             // Store the non-terminal symbols.
std::set<std::string> TerminalSet;                // Store the terminal symbols.

// For semantic analysis.
std::stack<std::shared_ptr<Token>> TokenStack;

std::map<string, std::shared_ptr<vector<string>>> FirstSet;  // Store the First Set.
std::map<string, std::shared_ptr<vector<string>>> FollowSet; // Store the Follow Set.

std::vector<std::vector<Item>> ClosureSet; // Store the Closures' set.

// Store the Action information of Reduce, Shift and Goto.
std::map<int, std::shared_ptr<std::map<std::string, int>>> ActionTable;

std::stack<int> StateStack;  // Store the State Stack information.
std::stack<std::string> SymbolStack; // Store the Symbol Stack information.

// Read the grammar from file.
int read_grammar(const std::string &filename) {
    std::ifstream is(filename);
    if (!is) {
        std::cerr << "Error when open file " << filename << std::endl;
        return 1;
    }
    std::string line;
    while (getline(is, line)) {
        auto tmp_vec = split(line, "->");
        std::string left = trim(tmp_vec[0]);
        if (!contains(NonTerminalSet, left))
            NonTerminalSet.insert(left);
        std::vector<std::string> right;
        tmp_vec = split(trim(tmp_vec[1]), " ");
        for (int i = 0; i < tmp_vec.size(); ++i)
            right.push_back(trim(tmp_vec[i]));
        ProdVec.push_back(std::make_shared<Production>(left, right));
    }
    is.close();
    for (auto p : ProdVec) 
        for (auto sym : p->rights) 
            if (!contains(NonTerminalSet, sym) && sym != "$")
                TerminalSet.insert(sym);
    return 0;
}

// Judge whether the production could be null directly.
bool could_be_null(const string &prod) {
    for (auto pptr : ProdVec) 
        if (pptr->rights[0] == "$")
            return true;
    return false;
}

void getFirstSet() {
    std::shared_ptr<vector<string>> first;
    for (auto var : TerminalSet) { // Terminal symbol's first set is itself.
        FirstSet[var] = std::make_shared<vector<string>>();
        FirstSet[var]->push_back(var);
    }
    for (auto var : NonTerminalSet) // Initilize the non-terminal symbol's first set.
        FirstSet[var] = std::make_shared<vector<string>>();
    bool extending = true;
    while (true) {
        extending = false;
        for (auto pptr : ProdVec) {
            auto left = pptr->left;
            for (auto right : pptr->rights) {
                if (right != "$") {
                    for (auto new_sym : *FirstSet[right])
                        if (!contains(*FirstSet[left], new_sym)) {
                            FirstSet[left]->push_back(new_sym);
                            extending = true;
                        }
                }
                if (!could_be_null(right))
                    break;
            }
        }
        if (!extending)
            break;
    }
}

// Get the FOLLOW Set of the grammar.
void getFollowSet() {
    getFirstSet();
    for (auto var : NonTerminalSet) 
        FollowSet[var] = std::make_shared<vector<string>>();
    FollowSet["S"]->push_back("#");
    bool flag, fab;
    while (true) {
        flag = true;
        for (int i = 0; i < ProdVec.size(); ++i) {
            string left, right;
            vector<string> rights = ProdVec[i]->rights;
            for (int j = 0; j < rights.size(); ++j) {
                right = rights[j];
                if (contains(NonTerminalSet, right)) {
                    fab = true;
                    for (int k = j+1; k < rights.size(); ++k) {
                        for (int v = 0; v < FirstSet[rights[k]]->size(); ++v) {
                            if (contains(*FollowSet[right], (*FirstSet[rights[k]])[v])) {
                                continue;
                            } else {
                                FollowSet[right]->push_back((*FirstSet[rights[k]])[v]);
                                flag = false;
                            }
                        } /* for (int v ...) loop */
                        if (could_be_null(rights[k])) {
                            continue;
                        } else {
                            fab = false;
                            break;
                        }
                    } /* for (int k ...) loop */
                    if (fab) {
                        left = ProdVec[i]->left;
                        for (int p = 0; p < FollowSet[left]->size(); ++p) {
                            if (contains(*FollowSet[right], (*FollowSet[left])[p])) {
                                continue;
                            } else {
                                FollowSet[right]->push_back((*FollowSet[left])[p]);
                                flag = false;
                            }
                        } /* for (int p ...) loop */
                    } /* if (fab) */
                } /* if (contains(...)) */
            } /* for (int j ...) loop */
        } /* for (int i ...) loop */
        if (flag) break;
    } /* while (true) */
}

void getI0() {
    getFollowSet();
    vector<string> rights = {"@"};
    for (auto var : ProdVec[0]->rights)
        rights.push_back(var);
    vector<Item> closure = {Item(ProdVec[0]->left, rights)};
    string left;
    for (size_t i = 0; i < closure.size(); ++i) {
        for (auto pptr : ProdVec) {
            if ((left = pptr->left) == closure[i].next_sym()) {
                rights = {"@"};
                for (auto var : pptr->rights) 
                    rights.push_back(var);
                auto new_item = Item(left, rights);
                if (!contains(closure, new_item))
                    closure.push_back(new_item);
            }
        } 
    }
    ClosureSet.push_back(closure);
}

void extend(vector<Item> &closure) {
    vector<string> rights;
    string left;
    for (size_t i = 0; i < closure.size(); ++i) {
        for (auto pptr : ProdVec) {
            if ((left = pptr->left) == closure[i].next_sym()) {
                rights = {"@"};
                for (auto var : pptr->rights) 
                    rights.push_back(var);
                auto new_item = Item(left, rights);
                if (!contains(closure, new_item))
                    closure.push_back(new_item);
            }
        } 
    }
}

void getClosureSet(std::ostream &os = std::cout) {
    getI0();
    vector<Item> closure;
    vector<string> tmpVec, rights; // Store the temp next_sym.
    for (int i = 0; i < ClosureSet.size(); ++i) {
        tmpVec.clear();
        for (auto item : ClosureSet[i]) {
            if (!item.could_reduce() && !contains(tmpVec, item.next_sym()))
                    tmpVec.push_back(item.next_sym());
        }
        for (auto sym : tmpVec) {
            closure.clear();
            for (auto item : ClosureSet[i]) {
                if (!item.could_reduce() && item.next_sym() == sym) {
                    closure.push_back(item.shift());
                }
            }
            extend(closure);
            bool found = false; // Mark whether find the closure from before closures.
            for (int j = 0; j < ClosureSet.size(); ++j) {
                if (ClosureSet[j] == closure) {
                    // Use new data structure. Here...
                    if (!contains(ActionTable, i)) {
                        ActionTable[i] = std::make_shared<std::map<string, int>>();
                        (*ActionTable[i])[sym] = j;
                    } else {
                        if (!contains(*ActionTable[i], sym)) {
                            (*ActionTable[i])[sym] = j;
                        } else {
                            for (auto item : closure) 
                                os << item << std::endl;
                            os << "Error! [1] The Gramma fill the ActionTable repeatly!" << std::endl;
                        }
                    } // ... Done
                    found = true;
                    break;
                }
            }
            if (!found) {
                ClosureSet.push_back(closure);
                // Use new data structure. Here...
                if (!contains(ActionTable, i)) {
                    ActionTable[i] = std::make_shared<std::map<string, int>>();
                    (*ActionTable[i])[sym] = ClosureSet.size()-1;
                } else {
                    if (!contains(*ActionTable[i], sym)) {
                        (*ActionTable[i])[sym] = ClosureSet.size()-1;
                    } else {
                        os << "Error! [2] The Gramma fill the ActionTable repeatly!" << std::endl;
                    }
                } // ... Done
            }
        }
    }
}

void fillReduceAction(std::ostream &os = std::cout) {
    int base = ClosureSet.size();
    for (int i = 0; i < ClosureSet.size(); ++i) {
        for (auto item : ClosureSet[i]) {
            if (item.could_reduce()) {
                for (int j = 0; j < ProdVec.size(); ++j) {
                    if (item.reduce_from(*ProdVec[j])) {
                        for (auto sym : *FollowSet[item.left]) {
                            if (!contains(ActionTable, i)) {
                                ActionTable[i] = std::make_shared<std::map<string, int>>();
                                (*ActionTable[i])[sym] = base + j;
                            } else {
                                if (!contains(*ActionTable[i], sym)) {
                                    (*ActionTable[i])[sym] = base + j;
                                } else if ((*ActionTable[i])[sym] != base+j) {
                                    os << "\nError! [3] The Gramma fill the ActionTable repeatly!" << std::endl;
                                    os << "Production: " << *ProdVec[j] << std::endl;
                                    os << "Pos: " << i << ", " << item.search << std::endl;
                                    os << "Have Existed: " << (*ActionTable[i])[item.search] << std::endl;
                                }
                                    
                            }
                        }
                        break;
                    }
                } /* for (size_t j = 0; ...) loop */
            } /* if (item.could_reduce()) */
        } /* for (auto item : ...) loop */
    } /* for (size_t i = 0; ...) loop */
}

// Search the ActionTable to make sure which action will do.
int searchTable(int state, const string &sym, std::ostream &os = std::cout) {
    if (!contains(ActionTable, state))
        return -1;
    if (!contains(*ActionTable[state], sym))
        return -1;
    int res = (*ActionTable[state])[sym];
    int base = ClosureSet.size();
    if (res >= base) {
        os << "R" << res-base << ": " << *ProdVec[res-base] << std::endl;
        semantic(res-base); // Using semantic actions.
    } else if (contains(NonTerminalSet, sym)) {
        os << "GOTO:" << res << std::endl; 
    } else {
        os << "S" << res << std::endl; 
    }
    return res;
}

/**
 * Parse the string sequence.
 * @param seq The string sequence need to parse. The symbol in string is the grammar
 *   symbol
 * @param os The output stream redriected to.
 */
void analysis(const vector<string> &seq, std::ostream &os) {
    getClosureSet();
    fillReduceAction();
    int base = ClosureSet.size(), idx = 0; 
    bool accepted = false, done = false;
    StateStack.push(0);
    while (!done) {
        int res = searchTable(StateStack.top(), seq[idx], os);
        if (res > -1 && res < base) {
            StateStack.push(res);
            SymbolStack.push(seq[idx]);
            // Store the tokens whose type is ID. The "id" symbol is related to grammar.
            if (seq[idx] == "id" || seq[idx] == "CINT" || seq[idx] == "CFLOAT" || seq[idx] == "CBOOL") 
                TokenStack.push(TokenVec[idx]);
            ++idx;
        } else if (res >= base) {
            if (res == base && seq[idx] == "#") {
                accepted = true;
                break;
            }
            if (ProdVec[res-base]->rights[0] != "$") // TODO
                for (int k = 0; k < ProdVec[res-base]->rights.size(); ++k) {
                    StateStack.pop();
                    SymbolStack.pop();
                }
            SymbolStack.push(ProdVec[res-base]->left);
            if ((res = searchTable(StateStack.top(), SymbolStack.top(), os)) > -1) {
                StateStack.push(res);
            } else {
                done = true;
            }
        } else {
            done = true;
        }
    }
    if (accepted) { 
        os << "Accepted!" << std::endl;
    } else {
        os << "Error!" << std::endl << "Remain string: ";
        while (idx < seq.size()) {
            os << seq[idx] << " ";
            ++idx;
        }
        os << std::endl;
    }
}

/**
 * Parse the Token sequences.
 * @param token_seq The token sequence need to parse.
 * @param os The output stream redriected to.
 */
void parse(const vector<std::shared_ptr<Token>> &token_seq, std::ostream &os) {
    vector<string> sym_seq;
    for (auto token : token_seq)
        sym_seq.push_back(token2string(*token));
    sym_seq.push_back("#");
    analysis(sym_seq, os);   
}

/**
 * Check the grammar whether match the rules in grammar.txt
 * @param os The output stream redriected to.
 */
void checkGrammar(std::ostream &os) {
    getClosureSet();
    os << "STATUS NUMBER: " << ClosureSet.size() << std::endl;
    fillReduceAction();
}

/**
 * Print the Reduce action information from the ActionTable, which is used to write
 * log information for UI program.
 * @param os The output stream redriected to.
 */
void print_ReduceTable(std::ostream &os) {
    auto base = ClosureSet.size();
    for (auto status : ActionTable) {
        for (auto item : *status.second) {
            if (item.second >= base) {
                os << status.first << "\t" << item.first << "\tR" << (item.second-base);
                os << ": " << *ProdVec[item.second-base] << std::endl;
            }
        }
    }
}

/**
 * Print the Shift action information from the ActionTable, which is used to write
 * log information for UI program.
 * @param os The output stream redriected to.
 */
void print_ShiftTable(std::ostream &os) {
    for (auto status : ActionTable) {
        for (auto item : *status.second) {
            if (!contains(NonTerminalSet, item.first)) {
                os << status.first << "\t" << item.first << "\t" << item.second 
                   << std::endl;
            }
        }
    }
}

/**
 * Print the Goto action information from the ActionTable, which is used to write
 * log information for UI program.
 * @param os The output stream redriected to.
 */
void print_GotoTable(std::ostream &os) {
    for (auto status : ActionTable) {
        for (auto item : *status.second) {
            if (contains(NonTerminalSet, item.first)) {
                os << status.first << "\t" << item.first << "\t" << item.second
                   << std::endl;
            }
        }
    }
}
