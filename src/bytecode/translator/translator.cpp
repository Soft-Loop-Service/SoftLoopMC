#include "./../bytecode_definition.hpp"
#include "./translator.hpp"

#include <iostream>
#include <fstream>

namespace Bytecode
{
    namespace Translator
    {
        using namespace BytecodeIO;
        typedef map<string, int> JumpMap;

        void recursionTree(vSyntacticTree &tree, BytecodeOutput *bo, JumpMap jump_map, int parent_node_index, int current_node_index);

        bool isFloat(string token)
        {
            return (token.find(".") != string::npos);
        }

        int getChildrenNodeIndex(vSyntacticTree &tree, int request_node_index, int request_children_index)
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
            int i = current_node.children[request_children_index];
            return i;
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

        struct MethodChainLiner
        {
            int type; // 0 : value 1 : method
            int node_index;
        };

        // expression以下のメソッドチェーンを解析する
        void recursionExpressionToLinear(vSyntacticTree &tree, BytecodeOutput *bo, vector<MethodChainLiner> &mcl, int parent_node_index, int current_node_index)
        {
            SyntacticTreeNode current_node = getNode(tree, current_node_index);

            if (current_node.parent_token == "<value_name>")
            {
                mcl.emplace_back(MethodChainLiner{0, current_node_index});
                return;
            }
            else if (current_node.token_label == is_id_NonterminalSymbol && current_node.token == "<function_message_passing>")
            {
                mcl.emplace_back(MethodChainLiner{1, current_node_index});
                return;
            }

            for (int i = 0; i < current_node.children.size(); i++)
            {
                printf("Translator RecursionTree %15s : %5d | %5d | %20s | %20s -> %5d | %20s\n",
                       "expression", current_node_index, current_node.token_label, current_node.token.c_str(), current_node.parent_token.c_str(), current_node.children[i], tree[current_node.children[i]].token.c_str());
                recursionExpressionToLinear(tree, bo, mcl, current_node_index, current_node.children[i]);
                printf("Translator RecursionTree %15s : %5d | Return \n", "expression", current_node_index);
            }
        }

