
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

        int SclExpr::getChildSize(int node_index)
        {
            return this->tree[node_index].children.size();
        }
        int SclExpr::getChildNodeIndex(int node_index, int children_node_index)
        {
            return this->tree[node_index].children[children_node_index];
        }
        TN SclExpr::getChildNode(int node_index, int children_node_index)
        {
            return getNode(this->tree[node_index].children[children_node_index]);
        }
        bool SclExpr::hasNode(int node_index)
        {
            if (node_index < 0)
            {
                return false;
            }

            if (tree.size() <= node_index)
            {
                return false;
            }

            return true;
        }
        TN SclExpr::getNode(int node_index)
        {
            return tree[node_index];
        }
        int SclExpr::pushLiteral(SclLiteral c)
        {
            scl_literals.push_back(c);
            int index = scl_literals.size() - 1;
            scl_literals[index].setLiteralIndex(index);
            return index;
        }
        int SclExpr::pushVariable(SclVariable c)
        {
            scl_variables.push_back(c);
            int index = scl_variables.size() - 1;
            scl_variables[index].setVariableIndex(index);
            return index;
        }
        int SclExpr::pushContainer(SclContainer c)
        {
            scl_containers.push_back(c);
            int index = scl_containers.size() - 1;
            scl_containers[index].setContainerIndex(index);
            return index;
        }

        int SclExpr::searchContainer(string major_type, string minor_type)
        {
            for (int i = 0; i < scl_containers.size(); i++)
            {
                if (scl_containers[i].getContainerMajorType() == major_type && scl_containers[i].getContainerMinorType() == minor_type)
                {
                    return i;
                }
            }
        }
        int SclExpr::searchContainer(string major_type, string minor_type, string container_name)
        {
            for (int i = 0; i < scl_containers.size(); i++)
            {
                if (scl_containers[i].getContainerMajorType() == major_type &&
                    scl_containers[i].getContainerMinorType() == minor_type &&
                    scl_containers[i].getContainerName() == container_name)
                {
                    return i;
                }
            }
        }
        vint SclExpr::searchContainers(string major_type, string minor_type)
        {
            vint r = {};
            for (int i = 0; i < scl_containers.size(); i++)
            {
                if (scl_containers[i].getContainerMajorType() == major_type && scl_containers[i].getContainerMinorType() == minor_type)
                {
                    r.push_back(i);
                }
            }
            return r;
        }
        vint SclExpr::searchContainers(string major_type, string minor_type, string container_name)
        {
            vint r = {};
            for (int i = 0; i < scl_containers.size(); i++)
            {
                if (scl_containers[i].getContainerMajorType() == major_type &&
                    scl_containers[i].getContainerMinorType() == minor_type &&
                    scl_containers[i].getContainerName() == container_name)
                {
                    r.push_back(i);
                }
            }
            return r;
        }

    };

};
