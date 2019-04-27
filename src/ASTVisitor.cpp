#include "lib/ASTVisitor.hpp"

void visit_node(std::shared_ptr<Node> &self, std::shared_ptr<Visitor> &visitor) {
    VisitFunc visit = nullptr;
    if (self == nullptr)
        return;
    self->child_count = 1;
    switch (self->kind) {
        case P0 :
            visit = visitor->P0;
            break;
        case P1 :
            visit = visitor->P1;
            break;
        case P2 :
            visit = visitor->P2;
            break;
        case P3 :
            visit = visitor->P3;
            break;
        case P4 :
            visit = visitor->P4;
            break;
        case P5 :
            visit = visitor->P5;
            break;
        case P6 :
            visit = visitor->P6;
            break;
        case P7 :
            visit = visitor->P7;
            break;
        case P8 :
            visit = visitor->P8;
            break;
        case P9 :
            visit = visitor->P9;
            break;
        case P10 :
            visit = visitor->P10;
            break;
        case P11 :
            visit = visitor->P11;
            break;
        case P12 :
            visit = visitor->P12;
            break;
        case P13 :
            visit = visitor->P13;
            break;
        case P14 :
            visit = visitor->P14;
            break;
        case P15 :
            visit = visitor->P15;
            break;
        case P16 :
            visit = visitor->P16;
            break;
        case P17 :
            visit = visitor->P17;
            break;
        case P18 :
            visit = visitor->P18;
            break;
        case P19 :
            visit = visitor->P19;
            break;
        case P20 :
            visit = visitor->P20;
            break;
        case P21 :
            visit = visitor->P21;
            break;
        case P22 :
            visit = visitor->P22;
            break;
        case P23 :
            visit = visitor->P23;
            break;
        case P24 :
            visit = visitor->P24;
            break;
        case P25 :
            visit = visitor->P25;
            break;
        case P26 :
            visit = visitor->P26;
            break;
        case P27 :
            visit = visitor->P27;
            break;
        case P28 :
            visit = visitor->P28;
            break;
        case P29 :
            visit = visitor->P29;
            break;
        case P30 :
            visit = visitor->P30;
            break;
        case P31 :
            visit = visitor->P31;
            break;
        case P32 :
            visit = visitor->P32;
            break;
        case P33 :
            visit = visitor->P33;
            break;
        case P34 :
            visit = visitor->P34;
            break;
        case P35 :
            visit = visitor->P35;
            break;
        case P36 :
            visit = visitor->P36;
            break;
        case P37 :
            visit = visitor->P37;
            break;
        case P38 :
            visit = visitor->P38;
            break;
        case P39 :
            visit = visitor->P39;
            break;
        case P40 :
            visit = visitor->P40;
            break;
        case P41 :
            visit = visitor->P41;
            break;
        case P42 :
            visit = visitor->P42;
            break;
        case P43 :
            visit = visitor->P43;
            break;
        case P44 :
            visit = visitor->P44;
            break;
        case P45 :
            visit = visitor->P45;
            break;
        case P46 :
            visit = visitor->P46;
            break;
        case P47 :
            visit = visitor->P47;
            break;
        default :
            break;
    }
    if (visit)
        visit(visitor, self);
}

void visit_node_children(std::shared_ptr<Node> &self, std::shared_ptr<Visitor> &visitor) {
    for (auto ptr = self; ptr != nullptr; ptr = ptr->sibling)
        visit_node(ptr, visitor);
}
