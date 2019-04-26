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

// For semantic analysis.
int idx;
// For semantic analysis.
std::stack<std::shared_ptr<Token>> TokenStack;

std::vector<std::shared_ptr<Production>> ProdVec; // Store the Production sequence.
std::set<std::string> NonTerminalSet;             // Store the non-terminal symbols.
std::set<std::string> TerminalSet;                // Store the terminal symbols.

std::map<string, std::shared_ptr<vector<string>>> FirstSet;  // Store the First Set.

std::vector<std::set<Item>> ClosureSet; // Store the Closures' set.

// Store the Action information of Reduce, Shift and Goto.
std::map<int, std::shared_ptr<std::map<std::string, int>>> ActionTable;

std::stack<int> StateStack;  // Store the State Stack information.
std::stack<std::string> SymbolStack; // Store the Symbol Stack information.

// Read the grammar from file.
// The function could generate the TerminalSet and NonTerminalSet.
// The TerminalSet generated will contains '$', which is used to compute FirstSet.
int read_grammar(const std::string &filename) {
    std::ifstream is(filename);
    if (!is) {
        std::cerr << "Error when open file " << filename << std::endl;
        return EXIT_FAILURE;
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
            if (!contains(NonTerminalSet, sym)) // Warning : here see the '$' as the terminal symbol.
                TerminalSet.insert(sym);
    return EXIT_SUCCESS;
}

// Judge whether the symbol could generate null symbol.
// For example, S -> E, E -> $, we see the S could generate '$'.
bool could_be_null(const string &sym) {
    return contains(*FirstSet[sym], string("$"));
}

// Generate the FirstSet. And the FirstSet generated will contains '$' for each non-terminal if 
// the symbol could be null.
// At the end of the function, it will remove '$' from FirstSet and TerminalSet.
void getFirstSet() {
    std::shared_ptr<vector<string>> first;
    for (auto var : TerminalSet) {
        FirstSet[var] = std::make_shared<vector<string>>();
        FirstSet[var]->push_back(var);
    }
    for (auto var : NonTerminalSet)
        FirstSet[var] = std::make_shared<vector<string>>();
    bool extending = true;
    while (extending) {
        extending = false;
        for (auto pptr : ProdVec) {
            auto left = pptr->left;
            bool add_null = false;
            for (size_t i = 0; i < pptr->rights.size(); ++i) {
                if (could_be_null(pptr->rights[i]) && i == pptr->rights.size()-1) {
                    add_null = true;
                } else if (!could_be_null(pptr->rights[i])) {
                    for (auto var : *FirstSet[pptr->rights[i]]) {
                        if (!contains(*FirstSet[left], var)) {
                            FirstSet[left]->push_back(var);
                            extending = true;
                        }
                    }
                    break;
                } else if (could_be_null(pptr->rights[i])) {
                    for (auto var : *FirstSet[pptr->rights[i]]) {
                        if (!contains(*FirstSet[left], var) && var != "$") {
                            FirstSet[left]->push_back(var);
                            extending = true;
                        }
                    }
                }
            }
            if (add_null && !contains(*FirstSet[left], string("$")))
                FirstSet[left]->push_back("$");
        }
    }
    for (auto iter = TerminalSet.begin(); iter != TerminalSet.end(); ++iter) // Remove '$' from TerminalSet
        if (*iter == "$") {
            TerminalSet.erase(iter);
            break;
        }
    for (auto iter = FirstSet.begin(); iter != FirstSet.end(); ++iter) // Remove '$' from FirstSet
        if (iter->first == "$") {
            FirstSet.erase(iter);
            break;
        }
}

// Extend the closure when the closure need to extending.
// For example, we can use the function to get status I0 after we put start item in closure.
void extend(std::set<Item> &closure) {
    vector<string> rights;
    string left;
    bool extending = true;
    while (extending) {
        extending = false;
        for (auto iter = closure.begin(); iter != closure.end(); ++iter) {
            for (auto pptr : ProdVec) {
                if ((left = pptr->left) == iter->next_sym()) {
                    rights = {"@"};
                    for (auto var : pptr->rights) 
                        rights.push_back(var);
                    auto new_item = Item(left, rights, iter->search); // The item's search is from extending.
                    int pos = iter->pos() + 1;
                    bool need_add = true;
                    while (pos != iter->rights.size()-1) {
                        ++pos;
                        for (auto sym : *FirstSet[iter->rights[pos]]) {
                            auto add_new_item = Item(left, rights, sym);
                            if (!contains(closure, add_new_item) && sym != "$") {
                                closure.insert(add_new_item);
                                extending = true;
                            }
                        }
                        if (!could_be_null(iter->rights[pos])) {
                            need_add = false;
                            break;
                        }
                    }
                    if (need_add && !contains(closure, new_item)) {
                        closure.insert(new_item);
                        extending = true;
                    }
                } /* if ((left = pptr->left) ...) */
            } /* for (auto pptr : ...) loop */
        } /* for (auto iter ...) loop */
    } /* while (extending) */
}

// Get the first closure.
void getI0() {
    getFirstSet();
    vector<string> rights = {"@"};
    for (auto var : ProdVec[0]->rights)
        rights.push_back(var);
    std::set<Item> closure = {Item(ProdVec[0]->left, rights, "#")}; // Put the start item in closure.
    extend(closure);
    ClosureSet.push_back(closure);
}

// Get all the closures.
// This function also is the key to judge whether the grammar is LR grammar.
void getClosureSet(std::ostream &os = std::cout) {
    getI0();
    std::set<Item> closure;
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
                    closure.insert(item.shift());
                }
            }
            extend(closure);
            bool found = false;
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

