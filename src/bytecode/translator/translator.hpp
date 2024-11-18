

#ifndef __BYTECODE_T
#define __BYTECODE_T
#include "./../bytecode_definition.hpp"
#include "./../bytecodeio/output.hpp"
#include "./../../syntactic/syntactic_analysis.hpp"

namespace Bytecode
{
    namespace Translator
    {
        using namespace Syntactic;
        void translator(vSyntacticTree &, string);
    }
}

#endif