        void recursionLogicalOperators(vSyntacticTree &tree, BytecodeOutput *bo, JumpMap jump_map, int parent_node_index, int current_node_index, int destination_label)
        {
            SyntacticTreeNode current_node = getNode(tree, current_node_index);

            if (current_node.token_label == is_id_TerminalSymbol)
            {
                if (current_node.token == "&&")
                {
                    int label_1 = bo->getOpecodeLabelUniqueId();
                    int label_2 = bo->getOpecodeLabelUniqueId();
                    recursionTree(tree, bo, jump_map, current_node_index, current_node.children[0]);
                    bo->putOpecode(Opecode::s_if_true, label_1);
                    recursionTree(tree, bo, jump_map, current_node_index, current_node.children[1]);
                    bo->putOpecode(Opecode::s_if_true, label_1);
                    bo->putOpecode(Opecode::push, Opecode::d_boolean, Opecode::bool_true);
                    bo->putOpecode(Opecode::s_jump, label_2);
                    bo->putOpecode(Opecode::s_label_point, label_1);
                    bo->putOpecode(Opecode::push, Opecode::d_boolean, Opecode::bool_false);
                    bo->putOpecode(Opecode::s_label_point, label_2);

                    return;
                }

                if (current_node.token == "||")
                {
                    int label_1 = bo->getOpecodeLabelUniqueId();
                    int label_2 = bo->getOpecodeLabelUniqueId();
                    recursionTree(tree, bo, jump_map, current_node_index, current_node.children[0]);
                    bo->putOpecode(Opecode::s_if_false, label_1);
                    recursionTree(tree, bo, jump_map, current_node_index, current_node.children[1]);
                    bo->putOpecode(Opecode::s_if_false, label_1);
                    bo->putOpecode(Opecode::push, Opecode::d_boolean, Opecode::bool_false);
                    bo->putOpecode(Opecode::s_jump, label_2);
                    bo->putOpecode(Opecode::s_label_point, label_1);
                    bo->putOpecode(Opecode::push, Opecode::d_boolean, Opecode::bool_true);
                    bo->putOpecode(Opecode::s_label_point, label_2);
                    return;
                }
            }

            for (int i = 0; i < current_node.children.size(); i++)
            {
                printf("Translator RecursionTree %15s : %5d | %5d | %20s | %20s -> %5d | %20s\n",
                       "operators", current_node_index, current_node.token_label, current_node.token.c_str(), current_node.parent_token.c_str(), current_node.children[i], tree[current_node.children[i]].token.c_str());
                recursionTree(tree, bo, jump_map, current_node_index, current_node.children[i]);
                printf("Translator RecursionTree %15s : %5d | Return \n", "operators", current_node_index);
            }

            if (current_node.token_label == is_id_TerminalSymbol)
            {
                if (current_node.token == "==")
                {
                    bo->putOpecode(Opecode::s_if_icmpeq, destination_label);
                }
                else if (current_node.token == "!=")
                {
                    bo->putOpecode(Opecode::s_if_icmpne, destination_label);
                }
                else if (current_node.token == ">=")
                {
                    bo->putOpecode(Opecode::s_if_icmpge, destination_label);
                }
                else if (current_node.token == "<=")
                {
                    bo->putOpecode(Opecode::s_if_icmple, destination_label);
                }
                else if (current_node.token == ">")
                {
                    bo->putOpecode(Opecode::s_if_icmpgt, destination_label);
                }
                else if (current_node.token == "<")
                {
                    bo->putOpecode(Opecode::s_if_icmplt, destination_label);
                }
                else
                {
                    recursionTree(tree, bo, jump_map, parent_node_index, current_node_index);
                    bo->putOpecode(Opecode::s_if_true, destination_label);
                }
            }
        }

        void recursionIfWhileAdvance(vSyntacticTree &tree, BytecodeOutput *bo, vint &label_list, int parent_node_index, int current_node_index)
        {
            SyntacticTreeNode current_node = getNode(tree, current_node_index);

            if (current_node.token == "<ifexpr>") // このnodeが存在するということは、else else if が存在する
            {
                // if/while文
                recursionIfWhileAdvance(tree, bo, label_list, current_node_index, getChildrenNodeIndex(tree, current_node_index, 0));

                // else if/else文
                recursionIfWhileAdvance(tree, bo, label_list, current_node_index, getChildrenNodeIndex(tree, current_node_index, 1));
            }

            else if (current_node.token == "<if_while>")
            {
                int current_label = bo->getOpecodeLabelUniqueId();
                label_list.push_back(current_label);
            }
            else if (current_node.token == "<else>")
            {
                int current_label = bo->getOpecodeLabelUniqueId();
                label_list.push_back(current_label);

                if (current_node.children.size() == 2) // else block
                {
                }
                else if (current_node.children.size() == 4) // else if 評価式 block
                {
                }
                else if (current_node.children.size() == 5) // else if 評価式 block else
                {
                    recursionIfWhileAdvance(tree, bo, label_list, current_node_index, getChildrenNodeIndex(tree, current_node_index, 4));
                }
            }
        }

