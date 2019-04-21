#include <fstream>

#include "lib/Base.hpp"
#include "lib/BasicFunc.hpp"
#include "lib/Item.hpp"
#include "lib/Production.hpp"
#include "lib/SymbolTable.hpp"
#include "lib/Token.hpp"
#include "lib/parser.hpp"
#include "lib/scanner.hpp"

vector<string> SymVec; // Store the Token's symbols.

// Shift Token sequence to standard symbols in grammar.txt.
void TokenShift() {
    for (auto token : TokenVec)
        SymVec.push_back(token2string(*token));
    SymVec.push_back("#");
}

int execute_ui_command(int argc, char *argv[]) {
    if (argc != 3)
        return EXIT_FAILURE;
    /* Lex Analysis */
    std::ofstream output("../file/log/lex_analysis.log");
    if (!output)
        return EXIT_FAILURE;
    std::cout << "Hello" << std::endl;
    scan_file(argv[2], output);
    output.close();
    output.open("../file/log/SymbolTableInfo.log");
    print_symbol_table(SymbolTable, output);
    output.close();
    /* Gramma Analysis */
    read_grammar("../file/grammar/grammar.txt");
    TokenShift(); // Shift the token sequence to standard symbols.
    output.open("../file/log/gramma_analysis.log");
    analysis(SymVec, output);
    output.close();
    output.open("../file/log/ReduceTableInfo.log");
    print_ReduceTable(output);
    output.close();
    output.open("../file/log/ShiftTableInfo.log");
    print_ShiftTable(output);
    output.close();
    output.open("../file/log/GotoTableInfo.log");
    print_GotoTable(output);
    output.close();
    return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {
    if (argc > 1 && !strcmp("-n", argv[1])) {
        return execute_ui_command(argc, argv);
    } else if (argc == 2 && !strcmp("-1", argv[1])) {
        scan_file("../test/test.cpp");
        read_grammar("../file/grammar/grammar.txt");
        TokenShift();
        analysis(SymVec);
    } else if (argc == 2 && !strcmp("-2", argv[1])) {
        scan_file("../test/test1.cpp");
        read_grammar("../file/grammar/std_grammar.txt");
        TokenShift();
        analysis(SymVec);
    }
    return EXIT_SUCCESS;
}
