#include <stack>
#include <vector>
#include <sstream>

#include "lib/ASTNode.hpp"
#include "lib/BasicFunc.hpp"

#include "lib/scanner.hpp"
#include "lib/parser.hpp"
#include "lib/translator.hpp"

std::vector<std::shared_ptr<Code>> CodeVec;
std::shared_ptr<Node> Root;
int nextquad;

// Store the temp middle nodes of AST.
std::stack<std::shared_ptr<Node>> NodeStack;

template <typename T>
std::string get_val(T val) {
    std::stringstream ss;
    ss << val;
    std::string res;
    ss >> res;
    return res;
}

void backpatch(std::vector<int> lst, int quad) {
    for (auto idx : lst)
        CodeVec[idx]->result = get_val(quad);
}

std::vector<int> makelist() {
    std::vector<int> res = { nextquad };
    return res;
}

std::vector<int> merge(std::vector<int> lst1, std::vector<int> lst2) {
    std::vector<int> ret;
    for (auto var : lst1)
        ret.push_back(var);
    for (auto var : lst2)
        ret.push_back(var);
    return ret;
}

std::string new_temp() {
    static int i = 0;
    std::stringstream ss;
    std::string res;
    ss << i;
    ss >> res;
    ++i;
    return "t" + res;
}

std::shared_ptr<Code> new_code() {
    return std::make_shared<Code>();
}

