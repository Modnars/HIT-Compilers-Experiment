#include <iostream>
#include <fstream>
#include <stack>

#include <cstdlib>

#include "lib/Production.hpp"
#include "lib/BasicFunc.hpp"
#include "lib/Item.hpp"

using std::string;
using std::vector;

std::vector<std::shared_ptr<Production>> ProdVec; // Store the Production sequence.
std::set<std::string> NonTerminalSet;             // Store the non-terminal symbols.
std::set<std::string> TerminalSet;                // Store the terminal symbols.

std::map<string, std::shared_ptr<vector<string>>> FirstSet;  // Store the First Set.
std::map<string, std::shared_ptr<vector<string>>> FollowSet; // Store the Follow Set.

std::vector<std::vector<Item>> ClosureSet; // Store the Closures' set.

std::vector<ReduceItem> ReduceTable; // Store the Ruduce Action information.
std::vector<GotoItem> ShiftTable;    // Store the Shift Action information.
std::vector<GotoItem> GotoTable;     // Store the Goto Action information.

std::stack<int> StateStack;  // Store the State Stack information.
std::stack<std::string> SymbolStack; // Store the Symbol Stack information.

// Judge whether the production could be null.
bool could_be_null(const string &prod) {
    vector<string> rights;
    for (int i = 0; i < ProdVec.size(); ++i) {
        if (ProdVec[i]->get_left() == prod) {
            rights = ProdVec[i]->get_right();
            if (rights[0] == "$") {
                return true;
            }
        }
    }
    return false;
}

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
    for (auto p : ProdVec) 
        for (auto sym : p->get_right()) 
            if (!contains(NonTerminalSet, sym) && sym != "$")
                TerminalSet.insert(sym);
    return 0;
}

// Print the debug information.
void print_info(std::ostream &os = std::cout) {
//    os << "Production Vector:" << std::endl;
//    for (auto var : ProdVec)
//        os << *var << std::endl;
//    os << "Non-Terminal Symbols:" << std::endl;
//    for (auto var : NonTerminalSet)
//        os << var << " ";
//    os << std::endl;
//    os << "Terminal Symbols:" << std::endl;
//    for (auto var : TerminalSet)
//        os << var << " ";
//    os << std::endl << std::endl;
//    os << "FIRST Set:" << std::endl;
//    for (auto item : FirstSet) {
//        os << item.first << ": ";
//        for (auto var : *item.second)
//            os << var << " ";
//        os << std::endl;
//    }
//    os << std::endl;
//    os << "FOLLOW Set:" << std::endl;
//    for (auto item : FollowSet) {
//        os << item.first << ": ";
//        for (auto var : *item.second)
//            os << var << " ";
//        os << std::endl;
//    }
    os << "ReduceAction: " << std::endl;
    for (auto var : ReduceTable) {
        os << "START: " << var.state;
        os << "\tMeet: " << std::left << std::setw(10) << var.symbol;
        os << "\tR" << var.prod_id << ": " << *ProdVec[var.prod_id] << std::endl;
    }
}

// Get the FIRST Set of the grammar.
void getFirstSet() {
    std::shared_ptr<vector<string>> first;
    for (auto var : TerminalSet) {
        first = std::make_shared<vector<string>>();
        first->push_back(var);
        FirstSet[var] = first;
    }
    for (auto var : NonTerminalSet) {
        first = std::make_shared<vector<string>>();
        FirstSet[var] = first;
    }
    bool flag;
    while (true) {
        flag = true;
        string left, right;
        vector<string> rights;
        for (int i = 0; i < ProdVec.size(); ++i) {
            left = ProdVec[i]->get_left();
            rights = ProdVec[i]->get_right();
            for (int j = 0; j < rights.size(); ++j) {
                right = rights[j];
                if (right != "$") {
                    for (int l = 0; l < FirstSet[right]->size(); ++l)
                        if (contains(*FirstSet[left], (*FirstSet[right])[l])) {
                            continue;
                        } else {
                            FirstSet[left]->push_back((*FirstSet[right])[l]);
                            flag = false;
                        }
                }
                if (could_be_null(right)) {
                    continue;
                } else {
                    break;
                }
            }
        }
        if (flag) break;
    }
}

