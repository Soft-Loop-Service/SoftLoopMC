
#ifndef __SCL
#define __SCL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>

#include "./../definition.hpp"
#include "./../lexical/lexical_analysis.hpp"
#include "./../syntactic/syntactic_analysis.hpp"

#include "./tree/concrete.hpp"
#include "./tree/container.hpp"
#include "./tree/container_stack.hpp"

#include "./tree/literal.hpp"
#include "./tree/variable.hpp"

// SCL解析系
namespace LanguageSpecifications
{
    using namespace Syntactic;

    namespace Scl
    {
        typedef SyntacticTreeNode TN;

        class SclExpr;
        class SclContainer;
        typedef std::vector<SclContainer> vSclContainer;

        class SclExpr
        {
        private:
            Syntactic::vSyntacticTree tree;
            vSclContainer scl_containers;
            SclContainerStack scl_container_stack;

            vSclVariable scl_variables;
            vSclLiteral scl_literals;

        public:
            SclExpr();
            SclExpr(vSyntacticTree);

            vSclVariable getSclVariables();
            vSclLiteral getSclLiterals();
            SclLiteral getSclLiteral(int);
            SclVariable getSclVariable(int);

            bool searchSclLiteral(vSclLiteral &, int, string);
            bool getSclLiteral(SclLiteral &, SclVariable);
            bool getSclLiteral(vSclLiteral &, SclVariable);
            bool searchSclVariable(SclVariable &, int, string);

            void exprComponentBlock(int);
            void exprComponentName(int);
            void exprComponentType(int);
            void exprComponent(int);
            void exprIf(int);

            int pushLiteral(SclLiteral);
            int pushVariable(SclVariable);
            int pushContainer(SclContainer);

            int getChildSize(int);
            TN getChildNode(int, int);
            int getChildNodeIndex(int, int);

            bool hasNode(int);
            TN getNode(int);

            void variableExpr(int, vint &);
            void leftExpr(int, int &);
            void rightExpr(int, vint &);

            void assignExpr(int);
            void calcExpr(int);
            bool treeExpr(int);
            bool descendExpr(int);

            int searchContainer(string, string);
            int searchContainer(string, string, string);

            vint searchContainers(string, string);
            vint searchContainers(string, string, string);

            void debug()
            {
                printf("[scl-debug] SclExpr Containers * * *\n");
                for (int i = 0; i < scl_containers.size(); i++)
                {
                    printf("%3d |", i);
                    scl_containers[i].debug();
                }
                printf("[scl-debug] SclExpr Variable * * *\n");
                for (int i = 0; i < scl_variables.size(); i++)
                {
                    printf("%3d |", i);
                    scl_variables[i].debug();
                }
                printf("[scl-debug] SclExpr Literal * * *\n");
                for (int i = 0; i < scl_literals.size(); i++)
                {
                    printf("%3d |", i);
                    scl_literals[i].debug();
                }
            }
        };

    };

    void parse(Scl::SclExpr &);
};

#endif