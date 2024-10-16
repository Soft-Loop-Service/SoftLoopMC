
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

        SclExpr::SclExpr()
        {
            this->tree = {};
            this->scl_containers = {};
            this->scl_variables = {};
            this->scl_literals = {};
        }
        SclExpr::SclExpr(vSyntacticTree tree)
        {
            this->tree = tree;
            this->scl_containers = {};
            this->scl_variables = {};
            this->scl_literals = {};
        }
        vSclVariable SclExpr::getSclVariables()
        {
            return scl_variables;
        }
        vSclLiteral SclExpr::getSclLiterals()
        {
            return scl_literals;
        }
        SclVariable SclExpr::getSclVariable(int index)
        {
            return scl_variables[index];
        }
        SclLiteral SclExpr::getSclLiteral(int index)
        {
            return scl_literals[index];
        }

        bool SclExpr::searchSclLiteral(vSclLiteral &search_scl_literals, int scl_container_index, string val_name)
        {
            SclContainer c = scl_containers[scl_container_index];
            vint vs = c.getVariables();

            for (int i = 0; i < vs.size(); i++)
            {
                int v = vs[i];

                SclVariable variable = scl_variables[v];

                if (variable.getName() == val_name)
                {
                    getSclLiteral(search_scl_literals, variable);
                    return true;
                }
            }
            return false;
        }

        bool SclExpr::getSclLiteral(SclLiteral &search_scl_literal, SclVariable scl_variable)
        {
            vint literals = scl_variable.getLiterals();

            for (int i = 0; i < literals.size(); i++)
            {
                int literal_index = literals[i];
                SclLiteral literal = scl_literals[literal_index];
                search_scl_literal = literal;
                return true;
            }

            return true;
        }
        bool SclExpr::getSclLiteral(vSclLiteral &search_scl_literals, SclVariable scl_variable)
        {
            vint literals = scl_variable.getLiterals();

            for (int i = 0; i < literals.size(); i++)
            {
                int literal_index = literals[i];
                SclLiteral literal = scl_literals[literal_index];
                search_scl_literals.push_back(literal);
            }

            return true;
        }

        bool SclExpr::searchSclVariable(SclVariable &search_scl_variable, int scl_container_index, string val_name)
        {
            SclContainer c = scl_containers[scl_container_index];
            vint vs = c.getVariables();

            for (int i = 0; i < vs.size(); i++)
            {
                int v = vs[i];

                SclVariable variable = scl_variables[v];

                if (variable.getName() == val_name)
                {
                    search_scl_variable = variable;
                    return true;
                }
            }
            return false;
        }
    };
    void parse(Scl::SclExpr &scl_expr)
    {

        scl_expr.treeExpr(scl_expr.getNode(0).children[0]);
        scl_expr.debug();
    };
};
