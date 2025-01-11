#include "./../bytecode_definition.hpp"
#include "./translator.hpp"

#include <iostream>
#include <fstream>

namespace Bytecode
{
    namespace Translator
    {
        using namespace BytecodeIO;

        // argument制御
        void recursionArgument(vSyntacticTree &tree, BytecodeOutput *bo, int parent_node_index, int current_node_index)
        {
            SyntacticTreeNode current_node = tree[current_node_index];

            if (current_node.token_label == is_id_NonterminalSymbol)
            {
                if (current_node.token == "<value_definition>")
                {
                    // スタックマシンからローカル変数へ
                    printf("putHex %d %d %d\n", Opecode::s_store, Opecode::resolvOpecrType(tree.begin()->token), Opecode::s_store + Opecode::resolvOpecrType(tree.begin()->token));

                    int index = bo->newLocalVariable(tree[current_node.children[1]].token, Opecode::resolvOpecrType(tree[current_node.children[0]].token));
                    bo->putOpecode(Opecode::s_store + Opecode::resolvOpecrType(tree.begin()->token), index);
                    return;
                }
                else if (current_node.token == "<argument>")
                {
                    for (int i = 0; i < current_node.children.size(); i++)
                    {
                        recursionArgument(tree, bo, current_node_index, current_node.children[i]);
                    }
                    return;
                }
            }
        }

        void recursionTree(vSyntacticTree &tree, BytecodeOutput *bo, int parent_node_index, int current_node_index)
        {
            SyntacticTreeNode current_node = tree[current_node_index];

            if (current_node.token_label == is_id_NonterminalSymbol)
            {
                if (current_node.token == "<function>")
                {
                    bo->switchFunction();

                    // 引数がある場合 index 1は => 2は記号
                    if (current_node.children.size() == 3)
                    {
                        recursionArgument(tree, bo, current_node_index, current_node.children[0]);
                        recursionTree(tree, bo, current_node_index, current_node.children[2]);
                    }
                    // 引数がない場合
                    if (current_node.children.size() == 2)
                    {
                        recursionTree(tree, bo, current_node_index, current_node.children[1]);
                    }

                    bo->returnFunction();
                    return;
                }
            }

            for (int i = 0; i < current_node.children.size(); i++)
            {
                printf("Translator RecursionTree : %5d | %5d | %20s | %20s -> %5d | %20s\n",
                       current_node_index, current_node.token_label, current_node.token.c_str(), current_node.parent_token.c_str(), current_node.children[i], tree[current_node.children[i]].token.c_str());
                recursionTree(tree, bo, current_node_index, current_node.children[i]);
                printf("Translator RecursionTree : %5d | Return \n", current_node_index);
            }

            if (parent_node_index < 0)
            {
                return;
            }
            // printf("Translator RecursionTree : %d %d %s %s\n", current_node_index, current_node.token_label, current_node.token.c_str(), current_node.parent_token.c_str());

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
                if (current_node.token == "if")
                {
                    bo->putOpecode(Opecode::s_goto, 0);
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
            // printf("Translator RecursionTree %d Return \n", current_node_index);
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