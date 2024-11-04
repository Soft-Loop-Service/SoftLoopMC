

// 構文木

#include "./../lr_table/lr_table.hpp"
#include "./syntactic_analysis.hpp"

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

    /// @brief シンプルにノード間関係を変更するメソッド
    /// @param tree グラフ(木)
    /// @param parent_node_index 親要素
    /// @param parent_node_child_index 親要素の子要素index 変更するノード
    /// @param move_node_index 移動するノード
    /// @return

    bool deleteChild(vSyntacticTree &tree, int current_node_index)
    {
        tree[current_node_index].parent_node_index = -1;
        tree[current_node_index].token_label = is_id_Unnecessary;
    }

    /// @brief 親要素の組み換え
    /// @param tree
    /// @param parent_node_index 親要素
    /// @param parent_node_child_index 親要素に指定されている子要素リストのindex
    /// @param move_node_index 新しい子要素
    /// @return
    bool moveChild(vSyntacticTree &tree, int parent_node_index, int parent_node_child_index, int move_node_index)
    {
        printf("\n%3d :  ***********  - moveChild p:%d pci:%d mn:%d pc-size:%d pc-換:%d\n", -2, parent_node_index, parent_node_child_index, move_node_index, tree[parent_node_index].children.size(), tree[parent_node_index].children[parent_node_child_index]);
        tree[parent_node_index].children[parent_node_child_index] = move_node_index;
        tree[move_node_index].setParent(parent_node_index);
    }

    /// @brief 自分自身を削除して、親要素と子要素をつなげる
    /// @param tree
    /// @param parent_node_index
    /// @param current_node_index
    /// @param leftright
    /// @return
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

    /// @brief 自分自身を削除して、親要素が保持する子要素リストにも抹消要求をする
    /// @param tree
    /// @param parent_node_index
    /// @param delete_child_node
    /// @return
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

    /// @brief 子要素が複数ある時の、親要素の接続処理
    /// @param tree
    /// @param parent_node_index
    /// @param delete_child_node_index
    /// @param move_child_node_index
    /// @return
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

    /// @brief 子要素が複数ある時の、孫要素→子要素接続処理
    /// @param tree
    /// @param flow_parent 親候補
    /// @param flow 移動要素
    /// @param leftright 左結合/右結合(巻き上げ要求と逆)
    /// @return
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

    /// @brief 左巻き上げ・以下2分木であることが条件
    /// @param tree
    /// @param parent_node_index 親ノード
    /// @param delete_child_node_index 削除するノード(自身)
    /// @param move_child_node_index 自身のところに持ち上げる子要素
    /// @return
    bool cutSelfChildLeft(vSyntacticTree &tree, int parent_node_index, int delete_child_node_index, int move_child_node_index)
    {
        SyntacticTreeNode delete_node = tree[delete_child_node_index];
        SyntacticTreeNode move_node = tree[move_child_node_index];

        printf("%3d : - cutSelfChildLeft p:%d d:%d m:%d\n", -2, parent_node_index, delete_child_node_index, move_child_node_index);
        bool run_parent_check = cutSelfChildParent(tree, parent_node_index, delete_child_node_index, move_child_node_index);

        if (!run_parent_check)
        {
            return false;
        }

        // 二分木であることを維持するため、巻き上げられる子要素の子要素(孫要素)を孫要素の末端に移動させる必要がある
        int flow_parent = move_child_node_index;
        int flow = delete_node.children[1];

        bool run_flow_check = cutSelfChildFlow(tree, flow_parent, flow);

        // bool run_flow_check = cutSelfChildFlow(tree, parent_node_index, delete_child_node_index, move_child_node_index, flow_parent, flow_right_node_index);

        return run_flow_check;
    }

    void cutNonterminalNode(vSyntacticTree &construction_tree, int current_node_index)
    {

        SyntacticTreeNode current_node = construction_tree[current_node_index];
        int parent_node_index = current_node.parent_node_index;
        int children_size = current_node.children.size();
        if (parent_node_index < 0)
        {
            return;
        }
        if (current_node.token_label != is_id_NonterminalSymbol)
        {
            return;
        }
        int left_child_node = construction_tree[current_node_index].children[0];
        int right_child_node = construction_tree[current_node_index].children[1];

        printf("%3d : 非末端記号削除 %d %d\n", current_node_index, left_child_node, right_child_node);

        // 子が両方とも存在しない
        if (left_child_node < 0 && right_child_node < 0)
        {
            // 自分の親に対して、自分の存在を抹消するよう要求する
            cutChild(construction_tree, parent_node_index, current_node_index);
            debugSyntacticAnalysisTree(construction_tree, false);
            return;
        }
        // 子が両方とも存在する
        if (left_child_node >= 0 && right_child_node >= 0)
        {
            cutSelfChildLeft(construction_tree, parent_node_index, current_node_index, left_child_node);
            debugSyntacticAnalysisTree(construction_tree, false);
            return;
        }

        // 左の子が存在する(右の子は存在しない)
        if (left_child_node >= 0)
        {
            climbChild(construction_tree, parent_node_index, current_node_index, 0);
            debugSyntacticAnalysisTree(construction_tree, false);
            return;
        }
        // 右の子が存在する(左の子は存在しない)
        if (right_child_node >= 0)
        {
            climbChild(construction_tree, parent_node_index, current_node_index, 1);
            debugSyntacticAnalysisTree(construction_tree, false);
            return;
        }
    }

    void parseBinaryRotateTree(vSyntacticTree &construction_tree, int current_node_index)
    {
        printf("%3d :(開始) 二分木回転構築(置換)\n", current_node_index);
        SyntacticTreeNode current_node = construction_tree[current_node_index];

        construction_tree[current_node_index].children = {-2, -2};

        int i = 0;
        int count = 0;
        int parent = current_node_index;
        while (i < current_node.children.size())
        {
            int child_node_index = current_node.children[i];
            SyntacticTreeNode child_node = construction_tree[child_node_index];

            int c_left = construction_tree[parent].children[0];
            int c_right = construction_tree[parent].children[1];

            if (count % 2 == 0 && c_left >= 0)
            {
                count++;
                parent = c_left;
                continue;
            }

            if (count % 2 == 1 && c_right >= 0)
            {
                count++;
                parent = c_right;
                continue;
            }

            if (count % 2 == 0)
            {

                printf("%3d : 二分木構築(置換) 左 旧:%d -> 親:%d 換:%d\n", child_node_index, current_node_index, parent, c_left);

                moveChild(construction_tree, parent, 0, child_node_index);
            }
            if (count % 2 == 1)
            {

                printf("%3d : 二分木構築(置換) 右 旧:%d -> 親:%d 換:%d\n", child_node_index, current_node_index, parent, c_right);

                moveChild(construction_tree, parent, 1, child_node_index);
                parent = child_node_index;
            }

            i++;
            count++;
        }
    }

    void parseBinaryRotateTree(vSyntacticTree cst, vSyntacticTree &construction_tree, int current_node_index)
    {
        printf("%3d :(開始) 二分木回転構築(新規)\n", current_node_index);
        SyntacticTreeNode current_node = cst[current_node_index];

        int i = 0;
        int count = 0;
        int parent = current_node_index;
        while (i < current_node.children.size())
        {
            int child_node_index = current_node.children[i];
            SyntacticTreeNode child_node = cst[child_node_index];

            if (isTokenSkepSyntacticAnalysis(child_node.token))
            {
                // cutChild(construction_tree, parent, child_node_index);
                deleteChild(construction_tree, child_node_index);
                i++;
                continue;
            }
            int c_left = construction_tree[parent].children[0];
            int c_right = construction_tree[parent].children[1];

            if (count % 2 == 0 && c_left >= 0)
            {
                count++;
                parent = c_left;
                continue;
            }

            if (count % 2 == 1 && c_right >= 0)
            {
                count++;
                parent = c_right;
                continue;
            }

            if (count % 2 == 0)
            {

                printf("%3d : 二分木構築 左 旧:%d -> 親:%d 換:%d\n", child_node_index, current_node_index, parent, c_left);

                moveChild(construction_tree, parent, 0, child_node_index);
            }
            if (count % 2 == 1)
            {

                printf("%3d : 二分木構築 右 旧:%d -> 親:%d 換:%d\n", child_node_index, current_node_index, parent, c_right);

                moveChild(construction_tree, parent, 1, child_node_index);
                parent = child_node_index;
            }

            i++;
            count++;
        }

        printf("%3d :(離脱) 二分木回転構築\n", current_node_index);
    }

    bool parseBinaryTreeTerminal(vSyntacticTree &construction_tree, int current_node_index)
    {
        SyntacticTreeNode current_node = construction_tree[current_node_index];
        int new_middle = -1;
        vint new_children = {};

        for (int i = 0; i < current_node.children.size(); i++)
        {
            int child_node_index = current_node.children[i];
            SyntacticTreeNode child_node = construction_tree[child_node_index];
            if (isExpr(child_node.token))
            {
                new_middle = child_node_index;
                break;
            }
        }

        for (int i = 0; i < current_node.children.size(); i++)
        {
            int child_node_index = current_node.children[i];
            SyntacticTreeNode child_node = construction_tree[child_node_index];
            if (isTokenSkepSyntacticAnalysis(child_node.token))
            {
                construction_tree[child_node_index].token_label == is_id_Unnecessary;
                continue;
            }

            if (new_middle < 0 && child_node.token_label == is_id_TerminalSymbol)
            {
                new_middle = child_node_index;
                continue;
            }
            if (new_middle == child_node_index)
            {
                continue;
            }

            new_children.push_back(child_node_index);
        }

        while (new_children.size() < 2)
        {
            new_children.push_back(-2);
        }
        printf("%3d :二分木構築(NM分離B) nm:%d p:%d {", -2, new_middle, current_node_index);
        // 末端記号が存在することによる制御
        if (new_middle >= 0)
        {

            construction_tree[current_node_index].children = {new_middle, -2};
            construction_tree[new_middle].setParent(current_node_index);
            construction_tree[new_middle].children = new_children;

            for (int i = 0; i < new_children.size(); i++)
            {
                construction_tree[new_children[i]].setParent(new_middle);
                printf(" %d", new_children[i]);
            }

            printf(" }\n");

            if (new_children.size() >= 3)
            {
                parseBinaryTreeTerminal(construction_tree, new_middle);
            }

            return true;
        }

        parseBinaryRotateTree(construction_tree, current_node_index);
        return false;
    }
    bool parseBinaryTreeTerminal(vSyntacticTree cst, vSyntacticTree &construction_tree, int current_node_index)
    {
        SyntacticTreeNode current_node = cst[current_node_index];
        int new_middle = -1;
        vint new_children = {};

        for (int i = 0; i < current_node.children.size(); i++)
        {
            int child_node_index = current_node.children[i];
            SyntacticTreeNode child_node = cst[child_node_index];
            printf("pickup %s %d %d\n", child_node.token.c_str(), isExpr(child_node.token), child_node_index);

            if (isExpr(child_node.token))
            {
                new_middle = child_node_index;
                break;
            }
        }

        for (int i = 0; i < current_node.children.size(); i++)
        {
            int child_node_index = current_node.children[i];
            SyntacticTreeNode child_node = cst[child_node_index];
            if (isTokenSkepSyntacticAnalysis(child_node.token))
            {
                construction_tree[child_node_index].token_label == is_id_Unnecessary;
                continue;
            }

            if (new_middle < 0 && child_node.token_label == is_id_TerminalSymbol)
            {
                new_middle = child_node_index;
                continue;
            }
            if (new_middle == child_node_index)
            {
                continue;
            }

            new_children.push_back(child_node_index);
        }

        while (new_children.size() < 2)
        {
            new_children.push_back(-2);
        }
        // 末端記号が存在することによる制御
        if (new_middle >= 0)
        {
            printf("%3d :二分木構築(NM分離A) nm:%d p:%d {", -2, new_middle, current_node_index);
            construction_tree[current_node_index].children = {new_middle, -2};
            construction_tree[new_middle].setParent(current_node_index);
            construction_tree[new_middle].children = new_children;

            for (int i = 0; i < new_children.size(); i++)
            {
                construction_tree[new_children[i]].setParent(new_middle);
                printf(" %d", new_children[i]);
            }

            printf(" }\n");

            if (new_children.size() >= 3)
            {
                parseBinaryTreeTerminal(construction_tree, new_middle);
            }

            return true;
        }
        return false;
    }
    void parseBinaryTree(vSyntacticTree cst, vSyntacticTree &construction_tree, int current_node_index)
    {
        printf("%3d :(開始) 二分木構築\n", current_node_index);
        SyntacticTreeNode current_node = cst[current_node_index];
        if (current_node.children.size() >= 3)
        {
            if (parseBinaryTreeTerminal(cst, construction_tree, current_node_index))
            {
                return;
            }
        }
        parseBinaryRotateTree(cst, construction_tree, current_node_index);

        printf("%3d :(離脱) 二分木構築\n", current_node_index);

        return;
    }

    /// @brief 子が右寄せになっているなら左に寄せる
    /// @param tree
    void childrenLeftAlignmentBinaryTree(vSyntacticTree &tree)
    {
        for (int i = 0; i < tree.size(); i++)
        {
            if (tree[i].children[0] < 0 && tree[i].children[1] >= 0)
            {
                tree[i].children[0] = tree[i].children[1];
                tree[i].children[1] = -1;
            }
        }
    }
    /// @brief
    /// @param syntactic_parse_tree
    /// @param current_node_index
    void recursionSyntacticBinaryTree(vSyntacticTree cst, vSyntacticTree &construction_tree, int current_node_index)
    {
        printf("%3d :(開始) AST構築作業\n", current_node_index);

        for (int i = 0; i < cst[current_node_index].children.size(); i++)
        {
            recursionSyntacticBinaryTree(cst, construction_tree, cst[current_node_index].children[i]);
        }
        parseBinaryTree(cst, construction_tree, current_node_index);

        printf("%3d :(離脱) AST構築作業\n", current_node_index);
    }

    void recursionCutBinaryTree(vSyntacticTree &construction_tree, int current_node_index)
    {
        printf("%3d :(開始) AST刈り取り作業\n", current_node_index);

        for (int i = 0; i < construction_tree[current_node_index].children.size(); i++)
        {
            int c = construction_tree[current_node_index].children[i];
            if (c >= 0)
            {
                printf("%3d : - cb %d \n", current_node_index, c);
                recursionCutBinaryTree(construction_tree, c);
            }
        }
        // cutNonterminalNode(construction_tree, current_node_index);
        // childrenLeftAlignmentBinaryTree(construction_tree);

        printf("%3d :(離脱) AST刈り取り作業\n", current_node_index);
    }

    void syntacticParseTree(vSyntacticTree cst, vSyntacticTree &ast)
    {
        vSyntacticTree construction_tree = cst;

        for (int i = 0; i < construction_tree.size(); i++)
        {
            construction_tree[i].children = {-2, -2};
        }
        printf("AST解析 - A\n");
        recursionSyntacticBinaryTree(cst, construction_tree, 0);
        debugSyntacticAnalysisTree(construction_tree, false);
        printf("AST解析  - B\n");
        recursionCutBinaryTree(construction_tree, 0);
        debugSyntacticAnalysisTree(construction_tree, false);
        printf("AST解析  - C\n");
        debugSyntacticAnalysisTree(construction_tree);
        debugSyntacticAnalysisTree(construction_tree, false);
        printf("AST解析  - D\n");

        ast = construction_tree;
        return;
    }
}
