

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

    void syntacticParseTree(vSyntacticTree cst, vSyntacticTree &ast)
    {
        vSyntacticTree construction_tree = cst;

        for (int i = 0; i < construction_tree.size(); i++)
        {
            construction_tree[i].children = {-2, -2};
        }
        printf("AST解析 - A\n");
        // recursionSyntacticBinaryTree(cst, construction_tree, 0);
        debugSyntacticAnalysisTree(construction_tree, false);
        printf("AST解析  - B\n");
        // recursionCutBinaryTree(construction_tree, 0);
        // debugSyntacticAnalysisTree(construction_tree, false);
        printf("AST解析  - C\n");
        // debugSyntacticAnalysisTree(construction_tree);
        debugSyntacticAnalysisTree(construction_tree, false);
        printf("AST解析  - D\n");

        ast = construction_tree;
        return;
    }
}
