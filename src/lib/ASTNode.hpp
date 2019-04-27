#ifndef ASTNODE_HPP
#define ASTNODE_HPP

#include "Base.hpp"
#include "SymbolTable.hpp"
#include "Code.hpp"

#include <vector>
#include <string>

typedef enum {
    P0,  P1,  P2,  P3,  P4,  P5,  P6,  P7,  P8,  P9,
    P10, P11, P12, P13, P14, P15, P16, P17, P18, P19,
    P20, P21, P22, P23, P24, P25, P26, P27, P28, P29,
    P30, P31, P32, P33, P34, P35, P36, P37, P38, P39,
    P40, P41, P42, P43, P44, P45, P46, P47, P48
} Kind;

class Node {
public:
    Node(const std::string &name, TokenType type, std::shared_ptr<Symbol> symbol) :
        name(name), type(type), symbol(symbol), child_count(0), child(nullptr), 
        parent(nullptr), sibling(nullptr) { }

    std::string name;
    TokenType type;
    Kind kind;
    Value value;
    std::shared_ptr<Symbol> symbol;
    int child_count;
    std::vector<int> truelist;
    std::vector<int> falselist;
    std::vector<int> nextlist;
    std::shared_ptr<Node> child;
    std::shared_ptr<Node> parent;
    std::shared_ptr<Node> sibling;
};

extern std::shared_ptr<Node> new_node(const std::string &name, TokenType type, 
        std::shared_ptr<Symbol> symbol);
extern void destroy_node(std::shared_ptr<Node> &);
extern void print_nodes(std::shared_ptr<Node> &, std::ostream &os = std::cout);

extern Value get_node_value(std::shared_ptr<Node> &);
extern int get_node_value_as_int(std::shared_ptr<Node> &);
extern int get_node_child_counter(std::shared_ptr<Node> &);
extern bool check_node_error(std::shared_ptr<Node> &);
extern void add_child(std::shared_ptr<Node> &, std::shared_ptr<Node> &);
extern void add_subling(std::shared_ptr<Node> &, std::shared_ptr<Node> &);

#endif /* ASTNODE_H */
