
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>

#include "./variable.hpp"

namespace LanguageSpecifications
{
    using namespace Syntactic;

    namespace Scl
    {
        typedef SyntacticTreeNode TN;

        SclVariable::SclVariable()
        {
            type = "";
            name = "";
            variable_index = -1;
            literals_index = {};
        }
        SclVariable::SclVariable(string arg1, string arg2)
        {
            type = arg1;
            name = arg2;
            variable_index = -1;
            literals_index = {};
        }

        string SclVariable::getType()
        {
            return type;
        }

        string SclVariable::getName()
        {
            return name;
        }
        void SclVariable::setVariableIndex(int i)
        {
            variable_index = i;
        }

        void SclVariable::setLiteral(int e)
        {
            literals_index = {e};
        }
        void SclVariable::setLiteral(vint es)
        {
            literals_index = es;
        }
        void SclVariable::addLiteral(int e)
        {
            literals_index.push_back(e);
        }
        void SclVariable::addLiteral(vint es)
        {
            for (int i = 0; i < es.size(); i++)
            {
                literals_index.push_back(es[i]);
            }
        }
        int SclVariable::getLiteral(int i)
        {
            return literals_index[i];
        }
        vint SclVariable::getLiterals()
        {
            return literals_index;
        }
    };

};
