

#ifndef __BYTECODE_IO_I
#define __BYTECODE_IO_I

#include "./../bytecode_definition.hpp"

namespace Bytecode
{
    namespace BytecodeIO
    {
        using namespace std;

        class BytecodeOutput
        {
        private:
            ofstream *outputfile;

        public:
            BytecodeOutput(string file_name);
            ~BytecodeOutput();

            void putHex(opcr opecode);
            void putOpecode(opcr);
            void putOpecode(opcr, int);
            void putOpecode(opcr, string);
            void putOpecode(opcr, vint);
            void putOpecode(opcr, vstring);
        };

    }

}
#endif