void semantic(int ProdNo) {
    // The node to add, the const number node, the temp new node.
    std::shared_ptr<Node> tempnode, add_node, B1, B2, S1, S2, W1, W2, N; 
    std::shared_ptr<Code> code;
    TokenType _type;
    switch (ProdNo) {
        case 0 : // Program -> P
            tempnode = new_node("Program[0]", NONE, nullptr);
            tempnode->kind = P0;
            SymbolTable->offset = 0;
            Root = tempnode;
            NodeStack.pop();
            NodeStack.push(tempnode);
            break;
        case 1 : // P -> D P
            tempnode = new_node("P[1]", NONE, nullptr);
            tempnode->kind = P1;
            NodeStack.pop();
            NodeStack.pop();
            NodeStack.push(tempnode);
            break;
        case 2 : // P -> S P // TODO
            tempnode = new_node("P[2]", NONE, nullptr);
            tempnode->kind = P2;
            NodeStack.pop(); NodeStack.pop();
            NodeStack.push(tempnode);
            break;
        case 3 : // P -> $ // TODO
            tempnode = new_node("P[3]", NONE, nullptr);
            tempnode->kind = P3;
            NodeStack.push(tempnode);
            break;
        case 4 : // D -> proc X id ( M ) { P } // TODO
            tempnode = new_node("D[4]", PROC, nullptr);
            tempnode->kind = P4;
            add_child(tempnode, NodeStack.top()); NodeStack.pop(); // Add 'P'
            add_child(tempnode, NodeStack.top()); NodeStack.pop(); // Add 'M'
            add_node = new_node("id", ID, TokenStack.top()->sptr); TokenStack.pop();
            add_node->kind = P48;
            add_child(tempnode, add_node); // Add 'id'
            add_child(tempnode, NodeStack.top()); NodeStack.pop(); // Add 'X'
            NodeStack.push(tempnode);
            break;
        case 5 : // D -> T id A ;
            tempnode = new_node("D[5]", NONE, nullptr); // TODO Make sure the type.
            tempnode->kind = P5;
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            add_node = new_node(TokenStack.top()->sptr->name, ID, TokenStack.top()->sptr); 
            TokenStack.pop();  add_node->kind = P48; // Mark the node is terminal.
            add_child(tempnode, add_node);
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            NodeStack.push(tempnode);
            // Fill the specific id information.
            add_node->symbol->type = add_node->sibling->type; // id's type from T
            _type = add_node->symbol->type;
            if (_type == INT) {
                add_node->symbol->width = 4;
                add_node->symbol->offset = SymbolTable->offset;
                SymbolTable->offset += 4;
            } else if (_type == FLOAT) {
                add_node->symbol->width = 8;
                add_node->symbol->offset = SymbolTable->offset;
                SymbolTable->offset += 8;
            }
            if (tempnode->child->type == ASSIGN) {
                code = new_code();
                code->action = "=";
                code->arg1 = tempnode->child->name;
                code->arg2 = "_";
                code->result = add_node->symbol->name;
                CodeVec.push_back(code);
                ++nextquad;
            } else if (tempnode->child->type == COMMA) {
                auto ptr = tempnode->child;
                while (ptr->type == COMMA) {
                    auto tmp = ptr->child->sibling;
                    tmp->symbol->type = _type;
                    if (_type == INT) {
                        tmp->symbol->width = 4;
                        tmp->symbol->offset = SymbolTable->offset;
                        SymbolTable->offset += 4;
                    } else if (_type == FLOAT) {
                        tmp->symbol->width = 8;
                        tmp->symbol->offset = SymbolTable->offset;
                        SymbolTable->offset += 8;
                    }
                    ptr = ptr->child;
                }
            }
            break;
        case 6 : // D -> record id { P } // TODO
            tempnode = new_node("D[6]", RECORD, nullptr); // TODO Make sure the type.
            tempnode->kind = P6;
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            add_node = new_node("id", ID, TokenStack.top()->sptr); TokenStack.pop();
            add_node->kind = P48;
            add_child(tempnode, add_node);
            NodeStack.push(tempnode);
            break;
        case 7 : // A -> = F A // TODO
            tempnode = new_node("null", ASSIGN, nullptr);
            tempnode->kind = P7;
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            tempnode->name = NodeStack.top()->name;
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            NodeStack.push(tempnode);
            break;
        case 8 : // A -> , id A 
            tempnode = new_node("A[8]", COMMA, nullptr); // TODO Make sure the type.
            tempnode->kind = P8;
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            add_node = new_node("id", ID, TokenStack.top()->sptr); TokenStack.pop();
            add_node->kind = P48;
            add_child(tempnode, add_node);
            NodeStack.push(tempnode);
            break;
        case 9 : // A -> $
            tempnode = new_node("$", NONE, nullptr);
            tempnode->kind = P9;
            NodeStack.push(tempnode);
            break;
        case 10 : // M -> M , X id // TODO
            tempnode = new_node("M[10]", NONE, nullptr); // TODO Make sure the type.
            tempnode->kind = P10;
            add_node = new_node("id", ID, TokenStack.top()->sptr); TokenStack.pop();
            add_node->kind = P48;
            add_child(tempnode, add_node);
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            NodeStack.push(tempnode);
            break;
        case 11 : // M -> X id // TODO
            tempnode = new_node("M[11]", NONE, nullptr);
            tempnode->kind = P11;
            add_node = new_node("id", ID, TokenStack.top()->sptr); TokenStack.pop();
            add_node->symbol->type = NodeStack.top()->type;
            if (NodeStack.top()->type == INT) {
                add_node->symbol->width = 4;
                add_node->symbol->offset = SymbolTable->offset;
                SymbolTable->offset += 4;
            } else if (NodeStack.top()->type == FLOAT) {
                add_node->symbol->width = 8;
                add_node->symbol->offset = SymbolTable->offset;
                SymbolTable->offset += 8;
            }
            add_node->kind = P48;
            add_child(tempnode, add_node);
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            NodeStack.push(tempnode);
            break;
        case 12 : // M -> $ // TODO
            tempnode = new_node("M[12]", NONE, nullptr);
            tempnode->kind = P12;
            NodeStack.push(tempnode);
            break;
        case 13 : // T -> X C // TODO for array and matrix
            tempnode = new_node("T[13]", NONE, nullptr);
            tempnode->kind = P13;
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            tempnode->type = NodeStack.top()->type;
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            NodeStack.push(tempnode);
            break; 
        case 14 : // X -> int
            tempnode = new_node("X[14]", INT, nullptr);
            tempnode->kind = P14;
            NodeStack.push(tempnode);
            break;
        case 15 : // X -> float // TODO
            tempnode = new_node("X[15]", FLOAT, nullptr);
            tempnode->kind = P15;
            NodeStack.push(tempnode);
            break;
        case 16 : // C -> [ CINT ] C // TODO
            tempnode = new_node("C[16]", NONE, nullptr);
            tempnode->kind = P16;
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            add_node = new_node("CINT", CINT, nullptr); add_node->kind = P48;
            add_node->value.ival = TokenStack.top()->ival; TokenStack.pop();
            add_child(tempnode, add_node);
            NodeStack.push(tempnode);
            break;
        case 17 : // C -> $ // TODO
            tempnode = new_node("C[17]", NONE, nullptr);
            tempnode->kind = P17;
            NodeStack.push(tempnode);
            break;
        case 18 : // S -> L = E ; // TODO
            tempnode = new_node("S[18]", ASSIGN, nullptr);
            tempnode->kind = P18;
            code = new_code();
            code->action = "=";
            code->arg1 = NodeStack.top()->name; NodeStack.pop();
            code->arg2 = "_";
            code->result = NodeStack.top()->name; NodeStack.pop();
            CodeVec.push_back(code);
            ++nextquad;
            NodeStack.push(tempnode);
            break;
        case 19 : // S -> if B { W S N } R // TODO
            tempnode = new_node("S[19]", IF, nullptr);
            if (NodeStack.top()->name == "NULL") { // Single if statement.
                NodeStack.pop();
                N = NodeStack.top(); NodeStack.pop();
                S1 = NodeStack.top(); NodeStack.pop();
                W1 = NodeStack.top(); NodeStack.pop();
                B1 = NodeStack.top(); NodeStack.pop();
                backpatch(B1->truelist, W1->value.ival);
                tempnode->nextlist = merge(B1->falselist, S1->nextlist);
            } else if (NodeStack.top()->name == "else") {
                NodeStack.pop();
                S2 = NodeStack.top(); NodeStack.pop();
                W2 = NodeStack.top(); NodeStack.pop();
                N = NodeStack.top(); NodeStack.pop();
                S1 = NodeStack.top(); NodeStack.pop();
                W1 = NodeStack.top(); NodeStack.pop();
                B1 = NodeStack.top(); NodeStack.pop();
                backpatch(B1->truelist, W1->value.ival);
                backpatch(B1->falselist, W2->value.ival);
                tempnode->nextlist = merge(S1->nextlist, merge(N->nextlist, S2->nextlist));
            }
            NodeStack.push(tempnode);
            break;
        case 20 : // R -> else { W S } // TODO
            tempnode = new_node("else", ELSE, nullptr);
            NodeStack.push(tempnode);
            break;
        case 21 : // R -> $ // TODO
            tempnode = new_node("NULL", NONE, nullptr);
            NodeStack.push(tempnode);
            break;
        case 22 : // S -> while B { S } // TODO
            tempnode = new_node("S[22]", WHILE, nullptr);
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            NodeStack.push(tempnode);
            break;
        case 23 : // S -> call id ( Elist ) // TODO
            tempnode = new_node("S[23]", CALL, nullptr);
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            add_node = new_node("id", ID, TokenStack.top()->sptr); TokenStack.pop();
            add_node->kind = P48;
            add_child(tempnode, add_node); // Add 'id'
            NodeStack.push(tempnode);
            break;
        case 24 : // S -> return E ; // TODO
            tempnode = new_node("S[24]", RETURN, nullptr);
            tempnode->kind = P24;
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            NodeStack.push(tempnode);
            break;
        case 25 : // S -> L [ CINT ] // TODO
            tempnode = new_node("S[25]", NONE, nullptr);
            tempnode->kind = P25;
            add_node = new_node("CINT", CINT, nullptr); // HERE
            add_node->kind = P48;
            add_node->value.ival = TokenStack.top()->ival; TokenStack.pop();
            add_child(tempnode, add_node);
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            NodeStack.push(tempnode);
            break;
        case 26 : // L -> id // TODO
            tempnode = new_node(TokenStack.top()->sptr->name, ID, 
                TokenStack.top()->sptr); TokenStack.pop();
            tempnode->kind = P26;
            NodeStack.push(tempnode);
            break;
        case 27 : // E -> E + G // TODO
            tempnode = new_node(new_temp(), PLUS, nullptr);
            tempnode->kind = P27;
            code = new_code();
            code->action = "+";
            code->arg2 = NodeStack.top()->name; NodeStack.pop();
            code->arg1 = NodeStack.top()->name; NodeStack.pop();
            code->result = tempnode->name;
            CodeVec.push_back(code);
            ++nextquad;
            NodeStack.push(tempnode);
            break;
        case 28 : // E -> G 
            NodeStack.top()->kind = P28;
            break;
        case 29 : // G -> G * F // TODO
            tempnode = new_node(new_temp(), MULTI, nullptr);
            tempnode->kind = P29;
            code = new_code();
            code->action = "*";
            code->arg2 = NodeStack.top()->name; NodeStack.pop();
            code->arg1 = NodeStack.top()->name; NodeStack.pop();
            code->result = tempnode->name;
            CodeVec.push_back(code);
            ++nextquad;
            NodeStack.push(tempnode);
            break;
        case 30 : // G -> F 
            NodeStack.top()->kind = P30;
            break;
        case 31 : // F -> ( E ) 
            NodeStack.top()->kind = P31;
            break;
        case 32 : // F -> CINT
            tempnode = new_node(new_temp(), CINT, nullptr);
            tempnode->kind = P32;
            tempnode->value.ival = TokenStack.top()->ival; TokenStack.pop();
            code = new_code();
            code->action = "=";
            code->arg1 = get_val(tempnode->value.ival);
            code->arg2 = "_";
            code->result = tempnode->name;
            CodeVec.push_back(code);
            ++nextquad;
            NodeStack.push(tempnode);
            break;
        case 33 : // F -> id 
            tempnode = new_node(TokenStack.top()->sptr->name, ID, 
                TokenStack.top()->sptr); TokenStack.pop();
            tempnode->kind = P33;
            NodeStack.push(tempnode);
            break;
        case 34 : // B -> ( B || Q B ) 
            tempnode = new_node("B[34]", LOR, nullptr);
            tempnode->kind = P34;
            B2 = NodeStack.top(); NodeStack.pop();
            add_node = NodeStack.top(); NodeStack.pop();
            B1 = NodeStack.top(); NodeStack.pop();
            backpatch(B1->falselist, add_node->value.ival);
            tempnode->truelist = merge(B1->truelist, B2->truelist);
            tempnode->falselist = B2->falselist;
            NodeStack.push(tempnode);
            break;
        case 35 : // B -> ( B && Q B ) // TODO
            tempnode = new_node("B[35]", LAND, nullptr);
            tempnode->kind = P35;
            B2 = NodeStack.top(); NodeStack.pop();
            add_node = NodeStack.top(); NodeStack.pop();
            B1 = NodeStack.top(); NodeStack.pop();
            backpatch(B1->truelist, add_node->value.ival);
            tempnode->truelist = B2->truelist;
            tempnode->falselist = merge(B1->falselist, B2->falselist);
            NodeStack.push(tempnode);
            break;
        case 36 : // B -> ! B  // TODO
            tempnode = new_node("B[36]", LNOT, nullptr);
            tempnode->kind = P36;
            add_node = NodeStack.top(); NodeStack.pop();
            tempnode->truelist = add_node->falselist;
            tempnode->falselist = add_node->truelist;
            NodeStack.push(tempnode);
            break;
        case 37 : // B -> ( B ) // TODO
            NodeStack.top()->kind = P37;
            break;
        case 38 : // B -> E relop E // TODO
            tempnode = new_node("B[38]", NONE, nullptr);
            tempnode->kind = P38;
            B2 = NodeStack.top(); NodeStack.pop();
            add_node = NodeStack.top(); NodeStack.pop();
            B1 = NodeStack.top(); NodeStack.pop();
            tempnode->truelist = makelist(); ++nextquad;
            tempnode->falselist = makelist();
            code = new_code();
            code->action = "J" + add_node->name;
            code->arg1 = B1->name;
            code->arg2 = B2->name;
            code->result = "goto -";
            CodeVec.push_back(code); ++nextquad;
            code = new_code();
            code->action = "J ";
            code->arg1 = "_";
            code->arg2 = "_";
            code->result = "goto -";
            CodeVec.push_back(code); 
            NodeStack.push(tempnode);
            break;
        case 39 : // B -> CBOOL // TODO
            tempnode = new_node("B[39]", CBOOL, nullptr);
            tempnode->kind = P39;
            tempnode->value.bval = TokenStack.top()->bval;
            TokenStack.pop();
            if (tempnode->value.bval)
                tempnode->truelist = makelist();
            else 
                tempnode->falselist = makelist();
            code = new_code();
            code->action = "J";
            code->arg1 = "_";
            code->arg2 = "_";
            code->result = "_";
            CodeVec.push_back(code);
            ++nextquad;
            NodeStack.push(tempnode);
            break;
        case 40 : // relop -> < // TODO
            tempnode = new_node("<", LT, nullptr);
            tempnode->kind = P40;
            NodeStack.push(tempnode);
            break;
        case 41 : // relop -> <= // TODO
            tempnode = new_node("<=", LE, nullptr);
            tempnode->kind = P41;
            NodeStack.push(tempnode);
            break;
        case 42 : // relop -> >  // TODO
            tempnode = new_node(">", GT, nullptr);
            tempnode->kind = P42;
            NodeStack.push(tempnode);
            break;
        case 43 : // relop -> >= // TODO
            tempnode = new_node(">=", GE, nullptr);
            tempnode->kind = P43;
            NodeStack.push(tempnode);
            break;
        case 44 : // relop -> == // TODO
            tempnode = new_node("==", EQ, nullptr);
            tempnode->kind = P44;
            NodeStack.push(tempnode);
            break;
        case 45 : // relop -> != // TODO
            tempnode = new_node("!=", NE, nullptr);
            tempnode->kind = P45;
            NodeStack.push(tempnode);
            break;
        case 46 : // Elist -> Elist , E // TODO
            tempnode = new_node("Elist[46]", NONE, nullptr);
            tempnode->kind = P46;
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            NodeStack.push(tempnode);
            break;
        case 47 : // Elist -> E // TODO
            tempnode = new_node("Elist[47]", NONE, nullptr);
            tempnode->kind = P47;
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            NodeStack.push(tempnode);
            break;
        case 48 : // Q -> $
            tempnode = new_node("Q.quad", NONE, nullptr);
            tempnode->kind = P48;
            tempnode->value.ival = nextquad;
            NodeStack.push(tempnode);
            break;
        case 49 : // W -> $
            tempnode = new_node("W.quad", NONE, nullptr);
            tempnode->kind = P48;
            tempnode->value.ival = nextquad;
            NodeStack.push(tempnode);
            break;
        case 50 : // N -> $
            tempnode = new_node("N.quad", NONE, nullptr);
            tempnode->kind = P48;
            tempnode->nextlist = makelist();
            code = new_code();
            code->action = "J ";
            code->arg1 = "_";
            code->arg2 = "_";
            code->result = "goto -";
            CodeVec.push_back(code); ++nextquad;
            NodeStack.push(tempnode);
            break;
        default : 
            break;
    }
}

void translate(std::ostream &os) {
    os << NodeStack.size() << std::endl;
//    print_nodes(Root, os);
//    print_symbol_table(SymbolTable);
    for (int i = 0; i < CodeVec.size(); ++i)
        os << std::setw(3) << i << " " << *CodeVec[i] << std::endl;
}
