#include <iostream>
#include <fstream>
#include "./../bytecode_definition.hpp"
#include "./output.hpp"
#include <iomanip> // 16進数出力用

namespace Bytecode
{
    namespace BytecodeIO
    {

        BytecodeOutput::BytecodeOutput(string file_name)
        {
            outputfile = new ofstream(file_name);
        }
        void BytecodeOutput::putHex(opcr opecode)
        {
            *outputfile << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opecode);
        }
        void BytecodeOutput::putOpecode(opcr opecode)
        {
            putHex(opecode);
            *outputfile << "\n";
        };

        void BytecodeOutput::putOpecode(opcr opecode, int arg)
        {
            putHex(opecode);
            *outputfile << " ";
            *outputfile << arg;
            *outputfile << "\n";
        };
        void BytecodeOutput::putOpecode(opcr opecode, string arg)
        {
            putHex(opecode);
            *outputfile << " ";
            *outputfile << arg;
            *outputfile << "\n";
        };
        void BytecodeOutput::putOpecode(opcr opecode, vint args)
        {
            putHex(opecode);

            for (int i = 0; i < args.size(); i++)
            {
                *outputfile << args[i];
                *outputfile << " ";
            }
            *outputfile << "\n";
        };
        void BytecodeOutput::putOpecode(opcr opecode, vstring args)
        {
            putHex(opecode);

            for (int i = 0; i < args.size(); i++)
            {
                *outputfile << args[i];
                *outputfile << " ";
            }
            *outputfile << "\n";
        };

        BytecodeOutput::~BytecodeOutput()
        {
            outputfile->close();
        }
    }

}