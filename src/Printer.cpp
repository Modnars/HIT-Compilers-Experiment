#include <sstream>

#include "lib/Code.hpp"
#include "lib/Printer.hpp"

#include "lib/scanner.hpp"

std::vector<std::shared_ptr<Code>> CodeVec;
int nextquad;
int temp_no;

std::string get_num(int id) {
    std::stringstream ss;
    std::string res;
    ss << id;
    ss >> res;
    return res;
}

std::string get_num(double id) {
    std::stringstream ss;
    std::string res;
    ss << id;
    ss >> res;
    return res;
}

std::string gen_name(int id) {
    std::stringstream ss;
    std::string res;
    ss << id;
    ss >> res;
    return "t" + res;
}

std::shared_ptr<Visitor> new_printer() {
    std::shared_ptr<Visitor> visitor = std::make_shared<Visitor>();
    visitor->P0 = &func0;
    visitor->P1 = &func1;
    visitor->P2 = &func2;
    visitor->P3 = &func3;
    visitor->P4 = &func4;
    visitor->P5 = &func5;
    visitor->P6 = &func6;
    visitor->P7 = &func7;
    visitor->P8 = &func8;
    visitor->P9 = &func9;
    visitor->P10 = &func10;
    visitor->P11 = &func11;
    visitor->P12 = &func12;
    visitor->P13 = &func13;
    visitor->P14 = &func14;
    visitor->P15 = &func15;
    visitor->P16 = &func16;
    visitor->P17 = &func17;
    visitor->P18 = &func18;
    visitor->P19 = &func19;
    visitor->P20 = &func20;
    visitor->P21 = &func21;
    visitor->P22 = &func22;
    visitor->P23 = &func23;
    visitor->P24 = &func24;
    visitor->P25 = &func25;
    visitor->P26 = &func26;
    visitor->P27 = &func27;
    visitor->P28 = &func28;
    visitor->P29 = &func29;
    visitor->P30 = &func30;
    visitor->P31 = &func31;
    visitor->P32 = &func32;
    visitor->P33 = &func33;
    visitor->P34 = &func34;
    visitor->P35 = &func35;
    visitor->P36 = &func36;
    visitor->P37 = &func37;
    visitor->P38 = &func38;
    visitor->P39 = &func39;
    visitor->P40 = &func40;
    visitor->P41 = &func41;
    visitor->P42 = &func42;
    visitor->P43 = &func43;
    visitor->P44 = &func44;
    visitor->P45 = &func45;
    visitor->P46 = &func46;
    visitor->P47 = &func47;
    return visitor;
}

// Program -> P
void func0(std::shared_ptr<Visitor> &visitor, std::shared_ptr<Node> &node) {
    std::cout << "Func 0 @ Printer." << node->name << std::endl;
    nextquad = 0;
    temp_no = 0;
    SymbolTable->offset = 0;
    visit_node_children(node->child, visitor);
}

// P -> D P
void func1(std::shared_ptr<Visitor> &visitor, std::shared_ptr<Node> &node) {
    std::cout << "Func 1 @ Printer." << node->name << std::endl;
    visit_node_children(node->child, visitor);
}

// P -> S P
void func2(std::shared_ptr<Visitor> &visitor, std::shared_ptr<Node> &node) {
    std::cout << "Func 2 @ Printer." << node->name << std::endl;
    visit_node_children(node->child, visitor);
}

// P -> $
void func3(std::shared_ptr<Visitor> &visitor, std::shared_ptr<Node> &node) {
    std::cout << "Func 3 @ Printer." << node->name << std::endl;
    visit_node_children(node->child, visitor);
}

// D -> proc X id ( M ) { P }
void func4(std::shared_ptr<Visitor> &visitor, std::shared_ptr<Node> &node) {
}

