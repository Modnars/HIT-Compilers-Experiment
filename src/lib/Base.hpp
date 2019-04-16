#ifndef BASE_HPP
#define BASE_HPP

#define VOID(var) ((void *) &var)

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <utility>
#include <map>
#include <set>
#include <memory>

// Define Token type.
typedef enum {
    /* Data type */
    VOID, INT, CHAR, FLOAT, DOUBLE, BOOL, STRING, 
    /* Reserved word */
    BREAK, FOR, WHILE, CONTINUE, IF, ELSE, RETURN,
    /* Opearate symbol */
    PLUS, MINUS, MULTI, DEVIDE, LAND, LOR, LNOT, GT, GE, LT, LE, EQ, NE,
    INC, RED, AND, OR,
    /* Mark symbol */
    COMMA, SEMI, ASSIGN, DOT, QUOTE, 
    LPARTH, RPARTH, LBRACK, RBRACK, LBRACE, RBRACE,
    /* Value symbol */
    ID, CINT, CCHAR, CNUM, CFLOAT, CBOOL, CSTRING,
    /* State symbol */
    UNKNOWN, ERROR, NONE, ENDFILE
} TokenType;

// Use Token type to query the standard show string of the type.
// This is used to print Token.
std::vector<std::pair<TokenType, std::string>> type_name = 
        {{VOID, "VOID"},     {INT, "INT"},       {CHAR, "CHAR"}, 
         {FLOAT, "FLOAT"},   {DOUBLE, "DOUBLE"}, {BOOL, "BOOL"},     
         {STRING, "STRING"}, {BREAK, "BREAK"},   {FOR, "FOR"},       
         {WHILE, "WHILE"},   {IF, "IF"},         {CONTINUE, "CONTINUE"}, 
         {ELSE, "ELSE"},     {RETURN, "RETURN"}, {PLUS, "PLUS"}, 
         {MINUS, "MINUS"},   {MULTI, "MULTI"},   {DEVIDE, "DEVIDE"}, 
         {LAND, "LAND"},     {LOR, "LOR"},       {LNOT, "LNOT"},     
         {GT, "GT"},         {GE, "GE"},         {LT, "LT"},
         {LE, "LE"},         {EQ, "EQ"},         {NE, "NE"}, 
         {INC, "INC"},       {RED, "RED"},       {AND, "AND"}, 
         {OR, "OR"},         {COMMA, "COMMA"},   {SEMI, "SEMI"}, 
         {ASSIGN, "ASSIGN"}, {DOT, "DOT"},       {QUOTE, "QUOTE"},
         {LPARTH, "LPARTH"}, {RPARTH, "RPARTH"}, {LBRACK, "LBRACK"}, 
         {RBRACK, "RBRACK"}, {LBRACE, "LBRACE"}, {RBRACE, "RBRACE"}, 
         {ID, "ID"},         {CINT, "CINT"},     {CSTRING, "CSTRING"},
         {CNUM, "CNUM"},     {CFLOAT, "CFLOAT"}, {CBOOL, "CBOOL"},   
         {CCHAR, "CCHAR"},   {ERROR, "ERROR"},   {UNKNOWN, "UNKNOWN"},
         {NONE, "NONE"},     {ENDFILE, "ENDFILE"}};

// Store the special symbols. The key is the symbols' string, and the value is the
// symbol's token type.
std::map<std::string, TokenType> spec_syms = 
        {{"+", PLUS},   {"-", MINUS},  {"*", MULTI},  {"/", DEVIDE},
         {"++", INC},   {"--", RED},   {"&&", LAND},  {"||", LOR},   
         {"!", LNOT},   {">", GT},     {">=", GE},    {"<", LT},    
         {"<=", LE},    {"==", EQ},    {"!=", NE},    {",", COMMA},  
         {";", SEMI},   {"=", ASSIGN}, {".", DOT},    {"\"", QUOTE},
         {"(", LPARTH}, {")", RPARTH}, {"[", LBRACK}, {"]", RBRACK}, 
         {"{", LBRACE}, {"}", RBRACE}};

// Store the reserved words (key words).
// As for the recognize process, the token will first be recognized as ID, and then
// use this map to judge whether the type is a key word. Something to be told, 'true'
// and 'false' is seen as const value, so the recognization of these two words doesn't
// need this map.
std::map<std::string, TokenType> resd_words = 
        {{"void", VOID},     {"int", INT},           {"char", CHAR}, 
         {"float", FLOAT},   {"double", DOUBLE},     {"bool", BOOL}, 
         {"string", STRING}, {"break", BREAK},       {"for", FOR}, 
         {"while", WHILE},   {"continue", CONTINUE}, {"if", IF}, 
         {"else", ELSE},     {"return", RETURN}};

typedef union {
    int ival;
    bool bval;
    char cval;
    double dval;
} Value;

extern void set_value(Value*, TokenType, void*);
extern void set_value_from_int(Value*, int);
extern void set_value_from_bool(Value*, bool);
extern void set_value_from_char(Value*, char);
extern void set_value_from_real(Value*, double);
extern void print_value(Value*, TokenType, std::ostream &os = std::cout);

void set_value(Value *value, TokenType type, void *val) {
    if (value == nullptr) {
        std::cerr << "  [Base.hpp] ERROR : set_value : value == nullptr" << std::endl;
        exit(1);
    }
    if (type == VOID || val == nullptr) {
        value->ival = 0;
    } else if (type == INT) {
        value->ival = *((int *)val);
    } else if (type == DOUBLE) {
        value->dval = *((double *)val);
    } else if (type == CHAR) {
        value->cval = *((char *)val);
    } else {
        std::cerr << "  [Base.hpp] ERROR : set_value : Unknown type" << std::endl;
        exit(1);
    }
}

void set_value_from_int(Value *value, int val) {
    set_value(value, INT, VOID(val));
}

void set_value_from_bool(Value *value, bool val) {
    set_value(value, BOOL, VOID(val));
}

void set_value_from_char(Value *value, char val) {
    set_value(value, CHAR, VOID(val));
}

void set_value_from_real(Value *value, double val) {
    set_value(value, DOUBLE, VOID(val));
}

void print_value(Value *value, TokenType type, std::ostream &os) {
    if (type == INT) {
        os << std::right << std::setw(10) << value->ival;
    } else if (type == BOOL) {
        os << std::right << std::setw(10) << value->bval;
    } else if (type == CHAR) {
        os << std::right << std::setw(10) << value->cval;
    } else if (type == DOUBLE) {
        os << std::right << std::setw(10) << value->dval;
    }
    os << std::endl;
}

#endif /* BASE_HPP */
