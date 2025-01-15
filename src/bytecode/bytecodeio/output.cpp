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

        BytecodeOutput::BytecodeOutput(string file_name, vstring tct)
        {
            outputfile = new ofstream(file_name);
            local_stack = {};
            local_stack.emplace_back();
            current_local_stack_index = 0;
            token_class_type = tct;
        }
        string BytecodeOutput::getHex(opcr opecode)
        {
            std::ostringstream oss;
            oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opecode);
            return oss.str();
        }
        void BytecodeOutput::putHex(opcr opecode)
        {
            *(local_stack[getCurrentLocalStackIndex()].bytecode) << std::setw(3) << std::setfill('0') << opecode;
            return;
            // *(local_stack[getCurrentLocalStackIndex()].bytecode) << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opecode);
        }
        void BytecodeOutput::putOpecode(opcr opecode)
        {
            putHex(opecode);
            *(local_stack[getCurrentLocalStackIndex()].bytecode) << "\n";
        };

        void BytecodeOutput::putOpecode(opcr opecode, int arg)
        {
            putHex(opecode);
            *(local_stack[getCurrentLocalStackIndex()].bytecode) << " ";
            *(local_stack[getCurrentLocalStackIndex()].bytecode) << arg;
            *(local_stack[getCurrentLocalStackIndex()].bytecode) << "\n";
        };
        void BytecodeOutput::putOpecode(opcr opecode, string arg)
        {
            putHex(opecode);
            *(local_stack[getCurrentLocalStackIndex()].bytecode) << " ";
            *(local_stack[getCurrentLocalStackIndex()].bytecode) << arg;
            *(local_stack[getCurrentLocalStackIndex()].bytecode) << "\n";
        };
        void BytecodeOutput::putOpecode(opcr opecode, opcr opecode_d_type)
        {
            putHex(opecode);
            *(local_stack[getCurrentLocalStackIndex()].bytecode) << " ";
            putHex(opecode_d_type);
            *(local_stack[getCurrentLocalStackIndex()].bytecode) << "\n";
        };
        void BytecodeOutput::putOpecode(opcr opecode, opcr opecode_d_type, int arg)
        {
            putHex(opecode);
            *(local_stack[getCurrentLocalStackIndex()].bytecode) << " ";
            putHex(opecode_d_type);
            *(local_stack[getCurrentLocalStackIndex()].bytecode) << " ";
            *(local_stack[getCurrentLocalStackIndex()].bytecode) << arg;
            *(local_stack[getCurrentLocalStackIndex()].bytecode) << "\n";
        };
        void BytecodeOutput::putOpecode(opcr opecode, opcr opecode_d_type, string arg)
        {
            putHex(opecode);
            *(local_stack[getCurrentLocalStackIndex()].bytecode) << " ";
            putHex(opecode_d_type);
            *(local_stack[getCurrentLocalStackIndex()].bytecode) << " ";
            *(local_stack[getCurrentLocalStackIndex()].bytecode) << arg;
            *(local_stack[getCurrentLocalStackIndex()].bytecode) << "\n";
        };

        void BytecodeOutput::putOpecode(opcr opecode, opcr opecode_d_type, vint args)
        {
            putHex(opecode);
            *(local_stack[getCurrentLocalStackIndex()].bytecode) << " ";
            putHex(opecode_d_type);
            *(local_stack[getCurrentLocalStackIndex()].bytecode) << " ";

            for (int i = 0; i < args.size(); i++)
            {
                *(local_stack[getCurrentLocalStackIndex()].bytecode) << args[i];
                *(local_stack[getCurrentLocalStackIndex()].bytecode) << " ";
            }
            *(local_stack[getCurrentLocalStackIndex()].bytecode) << "\n";
        };
        void BytecodeOutput::putOpecode(opcr opecode, opcr opecode_d_type, vstring args)
        {
            putHex(opecode);
            *(local_stack[getCurrentLocalStackIndex()].bytecode) << " ";
            putHex(opecode_d_type);
            *(local_stack[getCurrentLocalStackIndex()].bytecode) << " ";

            for (int i = 0; i < args.size(); i++)
            {
                *(local_stack[getCurrentLocalStackIndex()].bytecode) << args[i];
                *(local_stack[getCurrentLocalStackIndex()].bytecode) << " ";
            }
            *(local_stack[getCurrentLocalStackIndex()].bytecode) << "\n";
        };

        void BytecodeOutput::putOpecode(opcr opecode, vint args)
        {
            putHex(opecode);
            *(local_stack[getCurrentLocalStackIndex()].bytecode) << " ";

            for (int i = 0; i < args.size(); i++)
            {
                *(local_stack[getCurrentLocalStackIndex()].bytecode) << args[i];
                *(local_stack[getCurrentLocalStackIndex()].bytecode) << " ";
            }
            *(local_stack[getCurrentLocalStackIndex()].bytecode) << "\n";
        };
        void BytecodeOutput::putOpecode(opcr opecode, vstring args)
        {
            putHex(opecode);
            *(local_stack[getCurrentLocalStackIndex()].bytecode) << " ";

            for (int i = 0; i < args.size(); i++)
            {
                *(local_stack[getCurrentLocalStackIndex()].bytecode) << args[i];
                *(local_stack[getCurrentLocalStackIndex()].bytecode) << " ";
            }
            *(local_stack[getCurrentLocalStackIndex()].bytecode) << "\n";
        };

        int BytecodeOutput::newLocalVariable(string name, opcr type)
        {

            int count = 0;
            for (int i = 0; i < local_stack.size(); i++)
            {
                count += local_stack[i].getLocalVariableMap().size();
            }
            printf("newLocalVariable ls:%10d ci:%d pci:%d\n", local_stack.size(), getCurrentLocalStackIndex(), count);

            return local_stack[getCurrentLocalStackIndex()].newLocalVariable(name, count, type);
        }

        BytecodeOutput::~BytecodeOutput()
        {
            for (int i = 0; i < local_stack.size(); i++)
            {
                printf(" %3d : p:%3d t:%3d | c:", i, local_stack[i].getParent(), local_stack[i].getLocalStackType());

                for (int j = 0; j < local_stack[i].getChidren().size(); j++)
                {
                    printf("%3d ", local_stack[i].getChidren()[j]);
                }

                std::map<std::string, LocalVariable> local_variable_map = local_stack[i].getLocalVariableMap();

                printf(" | v:%d ", local_variable_map.size());
                for (const auto &pair : local_variable_map)
                {
                    printf("n:%s(%s) t:%d i:%d ,", pair.first.c_str(), pair.second.name.c_str(), pair.second.type, pair.second.index); // pair.first がキー
                }

                printf("\n");
            }

            outputfile->close();
        }
        void BytecodeOutput::switchClass(int directly_index)
        {
            newLocalStack(local_stack_type_object, directly_index);
            *(local_stack[getCurrentLocalStackIndex()].bytecode) << "# class ci :" << getCurrentLocalStackIndex() << "| di :" << directly_index << "\n";
            putOpecode(Opecode::head_start_class, directly_index);
        }

        void BytecodeOutput::returnClass()
        {
            putOpecode(Opecode::head_end_class);

            vint has = local_stack[getCurrentLocalStackIndex()].getChidren();
            *(local_stack[getCurrentLocalStackIndex()].bytecode) << "#     has_function ";
            for (int i = 0; i < has.size(); i++)
            {
                *(local_stack[getCurrentLocalStackIndex()].bytecode) << has[i] << " ";
            }
            *(local_stack[getCurrentLocalStackIndex()].bytecode) << "\n";

            *(local_stack[getCurrentLocalStackIndex()].bytecode) << "# end class ci :" << getCurrentLocalStackIndex() << "| di :" << local_stack[getCurrentLocalStackIndex()].getDirectlyIndex() << "\n";
            processedStackTop();
        }

        void BytecodeOutput::switchFunction(int directly_index)
        {
            newLocalStack(local_stack_type_function, directly_index);
            *(local_stack[getCurrentLocalStackIndex()].bytecode) << "# function ci :" << getCurrentLocalStackIndex() << "| di :" << directly_index << "\n";
            putOpecode(Opecode::head_start_function, directly_index);
        }

        void BytecodeOutput::returnFunction()
        {
            putOpecode(Opecode::head_end_function);
            *(local_stack[getCurrentLocalStackIndex()].bytecode) << "# end function ci :" << getCurrentLocalStackIndex() << "| di :" << local_stack[getCurrentLocalStackIndex()].getDirectlyIndex() << "\n";
            processedStackTop();
        }

        string BytecodeOutput::getAnonymousFunctionName()
        {
            return "af_" + std::to_string(anonymous_function_count++);
        }
        int BytecodeOutput::getOpecodeLabelUniqueId()
        {
            return opcr_label_unique_id++;
        }

        void BytecodeOutput::newLocalStack(opcr s_type, int directly_index)
        {
            int parent = getCurrentLocalStackIndex();
            local_stack.emplace_back(s_type, parent, directly_index);

            int new_index = local_stack.size() - 1;
            printf("newLocalStack p:%10d cc:%10d -> nc:%10d \n", parent, getCurrentLocalStackIndex(), new_index);

            setCurrentLocalStackIndex(new_index);
            local_stack[parent].putChild(new_index);
        }
        void BytecodeOutput::newLocalStack()
        {
            local_stack.emplace_back();
            int new_index = local_stack.size() - 1;

            printf("newLocalStack %10d -> %10d \n", getCurrentLocalStackIndex(), new_index);
            setCurrentLocalStackIndex(new_index);
        }

        void BytecodeOutput::processedStackTop()
        {
            printf("processedStackTop c:%10d p:%10d\n", getCurrentLocalStackIndex(), local_stack[getCurrentLocalStackIndex()].getParent());

            *outputfile << "\n";
            *outputfile << local_stack[getCurrentLocalStackIndex()].bytecode->str();
            *outputfile << "\n";
            local_stack[getCurrentLocalStackIndex()].bytecode->clear();

            int parent = local_stack[getCurrentLocalStackIndex()].getParent();

            if (parent >= 0)
            {
                setCurrentLocalStackIndex(parent);
            }
        }

        bool BytecodeOutput::isFindLocalVariable(string name)
        {
            int search = getCurrentLocalStackIndex();
            while (search != -1)
            { // mapに存在するか
                if (local_stack[search].isFindLocalVariable(name))
                {
                    return true;
                }
                search = local_stack[search].getParent();
            }
            return false;
        }

        LocalVariable BytecodeOutput::findLocalVariable(string name)
        {
            int search = getCurrentLocalStackIndex();
            while (search != -1)
            { // mapに存在するか
                if (local_stack[search].isFindLocalVariable(name))
                {
                    return local_stack[search].getLocalVariable(name);
                }
                search = local_stack[search].getParent();
            }
            return LocalVariable("underfind", -1, 0);
        }

        bool BytecodeOutput::isFindLocalVariable(string name, opcr type)
        {
            int search = getCurrentLocalStackIndex();
            while (search != -1)
            { // mapに存在するか
                if (local_stack[search].isFindLocalVariable(name, type))
                {
                    printf("isFindLocalVariable | find:true %d %s %d\n", search, name.c_str(), type);
                    return true;
                }
                search = local_stack[search].getParent();
            }
            printf("isFindLocalVariable | find:false %d %s %d\n", search, name.c_str(), type);
            return false;
        }

        LocalVariable BytecodeOutput::findLocalVariable(string name, opcr type)
        {
            int search = getCurrentLocalStackIndex();
            while (search != -1)
            { // mapに存在するか
                if (local_stack[search].isFindLocalVariable(name, type))
                {
                    printf("FindLocalVariable | find:true %d %s %d\n", search, name.c_str(), type);
                    return local_stack[search].getLocalVariable(name);
                }
                search = local_stack[search].getParent();
            }
            printf("FindLocalVariable | find:false %d %s %d\n", search, name.c_str(), type);
            return LocalVariable("underfind", -1, 0);
        }

        int BytecodeOutput::getCurrentLocalStackIndex()
        {
            return current_local_stack_index;
        }

        void BytecodeOutput::setCurrentLocalStackIndex(int index)
        {
            printf("setCurrentLocalStackIndex %10d -> %10d\n", current_local_stack_index, index);
            current_local_stack_index = index;
        }

        LocalStack::LocalStack()
        {
            printf("LocalStack::LocalStack \n");

            local_variable_map = {};
            children = {};
            bytecode = new std::ostringstream();
            local_stack_type = local_stack_type_object;
            parent = -1;
        }
        LocalStack::LocalStack(int s_type, int p, int di)
        {
            printf("LocalStack::LocalStack %d %d\n", s_type, p);

            local_variable_map = {};
            children = {};
            bytecode = new std::ostringstream();
            local_stack_type = s_type;
            parent = p;
            directly_index = di;
        }

        LocalStack::~LocalStack()
        {
        }

        std::map<std::string, LocalVariable> LocalStack::getLocalVariableMap()
        {
            return local_variable_map;
        }

        int LocalStack::getLocalStackType()
        {
            return local_stack_type;
        }

        int LocalStack::getParent()
        {
            return parent;
        }

        vint LocalStack::getChidren()
        {
            return children;
        }

        int LocalStack::getDirectlyIndex()
        {
            return directly_index;
        }

        int LocalStack::newLocalVariable(string name, int index, opcr type)
        {
            LocalVariable lv = LocalVariable(name, index, type);
            local_variable_map[name] = lv;
            return index;
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

        bool LocalStack::isFindLocalVariable(string name, opcr type)
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

        void LocalStack::putChild(int index)
        {
            children.push_back(index);
        }

    }
}