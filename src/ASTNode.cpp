#include <sstream>

#include "lib/ASTNode.hpp"

std::shared_ptr<Node> new_node(const std::string &name, TokenType type, 
        std::shared_ptr<Symbol> symbol) {
    return std::make_shared<Node>(name, type, symbol);
}

void print_nodes(std::shared_ptr<Node> &self, std::ostream &os) {
    if (self) {
        print_nodes(self->child);
        print_nodes(self->sibling);
        os << self->name << " ";
        if (self->child) {
            os << "Children: ";
            for (auto ptr = self->child; ptr != nullptr; ptr = ptr->sibling)
                os << ptr->name << " ";
        } 
        if (self->sibling) {
            os << "Sibiling: ";
            for (auto ptr = self->sibling; ptr != nullptr; ptr = ptr->sibling)
                os << ptr->name << " ";
        }
        if (self->parent)
            os << "Parent: " << self->parent->name << std::endl;
        os << std::endl;
    }
}

void destroy_node(std::shared_ptr<Node> &self) {
    if (self) {
        destroy_node(self->child);
        destroy_node(self->sibling);
        self = nullptr;
    }
}

Value get_node_value(std::shared_ptr<Node> &self) {
    if (self->type == ID) 
        return self->symbol->value;
    return self->value;
}

int get_node_value_as_int(std::shared_ptr<Node> &self) {
    if (self->type == ID)
        return self->symbol->value.ival;
    return self->value.ival;
}

void add_child(std::shared_ptr<Node> &self, std::shared_ptr<Node> &child) {
    if (child == nullptr)
        return;
    if (self->child == nullptr) {
        child->parent = self;
        self->child = child;
    } else {
        add_subling(self->child, child);
    }
    for (auto ptr = child; ptr != nullptr; ptr = ptr->sibling) {
        ptr->parent = self;
    }
}

void add_subling(std::shared_ptr<Node> &self, std::shared_ptr<Node> &sibling) {
    if (sibling == nullptr)
        return;
    if (self->sibling == nullptr) {
        self->sibling = sibling;
    } else {
        auto ptr = self->sibling;
        while (ptr->sibling != nullptr)
            ptr = ptr->sibling;
        ptr->sibling = sibling;
    }
}
