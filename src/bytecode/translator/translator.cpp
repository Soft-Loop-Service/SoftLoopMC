#include "./../bytecode_definition.hpp"
#include "./translator.hpp"

#include <iostream>
#include <fstream>

namespace Bytecode
{
    namespace Translator
    {
        using namespace BytecodeIO;
        void recursionTree(vSyntacticTree &tree, BytecodeOutput *bo, int parent_node_index, int current_node_index)
        {
            SyntacticTreeNode current_node = tree[current_node_index];
            for (int i = 0; i < current_node.children.size(); i++)
            {
                recursionTree(tree, bo, current_node_index, current_node.children[i]);
            }

            if (parent_node_index < 0)
            {
                return;
            }
            printf("Translator RecursionTree : %d %d %s %s\n", current_node_index, current_node.token_label, current_node.token.c_str(), current_node.parent_token.c_str());

            if (current_node.token_label == is_id_TerminalSymbol)
            {
                if (current_node.parent_token == "<number>")
                {
                    bo->putOpecode(Opecode::push_int, current_node.token);
                }
                if (current_node.parent_token == "<string>")
                {
                    bo->putOpecode(Opecode::push_str, current_node.token);
                }
                if (current_node.token == "==")
                {
                    bo->putOpecode(Opecode::s_if_icmpeq);
                }
                if (current_node.token == ">=")
                {
                    bo->putOpecode(Opecode::s_if_icmpge);
                }
                if (current_node.token == "+")
                {
                    bo->putOpecode(Opecode::c_add);
                }
                if (current_node.token == "-")
                {
                    bo->putOpecode(Opecode::c_subtraction);
                }
                if (current_node.token == "*")
                {
                    bo->putOpecode(Opecode::c_multiplication);
                }
                if (current_node.token == "/")
                {
                    bo->putOpecode(Opecode::c_division);
                }
            }
        }
        void translator(vSyntacticTree &tree, string file_name)
        {
            BytecodeOutput *bo = new BytecodeOutput(file_name);
            recursionTree(tree, bo, -1, 0);
            delete bo;
            return;
        };
    };
};