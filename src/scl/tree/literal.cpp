
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>

#include "./literal.hpp"

namespace LanguageSpecifications
{
    using namespace Syntactic;

    namespace Scl
    {
        typedef SyntacticTreeNode TN;

        SclLiteral::SclLiteral()
        {
            type = "";
            literal = "";
            literal_index = -1;
        }
        SclLiteral::SclLiteral(string arg1, string arg2)
        {
            type = arg1;
            literal = arg2;
            literal_index = -1;
        }
        string SclLiteral::getType()
        {
            return type;
        }
        string SclLiteral::getLiteral()
        {
            return literal;
        }
        int SclLiteral::getLiteralIndex()
        {
            return literal_index;
        }

        void SclLiteral::setLiteralIndex(int i)
        {
            literal_index = i;
        }

    };

};
