#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include "Base.hpp"

class Symbol;
extern std::shared_ptr<Symbol> new_symbol(const std::string &);
extern std::shared_ptr<Symbol> insert_symbol(std::shared_ptr<Symbol>, 
        std::shared_ptr<Symbol>);
extern std::shared_ptr<Symbol> lookup_symbol(std::shared_ptr<Symbol>, 
        const std::string &);
extern void destroy_symbol_table(std::shared_ptr<Symbol>);
extern void create_symbol_params(std::shared_ptr<Symbol>, int);
extern bool is_function_symbol(std::shared_ptr<Symbol>);
extern void print_symbol(std::shared_ptr<Symbol>, std::ostream &os = std::cout);
extern void print_symbol_table(std::shared_ptr<Symbol>, std::ostream &os = std::cout);

class Symbol {
public:
    Symbol(const std::string &name) : name(name) { }
    std::string name;
    int decl_line_no;
    TokenType type; // Here, the type should only choose from basic data type
    Value value;

    // If the symbol is a function, define the follow attributes
    int params; // The symbol's params quantities
    TokenType *param_types;

    bool is_global;
    std::shared_ptr<Symbol> next;
};

std::shared_ptr<Symbol> new_symbol(const std::string &name) {
    std::shared_ptr<Symbol> symbol = std::make_shared<Symbol>(name);
    symbol->decl_line_no = 0;
    symbol->type = VOID;
    set_value(&symbol->value, VOID, nullptr);
    symbol->params = -1;
    symbol->param_types = nullptr;
    symbol->is_global = false;
    symbol->next = nullptr;
    return symbol;
}

std::shared_ptr<Symbol> insert_symbol(std::shared_ptr<Symbol> symtab, 
        std::shared_ptr<Symbol> symbol) {
    if (symbol == nullptr)
        return nullptr;
    std::shared_ptr<Symbol> sym = lookup_symbol(symtab, symbol->name);
    if (sym)
        return sym;
    symbol->next = symtab->next;
    symtab->next = symbol;
    return symbol;
}

std::shared_ptr<Symbol> lookup_symbol(std::shared_ptr<Symbol> symtab, 
        const std::string &search) {
    if (symtab == nullptr)
        return nullptr;
    for (auto temp = symtab->next; temp != nullptr; temp = temp->next)
        if (temp->name == search)
            return temp;
    return nullptr;
}

void destroy_symbol_table(std::shared_ptr<Symbol> symtab) {
    if (symtab)
        symtab->next = nullptr;
}

void create_symbol_params(std::shared_ptr<Symbol> symbol, int quantity) {
    symbol->params = quantity;
    if (symbol->params > 0)
        symbol->param_types = (TokenType *)malloc(sizeof(int) * quantity);
}

bool is_function_symbol(std::shared_ptr<Symbol> symbol) {
    return (symbol->params > -1);
}

void print_symbol(std::shared_ptr<Symbol> symbol, std::ostream &os) {
    if (symbol == nullptr) {
        os << "[NULL]" << std::endl;
        return;
    }
    os << std::left << std::setw(15) << "[Symbol]" << std::setw(15) << symbol 
       << std::endl << std::setw(15) << "[Name]" << std::setw(15) << symbol->name
       << std::endl << std::setw(15) << "[Type]" << std::setw(15) << symbol->type
       << std::endl << std::setw(15) << "[Value]";
    print_value(&symbol->value, symbol->type, os);
    os << std::left << std::setw(15) << "[Decl_pos]" 
       << std::setw(15) << symbol->decl_line_no << std::endl
       << std::setw(15) << "[Next]"
       << std::setw(15) << symbol->next << std::endl << std::endl;
}

void print_symbol_table(std::shared_ptr<Symbol> symtab, std::ostream &os) {
    auto temp = symtab;
    if (temp == nullptr) {
        print_symbol(temp, os);
        return;
    }
    for (temp = symtab->next; temp != nullptr; temp = temp->next) 
        print_symbol(temp, os);
}

#endif /* SYMBOLTABLE_H */
