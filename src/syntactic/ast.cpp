

// 構文木

#include "./../lr_table/lr_table.hpp"
#include "./syntactic_analysis.hpp"
#include "./tree_control.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>

namespace Syntactic
{

    void cutParentheses(vSyntacticTree &tree, int parent_node_index, int current_node_index)
    {
        if (parent_node_index >= 0)
        {
            if (tree[current_node_index].token_label == is_id_NonterminalSymbol && tree[current_node_index].children.size() == 0)
            {
                cutChild(tree, parent_node_index, current_node_index);
            }
            else if (tree[current_node_index].token_label == is_id_TerminalSymbol && isTokenSkepSyntacticAnalysis(tree[current_node_index].token))
            {
                cutChild(tree, parent_node_index, current_node_index);
            }
        }
        for (int i = 0; i < tree[current_node_index].children.size(); i++)
        {
            int c_index = tree[current_node_index].children[i];
            cutParentheses(tree, current_node_index, c_index);
        }
        if (parent_node_index >= 0)
        {

            if (tree[current_node_index].token_label == is_id_NonterminalSymbol && tree[current_node_index].children.size() == 1)
            {
                shortParentChild(tree, parent_node_index, current_node_index);
            }
        }
    }

    void syntacticParseTree(vSyntacticTree cst, vSyntacticTree &ast)
    {
        vSyntacticTree construction_tree = cst;

        debugSyntacticAnalysisTree(construction_tree, false);
        printf("AST解析  - A\n");

        cutParentheses(construction_tree, -1, 0);
        debugSyntacticAnalysisTree(construction_tree, false);

        printf("AST解析  - B\n");

        ast = construction_tree;
        return;
    }
}
