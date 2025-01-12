

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

        const int lacal_stack_type_function = 0;
        const int lacal_stack_type_object = 1;

        struct LocalVariable
        {
            string name;
            int index;
            int type; // type_mapに依存

            LocalVariable() : name("void"), index(-1), type(0) {}
            LocalVariable(string name, int index, int type) : name(name), index(index), type(type) {}
        };

        class LocalStack
        {
        private:
            bool is_prossesing; // 処理中ならtrue 処理済みならfalse
            std::map<std::string, LocalVariable> local_variable_map;
            int lacal_stack_type;
            vint has_function_list = {}; // このLocalStackがClassの場合、下級のFunction LocalStackのindexを保持する

        public:
            std::ostringstream *bytecode;

            LocalStack();
            LocalStack(int);
            ~LocalStack();
            int newLocalVariable(string, int);

            void setIsProcessed(int);
            bool getIsProcessed();

            bool isFindLocalVariable(string);
            bool isFindLocalVariable(string, int);
            bool isFindLocalVariable(string, string);

            vint getHasFunctionList();

            int getLocalStackType();
            void pushHasFunctionList(int);

            LocalVariable getLocalVariable(string);
        };

        class BytecodeOutput
        {
        private:
            ofstream *outputfile;
            int function_class_latest_id;

            vector<LocalStack> local_stack;
            int current_local_stack_index;

        public:
            BytecodeOutput(string file_name);
            ~BytecodeOutput();

            int newLocalVariable(string, int);

            void putHex(opcr opecode);
            string getHex(opcr opecode);
            void putOpecode(opcr);
            void putOpecode(opcr, int);
            void putOpecode(opcr, string);
            void putOpecode(opcr, vint);
            void putOpecode(opcr, vstring);

            void registryFunctionToClass(int);

            int getProcessingStackTop();

            void switchFunction();
            void returnFunction();
            void switchClass();
            void returnClass();
            // あたらにLocalStackを作成
            void newLocalStack();
            void newLocalStack(int);
            // 処理済みのLocalStackのフラグを帰る
            void processedStackTop();
            void processedStack(int);

            bool isFindLocalVariable(string);
            LocalVariable findLocalVariable(string);

            bool isFindLocalVariable(string, int);
            LocalVariable findLocalVariable(string, int);
        };

    }

}
#endif