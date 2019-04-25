#ifndef SYMBOLTABLE_HPP
#define SYMBOLTABLE_HPP

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
    int width;
    Value value;

    // If the symbol is a function, define the follow attributes
    int params; // The symbol's params quantities
    TokenType *param_types;

    bool is_global;
    std::shared_ptr<Symbol> next;
};

#endif /* SYMBOLTABLE_H */
