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
            function_class_latest_id = 0;
            current_local_stack_index = 0;
        }
        string BytecodeOutput::getHex(opcr opecode)
        {
            std::ostringstream oss;
            oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opecode);
            return oss.str();
        }
        void BytecodeOutput::putHex(opcr opecode)
        {
            *(local_stack[current_local_stack_index].bytecode) << std::setw(3) << std::setfill('0') << opecode;
            return;
            // *(local_stack[current_local_stack_index].bytecode) << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opecode);
        }
        void BytecodeOutput::putOpecode(opcr opecode)
        {
            putHex(opecode);
            *(local_stack[current_local_stack_index].bytecode) << "\n";
        };

        void BytecodeOutput::putOpecode(opcr opecode, int arg)
        {
            putHex(opecode);
            *(local_stack[current_local_stack_index].bytecode) << " ";
            *(local_stack[current_local_stack_index].bytecode) << arg;
            *(local_stack[current_local_stack_index].bytecode) << "\n";
        };
        void BytecodeOutput::putOpecode(opcr opecode, string arg)
        {
            putHex(opecode);
            *(local_stack[current_local_stack_index].bytecode) << " ";
            *(local_stack[current_local_stack_index].bytecode) << arg;
            *(local_stack[current_local_stack_index].bytecode) << "\n";
        };
        void BytecodeOutput::putOpecode(opcr opecode, vint args)
        {
            putHex(opecode);

            for (int i = 0; i < args.size(); i++)
            {
                *(local_stack[current_local_stack_index].bytecode) << args[i];
                *(local_stack[current_local_stack_index].bytecode) << " ";
            }
            *(local_stack[current_local_stack_index].bytecode) << "\n";
        };
        void BytecodeOutput::putOpecode(opcr opecode, vstring args)
        {
            putHex(opecode);

            for (int i = 0; i < args.size(); i++)
            {
                *(local_stack[current_local_stack_index].bytecode) << args[i];
                *(local_stack[current_local_stack_index].bytecode) << " ";
            }
            *(local_stack[current_local_stack_index].bytecode) << "\n";
        };

        int BytecodeOutput::newLocalVariable(string name, int type)
        {
            return local_stack[current_local_stack_index].newLocalVariable(name, type);
        }

        BytecodeOutput::~BytecodeOutput()
        {
            outputfile->close();
        }
        void BytecodeOutput::switchClass()
        {
            function_class_latest_id++;
            newLocalStack(lacal_stack_type_object);

            *(local_stack[current_local_stack_index].bytecode) << "class " << current_local_stack_index << "\n";
        }

        void BytecodeOutput::returnClass()
        {
            vint has = local_stack[current_local_stack_index].getHasFunctionList();
            *(local_stack[current_local_stack_index].bytecode) << "     has_function ";
            for (int i = 0; i < has.size(); i++)
            {
                *(local_stack[current_local_stack_index].bytecode) << has[i] << "";
            }
            *(local_stack[current_local_stack_index].bytecode) << "\n";

            *(local_stack[current_local_stack_index].bytecode) << "end class " << current_local_stack_index << "\n";
            processedStackTop();
        }

        void BytecodeOutput::switchFunction()
        {
            function_class_latest_id++;
            newLocalStack(lacal_stack_type_function);

            *(local_stack[current_local_stack_index].bytecode) << "function " << current_local_stack_index << "\n";
        }

        void BytecodeOutput::returnFunction()
        {
            *(local_stack[current_local_stack_index].bytecode) << "end function " << current_local_stack_index << "\n";
            processedStackTop();
            registryFunctionToClass(function_class_latest_id);
        }

        void BytecodeOutput::registryFunctionToClass(int function_id)
        {
            for (int i = local_stack.size() - 1; i >= 0; i--)
            {
                if (local_stack[i].getLocalStackType() == lacal_stack_type_object)
                {
                    local_stack[i].pushHasFunctionList(function_id);
                    return;
                }
            }
        }

        void BytecodeOutput::newLocalStack(int s_type)
        {
            printf("newLocalStack %10d\n", local_stack.size());
            local_stack.emplace_back(s_type);
            printf("newLocalStack %10d\n", local_stack.size());
            current_local_stack_index = getProcessingStackTop();
        }
        void BytecodeOutput::newLocalStack()
        {
            printf("newLocalStack %10d\n", local_stack.size());
            local_stack.emplace_back();
            printf("newLocalStack %10d\n", local_stack.size());
            current_local_stack_index = getProcessingStackTop();
        }

        void BytecodeOutput::processedStackTop()
        {
            if (!local_stack[current_local_stack_index].getIsProcessed())
            {
                return;
            }

            printf("processedStackTop %10d\n", local_stack.size());

            local_stack[current_local_stack_index].setIsProcessed(false);

            *outputfile << "\n";
            *outputfile << local_stack[current_local_stack_index].bytecode->str();
            *outputfile << "\n";
            local_stack[current_local_stack_index].bytecode->clear();
            current_local_stack_index = getProcessingStackTop();
        }

        int BytecodeOutput::getProcessingStackTop()
        {
            for (int i = local_stack.size() - 1; i >= 0; i--)
            {
                if (local_stack[i].getIsProcessed())
                {
                    return i;
                }
            }
            return 0;
        }

        bool BytecodeOutput::isFindLocalVariable(string name)
        {
            for (int i = local_stack.size() - 1; i >= 0; i--)
            {
                if (local_stack[i].getIsProcessed())
                {
                    // mapに存在するか
                    if (local_stack[i].isFindLocalVariable(name))
                    {
                        return true;
                    }
                }
            }

            return false;
        }

        LocalVariable BytecodeOutput::findLocalVariable(string name)
        {
            for (int i = local_stack.size() - 1; i >= 0; i--)
            {
                if (local_stack[i].getIsProcessed())
                {
                    // mapに存在するか
                    if (local_stack[i].isFindLocalVariable(name))
                    {
                        return local_stack[i].getLocalVariable(name);
                    }
                }
            }

            return LocalVariable("underfind", -1, 0);
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
            lacal_stack_type = lacal_stack_type_object;
        }
        LocalStack::LocalStack(int s_type)
        {
            is_prossesing = true;
            local_variable_map = {};
            bytecode = new std::ostringstream();
            lacal_stack_type = s_type;
        }

        LocalStack::~LocalStack()
        {
        }

        void LocalStack::pushHasFunctionList(int index)
        {
            has_function_list.push_back(index);
        }

        int LocalStack::getLocalStackType()
        {
            return lacal_stack_type;
        }
        void LocalStack::setIsProcessed(int flag)
        {
            is_prossesing = flag;
        }

        bool LocalStack::getIsProcessed()
        {
            return is_prossesing;
        }

        vint LocalStack::getHasFunctionList()
        {
            return has_function_list;
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