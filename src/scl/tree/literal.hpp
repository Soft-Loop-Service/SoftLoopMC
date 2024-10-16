
#ifndef __SCLTL
#define __SCLTL

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
        class SclLiteral;
        typedef std::vector<SclLiteral> vSclLiteral;

        class SclLiteral
        {
        private:
            string type;
            string literal;
            int literal_index;

        public:
            SclLiteral();
            SclLiteral(string, string);
            void setLiteralIndex(int);

            string getType();
            string getLiteral();
            int getLiteralIndex();

            void debug()
            {
                printf("[scl-debug] SclLiteral * * *\n");

                printf("%3s | type:%5s literal:%30s lit-index%2d\n", "-", type.c_str(), literal.c_str(), literal_index);
            }
        };
    };

}

#endif