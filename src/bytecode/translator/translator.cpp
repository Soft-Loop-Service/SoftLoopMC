#include "./../bytecode_definition.hpp"
#include "./translator.hpp"

#include <iostream>
#include <fstream>

namespace Bytecode
{
    namespace Translator
    {
        using namespace BytecodeIO;

        bool isFloat(string token)
        {
            return (token.find(".") != string::npos);
        }

        SyntacticTreeNode getChildrenNode(vSyntacticTree &tree, int request_node_index, int request_children_index)
        {
            if (request_node_index < 0 || request_node_index >= tree.size())
            {
                // 例外スロー
                printf("error : %d is out of range (tree)\n", request_node_index);
                exit(1);
            }
            SyntacticTreeNode current_node = tree[request_node_index];

            if (request_children_index < 0 || request_children_index >= current_node.children.size())
            {
                // 例外スロー
                printf("error : %d is out of range (children)\n", request_children_index);
                exit(1);
            }

            SyntacticTreeNode c = tree[current_node.children[request_children_index]];
            return c;
        }

        SyntacticTreeNode getNode(vSyntacticTree &tree, int request_node_index)
        {
            if (request_node_index < 0 || request_node_index >= tree.size())
            {
                // 例外スロー
                printf("error : %d is out of range\n", request_node_index);
                exit(1);
            }

            SyntacticTreeNode current_node = tree[request_node_index];
            return current_node;
        }

        int definitionValue(BytecodeOutput *bo, string name, string type)
        {
            int index = bo->newLocalVariable(name, Opecode::resolvOpecrType(type, bo->token_class_type));
            bo->putOpecode(Opecode::s_store, Opecode::resolvOpecrType(type, bo->token_class_type), index);
            return index;
        }
        int definitionValue(BytecodeOutput *bo, string name, opcr type)
        {
            int index = bo->newLocalVariable(name, type);
            bo->putOpecode(Opecode::s_store, type, index);
            return index;
        }
        void definitionValue(BytecodeOutput *bo, string name, string type, int index)
        {
            bo->putOpecode(Opecode::s_store, Opecode::resolvOpecrType(type, bo->token_class_type), index);
        }
        void definitionValue(BytecodeOutput *bo, string name, opcr type, int index)
        {
            bo->putOpecode(Opecode::s_store, type, index);
        }
        int definitionValue(vSyntacticTree &tree, BytecodeOutput *bo, int parent_node_index, int current_node_index)
        {
            SyntacticTreeNode current_node = getNode(tree, current_node_index);
            string type = getChildrenNode(tree, current_node_index, 0).token;
            string name = getChildrenNode(tree, current_node_index, 1).token;
            printf("definitionValue %s %s\n", type.c_str(), name.c_str());
            return definitionValue(bo, name, type);
        }

        void recursionLeftBefore(vSyntacticTree &tree, BytecodeOutput *bo, int parent_node_index, int current_node_index)
        {
            SyntacticTreeNode current_node = getNode(tree, current_node_index);

            if (current_node.token_label == is_id_NonterminalSymbol)
            {
                if (current_node.token == "<value_definition>")
                {
                    string type = getChildrenNode(tree, current_node_index, 0).token;
                    string name = getChildrenNode(tree, current_node_index, 1).token;
                    bo->newLocalVariable(name, Opecode::resolvOpecrType(type, bo->token_class_type));
                    return;
                }
            }
            for (int i = 0; i < current_node.children.size(); i++)
            {
                printf("Translator RecursionTree %15s : %5d | %5d | %20s | %20s -> %5d | %20s\n",
                       "left before", current_node_index, current_node.token_label, current_node.token.c_str(), current_node.parent_token.c_str(), current_node.children[i], tree[current_node.children[i]].token.c_str());
                recursionLeftBefore(tree, bo, current_node_index, current_node.children[i]);
                printf("Translator RecursionTree %15s : %5d | Return \n", "left before", current_node_index);
            }
        }
        void recursionLeftAfter(vSyntacticTree &tree, BytecodeOutput *bo, int parent_node_index, int current_node_index)
        {
            SyntacticTreeNode current_node = getNode(tree, current_node_index);

            if (current_node.token_label == is_id_NonterminalSymbol)
            {
                if (current_node.token == "<value_definition>")
                {
                    string type = getChildrenNode(tree, current_node_index, 0).token;
                    string name = getChildrenNode(tree, current_node_index, 1).token;
                    LocalVariable lv = bo->findLocalVariable(name);
                    if (lv.name == "underfind")
                    {
                        printf("error : %s is underfind\n", current_node.token.c_str());
                        return;
                    }
                    bo->putOpecode(Opecode::s_store, lv.type, lv.index);
                    return;
                }
            }

            if (current_node.token_label == is_id_TerminalSymbol)
            {
                if (current_node.parent_token == "<value_name>")
                {
                    LocalVariable lv = bo->findLocalVariable(current_node.token);
                    if (lv.name == "underfind")
                    {
                        printf("error : %s is underfind\n", current_node.token.c_str());
                        return;
                    }
                    bo->putOpecode(Opecode::s_store, lv.type, lv.index);
                }
            }

            for (int i = 0; i < current_node.children.size(); i++)
            {
                printf("Translator RecursionTree %15s : %5d | %5d | %20s | %20s -> %5d | %20s\n",
                       "left after", current_node_index, current_node.token_label, current_node.token.c_str(), current_node.parent_token.c_str(), current_node.children[i], tree[current_node.children[i]].token.c_str());
                recursionLeftAfter(tree, bo, current_node_index, current_node.children[i]);
                printf("Translator RecursionTree %15s : %5d | Return \n", "left after", current_node_index);
            }
        }

