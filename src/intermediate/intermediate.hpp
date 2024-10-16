
#ifndef __EST
#define __EST

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>

#include "./../definition.hpp"
#include "./../lexical/lexical_analysis.hpp"
#include "./../syntactic/syntactic_analysis.hpp"

#include "./../trans_kit/trans_kit.hpp"
#include "./../scl/scl.hpp"

#include "./effect.hpp"

namespace LanguageIntermediateSystem
{

    class Intermediate
    {
    private:
        LanguageSpecifications::Scl::SclExpr scl_expr;
        TransSystem::TransKit trans_kit;

    public:
        Intermediate();
        Intermediate(LanguageSpecifications::Scl::SclExpr, TransSystem::TransKit);
        void intermediateComponentEffect(string);
        void intermediateComponentLink(string);

        void recursionComponent(int);
        void recursionComponentArea(string);

        TransSystem::TransKit getTransKit();
    };

    void intermediate(LanguageSpecifications::Scl::SclExpr, Intermediate &);

};

#endif