// D -> T id A ;
void func5(std::shared_ptr<Visitor> &visitor, std::shared_ptr<Node> &node) {
    std::cout << "Func 5 @ Printer." << node->name << std::endl;
    visit_node_children(node->child, visitor);
    auto id = node->child->sibling;
    auto T = id->sibling;
    if (T->type == INT) {
        node->child->type = INT;
        id->symbol->type = INT;
        id->symbol->width = 4;
        id->symbol->offset = SymbolTable->offset;
        SymbolTable->offset += 4;
    } else if (T->type == FLOAT) {
        node->child->type = FLOAT;
        id->symbol->type = FLOAT;
        id->symbol->width = 8;
        id->symbol->offset = SymbolTable->offset;
        SymbolTable->offset += 8;
    } else {
        std::cerr << "Error! @ Printer: func5" << std::endl;
    }
    if (node->child->child) { // Generate code.
        auto code = std::make_shared<Code>();
        code->result = id->symbol->name;
        code->action = "=";
        code->arg1 = gen_name(temp_no++);
        code->arg2 = "_";
        std::cout << *code << std::endl;
    }
}

// D -> record id { P }
void func6(std::shared_ptr<Visitor> &visitor, std::shared_ptr<Node> &node) {
}

// A -> = F A
void func7(std::shared_ptr<Visitor> &visitor, std::shared_ptr<Node> &node) {
    std::cout << "Func 7 @ Printer." << node->name << std::endl;
    visit_node_children(node->child, visitor);
    auto code = std::make_shared<Code>();
    if (node->child->sibling->type == CINT) {
        code->action = "=";
        code->arg1 = get_num(node->child->sibling->value.ival);
        code->arg2 = "_";
        code->result = gen_name(temp_no);
    } else if (node->child->sibling->type == CFLOAT) {
        code->action = "=";
        code->arg1 = get_num(node->child->sibling->value.dval);
        code->arg2 = "_";
        code->result = gen_name(temp_no);
    } else if (node->child->sibling->type == ID) {
        code->action = "=";
        code->arg1 = node->child->sibling->symbol->name;
        code->arg2 = "_";
        code->result = gen_name(temp_no);
    }
    std::cout << *code << std::endl;
}

// A -> , id A
void func8(std::shared_ptr<Visitor> &visitor, std::shared_ptr<Node> &node) {
    std::cout << "Func 8 @ Printer." << node->name << std::endl;
    node->child->type = node->type;
    node->child->sibling->symbol->type = node->type;
    if (node->type == INT) {
        node->child->sibling->symbol->width = 4;
        node->child->sibling->symbol->offset = SymbolTable->offset;
        SymbolTable->offset += 4;
    } else if (node->type == FLOAT) {
        node->child->sibling->symbol->width = 8;
        node->child->sibling->symbol->offset = SymbolTable->offset;
        SymbolTable->offset += 8;
    }
    visit_node_children(node->child, visitor);
}

// A -> $
void func9(std::shared_ptr<Visitor> &visitor, std::shared_ptr<Node> &node) {
    std::cout << "Func 9 @ Printer." << node->name << std::endl;
}

// M -> M , X id
void func10(std::shared_ptr<Visitor> &visitor, std::shared_ptr<Node> &node) {
}

// M -> X id
void func11(std::shared_ptr<Visitor> &visitor, std::shared_ptr<Node> &node) {
}

// M -> $
void func12(std::shared_ptr<Visitor> &visitor, std::shared_ptr<Node> &node) {
}

// T -> X C
void func13(std::shared_ptr<Visitor> &visitor, std::shared_ptr<Node> &node) {
    std::cout << "Func 13 @ Printer." << node->name << std::endl;
}

// X -> int
void func14(std::shared_ptr<Visitor> &visitor, std::shared_ptr<Node> &node) {
    std::cout << "Func 14 @ Printer." << node->name << std::endl;
    node->type = INT;
}

// X -> float
void func15(std::shared_ptr<Visitor> &visitor, std::shared_ptr<Node> &node) {
    std::cout << "Func 15 @ Printer." << node->name << std::endl;
    node->type = FLOAT;
}

// C -> [ CINT ] C
void func16(std::shared_ptr<Visitor> &visitor, std::shared_ptr<Node> &node) {
}

// C -> $
void func17(std::shared_ptr<Visitor> &visitor, std::shared_ptr<Node> &node) {
    std::cout << "P17 from Printer." << node->name << std::endl;
}

