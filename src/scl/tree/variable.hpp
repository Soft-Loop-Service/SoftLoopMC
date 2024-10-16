
#ifndef __SCLTV
#define __SCLTV

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
        class SclVariable;
        typedef std::vector<SclVariable> vSclVariable;

        class SclVariable
        {
        private:
            string type;
            string name;
            vint literals_index;
            int variable_index;

        public:
            SclVariable();
            SclVariable(string, string);

            string getType();
            string getName();

            void setVariableIndex(int);
            void setLiteral(int);
            void setLiteral(vint);
            void addLiteral(int);
            void addLiteral(vint);
            int getLiteral(int);
            vint getLiterals();

            void debug()
            {
                printf("[scl-debug] SclVariable * * *\n");

                printf("%3s | type:%5s name:%5s val-index%2d\n", "-", type.c_str(), name.c_str(), variable_index);

                printf("    {");

                for (int i = 0; i < literals_index.size(); i++)
                {
                    printf("%3d", literals_index[i]);
                }

                printf("}\n");
            }
        };

    }
}

#endif