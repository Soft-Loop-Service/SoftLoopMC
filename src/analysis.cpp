#include "./lexical/lexical_analysis.hpp"
#include "./syntactic/syntactic_analysis.hpp"
#include "./definition.hpp"
#include "./lr_table/lr_table.hpp"
#include "./BNF/bnf.hpp"
#include "./input_table.hpp"

#include "./scl/scl.hpp"
#include "./trans_kit/trans_kit.hpp"

#include "./bytecode/translator/translator.hpp"

// source_code_file_name
// table_file_name
// bytecode
int main(int argc, char *argv[])
{
    char *source_code = new char[source_code_size * sizeof(char)];
    string table_file_name = argv[2];
    // string workspace_directory = argv[3];

    loadText(source_code, argv[1], source_code_size);
    string source_code_file_name = argv[1];

    LexicalAnalysis::vLexicalToken token_string_vector = LexicalAnalysis::lexSyntax(source_code);

    LRTable::LRTableMultilayer LR_table_multilayer;
    inputTable(table_file_name, LR_table_multilayer);

    printf("syntacticAnalysis\n");

    LRTable::vReduceFormula syntactic_analysis_formula = {};
    Syntactic::syntacticAnalysis(LR_table_multilayer, token_string_vector, syntactic_analysis_formula);

    printf("syntacticPurseTree\n");
    Syntactic::vSyntacticTree syntactic_analysis_tree = {};

    Syntactic::syntacticAnalysisTree(syntactic_analysis_formula, syntactic_analysis_tree, token_string_vector);
    Syntactic::vSyntacticTree cst = syntactic_analysis_tree;
    Syntactic::vSyntacticTree ast = {};
    Syntactic::syntacticParseTree(cst, ast);

    LanguageSpecifications::Scl::SclExpr scl_expr(ast);
    LanguageSpecifications::parse(scl_expr);
    TransSystem::TransKit trans_kit;
    string bytecode_name = argv[3];

    Bytecode::Translator::translator(ast, bytecode_name);

    // LanguageIntermediateSystem::Intermediate intermediate_system(scl_expr, trans_kit);
    // LanguageIntermediateSystem::intermediate(scl_expr, intermediate_system);

    printf("analysis - end\n");
}
