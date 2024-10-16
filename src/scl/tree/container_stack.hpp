
#ifndef __SCLTCS
#define __SCLTCS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>

#include "./../../definition.hpp"
#include "./../../lexical/lexical_analysis.hpp"
#include "./../../syntactic/syntactic_analysis.hpp"

// SCL解析系
namespace LanguageSpecifications
{
    using namespace Syntactic;

    namespace Scl
    {
        typedef SyntacticTreeNode TN;

        class SclContainerStack
        {
        private:
            vint scl_container_stack; // vSclContainerのindex
            vSyntacticTree tree;

        public:
            SclContainerStack();
            SclContainerStack(vSyntacticTree tree);

            /// @brief vSclContainerのindexをstackにpushする
            /// @param vSclContainerのindex
            void push(int);

            /// @brief vSclContainerのindexをstackよりpopする
            /// @return vSclContainerのindex
            int pop();

            /// @brief vSclContainerのindexをstackのtopより取得する
            /// @return vSclContainerのindex
            int getTop();
        };
    }

};

#endif