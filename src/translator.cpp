#include "lib/ASTNode.hpp"

#include "lib/scanner.hpp"
#include "lib/parser.hpp"
#include "lib/translator.hpp"

std::shared_ptr<Node> Root;

void semantic(int ProdNo) {
    switch (ProdNo) {
        case 0 :
            Root = new_node(ProdVec[ProdNo]->name, );
            std::cerr << *ProdVec[ProdNo] << std::endl;
            std::cerr << idx << std::endl;
            std::cerr << *TokenVec[idx-1] << std::endl;
            break;
        case 1 :
            std::cerr << *ProdVec[ProdNo] << std::endl;
            std::cerr << idx << std::endl;
            std::cerr << *TokenVec[idx-1] << std::endl;
            break;
        default : 
            std::cerr << *ProdVec[ProdNo] << std::endl;
            std::cerr << idx << std::endl;
            std::cerr << *TokenVec[idx-1] << std::endl;
    }
}
