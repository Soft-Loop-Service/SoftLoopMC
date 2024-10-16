
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>

#include "./container_stack.hpp"

namespace LanguageSpecifications
{
    using namespace Syntactic;

    namespace Scl
    {
        typedef SyntacticTreeNode TN;

        SclContainerStack::SclContainerStack()
        {
            scl_container_stack = {};
        }
        SclContainerStack::SclContainerStack(vSyntacticTree arg_tree)
        {
            scl_container_stack = {};
            this->tree = arg_tree;
        }
        void SclContainerStack::push(int push_index)
        {
            scl_container_stack.push_back(push_index);
        }
        int SclContainerStack::pop()
        {
            int pop_node = scl_container_stack[scl_container_stack.size() - 1];
            scl_container_stack.pop_back();
            return pop_node;
        }
        int SclContainerStack::getTop()
        {
            int top_node = scl_container_stack[scl_container_stack.size() - 1];
            return top_node;
        }
    };

};
