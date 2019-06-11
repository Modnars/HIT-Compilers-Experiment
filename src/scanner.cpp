#include <iostream>
#include <fstream>

#include <cctype>
#include <cstdlib>
#include <cstring>

#include "lib/Token.hpp"
#include "lib/SymbolTable.hpp"
#include "lib/scanner.hpp"

// Mark the current line's number.
int line_no = 1; 
// Store the recognized token sequence. [Global]
std::vector<std::shared_ptr<Token>> TokenVec;
// Store the global symbol table. [Global]
auto SymbolTable = std::make_shared<Symbol>("SymbolTable"); 

// Define the DFA's states.
typedef enum {
    START, ISCOMMENT, ISID, ISNUM, ISSTR, DONE
} State;

/**
 * Add the recognized token to TokenVec.
 * 
 * @param tt The recognized token's special type.
 * @param str The recognized string content.
 *
 * @call bool is_ctrl_type(const TokenType &);
 * @call std::shared_ptr<Token> make_token(const TokenType &, const std::string &);
 * @call std::shared_ptr<Symbol> insert_symbol(std::shared_ptr<Symbol>, 
 *               std::shared_ptr<Symbol>);
 * @visit TokenVec
 */
void addToken(const TokenType &tt, const std::string &str) {
    if (is_ctrl_type(tt)) return;
    auto token = make_token(tt, str);
    if (token->type == ID) {
        auto symbol = std::make_shared<Symbol>(str);
        symbol->decl_line_no = line_no;
        token->sptr = insert_symbol(SymbolTable, symbol);
    }
    TokenVec.push_back(token);
}

/**
 * Judge whether the character is the escape character when have met '\'.
 *
 * @param ch The character need to judge when have met '\'.
 * @return true if '\ch' is an escape character, otherwise return false.
 */
bool is_esc_char(char ch) {
    if (ch == 'a' || ch == 'b' || ch == 'f' || ch == 'n' || ch == 'r' || ch == 't' ||
        ch == 'v' || ch == '\\' || ch == '\?' || ch == '\"' || ch == '0')
        return true;
    return false;
}

/**
 * Write the program's log information as running.
 *
 * @param os The output stream.
 * @param tt The token's special type.
 * @param str The recognized string.
 */
void write_log(std::ostream &os, const TokenType &tt, const std::string &str) {
    if (tt == NONE)
        os << "Comment: [line:" << line_no-1 << "]  " << str << std::endl;
    else if (tt == ERROR)
        os << "Error: [line:" << line_no << "]  " << str << std::endl;
    else if (tt == UNKNOWN)
        os << "Unknown: [line:" << line_no << "]  " << str << std::endl;
    else if (tt == ENDFILE)
        os << std::endl;
    else 
        os << *TokenVec.back();
}

/**
 * Recognize all the tokens and store them from source file (as a input stream).
 *
 * @param is The input stream which could be a file stream.
 * @param os The program's output stream.
 *
 * @call void addToken(const TokenType &, const std::string &);
 * @call void write_log(std::ostream &, const TokenType &, const std::string &);
 */
