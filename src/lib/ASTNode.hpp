#ifndef ASTNODE_HPP
#define ASTNODE_HPP

#include "Base.hpp"
#include "SymbolTable.hpp"

#include <vector>
#include <string>

class Node {
public:
    Node(const std::string &name, TokenType type, std::shared_ptr<Symbol> symbol) :
        name(name), type(type), symbol(symbol), child_count(0), child(nullptr), 
        parent(nullptr), sibling(nullptr) { }

    std::string name;
    TokenType type;
    Value value;
    std::shared_ptr<Symbol> symbol;
    int child_count;
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