        // if while文制御
        void recursionIfWhile(vSyntacticTree &tree, BytecodeOutput *bo, JumpMap jump_map, vint &label_list, int &current_label_index, int parent_node_index, int current_node_index)
        {
            SyntacticTreeNode current_node = getNode(tree, current_node_index);

            if (current_node.token == "<ifexpr>") // このnodeが存在するということは、else else if が存在する
            {
                // if/while文
                recursionIfWhile(tree, bo, jump_map, label_list, current_label_index, current_node_index, getChildrenNodeIndex(tree, current_node_index, 0));

                // else if/else文
                recursionIfWhile(tree, bo, jump_map, label_list, current_label_index, current_node_index, getChildrenNodeIndex(tree, current_node_index, 1));

                bo->putOpecode(Opecode::s_label_point, label_list[current_label_index + 1]);
            }

            else if (current_node.token == "<if_while>")
            {
                SyntacticTreeNode left = getChildrenNode(tree, current_node_index, 0);
                int left_index = getChildrenNodeIndex(tree, current_node_index, 0);
                int center_index = getChildrenNodeIndex(tree, current_node_index, 1);
                int right_index = getChildrenNodeIndex(tree, current_node_index, 2);

                printf("Translator RecursionTree %15s : %5d | %5d | %20s | %20s -> %5d | %20s\n",
                       "if_while", current_node_index, current_node.token_label, current_node.token.c_str(), current_node.parent_token.c_str(), left_index, left.token.c_str());

                if (left.token == "if")
                {
                    recursionLogicalOperators(tree, bo, jump_map, current_node_index, center_index, label_list[current_label_index + 1]);
                    recursionTree(tree, bo, jump_map, current_node_index, right_index);
                    bo->putOpecode(Opecode::s_jump, label_list.back());
                    bo->putOpecode(Opecode::s_label_point, label_list[current_label_index + 1]);

                    printf("use label list if %d: %d\n", current_label_index, label_list[current_label_index + 1]);
                }
                else if (left.token == "while")
                {
                    JumpMap jump_map_cp;
                    jump_map_cp.insert(jump_map.begin(), jump_map.end());

                    jump_map_cp["continue"] = label_list[0];
                    jump_map_cp["break"] = label_list.back();

                    bo->putOpecode(Opecode::s_label_point, label_list[current_label_index + 0]);
                    recursionLogicalOperators(tree, bo, jump_map_cp, current_node_index, center_index, label_list[current_label_index + 1]);
                    recursionTree(tree, bo, jump_map_cp, current_node_index, right_index);
                    bo->putOpecode(Opecode::s_jump, label_list[current_label_index + 0]);
                    bo->putOpecode(Opecode::s_label_point, label_list[current_label_index + 1]);

                    printf("use label list while %d: %d %d\n", current_label_index, label_list[current_label_index], label_list[current_label_index + 1]);
                }

                printf("Translator RecursionTree %15s : %5d | Return \n", "if_while", current_node_index);

                current_label_index++;
            }
            else if (current_node.token == "<else>")
            {
                if (current_node.children.size() == 2) // else block
                {
                    recursionTree(tree, bo, jump_map, current_node_index, getChildrenNodeIndex(tree, current_node_index, 1));
                    printf("use label list 2 %d: %d\n", current_label_index, label_list[current_label_index + 1]);
                }
                else if (current_node.children.size() == 4) // else if 評価式 block
                {
                    recursionLogicalOperators(tree, bo, jump_map, current_node_index, getChildrenNodeIndex(tree, current_node_index, 2), label_list[current_label_index + 1]);
                    recursionTree(tree, bo, jump_map, current_node_index, getChildrenNodeIndex(tree, current_node_index, 3));
                    bo->putOpecode(Opecode::s_jump, label_list.back());
                    bo->putOpecode(Opecode::s_label_point, label_list[current_label_index + 1]);
                    printf("use label list 4 %d: %d\n", current_label_index, label_list[current_label_index + 1]);
                    current_label_index++;
                }
                else if (current_node.children.size() == 5) // else if 評価式 block else
                {
                    recursionLogicalOperators(tree, bo, jump_map, current_node_index, getChildrenNodeIndex(tree, current_node_index, 2), label_list[current_label_index + 1]);
                    recursionTree(tree, bo, jump_map, current_node_index, getChildrenNodeIndex(tree, current_node_index, 3));
                    bo->putOpecode(Opecode::s_jump, label_list.back());
                    bo->putOpecode(Opecode::s_label_point, label_list[current_label_index + 1]);
                    printf("use label list 6 %d: %d\n", current_label_index, label_list[current_label_index + 1]);
                    // label_list.erase(label_list.begin());
                    current_label_index++;
                    recursionIfWhile(tree, bo, jump_map, label_list, current_label_index, current_node_index, getChildrenNodeIndex(tree, current_node_index, 4));
                }
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
        void recursionFunctionMessagePassing(vSyntacticTree &tree, BytecodeOutput *bo, JumpMap jump_map, int parent_node_index, int current_node_index, vint search)
        {
            SyntacticTreeNode current_node = getNode(tree, current_node_index);
            printf("Translator RecursionTree %15s : %5d | %5d | %20s | %20s\n",
                   "function_message_passing(search)", current_node_index, current_node.token_label, current_node.token.c_str(), current_node.parent_token.c_str());

            if (current_node.children.size() == 0 || current_node.children.size() > 2)
            {
                return;
            }

            if (current_node.children.size() == 2)
            {
                recursionTree(tree, bo, jump_map, current_node_index, current_node.children[1]);
            }

            bo->putOpecode(Opecode::s_inside, search);
            bo->putOpecode(Opecode::s_invokevirtual);
            printf("Translator RecursionTree %15s : %5d | Return \n", "function_message_passing(search)", current_node_index);
        }

        void recursionFunctionMessagePassing(vSyntacticTree &tree, BytecodeOutput *bo, JumpMap jump_map, int parent_node_index, int current_node_index)
        {
            SyntacticTreeNode current_node = getNode(tree, current_node_index);
            printf("Translator RecursionTree %15s : %5d | %5d | %20s | %20s\n",
                   "function_message_passing", current_node_index, current_node.token_label, current_node.token.c_str(), current_node.parent_token.c_str());

            if (current_node.children.size() == 0 || current_node.children.size() > 2)
            {
                return;
            }

            if (current_node.children.size() == 2)
            {
                recursionTree(tree, bo, jump_map, current_node_index, current_node.children[1]);
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
        }

        // void recursionTree(vSyntacticTree &tree, BytecodeOutput *bo, int parent_node_index, int current_node_index)
        // {
        //     recursionTree(tree, bo, {}, parent_node_index, current_node_index);
        // }
        void recursionTree(vSyntacticTree &tree, BytecodeOutput *bo, JumpMap jump_map, int parent_node_index, int current_node_index)
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
                        recursionTree(tree, bo, jump_map, current_node_index, current_node.children[2]);
                    }
                    // 引数がない場合
                    if (current_node.children.size() == 2)
                    {
                        recursionTree(tree, bo, jump_map, current_node_index, current_node.children[1]);
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
                        recursionTree(tree, bo, jump_map, current_node_index, current_node.children[1]);
                    }
                    if (current_node.children.size() == 1)
                    {
                    }
                    printf("Translator RecursionTree %15s : %5d | Return \n", "return", current_node_index);

                    return;
                }
                else if (current_node.token == "<function_message_passing>")
                {
                    recursionFunctionMessagePassing(tree, bo, jump_map, parent_node_index, current_node_index);
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
                        recursionTree(tree, bo, jump_map, current_node_index, current_node.children[2]);
                    }
                    if (current_node.children.size() == 4)
                    {
                        recursionTree(tree, bo, jump_map, current_node_index, current_node.children[3]);
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

                        recursionTree(tree, bo, jump_map, current_node_index, current_node.children[2]);
                        string type = getChildrenNode(tree, current_node_index, 1).token;
                        LocalVariable lv = bo->findLocalVariable(type, Opecode::resolvOpecrType(type, bo->token_class_type));
                        bo->putOpecode(Opecode::s_instance, lv.index);
                        printf("Translator RecursionTree %15s : %5d | Return \n", "new_class B", current_node_index);

                        return;
                    }
                    return;
                }

