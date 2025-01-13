

#ifndef __BYTECODE_D
#define __BYTECODE_D
#include "./../syntactic/syntactic_analysis.hpp"
#include <map>

namespace Bytecode
{
    typedef unsigned int opcr;
    namespace Opecode
    {
        const opcr d_int = 100;
        const opcr d_str = 200;
        const opcr d_float = 300;
        const opcr d_pointer = 400;   // ポインタ
        const opcr d_reference = 500; // 参照 インスタンス化したものを含む
        const opcr d_html_dom = 600;
        const opcr d_function = 700; // 関数
        const opcr d_class = 800;    // クラスやコンポーネントなど、インスタンス化できるもの。

        inline const std::map<std::string, opcr> type_map = {
            {"int", d_int},
            {"str", d_str},
            {"float", d_float},
            {"pointer", d_pointer},
            {"reference", d_reference},
            {"html_dom", d_html_dom},
            {"function", d_function},
            {"class", d_class}};

        inline opcr resolvOpecrType(std::string type, vstring token_class_type)
        {
            std::map<std::string, opcr> cp_type_map = type_map;

            printf("resolvOpecrType %s |", type.c_str());

            for (int i = 0; i < token_class_type.size(); i++)
            {
                cp_type_map[token_class_type[i]] = i + d_class + 1;
                cp_type_map["r_" + token_class_type[i]] = i + d_reference + 1;
                cp_type_map["p_" + token_class_type[i]] = i + d_pointer + 1;
            }

            for (auto itr = cp_type_map.begin(); itr != cp_type_map.end(); ++itr)
            {
                printf(" %s:%d |", itr->first.c_str(), itr->second);
            }

            printf("\n");

            if (cp_type_map.find(type) == cp_type_map.end())
            {
                return 0;
            }

            return cp_type_map.at(type);
        }

        inline opcr resolvOpecrType(std::string type)
        {
            if (type_map.find(type) == type_map.end())
            {
                return 0;
            }

            return type_map.at(type);
        }

        // stack関連
        const opcr push = 10;
        const opcr pop = 11;

        const opcr s_invokevirtual = 20;             // 関数呼び出し
        const opcr s_invokevirtual_constructor = 21; // コンストラクタ呼び出し
        const opcr s_instance = 22;                  // インスタンス化したものをスタックに積む

        // ローカル変数関連

        // オペランドスタック → ローカル変数
        // 第3引数はローカル変数のインデックス (格納先)
        const opcr s_store = 30;

        // ローカル変数 → オペランドスタック
        // 第3引数はローカル変数のインデックス
        const opcr s_load = 40;

        const opcr c_add = 100;
        const opcr c_subtraction = 101;
        const opcr c_multiplication = 102;
        const opcr c_division = 103;

        const opcr s_if_acmpeq = 111; //== (オブジェクト参照)
        const opcr s_if_acmpne = 112; //!= (オブジェクト参照)
        const opcr s_if_icmpeq = 113; // ==
        const opcr s_if_icmpge = 114; // >=
        const opcr s_if_icmpgt = 115; // >
        const opcr s_if_icmple = 116; // <=
        const opcr s_if_icmplt = 117; // <

        /*
        1. **if_acmpeq**: スタック上の2つのオブジェクト参照を比較し、等しい場合に分岐します。
        2. **if_acmpne**: スタック上の2つのオブジェクト参照を比較し、等しくない場合に分岐します。
        3. **if_icmpeq**: スタック上の2つの整数を比較し、等しい場合に分岐します。
        4. **if_icmpge**: スタック上の2つの整数を比較し、最初の値が2番目の値以上の場合に分岐します。
        5. **if_icmpgt**: スタック上の2つの整数を比較し、最初の値が2番目の値より大きい場合に分岐します。
        6. **if_icmple**: スタック上の2つの整数を比較し、最初の値が2番目の値以下の場合に分岐します。
        7. **if_icmplt**: スタック上の2つの整数を比較し、最初の値が2番目の値より小さい場合に分岐します。
        8. **if_icmpne**: スタック上の2つの整数を比較し、等しくない場合に分岐します。
        9. **ifeq**: スタック上の整数が0の場合に分岐します。
        10. **ifge**: スタック上の整数が0以上の場合に分岐します。
        11. **ifgt**: スタック上の整数が0より大きい場合に分岐します。
        12. **ifle**: スタック上の整数が0以下の場合に分岐します。
        13. **iflt**: スタック上の整数が0より小さい場合に分岐します。
        14. **ifne**: スタック上の整数が0でない場合に分岐します。
        15. **ifnonnull**: スタック上のオブジェクト参照がnullでない場合に分岐します。
        16. **ifnull**: スタック上のオブジェクト参照がnullの場合に分岐します。
        17. **iinc**: ローカル変数に対して定数値を加算します。この命令は、ループカウンタのインクリメントなどに使用されます。
    */
    }
}

#endif