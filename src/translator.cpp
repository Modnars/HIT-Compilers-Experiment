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
        case 2 : // P -> S P
            break;
        case 3 : // P -> $
            tempnode = new_node("P[3]", NONE, nullptr);
            add_node = new_node("$", NONE, nullptr);
            add_child(tempnode, add_node);
            NodeStack.push(tempnode);
            break;
        case 4 : // D -> proc X id ( M ) { P }
            break;
        case 5 : // D -> T id A ;
            tempnode = new_node("D[5]", NONE, nullptr);
            add_node = new_node(";", SEMI, nullptr);
            add_child(tempnode, add_node);
            add_child(tempnode, NodeStack.top());
            NodeStack.pop();
            add_node = new_node("id", ID, TokenStack.top()->sptr);
            TokenStack.pop(); // TODO
            add_child(tempnode, add_node);
            add_child(tempnode, NodeStack.top());
            if (tempnode->sibling)
                std::cout << tempnode->sibling->name << std::endl;
            NodeStack.pop();
            NodeStack.push(tempnode);
            break;
        case 6 : // D -> record id { P } // TODO
            break;
        case 7 : // A -> = F A
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
        case 10 : // M -> M , X id
            break;
        case 11 :
            break;
        case 12 : 
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
            add_node = new_node("INT", INT, nullptr);
            add_child(tempnode, add_node);
            NodeStack.push(tempnode);
            break;
        case 15 : 
            break;
        case 16 : 
            break;
        case 17 : // C -> $
            tempnode = new_node("C[17]", NONE, nullptr);
            add_node = new_node("$", NONE, nullptr);
            add_child(tempnode, add_node);
            NodeStack.push(tempnode);
            break;
        case 18 : 
            break;
        case 19 : 
            break;
        case 20 : 
            break;
        case 21 : 
            break;
        case 22 : 
            break;
        case 23 : 
            break;
        case 24 : 
            break;
        case 25 : 
            break;
        case 32 : // F -> CINT
            tempnode = new_node("F[32]", NONE, nullptr);
            add_node = new_node("CINT", CINT, nullptr);
            add_node->value.ival = TokenVec[idx-1]->ival;
            add_child(tempnode, add_node);
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
