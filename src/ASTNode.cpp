#include "lib/ASTNode.hpp"

std::shared_ptr<Node> new_node(const std::string &name, int kind,
        TokenType type, int line_no, std::shared_ptr<Symbol> symbol) {
    return std::make_shared<Node>(name, kind, type, line_no, symbol);
}

void destroy_node(std::shared_ptr<Node> &node) {
    if (!node) {
        destroy_node(node->child);
        destroy_node(node->sibling);
        node = nullptr;
    }
}

Value get_node_value(std::shared_ptr<Node> &node) {
    if (node->type == ID) 
        return node->symbol->value;
    return node->value;
}

int get_node_value_as_int(std::shared_ptr<Node> &node) {
    if (node->type == ID)
        return node->symbol->value.ival;
    return node->value.ival;
}

int get_node_child_counter(std::shared_ptr<Node> &node) {
    return node->child_count++;
}

bool check_node_error(std::shared_ptr<Node> &node) {
    if (node == nullptr || node->type == ERROR)
        return true;
    for (auto ptr = node->child; ptr != nullptr; ptr = ptr->sibling)
        if (check_node_error(ptr))
            return true;
    return false;
}

void add_child(std::shared_ptr<Node> &node, std::shared_ptr<Node> &child) {
    if (child == nullptr)
        return;
    if (node->child == nullptr) {
        child->parent = node;
        node->child = child;
    } else {
        add_subling(node->child, child);
    }
    for (auto ptr = child; ptr != nullptr; ptr = ptr->sibling)
        ptr->parent = node;
}

void add_subling(std::shared_ptr<Node> &node, std::shared_ptr<Node> &sibling) {
    if (sibling == nullptr)
        return;
    if (node->sibling == nullptr) {
        node->sibling = sibling;
    } else {
        auto ptr = node->sibling;
        while (ptr->sibling != nullptr)
            ptr = ptr->sibling;
        ptr->sibling = sibling;
    }
}
