

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
    bool isTokenSkepSyntacticAnalysis(string token_str)
    {
        const char *token = token_str.c_str();
        return (strchr("(){}[];,\'\"", token[0]) != 0);
    }

    bool deleteChild(vSyntacticTree &tree, int current_node_index)
    {
        tree[current_node_index].parent_node_index = -1;
        tree[current_node_index].token_label = is_id_Unnecessary;
    }

    bool moveChild(vSyntacticTree &tree, int parent_node_index, int parent_node_child_index, int move_node_index)
    {
        printf("\n%3d :  ***********  - moveChild p:%d pci:%d mn:%d pc-size:%d pc-換:%d\n", -2, parent_node_index, parent_node_child_index, move_node_index, tree[parent_node_index].children.size(), tree[parent_node_index].children[parent_node_child_index]);
        tree[parent_node_index].children[parent_node_child_index] = move_node_index;
        tree[move_node_index].setParent(parent_node_index);
    }

    bool climbChild(vSyntacticTree &tree, int parent_node_index, int current_node_index, int leftright)
    {
        int climb_node_index = tree[current_node_index].children[leftright];
        printf("%3d : - climbChild p:%d c:%d lr:%d pc-size:%d", -2, parent_node_index, current_node_index, leftright, tree[parent_node_index].children.size());
        printf(" pc-inside:(");
        for (int i = 0; i < tree[parent_node_index].children.size(); i++)
        {
            printf("%d ", tree[parent_node_index].children[i]);
            if (tree[parent_node_index].children[i] == current_node_index)
            {
                moveChild(tree, parent_node_index, i, climb_node_index);

                deleteChild(tree, current_node_index);
                printf(" - - - true pc:%d\n", i);
                return true;
            }
        }
        printf(" )");
        printf(" - - - false\n");
        debugSyntacticAnalysisTree(tree);
        return false;
    }

    bool cutChild(vSyntacticTree &tree, int parent_node_index, int delete_child_node)
    {
        printf("%3d : - cutChild p:%d d:%d", -2, parent_node_index, delete_child_node);
        for (int i = 0; i < tree[parent_node_index].children.size(); i++)
        {
            if (tree[parent_node_index].children[i] == delete_child_node)
            {
                tree[parent_node_index].children[i] = -1;

                deleteChild(tree, delete_child_node);
                printf(" - - - true pc:%d\n", i);
                return true;
            }
        }
        printf(" - - - false\n");
        debugSyntacticAnalysisTree(tree);
        return false;
    }

    bool cutSelfChildParent(vSyntacticTree &tree, int parent_node_index, int delete_child_node_index, int move_child_node_index)
    {
        printf("%3d : - cutSelfChildCommon d:%d m:%d ", -2, delete_child_node_index, move_child_node_index);

        SyntacticTreeNode delete_node = tree[delete_child_node_index];
        SyntacticTreeNode move_node = tree[move_child_node_index];

        for (int i = 0; i < tree[parent_node_index].children.size(); i++)
        {
            printf("%d ", tree[parent_node_index].children[i]);
            if (tree[parent_node_index].children[i] == delete_child_node_index)
            {

                // 上位層への切り替え要求
                moveChild(tree, parent_node_index, i, move_child_node_index);

                tree[delete_child_node_index].token_label = is_id_Unnecessary;

                printf(" - parent true pc:%d", i);
                return true;
            }
        }

        printf(" - - - parent false\n");
        debugSyntacticAnalysisTree(tree);
        return false;
    }

    bool cutSelfChildFlow(vSyntacticTree &tree, int flow_parent, int flow)
    {
        if (flow_parent < 0 || flow < 0)
        {
            printf(" - - - notflow true\n");
            return true;
        }

        int new_flow_parent = -1;

        while (true)
        {

            if ((tree[flow_parent].children[0] < 0 && tree[flow_parent].children[1] < 0))
            {
                new_flow_parent = flow_parent;
                break;
            }
            if ((tree[flow_parent].children[0] >= 0 && tree[flow_parent].children[1] >= 0))
            {
                flow_parent = tree[flow_parent].children[1];
                continue;
            }

            if (tree[flow_parent].children[0] >= 0)
            {
                flow_parent = tree[flow_parent].children[0];
                continue;
            }

            if (tree[flow_parent].children[1] >= 0)
            {
                flow_parent = tree[flow_parent].children[1];
                continue;
            }

            printf(" - - - flow_parent false\n");
            debugSyntacticAnalysisTree(tree);
            return false;
        }
        printf(" - flow_parent true fp:%d mv:%d", new_flow_parent, flow);

        moveChild(tree, new_flow_parent, 0, flow);
        printf("\n");
        return true;
    }
}