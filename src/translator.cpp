#include <stack>
#include <vector>

#include "lib/ASTNode.hpp"

#include "lib/scanner.hpp"
#include "lib/parser.hpp"
#include "lib/translator.hpp"

std::shared_ptr<Node> Root;

std::stack<std::shared_ptr<Node>> NodeStack;
//std::vector<std::shared_ptr<Node>> NodeVec;

void semantic(int ProdNo) {
    // The node to add, the const number node, the temp new node.
    std::shared_ptr<Node> tempnode, add_node; 
    switch (ProdNo) {
        case 0 : // Program -> P
            Root = new_node("Program[0]", NONE, nullptr);
            add_child(Root, NodeStack.top());
            NodeStack.pop();
            NodeStack.push(Root);
            break;
        case 1 : // P -> D P
            tempnode = new_node("P[1]", NONE, nullptr);
            add_child(tempnode, NodeStack.top());
            NodeStack.pop();
            add_child(tempnode, NodeStack.top());
            NodeStack.pop();
            NodeStack.push(tempnode);
            break;
        case 2 : // P -> S P // TODO
            tempnode = new_node("P[2]", NONE, nullptr);
            add_child(tempnode, NodeStack.top());
            NodeStack.pop();
            add_child(tempnode, NodeStack.top());
            NodeStack.pop();
            NodeStack.push(tempnode);
            break;
        case 3 : // P -> $ // TODO
            tempnode = new_node("P[3]", NONE, nullptr);
            add_node = new_node("$", NONE, nullptr);
            add_child(tempnode, add_node);
            NodeStack.push(tempnode);
            break;
        case 4 : // D -> proc X id ( M ) { P } // TODO
            tempnode = new_node("D[4]", NONE, nullptr);
            add_node = new_node("}", RBRACE, nullptr);
            add_child(tempnode, add_node); // Add '}'
            add_child(tempnode, NodeStack.top()); // Add 'P'
            NodeStack.pop();
            add_node = new_node("{", LBRACE, nullptr);
            add_child(tempnode, add_node); // Add '{'
            add_node = new_node(")", RPARTH, nullptr);
            add_child(tempnode, add_node); // Add ')'
            add_child(tempnode, NodeStack.top()); // Add 'M'
            NodeStack.pop();
            add_node = new_node("(", LPARTH, nullptr); 
            add_child(tempnode, add_node); // Add '('
            add_node = new_node("id", ID, TokenStack.top()->sptr); TokenStack.pop();
            add_child(tempnode, add_node); // Add 'id'
            add_child(tempnode, NodeStack.top()); // Add 'X'
            NodeStack.pop();
            add_node = new_node("proc", PROC, nullptr);
            add_child(tempnode, add_node); // Add 'proc'
            NodeStack.push(tempnode);
            break;
        case 5 : // D -> T id A ;
            tempnode = new_node("D[5]", NONE, nullptr);
            add_node = new_node(";", SEMI, nullptr);
            add_child(tempnode, add_node);
            add_child(tempnode, NodeStack.top());
            NodeStack.pop();
            add_node = new_node("id", ID, TokenStack.top()->sptr); TokenStack.pop(); 
            add_child(tempnode, add_node);
            add_child(tempnode, NodeStack.top());
            if (tempnode->sibling)
                std::cout << tempnode->sibling->name << std::endl;
            NodeStack.pop();
            NodeStack.push(tempnode);
            break;
        case 6 : // D -> record id { P } // TODO
            tempnode = new_node("D[6]", NONE, nullptr);
            add_node = new_node("}", RBRACE, nullptr);
            add_child(tempnode, add_node);
            add_child(tempnode, NodeStack.top());
            NodeStack.pop();
            add_node = new_node("{", LBRACE, nullptr);
            add_child(tempnode, add_node);
            add_node = new_node("id", ID, TokenStack.top()->sptr); TokenStack.pop();
            add_child(tempnode, add_node);
            add_node = new_node("record", RECORD, nullptr);
            add_child(tempnode, add_node);
            NodeStack.push(tempnode);
            break;
        case 7 : // A -> = F A // TODO
            tempnode = new_node("A[7]", NONE, nullptr);
            add_child(tempnode, NodeStack.top());
            NodeStack.pop();
            add_child(tempnode, NodeStack.top());
            NodeStack.pop();
            add_node = new_node("=", ASSIGN, nullptr);
            add_child(tempnode, add_node);
            NodeStack.push(tempnode);
            break;
        case 8 : // A -> , id A // TODO
            tempnode = new_node("A[8]", NONE, nullptr);
            add_child(tempnode, NodeStack.top());
            NodeStack.pop();
            add_node = new_node("id", ID, TokenStack.top()->sptr);
            TokenStack.pop();
            add_child(tempnode, add_node);
            add_node = new_node(",", COMMA, nullptr);
            add_child(tempnode, add_node);
            NodeStack.push(tempnode);
            break;
        case 9 : // A -> $
            tempnode = new_node("A[9]", NONE, nullptr);
            add_node = new_node("$", NONE, nullptr);
            add_child(tempnode, add_node);
            NodeStack.push(tempnode);
            break;
        case 10 : // M -> M , X id // TODO
            tempnode = new_node("M[10]", NONE, nullptr);
            add_node = new_node("id", ID, TokenStack.top()->sptr); TokenStack.pop();
            add_child(tempnode, add_node);
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            add_node = new_node(",", COMMA, nullptr);
            add_child(tempnode, add_node);
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            NodeStack.push(tempnode);
            break;
        case 11 : // M -> X id // TODO
            tempnode = new_node("M[11]", NONE, nullptr);
            add_node = new_node("id", ID, TokenStack.top()->sptr); TokenStack.pop();
            add_child(tempnode, add_node);
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            NodeStack.push(tempnode);
            break;
        case 12 : // M -> $ // TODO
            tempnode = new_node("M[12]", NONE, nullptr);
            add_node = new_node("$", NONE, nullptr);
            add_child(tempnode, add_node);
            NodeStack.push(tempnode);
            break;
        case 13 : // T -> X C
            tempnode = new_node("T[13]", NONE, nullptr);
            add_child(tempnode, NodeStack.top());
            NodeStack.pop();
            add_child(tempnode, NodeStack.top());
            NodeStack.pop();
            NodeStack.push(tempnode);
            break; 
        case 14 : // X -> int
            tempnode = new_node("X[14]", NONE, nullptr);
            add_node = new_node("int", INT, nullptr);
            add_child(tempnode, add_node);
            NodeStack.push(tempnode);
            break;
        case 15 : // X -> float // TODO
            tempnode = new_node("X[15]", NONE, nullptr);
            add_node = new_node("float", FLOAT, nullptr);
            add_child(tempnode, add_node);
            NodeStack.push(tempnode);
            break;
        case 16 : // C -> [ CINT ] C // TODO
            tempnode = new_node("C[16]", NONE, nullptr);
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            add_node = new_node("]", RBRACK, nullptr);
            add_child(tempnode, add_node);
            add_node = new_node("CINT", CINT, nullptr); // HERE
            std::cerr << *TokenStack.top() << std::endl;
            add_node->value.ival = TokenStack.top()->ival; TokenStack.pop();
            add_child(tempnode, add_node);
            add_node = new_node("[", LBRACK, nullptr);
            add_child(tempnode, add_node);
            NodeStack.push(tempnode);
            break;
        case 17 : // C -> $ // TODO
            tempnode = new_node("C[17]", NONE, nullptr);
            add_node = new_node("$", NONE, nullptr);
            add_child(tempnode, add_node);
            NodeStack.push(tempnode);
            break;
        case 18 : // S -> L = E ; // TODO
            tempnode = new_node("S[18]", NONE, nullptr);
            add_node = new_node(";", SEMI, nullptr);
            add_child(tempnode, add_node);
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            add_node = new_node("=", ASSIGN, nullptr);
            add_child(tempnode, add_node);
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            NodeStack.push(tempnode);
            break;
        case 19 : // S -> if B { S } R // TODO
            tempnode = new_node("S[19]", NONE, nullptr);
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            add_node = new_node("}", RBRACE, nullptr);
            add_child(tempnode, add_node); // Add '}'
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            add_node = new_node("{", LBRACE, nullptr);
            add_child(tempnode, add_node); // Add '{'
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            add_node = new_node("if", IF, nullptr);
            add_child(tempnode, add_node);
            NodeStack.push(tempnode);
            break;
        case 20 : // R -> else { S } // TODO
            tempnode = new_node("R[20]", NONE, nullptr);
            add_node = new_node("}", RBRACE, nullptr);
            add_child(tempnode, add_node); // Add '}'
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            add_node = new_node("{", LBRACE, nullptr);
            add_child(tempnode, add_node); // Add '{'
            add_node = new_node("else", ELSE, nullptr);
            add_child(tempnode, add_node);
            NodeStack.push(tempnode);
            break;
        case 21 : // R -> $ // TODO
            tempnode = new_node("R[21]", NONE, nullptr);
            add_node = new_node("$", NONE, nullptr);
            add_child(tempnode, add_node);
            NodeStack.push(tempnode);
            break;
        case 22 : // S -> while B { S } // TODO
            tempnode = new_node("S[22]", NONE, nullptr);
            add_node = new_node("}", RBRACE, nullptr);
            add_child(tempnode, add_node); // Add '}'
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            add_node = new_node("{", LBRACE, nullptr);
            add_child(tempnode, add_node); // Add '{'
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            add_node = new_node("while", WHILE, nullptr);
            add_child(tempnode, add_node);
            NodeStack.push(tempnode);
            break;
        case 23 : // S -> call id ( Elist ) // TODO
            tempnode = new_node("S[23]", NONE, nullptr);
            add_node = new_node(")", RPARTH, nullptr);
            add_child(tempnode, add_node); // Add ')'
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            add_node = new_node("(", LPARTH, nullptr); 
            add_child(tempnode, add_node); // Add '('
            add_node = new_node("id", ID, TokenStack.top()->sptr); TokenStack.pop();
            add_child(tempnode, add_node); // Add 'id'
            add_node = new_node("call", CALL, nullptr);
            add_child(tempnode, add_node);
            NodeStack.push(tempnode);
            break;
        case 24 : // S -> return E ; // TODO
            tempnode = new_node("S[24]", NONE, nullptr);
            add_node = new_node(";", SEMI, nullptr);
            add_child(tempnode, add_node);
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            add_node = new_node("return", RETURN, nullptr);
            add_child(tempnode, add_node);
            NodeStack.push(tempnode);
            break;
        case 25 : // S -> L [ CINT ] // TODO
            tempnode = new_node("S[25]", NONE, nullptr);
            add_node = new_node("]", RBRACK, nullptr);
            add_child(tempnode, add_node);
            add_node = new_node("CINT", CINT, nullptr); // HERE
            std::cerr << *TokenStack.top() << std::endl;
            add_node->value.ival = TokenStack.top()->ival; TokenStack.pop();
            add_child(tempnode, add_node);
            add_node = new_node("[", LBRACK, nullptr);
            add_child(tempnode, add_node);
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            NodeStack.push(tempnode);
            break;
        case 26 : // L -> id // TODO
            tempnode = new_node("L[26]", NONE, nullptr);
            add_node = new_node("id", ID, TokenStack.top()->sptr); TokenStack.pop();
            add_child(tempnode, add_node);
            NodeStack.push(tempnode);
            break;
        case 27 : // E -> E + G // TODO
            tempnode = new_node("E[27]", NONE, nullptr);
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            add_node = new_node("+", PLUS, nullptr);
            add_child(tempnode, add_node);
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            NodeStack.push(tempnode);
            break;
        case 28 : // E -> G // TODO
            tempnode = new_node("E[28]", NONE, nullptr);
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            NodeStack.push(tempnode);
            break;
        case 29 : // G -> G * F // TODO
            tempnode = new_node("G[29]", NONE, nullptr);
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            add_node = new_node("*", MULTI, nullptr);
            add_child(tempnode, add_node);
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            NodeStack.push(tempnode);
            break;
        case 30 : // G -> F // TODO
            tempnode = new_node("G[30]", NONE, nullptr);
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            NodeStack.push(tempnode);
            break;
        case 31 : // F -> ( E ) // TODO
            tempnode = new_node("F[31]", NONE, nullptr);
            add_node = new_node(")", RPARTH, nullptr);
            add_child(tempnode, add_node); // Add ')'
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            add_node = new_node("(", LPARTH, nullptr); 
            add_child(tempnode, add_node); // Add '('
            NodeStack.push(tempnode);
            break;
        case 32 : // F -> CINT
            tempnode = new_node("F[32]", NONE, nullptr);
            add_node = new_node("CINT", CINT, nullptr);
            std::cerr << *TokenStack.top() << std::endl;
            add_node->value.ival = TokenStack.top()->ival; TokenStack.pop();
            add_child(tempnode, add_node);
            NodeStack.push(tempnode);
            break;
        case 33 : // F -> id // TODO
            tempnode = new_node("F[33]", NONE, nullptr);
            add_node = new_node("id", ID, TokenStack.top()->sptr); TokenStack.pop();
            add_child(tempnode, add_node);
            NodeStack.push(tempnode);
            break;
        case 34 : // B -> ( B || B ) // TODO
            tempnode = new_node("B[34]", NONE, nullptr);
            add_node = new_node(")", RPARTH, nullptr);
            add_child(tempnode, add_node); // Add ')'
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            add_node = new_node("||", LOR, nullptr);
            add_child(tempnode, add_node);
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            add_node = new_node("(", LPARTH, nullptr); 
            add_child(tempnode, add_node); // Add '('
            NodeStack.push(tempnode);
            break;
        case 35 : // B -> ( B && B ) // TODO
            tempnode = new_node("B[35]", NONE, nullptr);
            add_node = new_node(")", RPARTH, nullptr);
            add_child(tempnode, add_node); // Add ')'
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            add_node = new_node("&&", LAND, nullptr);
            add_child(tempnode, add_node);
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            add_node = new_node("(", LPARTH, nullptr); 
            add_child(tempnode, add_node); // Add '('
            NodeStack.push(tempnode);
            break;
        case 36 : // B -> ! B  // TODO
            tempnode = new_node("B[36]", NONE, nullptr);
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            add_node = new_node("!", LNOT, nullptr);
            add_child(tempnode, add_node);
            NodeStack.push(tempnode);
            break;
        case 37 : // B -> ( B ) // TODO
            tempnode = new_node("B[37]", NONE, nullptr);
            add_node = new_node(")", RPARTH, nullptr);
            add_child(tempnode, add_node); // Add ')'
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            add_node = new_node("(", LPARTH, nullptr); 
            add_child(tempnode, add_node); // Add '('
            NodeStack.push(tempnode);
            break;
        case 38 : // B -> E relop E // TODO
            tempnode = new_node("B[38]", NONE, nullptr);
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            NodeStack.push(tempnode);
            break;
        case 39 : // B -> CBOOL // TODO
            tempnode = new_node("B[39]", NONE, nullptr);
            add_node = new_node("CBOOL", CBOOL, nullptr);
            if (!(TokenStack.top()->type == CBOOL))
                std::cerr << "Error! @ translator.cpp [376]" << std::endl;
            else 
                std::cerr << "Bool" << TokenStack.top()->bval << std::endl;
            add_node->value.bval = TokenStack.top()->bval;
            TokenStack.pop();
            add_child(tempnode, add_node);
            NodeStack.push(tempnode);
            break;
        case 40 : // relop -> < // TODO
            tempnode = new_node("Relop[41]", NONE, nullptr);
            add_node = new_node("<", LT, nullptr);
            add_child(tempnode, add_node);
            NodeStack.push(tempnode);
            break;
        case 41 : // relop -> <= // TODO
            tempnode = new_node("Relop[42]", NONE, nullptr);
            add_node = new_node("<=", LE, nullptr);
            add_child(tempnode, add_node);
            NodeStack.push(tempnode);
            break;
        case 42 : // relop -> >  // TODO
            tempnode = new_node("Relop[42]", NONE, nullptr);
            add_node = new_node(">", GT, nullptr);
            add_child(tempnode, add_node);
            NodeStack.push(tempnode);
            break;
        case 43 : // relop -> >= // TODO
            tempnode = new_node("Relop[43]", NONE, nullptr);
            add_node = new_node("<", GE, nullptr);
            add_child(tempnode, add_node);
            NodeStack.push(tempnode);
            break;
        case 44 : // relop -> == // TODO
            tempnode = new_node("Relop[44]", NONE, nullptr);
            add_node = new_node("==", EQ, nullptr);
            add_child(tempnode, add_node);
            NodeStack.push(tempnode);
            break;
        case 45 : // relop -> != // TODO
            tempnode = new_node("Relop[45]", NONE, nullptr);
            add_node = new_node("!=", NE, nullptr);
            add_child(tempnode, add_node);
            NodeStack.push(tempnode);
            break;
        case 46 : // Elist -> Elist , E // TODO
            tempnode = new_node("Elist[46]", NONE, nullptr);
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            add_node = new_node(",", COMMA, nullptr);
            add_child(tempnode, add_node);
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            NodeStack.push(tempnode);
            break;
        case 47 : // Elist -> E // TODO
            tempnode = new_node("Elist[47]", NONE, nullptr);
            add_child(tempnode, NodeStack.top()); NodeStack.pop();
            NodeStack.push(tempnode);
            break;
        default : 
            break;
    }
}

void translate(std::ostream &os) {
    os << NodeStack.size() << std::endl;
    print_nodes(Root, os);
    print_symbol_table(SymbolTable);
}

void gencode(std::ostream &os) {
    os << "[" << "]" << std::endl;
}
