
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include "./../syntactic/syntactic_analysis.hpp"
#include "./scl.hpp"

namespace LanguageSpecifications
{
    using namespace Syntactic;

    namespace Scl
    {

        typedef SyntacticTreeNode TN;

        // 変数及び配列の解決
        void SclExpr::variableExpr(int node_index, vint &literals)
        {
            // 論理式ではないなら実行しない
            if (treeExpr(node_index))
            {
                return;
            }
            TN node = getNode(node_index);

            printf("SclExpr | %20s node:%d\n", "[variableExpr]", node_index);

            SclLiteral sl("lit", node.token);
            int index = pushLiteral(sl);
            literals.push_back(index);
            variableExpr(getChildNodeIndex(node_index, 0), literals);
        }

        // 左辺解決
        void SclExpr::leftExpr(int node_index, int &variable_index)
        {
            TN node = getNode(node_index);
            printf("SclExpr | %20s node:%d\n", "[leftExpr]", node_index);

            SclVariable sv("val", node.token);
            int index = pushVariable(sv);
            variable_index = index;

            descendExpr(node_index);

            return;
        }
        // 右辺解決
        void SclExpr::rightExpr(int node_index, vint &literals)
        {
            printf("SclExpr | %20s node:%d\n", "[rightExpr]", node_index);

            variableExpr(node_index, literals);
        }

        void SclExpr::assignExpr(int node_index)
        {
            printf("SclExpr | %20s node:%d\n", "[assignExpr]", node_index);

            vint literals = {};
            int variable_index = -1;
            leftExpr(getChildNodeIndex(node_index, 0), variable_index);
            rightExpr(getChildNodeIndex(node_index, 1), literals);

            if (variable_index < 0)
            {
                return;
            }

            scl_variables[variable_index].addLiteral(literals);
            int vc_index = scl_container_stack.getTop();
            scl_containers[vc_index].addVariable(variable_index);
        }

        void SclExpr::calcExpr(int node_index)
        {
        }

        bool SclExpr::treeExpr(int node_index)
        {

            if (node_index < 0)
            {
                return true;
            }
            if (tree.size() <= node_index)
            {
                return true;
            }

            TN node = getNode(node_index);

            if (isAssignExpr(node.token))
            {
                assignExpr(node_index);
                return true;
            }

            if (isExpr(node.token))
            {
                calcExpr(node_index);
                return true;
            }

            if (node.token == "Component")
            {
                exprComponent(node_index);
                return true;
            }

            return false;
        }

        bool SclExpr::descendExpr(int node_index)
        {
            int left = getChildNodeIndex(node_index, 0);
            int right = getChildNodeIndex(node_index, 1);

            if (left > 0)
            {
                treeExpr(left);
            }
            if (right > 0)
            {
                treeExpr(right);
            }

            bool ans = left > 0 || right > 0;
            return ans;
        }

    };

};