// Get the FOLLOW Set of the grammar.
void getFollowSet() {
    for (auto var : NonTerminalSet) 
        FollowSet[var] = std::make_shared<vector<string>>();
    FollowSet["S"]->push_back("#");
    bool flag, fab;
    while (true) {
        flag = true;
        for (int i = 0; i < ProdVec.size(); ++i) {
            string left, right;
            vector<string> rights = ProdVec[i]->get_right();
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
                        left = ProdVec[i]->get_left();
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

//    // Remove the "#" in FOLLOW Set.
//    // 清除FOLLOW集中的"#"
//    for (auto var : NonTerminalSet) {
//        auto svec_ptr = FollowSet[var];
//        for (auto iter = svec_ptr->begin(); iter != svec_ptr->end();) {
//            if (*iter == "#") {
//                iter = svec_ptr->erase(iter);
//            } else {
//                ++iter;
//            }
//        }
//    }
}

void getI0() {
    vector<string> rights = {"@"};
    for (auto var : ProdVec[0]->get_right())
        rights.push_back(var);
    vector<Item> closure = {Item(ProdVec[0]->get_left(), rights)};
    string left;
    for (size_t i = 0; i < closure.size(); ++i) {
        for (auto pptr : ProdVec) {
            if ((left = pptr->get_left()) == closure[i].next_sym()) {
                rights = {"@"};
                for (auto var : pptr->get_right()) 
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
            if ((left = pptr->get_left()) == closure[i].next_sym()) {
                rights = {"@"};
                for (auto var : pptr->get_right()) 
                    rights.push_back(var);
                auto new_item = Item(left, rights);
                if (!contains(closure, new_item))
                    closure.push_back(new_item);
            }
        } 
    }
}

void getClosureSet() {
    getI0();
    vector<Item> closure;
    vector<string> tmpVec, rights; // Store the temp next_sym.
    for (size_t i = 0; i < ClosureSet.size(); ++i) {
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
            bool found = false;
            for (size_t j = 0; j < ClosureSet.size(); ++j) {
                if (ClosureSet[j] == closure) {
                    if (contains(NonTerminalSet, sym))
                        GotoTable.push_back(GotoItem(i, j, sym));
                    else 
                        ShiftTable.push_back(GotoItem(i, j, sym));
                    found = true;
                    break;
                }
            }
            if (!found) {
                ClosureSet.push_back(closure);
                if (contains(NonTerminalSet, sym))
                    GotoTable.push_back(GotoItem(i, ClosureSet.size()-1, sym));
                else 
                    ShiftTable.push_back(GotoItem(i, ClosureSet.size()-1, sym));
            }
        }
    }
}

void getReductionTable() {
    for (size_t i = 0; i < ClosureSet.size(); ++i) {
        for (auto item : ClosureSet[i]) {
            if (item.could_reduce()) {
                for (size_t j = 0; j < ProdVec.size(); ++j) {
                    if (item.reduce_from(*ProdVec[j])) {
                        for (auto sym : *FollowSet[item.left]) {
                            ReduceTable.push_back(ReduceItem(i, sym, j));
                        }
                        break;
                    }
                } /* for (size_t j = 0; ...) loop */
            } /* if (item.could_reduce()) */
        } /* for (auto item : ...) loop */
    } /* for (size_t i = 0; ...) loop */
}

int searchGotoTable(int state, const string &sym) {
    for (auto var : GotoTable) 
        if (var.start == state && var.symbol == sym) {
            std::cout << "GOTO:   ";
            print_stk(StateStack);
            std::cout << "#";
            print_stk(SymbolStack);
            std::cout << "\t\t" << sym;
            std::cout << "\t\tGOTO:" << var.end << std::endl;
            return var.end;
        }
    return -1;
}

int searchShiftTable(int state, const string &sym) {
    for (auto var : ShiftTable) 
        if (var.start == state && var.symbol == sym) {
            std::cout << "SHIFT:  ";
            print_stk(StateStack);
            std::cout << "#";
            print_stk(SymbolStack);
            std::cout << "\t\t" << sym;
            std::cout << "\t\tS" << var.end << std::endl;
            return var.end;
        }
    return -1;
}

int searchReduceTable(int state, const string &sym) {
    for (auto var : ReduceTable) 
        if (var.state == state && var.symbol == sym) {
            std::cout << "REDUCE: ";
            print_stk(StateStack);
            std::cout << "#";
            print_stk(SymbolStack);
            std::cout << "\t\t" << sym;
            std::cout << "\t\tR" << var.prod_id << ": " << *ProdVec[var.prod_id] << std::endl;
            return var.prod_id;
        }
    return -1;
}

void analysis(const vector<string> &seq) {
    StateStack.push(0);
    size_t i = 0;
    bool accepted = false, done = false;
    while (!done && !accepted) {
        int res;
        if (StateStack.top() == 1 && seq[i] == "#") {
            accepted = true;
        } else if ((res = searchShiftTable(StateStack.top(), seq[i])) > -1) {
            StateStack.push(res);
            SymbolStack.push(seq[i]);
            ++i;
        } else if ((res = searchReduceTable(StateStack.top(), seq[i])) > -1) {
            if (ProdVec[res]->get_right()[0] != "$") // TODO
                for (int k = 0; k < ProdVec[res]->get_right().size(); ++k) {
                    StateStack.pop();
                    SymbolStack.pop();
                }
            SymbolStack.push(ProdVec[res]->get_left());
            if ((res = searchGotoTable(StateStack.top(), SymbolStack.top())) > -1) {
                StateStack.push(res);
            } else {
                done = true;
            }
        } else {
            done = true;
        }
    }
    if (accepted) {
        std::cout << "Accepted!" << std::endl;
    } else {
        std::cout << "Error!" << std::endl << "Remain string: ";
        while (i < seq.size()) {
            std::cout << seq[i];
            ++i;
        }
        std::cout << std::endl;
    }
}

int main(int argc, char *argv[]) {
    read_grammar("../file/grammar/grammar.txt");
//    read_grammar("../file/grammar/std_grammar.txt");
    getFirstSet();
    getFollowSet();
    getClosureSet();
    getReductionTable();
    std::ofstream os("../file/log/debug.log");
    print_info(os);
    os.close();
//    for (auto closure : ClosureSet)
//        for (auto item : closure)
//            if (item.reduce_from(*ProdVec[2]))
//                std::cout << item << std::endl;
//    analysis({"id", "=", "id", "+", "id", "*", "id", ";", "#"});
//    analysis({"item", "=", "item", "+", "item", "*", "item", ";", "#"});
    analysis({"int", "id", "(", "int", "id", ")", ";", "#"});
//    int i = 0;
//    vector<string> input;
//    string tmp;
//    while (std::cin >> tmp) {
//        input.push_back(tmp);
//    }
//    input.push_back("#");
//    std::cout << input.size() << std::endl;
//    for (auto var : input) 
//        std::cout << var << " ";
//    std::cout << std::endl;
//    analysis(input);
    return EXIT_SUCCESS;
}
