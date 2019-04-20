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

typedef union {
    int ival;
    bool bval;
    char cval;
    double dval;
} Value;

// Use Token type to query the standard show string of the type.
// This is used to print Token.
extern std::vector<std::pair<TokenType, std::string>> type_name; 

// Use Token type to query the standard symbol in grammar file.
// Warning: This is added in for gramma analysis and the vector is related to the
//          grammar file!
extern std::vector<std::pair<TokenType, std::string>> TokenGrammaSymVec;

// Store the special symbols. The key is the symbols' string, and the value is the
// symbol's token type.
extern std::map<std::string, TokenType> spec_syms; 

// Store the reserved words (key words).
// As for the recognize process, the token will first be recognized as ID, and then
// use this map to judge whether the type is a key word. Something to be told, 'true'
// and 'false' is seen as const value, so the recognization of these two words doesn't
// need this map.
extern std::map<std::string, TokenType> resd_words; 

extern void set_value(Value*, TokenType, void*);
extern void set_value_from_int(Value*, int);
extern void set_value_from_bool(Value*, bool);
extern void set_value_from_char(Value*, char);
extern void set_value_from_real(Value*, double);
extern void print_value(Value*, TokenType, std::ostream &os = std::cout);

#endif /* BASE_HPP */
