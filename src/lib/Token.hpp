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

// Judge whether the id is reserved word.
//
// @param id The ID string need to be judged as reserved word or not
// @return true if the id is a reserved word
bool is_resd_word(const std::string &id) {
    if (resd_words.find(id) == resd_words.end())
        return false;
    return true;
}

// Judge whether the type is const value.
// 
// @param tt The token type need to be judged as const value type or not
// @return true if the type is a const value's type
bool is_cval_type(const TokenType &tt) {
    if (tt == CINT || tt == CBOOL || tt == CCHAR || tt == CFLOAT)
        return true;
    return false;
}

// Judge whether the token type is control token type.
//
// @param tt The token type need to be judged as control type or not
// @return true if the type is a control token type
bool is_ctrl_type(const TokenType &tt) {
    if (tt == UNKNOWN || tt == ERROR || tt == NONE || tt == ENDFILE)
        return true;
    return false;
}

// Get the specifical type name by token type.
//
// @param tt The token's specific type, and use this to get the token type's string
// @return the token type's string
std::string get_type_name(const TokenType &tt) {
    for (auto item : type_name) 
        if (item.first == tt)
            return item.second;
    return "";
}

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
    Token(const Token &t) : type(t.type) { copyUnion(t); }
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

Token &Token::operator=(const Token &t) {
    copyUnion(t);
    type = t.type;
    return *this;
}

Token &Token::operator=(int i) {
    ival = i;
    type = CINT;
    return *this;
}

Token &Token::operator=(bool b) {
    bval = b;
    type = CBOOL;
    return *this;
}

Token &Token::operator=(char c) {
    cval = c;
    type = CCHAR;
    return *this;
}

Token &Token::operator=(double d) {
    dval = d;
    type = CFLOAT;
    return *this;
}

void Token::copyUnion(const Token &t) {
    switch (t.type) {
        case CINT : ival = t.ival; break;
        case CBOOL : bval = t.bval; break;
        case CCHAR : cval = t.cval; break;
        case CFLOAT : dval = t.dval; break;
        case ID : sptr = t.sptr; break;
        default: break;
    }
}

// Get the specific Token object using token type (tt) and the string object which
// contains the token's information.
std::shared_ptr<Token> make_token(TokenType tt, std::string str) {
    if (tt == ID) {
        auto it = resd_words.find(str);
        // If the token is regard as a ID, need to judge whether it is a CBOOL,
        // such as "true" or "false". Besides, if need to reserve other key words, 
        // such as "nullptr", "NULL", also could judge at here.
        if (it == resd_words.end()) { 
            if (str == "true") {
                auto tk = std::make_shared<Token>(CBOOL, str);
                *tk = true;
                return tk;
            } else if (str == "false") {
                auto tk = std::make_shared<Token>(CBOOL, str);
                *tk = false;
                return tk;
            }
            return std::make_shared<Token>(tt, str);
        } else { // If is a keyword token
            tt = resd_words[str];
            return std::make_shared<Token>(tt, str);
        }
    } else if (tt == CNUM) {
        bool is_float = false;
        for (auto x : str) {
            if (x == '.') {
                is_float = true;
                break;
            }
        } 
        if (is_float) {
            auto tk = std::make_shared<Token>(CFLOAT, str);
            *tk = std::stod(str);
            return tk;
        } else {
            auto tk = std::make_shared<Token>(CINT, str);
            *tk = std::stoi(str);
            return tk;
        }
    } else if (tt == CCHAR) {
        auto tk = std::make_shared<Token>(CCHAR, str);
        if (str[1] != '\\')
            *tk = str[1];
        else {
            switch (str[2]) {
                case 'a':
                    *tk = '\a';
                    break;
                case 'b':
                    *tk = '\b';
                    break;
                case 'f':
                    *tk = '\f';
                    break;
                case 'n':
                    *tk = '\n';
                    break;
                case 'r':
                    *tk = '\r';
                    break;
                case 't':
                    *tk = '\t';
                    break;
                case 'v':
                    *tk = '\v';
                    break;
                case '\\':
                    *tk = '\\';
                    break;
                case '\?':
                    *tk = '\?';
                    break;
                case '\"':
                    *tk = '\"';
                    break;
                case '0':
                    *tk = '\0';
                    break;
                default:
                    break;
            }
        }
        return tk;
    } else {
        return std::make_shared<Token>(tt, str);
    }
}

// Overload the operator '<<', use this operator to print Token information formatly
std::ostream &operator<<(std::ostream &os, const Token &tk) {
    if (tk.type == ID) {
        os << std::left << std::setw(10) << tk.name << std::right << std::setw(10) 
           << get_type_name(tk.type) << std::setw(22) << tk.sptr << std::endl;
           return os;
    }
    os << std::left << std::setw(10) << tk.name << std::right << std::setw(10) 
       << get_type_name(tk.type) << std::setw(10) << tk.type << std::endl;
    return os;
}

#endif /* TOKEN_HPP */
