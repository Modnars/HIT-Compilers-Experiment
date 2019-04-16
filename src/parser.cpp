#include <iostream>
#include <fstream>

#include <cstdlib>

#include "lib/Production.hpp"
#include "lib/BasicFunc.hpp"

using std::string;
using std::vector;

std::vector<std::shared_ptr<Production>> ProdVec; // Store the Production sequence.
std::set<std::string> NonTerminalSet;             // Store the non-terminal symbols.
std::set<std::string> TerminalSet;                // Store the terminal symbols.

std::map<string, std::shared_ptr<vector<string>>> first_set;  // Store the First Set.
std::map<string, std::shared_ptr<vector<string>>> follow_set; // Store the Follow Set.

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
    os << "Production Vector:" << std::endl;
    for (auto var : ProdVec)
        os << *var << std::endl;
    os << "Non-Terminal Symbols:" << std::endl;
    for (auto var : NonTerminalSet)
        os << var << " ";
    os << std::endl;
    os << "Terminal Symbols:" << std::endl;
    for (auto var : TerminalSet)
        os << var << " ";
    os << std::endl << std::endl;
    os << "FIRST Set:" << std::endl;
    for (auto item : first_set) {
        os << item.first << ": ";
        for (auto var : *item.second)
            os << var << " ";
        os << std::endl;
    }
    os << std::endl;
    os << "FOLLOW Set:" << std::endl;
    for (auto item : follow_set) {
        os << item.first << ": ";
        for (auto var : *item.second)
            os << var << " ";
        os << std::endl;
    }
}

// Get the FIRST Set of the grammar.
void getFirstSet() {
    std::shared_ptr<vector<string>> first;
    for (auto var : TerminalSet) {
        first = std::make_shared<vector<string>>();
        first->push_back(var);
        first_set[var] = first;
    }
    for (auto var : NonTerminalSet) {
        first = std::make_shared<vector<string>>();
        first_set[var] = first;
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
                    for (int l = 0; l < first_set[right]->size(); ++l)
                        if (contains(*first_set[left], (*first_set[right])[l])) {
                            continue;
                        } else {
                            first_set[left]->push_back((*first_set[right])[l]);
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
        follow_set[var] = std::make_shared<vector<string>>();
    follow_set["S"]->push_back("#");
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
                        for (int v = 0; v < first_set[rights[k]]->size(); ++v) {
                            if (contains(*follow_set[right], (*first_set[rights[k]])[v])) {
                                continue;
                            } else {
                                follow_set[right]->push_back((*first_set[rights[k]])[v]);
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
                        for (int p = 0; p < follow_set[left]->size(); ++p) {
                            if (contains(*follow_set[right], (*follow_set[left])[p])) {
                                continue;
                            } else {
                                follow_set[right]->push_back((*follow_set[left])[p]);
                                flag = false;
                            }
                        } /* for (int p ...) loop */
                    } /* if (fab) */
                } /* if (contains(...)) */
            } /* for (int j ...) loop */
        } /* for (int i ...) loop */
        if (flag) break;
    } /* while (true) */

    // Remove the "#" in FOLLOW Set.
    // 清除FOLLOW集中的"#"
    for (auto var : NonTerminalSet) {
        auto svec_ptr = follow_set[var];
        for (auto iter = svec_ptr->begin(); iter != svec_ptr->end();) {
            if (*iter == "#") {
                iter = svec_ptr->erase(iter);
            } else {
                ++iter;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    read_grammar("../file/grammar/grammar.txt");
    getFirstSet();
    getFollowSet();
    print_info();
    return EXIT_SUCCESS;
}