// S -> L = E ;
void func18(std::shared_ptr<Visitor> &visitor, std::shared_ptr<Node> &node) {
    std::cout << "P18 from Printer." << node->name << std::endl;
    visit_node_children(node->child, visitor);
    auto code = std::make_shared<Code>();
    code->action = "=";
    code->arg1 = gen_name(temp_no++);
    code->arg2 = "_";
    code->result = gen_name(temp_no);
    std::cout << *code << std::endl;
}
// S -> if B { S } R
void func19(std::shared_ptr<Visitor> &visitor, std::shared_ptr<Node> &node) {
}
// R -> else { S }
void func20(std::shared_ptr<Visitor> &visitor, std::shared_ptr<Node> &node) {
}
// R -> $
void func21(std::shared_ptr<Visitor> &visitor, std::shared_ptr<Node> &node) {
}
// S -> while B { S }
void func22(std::shared_ptr<Visitor> &visitor, std::shared_ptr<Node> &node) {
}
// S -> call id ( Elist ) ;
void func23(std::shared_ptr<Visitor> &visitor, std::shared_ptr<Node> &node) {
}
// S -> return E ;
void func24(std::shared_ptr<Visitor> &visitor, std::shared_ptr<Node> &node) {
}
// L -> L [ CINT ] 
void func25(std::shared_ptr<Visitor> &visitor, std::shared_ptr<Node> &node) {
}
// L -> id
void func26(std::shared_ptr<Visitor> &visitor, std::shared_ptr<Node> &node) {
}
// E -> E + G
void func27(std::shared_ptr<Visitor> &visitor, std::shared_ptr<Node> &node) {
    std::cout << "Func27 @ Printer." << std::endl;
    visit_node_children(node->child, visitor);
}
// E -> G
void func28(std::shared_ptr<Visitor> &visitor, std::shared_ptr<Node> &node) {
}
// G -> G * F
void func29(std::shared_ptr<Visitor> &visitor, std::shared_ptr<Node> &node) {
}
// G -> F
void func30(std::shared_ptr<Visitor> &visitor, std::shared_ptr<Node> &node) {
}
// G -> ( E )
void func31(std::shared_ptr<Visitor> &visitor, std::shared_ptr<Node> &node) {
}
// F -> CINT
void func32(std::shared_ptr<Visitor> &visitor, std::shared_ptr<Node> &node) {
}
// F -> id
void func33(std::shared_ptr<Visitor> &visitor, std::shared_ptr<Node> &node) {
}
// B -> ( B || B )
void func34(std::shared_ptr<Visitor> &visitor, std::shared_ptr<Node> &node) {
}
// B -> ( B && B ) 
void func35(std::shared_ptr<Visitor> &visitor, std::shared_ptr<Node> &node) {
}
// B -> ! B
void func36(std::shared_ptr<Visitor> &visitor, std::shared_ptr<Node> &node) {
}
// B -> ( B )
void func37(std::shared_ptr<Visitor> &visitor, std::shared_ptr<Node> &node) {
}
// B -> E relop E
void func38(std::shared_ptr<Visitor> &visitor, std::shared_ptr<Node> &node) {
}
// B -> CBOOL
void func39(std::shared_ptr<Visitor> &visitor, std::shared_ptr<Node> &node) {
}
// relop -> <
void func40(std::shared_ptr<Visitor> &visitor, std::shared_ptr<Node> &node) {
}
// relop -> <=
void func41(std::shared_ptr<Visitor> &visitor, std::shared_ptr<Node> &node) {
}
// relop -> >
void func42(std::shared_ptr<Visitor> &visitor, std::shared_ptr<Node> &node) {
}
// relop -> >=
void func43(std::shared_ptr<Visitor> &visitor, std::shared_ptr<Node> &node) {
}
// relop -> ==
void func44(std::shared_ptr<Visitor> &visitor, std::shared_ptr<Node> &node) {
}
// relop -> !=
void func45(std::shared_ptr<Visitor> &visitor, std::shared_ptr<Node> &node) {
}
// relop -> Elist , E
void func46(std::shared_ptr<Visitor> &visitor, std::shared_ptr<Node> &node) {
}
// relop -> E
void func47(std::shared_ptr<Visitor> &visitor, std::shared_ptr<Node> &node) {
}