// Fill the ReduceTable when has gotten all the closures.
// This function also is the key to judge whether the grammar is LR grammar.
void fillReduceAction(std::ostream &os = std::cout) {
    int base = ClosureSet.size();
    for (int i = 0; i < ClosureSet.size(); ++i) {
        for (auto item : ClosureSet[i]) {
            if (item.could_reduce()) {
                for (int j = 0; j < ProdVec.size(); ++j) {
                    if (item.reduce_from(*ProdVec[j])) {
                        // Use new data structure. Here...
                        if (!contains(ActionTable, i)) {
                            ActionTable[i] = std::make_shared<std::map<string, int>>();
                            (*ActionTable[i])[item.search] = base + j;
                        } else {
                            if (!contains(*ActionTable[i], item.search)) {
                                (*ActionTable[i])[item.search] = base + j;
                            } else if ((*ActionTable[i])[item.search] != base + j) {
                                os << "\nError! [3] The Gramma fill the ActionTable repeatly!" << std::endl;
                                os << "Production: " << *ProdVec[j] << std::endl;
                                os << "Pos: " << i << ", " << item.search << std::endl;
                                os << "Have Existed: " << (*ActionTable[i])[item.search] << std::endl;
                            }
                        } // ... Done
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
        //os << "GOTO:" << res << std::endl; // TODO : remove the comment.
    } else {
        //os << "S" << res << std::endl; // TODO : remove the comment.
    }
    return res;
}

// Analysis the input string sequences, and the symbols from the sequences must be the 
// symbols from grammar file.
void analysis(const vector<string> &seq, std::ostream &os) {
    getClosureSet();
    fillReduceAction();
    int base = ClosureSet.size(); // TODO int -> size_t
    idx = 0;
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

// Parse the Token sequences.
// Could use TokenVec directly when referenced from "lib/scanner.hpp".
void parse(const vector<std::shared_ptr<Token>> &token_seq, std::ostream &os) {
    vector<string> sym_seq;
    for (auto token : token_seq)
        sym_seq.push_back(token2string(*token));
    sym_seq.push_back("#");
    analysis(sym_seq, os);   
}

void checkGrammar(std::ostream &os) {
    getClosureSet();
    os << "STATUS NUMBER: " << ClosureSet.size() << std::endl;
    fillReduceAction();
}

// int main(int argc, char *argv[]) {
//     read_grammar("../file/grammar/grammar.txt");
// //    read_grammar("../file/grammar/grammar1.txt");
// //    read_grammar("../file/grammar/grammar2.txt");
// //    read_grammar("../file/grammar/grammar3.txt"); // Not Accepted!
// //    read_grammar("../file/grammar/grammar4.txt"); 
// //    read_grammar("../file/grammar/grammar5.txt"); 
// //    read_grammar("../file/grammar/grammar6.txt"); 
// //    getFirstSet();
// //    getI0();
// //    getClosureSet();
// //    fillReduceAction();
// //    std::vector<string> seq = {"id", "=", "*", "id", "#"}; // for grammar
// //    std::vector<string> seq = {"a", "b", "a", "b", "#"}; // for grammar1
// //    std::vector<string> seq = {"c", "c", "c", "d", "c", "d", "#"}; // for grammar2
// //    std::vector<string> seq = {"int", "id", "=", "id", "[", "id", "]", ";", "#"}; // for grammar3
// //    for (auto sym : FirstSet) {
// //        std::cout << sym.first << ": ";
// //        for (auto f : *sym.second)
// //            std::cout << f << " ";
// //        std::cout << std::endl;
// //    }
// //    getClosureSet();
// //    int i = 0;
// //    for (auto closure : ClosureSet) {
// //        std::cout << "STATUS" << i++ << std::endl;
// //        for (auto item : closure) 
// //            std::cout << item << std::endl;
// //    }
// //    fillReduceAction();
// //    std::vector<string> seq = {"id", "+", "id", "*", "id", "#"}; // for grammar3
// //    std::vector<string> seq = {"id", "=", "id", "+", "id", "*", "id", "#"}; // for grammar3
// //    analysis(seq);
// 
// //    for (auto val1 : ActionTable) {
// //        std::cout << "STATUS " << val1.first << std::endl;
// //        for (auto val2 : *val1.second)
// //            std::cout << val2.first << " " << val2.second << std::endl;
// //    }
//     std::vector<string> seq = {"int", "id", "(", ")", "{", "int", "id", "=", "digit", ",", "id", "=", "digit", ";", "int", "id", "=", "digit", ";", "while", "(", "id", ">", "digit", ")", "{", "id", "=", "id", "+", "id", ";", "id", "=", "id", "+", "digit", ";", "break", ";", "}", "if", "(", "id", ">", "digit", "&&", "id", "<", "digit", "||", "id", "==", "digit", ")", "{", "id", "=", "id", "-", "digit", ";", "}", "if", "(", "!", "id", ")", "{", "id", "=", "digit", ";", "}", "else", "{", "id", "=", "id", "+", "id", ";", "}", "return", "digit", ";", "}", "#"};
//     analysis(seq);
//     return EXIT_SUCCESS;
// }