void getToken(std::istream &is, std::ostream &os = std::cout) {
    TokenType tt;
    State state = START;
    std::string str = "";
    bool single_line = false;
    bool is_int = true;
    while (state != DONE) {
        char ch = is.get();
        switch (state) {

            case START :
                if (isalpha(ch) || ch == '_') {
                    state = ISID;
                    str += ch;
                } else if (isdigit(ch)) {
                    state = ISNUM;
                    str += ch;
                } else if (ch == '\t' || ch == ' ') { 
                    /* Nothing to do when meet white character */
                } else if (ch == '\n') {
                    ++line_no;
                } else if (ch == '/' && is.peek() == '*') {
                    str += ch;
                    str += is.get();
                    state = ISCOMMENT;
                } else if (ch == '/' && is.peek() == '/') {
                    str += ch;
                    str += is.get();
                    state = ISCOMMENT;
                    single_line = true;
                } else {
                    state = DONE;
                    str += ch;
                    switch (ch) {
                        case EOF : tt = ENDFILE; break;
                        case '+' : 
                            if (is.peek() == '+') {
                                tt = INC;
                                str += is.get();
                            } else tt = PLUS;
                            break;
                        case '-' : 
                            if (is.peek() == '-') {
                                tt = RED;
                                str += is.get();
                            } else tt = MINUS; 
                            break;
                        case '\'' : 
                            ch = is.get();
                            if (ch == '\n' || ch == '\t') {
                                tt = ERROR;
                                state = DONE;
                                break;
                            } else if (ch == '\'') {
                                str += ch;
                                tt = ERROR;
                                state = DONE;
                                break;
                            } else if (ch == EOF) {
                                tt = ERROR;
                                state = DONE;
                                is.putback(ch);
                                break;
                            }
                            if (is.peek() == '\'') {
                                str += ch;
                                str += is.get();
                                tt = CCHAR; 
                                break;
                            } else if (ch == '\\') {
                                str += ch;
                                ch = is.get();
                                str += ch;
                                if (is_esc_char(ch)) {
                                    if (is.peek() == '\'') {
                                        str += is.get();
                                        tt = CCHAR;
                                    } else {
                                        str += is.get();
                                        tt = ERROR;
                                    }
                                    state = DONE;
                                } else {
                                    if (is.peek() == '\'')
                                        str += is.get();
                                    tt = ERROR;
                                    state = DONE;
                                }
                            } else {
                                tt = ERROR;
                                state = DONE;
                                is.putback(ch);
                            }
                            break;
                        case '*' : tt = MULTI;  break;
                        case '/' : tt = DEVIDE; break;
                        case ',' : tt = COMMA;  break;
                        case ';' : tt = SEMI;   break;
                        case '.' : tt = DOT;    break;
                        case '(' : tt = LPARTH; break;
                        case ')' : tt = RPARTH; break;
                        case '[' : tt = LBRACK; break;
                        case ']' : tt = RBRACK; break;
                        case '{' : tt = LBRACE; break;
                        case '}' : tt = RBRACE; break;
                        case '"' :
                            state = ISSTR;
                            break;
                        case '=' :
                            if (is.peek() == '=') {
                                str += is.get();
                                tt = EQ;
                            } else tt = ASSIGN;
                            break;
                        case '>' :
                            if (is.peek() == '=') {
                                str += is.get();
                                tt = GE;
                            } else tt = GT;
                            break;
                        case '<' :
                            if (is.peek() == '=') {
                                str += is.get();
                                tt = LE;
                            } else tt = LT;
                            break;
                        case '!' :
                            if (is.peek() == '=') {
                                str += is.get();
                                tt = NE;
                            } else tt = LNOT;
                            break;
                        case '&' :
                            if (is.peek() == '&') {
                                str += is.get();
                                tt = LAND;
                            } else tt = AND;
                            break;
                        case '|' :
                            if (is.peek() == '|') {
                                str += is.get();
                                tt = LOR;
                            } else tt = OR;
                            break;
                        default :
                            tt = UNKNOWN;
                            break;
                    } /* switch (ch) */
                } 
                break; // Case START

            case ISCOMMENT :
                if (ch == '\n') {
                    ++line_no;
                    if (single_line) {
                        state = DONE;
                        tt = NONE;
                        break;
                    } 
                }
                if (ch == '*' && is.peek() == '/') {
                    str += ch;
                    ch = is.get();
                    state = DONE;
                    tt = NONE;
                } else if (is.eof()) {
                    state = DONE;
                    tt = ERROR;
                }
                str += ch;
                break; /* Case ISCOMMENT */

            case ISSTR:
                str += ch;
                if (ch == '\"') {
                    tt = CSTRING;
                    state = DONE;
                }
                if (ch == EOF) {
                    tt = ERROR;
                    state = DONE;
                    is.putback(ch);
                    break;
                } else if (is.peek() == '\"' && ch == '\\') {
                    str += is.get();
                } else if (ch == '\n') {
                    ++line_no;
                    tt = ERROR;
                    state = DONE;
                } 
                break;

            case ISNUM :
                if (!isdigit(ch)) {
                    if (ch == '.' && is_int) {
                        str += ch;
                        if (isdigit(is.peek())) {
                            is_int = false;
                        } else {
                            char tmp = is.peek();
                            if (tmp == ' ' || tmp == '\t' || tmp == '\n') {
                                tt = ERROR;
                                state = DONE;
                            }
                        }
                        break;
                    }
                    if (isalpha(ch) || ch == '.' || ch == '_') {
                        str += ch;
                        char tmp = is.peek();
                        if (tmp == ' ' || tmp == '\t' || tmp == '\n') {
                            tt = ERROR;
                            state = DONE;
                            break;
                        }
                        tt = ERROR;
                    } else {
                        if (tt != ERROR) 
                            tt = CNUM;
                        is.putback(ch);
                        state = DONE;
                    } 
                    break;
                }
                str += ch;
                break; /* Case ISNUM */

            case ISID :
                if (!isalpha(ch)) {
                    if (isdigit(ch) || ch == '_') {
                        str += ch;
                        char tmp = is.peek();
                        if (tmp == ' ' || tmp == '\t' || tmp == '\n') {
                            if (tt != ERROR) {
                                tt = ID;
                                state = DONE;
                                break;
                            }
                            state = DONE;
                        }
                    } else {
                        if (tt != ERROR)
                            tt = ID;
                        is.putback(ch);
                        state = DONE;
                    }
                    break;
                }
                str += ch;
                break; /* Case ID */

            case DONE :
                break; /* Case DONE */

            default :
                state = DONE;
                tt = ERROR;
                break;
        } /* switch (state) */
    } /* while (state != DONE) */
    // End loop when state is DONE or be broken by other exception.
    if (state == DONE) {
        addToken(tt, str); // Write the right token's information.
        write_log(os, tt, str); // Write the ERROR, NONE or UNKNOWN token information.
    }
}

/**
 * Scan the source file. [Global]
 * 
 * @param filename The source file's path.
 * @param os The program's output stream.
 * @return EXIT_FAILURE when meet any error, otherwise return EXIT_SUCCESS.
 *
 * @call void getToken(std::istream &, std::ostream &os = std::cout);
 * @call void print_symbol_table(std::shared_ptr<Symbol> &);
 */
int scan_file(const std::string &filename, std::ostream &os) {
    TokenVec.clear();
    std::ifstream input(filename);
    if (!input) {
        std::cerr << "[T_T] Error when opening file \"" << filename
            << "\"." << std::endl;
        return EXIT_FAILURE;
    }
    while (true) {
        getToken(input, os);
        if (!input) {
            std::cerr << "[^_^] Process file \"" << filename 
                      << "\" finished." << std::endl;
            break;
        }
    }
    std::cerr << "[*_*] Recognised Token's number: " << TokenVec.size()
              << std::endl << std::endl;
    input.close();
    print_symbol_table(SymbolTable);
    return EXIT_SUCCESS;
}
