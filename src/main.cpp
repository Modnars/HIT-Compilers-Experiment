#include <fstream>

#include "lib/parser.hpp"
#include "lib/scanner.hpp"
#include "lib/translator.hpp"

int execute_ui_command(int argc, char *argv[]) {
    if (argc != 3)
        return EXIT_FAILURE;
    /* Lex Analysis */
    std::ofstream output("../file/log/lex_analysis.log");
    if (!output)
        return EXIT_FAILURE;
    scan_file(argv[2], output);
    output.close();
    output.open("../file/log/SymbolTableInfo.log");
    print_symbol_table(SymbolTable, output);
    output.close();
    /* Gramma Analysis */
    read_grammar("../file/grammar/grammar.txt");
    output.open("../file/log/gramma_analysis.log");
    parse(TokenVec, output);
    output.close();
    output.open("../file/log/ReduceTableInfo.log");
    print_ReduceTable(output);
    output.close();
    output.open("../file/log/GotoTableInfo.log");
    print_GotoTable(output);
    output.close();
    output.open("../file/log/ShiftTableInfo.log");
    print_ShiftTable(output);
    output.close();
    /* Semantic Analysis */
    output.open("../file/log/Code.log");
    translate(false, output);
    output.close();
    output.open("../file/log/semantic_analysis.log");
    print_symbol_table(SymbolTable, output);
    output.close();
    return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {
    if (argc > 1 && !strcmp("-n", argv[1])) {
        return execute_ui_command(argc, argv);
    } else {
        scan_file("../test/test.cpp");
        read_grammar("../file/grammar/grammar.txt");
        parse(TokenVec);
        translate();
        print_symbol_table(SymbolTable);
    }
    return EXIT_SUCCESS;
}
