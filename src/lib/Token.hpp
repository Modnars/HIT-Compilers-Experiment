#ifndef TOKEN_HPP
#define TOKEN_HPP

#include "Base.hpp"
#include "SymbolTable.hpp"

class Token;
extern bool is_resd_word(const std::string &);
extern bool is_cval_type(const TokenType &);
extern bool is_ctrl_type(const TokenType &);
extern std::string get_type_name(const TokenType &);
extern std::shared_ptr<Token> make_token(TokenType, std::string);
extern std::ostream &operator<<(std::ostream &, const Token &);
extern std::string token2string(const Token &);

class Token {
public:
    TokenType type;
    std::string name;
    union {
        int ival;
        char cval;
        bool bval;
        double dval;
        std::shared_ptr<Symbol> sptr;
    };

    Token() : type(CINT), ival(0) { }
    Token(const Token &t) : type(t.type), name(t.name) { copyUnion(t); }
    Token &operator=(const Token &t);
    Token(const TokenType &tt, const std::string &nm) : 
            type(tt), name(nm), ival(tt) { } // New added.
    ~Token() { }

    Token &operator=(int);
    Token &operator=(char);
    Token &operator=(bool);
    Token &operator=(double);
    void copyUnion(const Token&);
};

#endif /* TOKEN_HPP */
