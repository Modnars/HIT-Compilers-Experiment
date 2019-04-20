#include "lib/Base.hpp"
#include "lib/BasicFunc.hpp"
#include "lib/Item.hpp"
#include "lib/Production.hpp"
#include "lib/SymbolTable.hpp"
#include "lib/Token.hpp"

#include "lib/parser.hpp"
#include "lib/scanner.hpp"

int main(int argc, char *argv[]) {
    vector<string> SymVec;
    scan_file("../test/test.cpp");
    read_grammar("../file/grammar/grammar.txt");
    std::cout << TokenVec.size() << std::endl;
    for (auto token : TokenVec)
        SymVec.push_back(token2string(*token));
    SymVec.push_back("#");
    print_vec(SymVec);
    analysis(SymVec);
    return 0;
}
