#include <iostream>
#include <fstream>
#include "./../bytecode_definition.hpp"
#include "./output.hpp"
#include <iomanip> // 16進数出力用
#include <iostream>

namespace Bytecode
{
    namespace BytecodeIO
    {

        BytecodeOutput::BytecodeOutput(string file_name)
        {
            outputfile = new ofstream(file_name);
            local_stack = {};
            local_stack.emplace_back();
            function_latest_id = 0;
        }
        string BytecodeOutput::getHex(opcr opecode)
        {
            std::ostringstream oss;
            oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opecode);
            return oss.str();
        }
        void BytecodeOutput::putHex(opcr opecode)
        {
            *(local_stack.back().bytecode) << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opecode);
        }
        void BytecodeOutput::putOpecode(opcr opecode)
        {
            putHex(opecode);
            *(local_stack.back().bytecode) << "\n";
        };

        void BytecodeOutput::putOpecode(opcr opecode, int arg)
        {
            putHex(opecode);
            *(local_stack.back().bytecode) << " ";
            *(local_stack.back().bytecode) << arg;
            *(local_stack.back().bytecode) << "\n";
        };
        void BytecodeOutput::putOpecode(opcr opecode, string arg)
        {
            putHex(opecode);
            *(local_stack.back().bytecode) << " ";
            *(local_stack.back().bytecode) << arg;
            *(local_stack.back().bytecode) << "\n";
        };
        void BytecodeOutput::putOpecode(opcr opecode, vint args)
        {
            putHex(opecode);

            for (int i = 0; i < args.size(); i++)
            {
                *(local_stack.back().bytecode) << args[i];
                *(local_stack.back().bytecode) << " ";
            }
            *(local_stack.back().bytecode) << "\n";
        };
        void BytecodeOutput::putOpecode(opcr opecode, vstring args)
        {
            putHex(opecode);

            for (int i = 0; i < args.size(); i++)
            {
                *(local_stack.back().bytecode) << args[i];
                *(local_stack.back().bytecode) << " ";
            }
            *(local_stack.back().bytecode) << "\n";
        };

        int BytecodeOutput::newLocalVariable(string name, int type)
        {
            return local_stack.back().newLocalVariable(name, type);
        }

        BytecodeOutput::~BytecodeOutput()
        {
            outputfile->close();
        }

        void BytecodeOutput::switchFunction()
        {
            newLocalStack();
            function_latest_id++;
            *(local_stack.back().bytecode) << "function " << function_latest_id << "\n";
        }

        void BytecodeOutput::returnFunction()
        {
            *(local_stack.back().bytecode) << "end function " << function_latest_id << "\n";
            processedStackTop();
        }

        void BytecodeOutput::newLocalStack()
        {
            printf("newLocalStack %10d\n", local_stack.size());
            local_stack.emplace_back();
            printf("newLocalStack %10d\n", local_stack.size());
        }

        void BytecodeOutput::processedStackTop()
        {
            local_stack.back().setIsProcessed(false);

            *outputfile << "\n";
            *outputfile << local_stack.back().bytecode->str();
            *outputfile << "\n";
            local_stack.back().bytecode->clear();
        }

        bool BytecodeOutput::isFindLocalVariable(string name, int type)
        {
            for (int i = local_stack.size() - 1; i >= 0; i--)
            {
                if (local_stack[i].getIsProcessed())
                {
                    // mapに存在するか
                    if (local_stack[i].isFindLocalVariable(name, type))
                    {
                        return true;
                    }
                }
            }

            return false;
        }

        LocalVariable BytecodeOutput::findLocalVariable(string name, int type)
        {
            for (int i = local_stack.size() - 1; i >= 0; i--)
            {
                if (local_stack[i].getIsProcessed())
                {
                    // mapに存在するか
                    if (local_stack[i].isFindLocalVariable(name, type))
                    {
                        return local_stack[i].getLocalVariable(name);
                    }
                }
            }

            return LocalVariable("underfind", -1, 0);
        }

        LocalStack::LocalStack()
        {
            is_prossesing = true;
            local_variable_map = {};
            bytecode = new std::ostringstream();
        }

        LocalStack::~LocalStack()
        {
        }

        void LocalStack::setIsProcessed(int flag)
        {
            is_prossesing = flag;
        }

        bool LocalStack::getIsProcessed()
        {
            return is_prossesing;
        }

        int LocalStack::newLocalVariable(string name, int type)
        {
            LocalVariable lv = LocalVariable(name, local_variable_map.size(), type);
            local_variable_map[name] = lv;
            // ここでは2行前から+1されているので、注意
            return local_variable_map.size();
        }

        bool LocalStack::isFindLocalVariable(string name)
        {
            return local_variable_map.find(name) != local_variable_map.end();
        }

        bool LocalStack::isFindLocalVariable(string name, string type)
        {
            int type_int = Opecode::resolvOpecrType(type);
            return isFindLocalVariable(name, type_int);
        }

        bool LocalStack::isFindLocalVariable(string name, int type)
        {
            auto iterator = local_variable_map.find(name);
            if (local_variable_map.find(name) == local_variable_map.end())
            {
                return false;
            }

            LocalVariable lv = iterator->second;
            return lv.type == type;
        }

        LocalVariable LocalStack::getLocalVariable(string name)
        {
            return local_variable_map[name];
        }

    }

}