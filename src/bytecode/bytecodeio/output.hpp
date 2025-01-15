

#ifndef __BYTECODE_IO_I
#define __BYTECODE_IO_I

#include "./../bytecode_definition.hpp"
#include <vector>
#include <string>
#include <map>
#include <iostream>

namespace Bytecode
{
    namespace BytecodeIO
    {
        using namespace std;

        const int local_stack_type_function = 0;
        const int local_stack_type_object = 1;

        struct LocalVariable
        {
            string name;
            int index;
            opcr type; // type_mapに依存

            LocalVariable() : name("void"), index(-1), type(0) {}
            LocalVariable(string name, int index, opcr type) : name(name), index(index), type(type) {}
        };

        class LocalStack
        {
        private:
            std::map<std::string, LocalVariable> local_variable_map;
            int local_stack_type;
            vint children = {}; // 木構造の子要素のindexを保持する
            int parent;
            int directly_index;

        public:
            std::ostringstream *bytecode;

            LocalStack();
            LocalStack(int, int, int);
            ~LocalStack();
            int newLocalVariable(string, int, opcr);

            bool isFindLocalVariable(string);
            bool isFindLocalVariable(string, opcr);
            bool isFindLocalVariable(string, string);

            vint getChidren();

            int getParent();

            int getLocalStackType();

            int getDirectlyIndex();

            void putChild(int);

            std::map<std::string, LocalVariable> getLocalVariableMap();
            LocalVariable getLocalVariable(string);
        };

        class BytecodeOutput
        {
        private:
            ofstream *outputfile;

            vector<LocalStack> local_stack;
            int current_local_stack_index;

            int anonymous_function_count = 0;
            int opcr_label_unique_id = 0;

        public:
            vstring token_class_type;
            BytecodeOutput(string file_name, vstring tct);
            ~BytecodeOutput();

            int newLocalVariable(string, opcr);

            void setCurrentLocalStackIndex(int);
            int getCurrentLocalStackIndex();

            string getAnonymousFunctionName();
            int getOpecodeLabelUniqueId();

            void putHex(opcr opecode);
            string getHex(opcr opecode);
            void putOpecode(opcr);
            void putOpecode(opcr, int);
            void putOpecode(opcr, string);

            void putOpecode(opcr, opcr);
            void putOpecode(opcr, opcr, int);
            void putOpecode(opcr, opcr, string);

            void putOpecode(opcr, vint);
            void putOpecode(opcr, vstring);

            void putOpecode(opcr, opcr, vint);
            void putOpecode(opcr, opcr, vstring);

            void processedStackTop();

            void switchFunction(int);
            void returnFunction();
            void switchClass(int);
            void returnClass();
            // あたらにLocalStackを作成
            void newLocalStack();
            void newLocalStack(opcr, int);

            bool isFindLocalVariable(string);
            LocalVariable findLocalVariable(string);

            bool isFindLocalVariable(string, opcr);
            LocalVariable findLocalVariable(string, opcr);

            // 対象となるLocalStackを明記したうえで検索
            bool isFindLocalVariable(string, opcr, int);
            LocalVariable findLocalVariable(string, opcr, int);

            vector<LocalVariable> findLocalVariableAll(string);
        };

    }
}
#endif