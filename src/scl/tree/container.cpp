
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>

#include "./container.hpp"

namespace LanguageSpecifications
{
    using namespace Syntactic;

    namespace Scl
    {
        typedef SyntacticTreeNode TN;

        SclContainer::SclContainer()
        {
            tree_node_index = -1;
            container_index = -1;
            container_name = "";
            container_major_type = "";
            container_minor_type = "";

            variables_index = {};
        }

        void SclContainer::setContainerIndex(int arg)
        {
            this->container_index = arg;
        }

        void SclContainer::setTreeNodeIndex(int arg)
        {
            this->tree_node_index = arg;
        }

        void SclContainer::setContainerName(string arg)
        {
            this->container_name = arg;
        }
        void SclContainer::setContainerType(string arg1, string arg2)
        {
            this->container_major_type = arg1;
            this->container_minor_type = arg2;
        };
        void SclContainer::setContainerMajorType(string arg)
        {
            this->container_major_type = arg;
        };
        void SclContainer::setContainerMinorType(string arg)
        {
            this->container_minor_type = arg;
        };

        void SclContainer::setVariable(int e)
        {
            variables_index = {e};
        }
        void SclContainer::setVariable(vint es)
        {
            variables_index = es;
        }
        void SclContainer::addVariable(int e)
        {
            variables_index.push_back(e);
        }
        void SclContainer::addVariable(vint es)
        {
            for (int i = 0; i < es.size(); i++)
            {
                variables_index.push_back(es[i]);
            }
        }
        int SclContainer::getVariable(int i)
        {
            return variables_index[i];
        }
        vint SclContainer::getVariables()
        {
            return variables_index;
        }

        int SclContainer::getTreeNodeIndex()
        {
            return tree_node_index;
        }
        int SclContainer::getContainerIndex()
        {
            return container_index;
        }
        string SclContainer::getContainerName()
        {
            return container_name;
        }
        string SclContainer::getContainerMajorType()
        {
            return container_major_type;
        }
        string SclContainer::getContainerMinorType()
        {
            return container_minor_type;
        }

    };

};