        // argument制御
        void recursionArgument(vSyntacticTree &tree, BytecodeOutput *bo, int parent_node_index, int current_node_index)
        {
            SyntacticTreeNode current_node = getNode(tree, current_node_index);

            if (current_node.token_label == is_id_NonterminalSymbol)
            {
                if (current_node.token == "<value_definition>")
                {
                    // スタックマシンからローカル変数へ
                    // printf("putHex %d %d %d\n", Opecode::s_store, Opecode::resolvOpecrType(tree.begin()->token), Opecode::s_store + Opecode::resolvOpecrType(tree.begin()->token));
                    definitionValue(tree, bo, parent_node_index, current_node_index);
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
            SyntacticTreeNode current_node = getNode(tree, current_node_index);

            printf("* * * recursionTree %d \n", current_node_index);

            if (current_node.token_label == is_id_NonterminalSymbol)
            {
                if (current_node.token == "<function>")
                {
                    printf("Translator RecursionTree %15s : %5d | %5d | %20s | %20s\n",
                           "function", current_node_index, current_node.token_label, current_node.token.c_str(), current_node.parent_token.c_str());

                    int directly_index = bo->newLocalVariable(bo->getAnonymousFunctionName(), Opecode::d_function);
                    bo->putOpecode(Opecode::push, Opecode::d_function, directly_index);

                    bo->switchFunction(directly_index);

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
                    printf("Translator RecursionTree %15s : %5d | Return \n", "function", current_node_index);

                    return;
                }
                else if (current_node.token == "<return>")
                {

                    printf("Translator RecursionTree %15s : %5d | %5d | %20s | %20s -> %5d | %20s\n",
                           "return", current_node_index, current_node.token_label, current_node.token.c_str(), current_node.parent_token.c_str(), current_node.children[1], getChildrenNode(tree, current_node_index, 1).token.c_str());

                    if (current_node.children.size() == 2)
                    {
                        recursionTree(tree, bo, current_node_index, current_node.children[1]);
                    }
                    if (current_node.children.size() == 1)
                    {
                    }
                    printf("Translator RecursionTree %15s : %5d | Return \n", "return", current_node_index);

                    return;
                }
                else if (current_node.token == "<function_message_passing>")
                {
                    printf("Translator RecursionTree %15s : %5d | %5d | %20s | %20s\n",
                           "function_message_passing", current_node_index, current_node.token_label, current_node.token.c_str(), current_node.parent_token.c_str());

                    if (current_node.children.size() == 0 || current_node.children.size() > 2)
                    {
                        return;
                    }

                    if (current_node.children.size() == 2)
                    {
                        recursionTree(tree, bo, current_node_index, current_node.children[1]);
                    }

                    LocalVariable lv = bo->findLocalVariable(getChildrenNode(tree, current_node_index, 0).token, Opecode::d_function);
                    if (lv.name == "underfind")
                    {
                        printf("error : function %s is underfind\n", getChildrenNode(tree, current_node_index, 0).token.c_str());
                        return;
                    }
                    bo->putOpecode(Opecode::s_load, lv.type, lv.index);
                    bo->putOpecode(Opecode::s_invokevirtual);
                    printf("Translator RecursionTree %15s : %5d | Return \n", "function_message_passing", current_node_index);

                    return;
                }
                else if (current_node.token == "<value_definition>")
                {
                    definitionValue(tree, bo, parent_node_index, current_node_index);
                    return;
                }
                else if (current_node.token == "<DEFINITION>")
                {
                    if (current_node.children.size() < 3 || 4 < current_node.children.size())
                    {
                        return;
                    }
                    printf("Translator RecursionTree %15s : %5d | %5d | %20s | %20s -> %5d | %20s\n",
                           "DEFINITION", current_node_index, current_node.token_label, current_node.token.c_str(), current_node.parent_token.c_str(), current_node.children[2], tree[current_node.children[2]].token.c_str());

                    string definition_type = getChildrenNode(tree, current_node_index, 0).token;
                    string definition_name = getChildrenNode(tree, current_node_index, 1).token;

                    int directly_index = -1;

                    if (definition_type == "class" || definition_type == "component")
                    {
                        opcr type = Opecode::resolvOpecrType(definition_name, bo->token_class_type);
                        directly_index = bo->newLocalVariable(definition_name, type);
                        bo->putOpecode(Opecode::push, type, directly_index);
                    }

                    bo->switchClass(directly_index);

                    if (current_node.children.size() == 3)
                    {
                        recursionTree(tree, bo, current_node_index, current_node.children[2]);
                    }
                    if (current_node.children.size() == 4)
                    {
                        recursionTree(tree, bo, current_node_index, current_node.children[3]);
                    }

                    bo->returnClass();
                    printf("Translator RecursionTree %15s : %5d | Return \n", "DEFINITION", current_node_index);

                    return;
                }

                else if (current_node.token == "<new_class>")
                {

                    if (current_node.children.size() == 2)
                    {
                        printf("Translator RecursionTree %15s : %5d | %5d | %20s | %20s -> %5d | %20s\n",
                               "new_class A", current_node_index, current_node.token_label, current_node.token.c_str(), current_node.parent_token.c_str(), current_node.children[1], getChildrenNode(tree, current_node_index, 1).token.c_str());
                        string type = getChildrenNode(tree, current_node_index, 1).token;
                        LocalVariable lv = bo->findLocalVariable(type, Opecode::resolvOpecrType(type, bo->token_class_type));
                        bo->putOpecode(Opecode::s_instance, lv.index);
                        printf("Translator RecursionTree %15s : %5d | Return \n", "new_class A", current_node_index);

                        return;
                    }
                    if (current_node.children.size() == 3)
                    {
                        printf("Translator RecursionTree %15s : %5d | %5d | %20s | %20s -> %5d | %20s\n",
                               "new_class A", current_node_index, current_node.token_label, current_node.token.c_str(), current_node.parent_token.c_str(), current_node.children[2], tree[current_node.children[2]].token.c_str());

                        recursionTree(tree, bo, current_node_index, current_node.children[2]);
                        string type = getChildrenNode(tree, current_node_index, 1).token;
                        LocalVariable lv = bo->findLocalVariable(type, Opecode::resolvOpecrType(type, bo->token_class_type));
                        bo->putOpecode(Opecode::s_instance, lv.index);
                        printf("Translator RecursionTree %15s : %5d | Return \n", "new_class B", current_node_index);

                        return;
                    }
                    return;
                }

                else if (current_node.token == "<if_while>")
                {
                }
            }
            if (current_node.token_label == is_id_TerminalSymbol)
            {
                if (current_node.token == "=")
                {
                    recursionLeftBefore(tree, bo, current_node_index, current_node.children[0]);
                    for (int i = 1; i < current_node.children.size(); i++)
                    {
                        printf("Translator RecursionTree %15s : %5d | %5d | %20s | %20s -> %5d | %20s\n",
                               "= right", current_node_index, current_node.token_label, current_node.token.c_str(), current_node.parent_token.c_str(), current_node.children[i], tree[current_node.children[i]].token.c_str());

                        recursionTree(tree, bo, current_node_index, current_node.children[i]);
                        printf("Translator RecursionTree %15s : %5d | Return \n", "= right", current_node_index);
                    }
                    printf("Translator RecursionTree %15s : %5d | %5d | %20s | %20s -> %5d | %20s\n",
                           "= left", current_node_index, current_node.token_label, current_node.token.c_str(), current_node.parent_token.c_str(), current_node.children[0], getChildrenNode(tree, current_node_index, 0).token.c_str());

                    recursionLeftAfter(tree, bo, current_node_index, current_node.children[0]);
                    printf("Translator RecursionTree %15s : %5d | Return \n", "= left", current_node_index);

                    return;
                }
            }
            for (int i = 0; i < current_node.children.size(); i++)
            {
                printf("Translator RecursionTree %15s : %5d | %5d | %20s | %20s -> %5d | %20s\n",
                       "main", current_node_index, current_node.token_label, current_node.token.c_str(), current_node.parent_token.c_str(), current_node.children[i], tree[current_node.children[i]].token.c_str());
                recursionTree(tree, bo, current_node_index, current_node.children[i]);
                printf("Translator RecursionTree %15s : %5d | Return \n", "main", current_node_index);
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
                    bo->putOpecode(Opecode::push, isFloat(current_node.token) ? Opecode::d_float : Opecode::d_int, current_node.token);
                }
                if (current_node.parent_token == "<string>")
                {
                    bo->putOpecode(Opecode::push, Opecode::d_int, current_node.token);
                }
                if (current_node.parent_token == "<value_name>")
                {
                    LocalVariable lv = bo->findLocalVariable(current_node.token);
                    if (lv.name == "underfind")
                    {
                        printf("error : %s is underfind\n", current_node.token.c_str());
                        return;
                    }
                    bo->putOpecode(Opecode::s_load, lv.type, lv.index);
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
        void translator(vSyntacticTree &tree, string file_name, vstring token_class_type)
        {
            BytecodeOutput *bo = new BytecodeOutput(file_name, token_class_type);
            recursionTree(tree, bo, -1, 0);
            bo->processedStackTop();
            delete bo;
            return;
        };
    };
};