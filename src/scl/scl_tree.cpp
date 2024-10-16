
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

        void SclExpr::exprIf(int node_index)
        {
            TN node = getNode(node_index);
        }

        // コンポーネント設計 : 通常のarrayと等しくになりました
        // Contact
        // - :
        // - element
        // - - element
        // - - - element
        // - - - - elemtnt

        // void SclExpr::exprContactElement(int node_index, vstring &contact_element)
        // {
        //     printf("SclExpr | %20s node:%d\n", "[exprContactElement]", node_index);

        //     // 論理式ではないなら実行しない
        //     if (treeExpr(node_index))
        //     {
        //         return;
        //     }
        //     TN node = getNode(node_index);
        //     contact_element.push_back(node.token);

        //     exprContactElement(getChildNodeIndex(node_index, 0), contact_element);
        // }

        // void SclExpr::exprContact(int node_index)
        // {
        //     printf("SclExpr | %20s node:%d\n", "[exprContact]", node_index);
        //     TN node = getNode(node_index);
        //     vstring contact_element = {};
        //     exprContactElement(getChildNodeIndex(node_index, 0), contact_element);
        //     exprContactElement(getChildNodeIndex(node_index, 1), contact_element);
        //     int vc_index = scl_container_stack.getTop();
        //     scl_containers[vc_index].setContactList(contact_element);
        // }

        // コンポーネント設計
        // Component
        // - ComponentType
        // - - ComponentName
        // - - ComponentBlock
        void SclExpr::exprComponentBlock(int node_index)
        {
            printf("SclExpr | %20s node:%d\n", "[exprComponentBlock]", node_index);

            if (!hasNode(node_index))
            {
                return;
            }

            TN node = getNode(node_index);
            treeExpr(node_index);
        }
        void SclExpr::exprComponentName(int node_index)
        {
            printf("SclExpr | %20s node:%d\n", "[exprComponentName]", node_index);
            TN node = getNode(node_index);

            int vc_index = scl_container_stack.getTop();
            scl_containers[vc_index].setContainerName(node.token);
            descendExpr(node_index);
        }
        void SclExpr::exprComponentType(int node_index)
        {
            printf("SclExpr | %20s node:%d\n", "[exprComponentType]", node_index);
            TN node = getNode(node_index);
            exprComponentName(getChildNodeIndex(node_index, 0));
            exprComponentBlock(getChildNodeIndex(node_index, 1));
            int vc_index = scl_container_stack.getTop();
            scl_containers[vc_index].setContainerType("component", node.token);
        }

        void SclExpr::exprComponent(int node_index)
        {
            printf("SclExpr | %20s node:%d\n", "[exprComponent]", node_index);
            SclContainer new_component;
            new_component.setTreeNodeIndex(node_index);
            int sc_index = this->pushContainer(new_component);

            scl_container_stack.push(sc_index);
            exprComponentType(getChildNodeIndex(node_index, 0));
            scl_container_stack.pop();
        }
    };

};
