#ifndef ASTVISTOR_HPP
#define ASTVISTOR_HPP

#include "ASTNode.hpp"

class Visitor {
public:
    void (*visit_program) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
    void (*visit_program_decl) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
    void (*visit_var_decl) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
    void (*visit_var_decl_list) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
    void (*visit_id) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
    void (*visit_id_list) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
    void (*visit_if_stmt) (std::shared_ptr<Visitor> &, std::shared_ptr<Node> &);
};

#endif /* ASTVISTOR_HPP */
