#ifndef ASTVISITOR_HPP
#define ASTVISITOR_HPP

#include "ASTNode.hpp"

class Visitor {
public:
    void (*P0) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
    void (*P1) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
    void (*P2) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
    void (*P3) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
    void (*P4) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
    void (*P5) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
    void (*P6) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
    void (*P7) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
    void (*P8) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
    void (*P9) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
    void (*P10) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
    void (*P11) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
    void (*P12) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
    void (*P13) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
    void (*P14) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
    void (*P15) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
    void (*P16) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
    void (*P17) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
    void (*P18) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
    void (*P19) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
    void (*P20) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
    void (*P21) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
    void (*P22) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
    void (*P23) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
    void (*P24) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
    void (*P25) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
    void (*P26) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
    void (*P27) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
    void (*P28) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
    void (*P29) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
    void (*P30) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
    void (*P31) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
    void (*P32) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
    void (*P33) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
    void (*P34) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
    void (*P35) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
    void (*P36) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
    void (*P37) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
    void (*P38) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
    void (*P39) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
    void (*P40) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
    void (*P41) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
    void (*P42) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
    void (*P43) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
    void (*P44) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
    void (*P45) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
    void (*P46) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
    void (*P47) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
};

typedef void (*VisitFunc) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);

extern void visit_node(std::shared_ptr<Node> &, std::shared_ptr<Visitor> &);
extern void visit_node_children(std::shared_ptr<Node> &, std::shared_ptr<Visitor> &);

#endif /* ASTVISTOR_HPP */