                else if (current_node.token == "<ifexpr>" || current_node.token == "<if_while>")
                {

                    printf("ifexpr/if_while AST : %s\n", current_node.token.c_str());

                    vint label_list = {};
                    recursionIfWhileAdvance(tree, bo, label_list, parent_node_index, current_node_index);
                    int current_label = bo->getOpecodeLabelUniqueId();
                    label_list.push_back(current_label);
                    printf("label_list - ");
                    for (int i = 0; i < label_list.size(); i++)
                    {
                        printf("%d:%d ", i, label_list[i]);
                    }
                    printf("\n");

                    int current_label_index = 0;
                    // std::reverse(label_list.begin(), label_list.end());
                    recursionIfWhile(tree, bo, {}, label_list, current_label_index, parent_node_index, current_node_index);
                    return;
                }
                else if (current_node.token == "<expression>")
                {
                    vector<MethodChainLiner> mcl = {};
                    recursionExpressionToLinear(tree, bo, mcl, parent_node_index, current_node_index);

                    int confirmed = true;

                    for (int i = 0; i < mcl.size(); i++)
                    {
                        MethodChainLiner m = mcl[i];
                        printf("mcl %d : %d %d\n", i, m.type, m.node_index);

                        if (m.type == 0 && i == 0)
                        {
                            LocalVariable lv = bo->findLocalVariable(getNode(tree, m.node_index).token);
                            bo->putOpecode(Opecode::s_load, lv.type, lv.index);
                        }
                        else if (m.type == 0)
                        {
                            vector<LocalVariable> lvs = bo->findLocalVariableAll(getNode(tree, m.node_index).token);

                            vector<int> op_list = {};

                            for (int j = 0; j < lvs.size(); j++)
                            {
                                LocalVariable lv = lvs[j];
                                op_list.push_back(lv.type);
                                op_list.push_back(lv.index);
                            }

                            bo->putOpecode(Opecode::s_inside, op_list);
                        }
                        else if (m.type == 1 && i == 0)
                        {
                            recursionFunctionMessagePassing(tree, bo, jump_map, parent_node_index, m.node_index);
                        }
                        else if (m.type == 1)
                        {
                            vector<LocalVariable> lvs = bo->findLocalVariableAll(getChildrenNode(tree, m.node_index, 0).token);

                            vector<int> op_list = {};

                            for (int j = 0; j < lvs.size(); j++)
                            {
                                LocalVariable lv = lvs[j];
                                op_list.push_back(lv.type);
                                op_list.push_back(lv.index);
                            }

                            recursionFunctionMessagePassing(tree, bo, jump_map, parent_node_index, m.node_index, op_list);
                        }
                    }
                    return;
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

                        recursionTree(tree, bo, jump_map, current_node_index, current_node.children[i]);
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
                recursionTree(tree, bo, jump_map, current_node_index, current_node.children[i]);
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
                if (current_node.parent_token == "<text>")
                {
                    bo->putOpecode(Opecode::push, Opecode::d_int, current_node.token);
                }
                if (current_node.parent_token == "<value_name>" && current_node.token == "true")
                {
                    bo->putOpecode(Opecode::push, Opecode::d_boolean, 1);
                }
                if (current_node.parent_token == "<value_name>" && current_node.token == "false")
                {
                    bo->putOpecode(Opecode::push, Opecode::d_boolean, 0);
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

                if (current_node.token == "break" && jump_map.find("break") != jump_map.end())
                {
                    bo->putOpecode(Opecode::s_jump, jump_map["break"]);
                }
                else if (current_node.token == "break")
                {
                    printf("error : break is not available at this location\n");
                }

                if (current_node.token == "continue" && jump_map.find("continue") != jump_map.end())
                {
                    bo->putOpecode(Opecode::s_jump, jump_map["continue"]);
                }
                else if (current_node.token == "continue")
                {
                    printf("error : continue is not available at this location\n");
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
            JumpMap jump_map = {};
            recursionTree(tree, bo, jump_map, -1, 0);
            bo->processedStackTop();
            delete bo;
            return;
        };
    };
};