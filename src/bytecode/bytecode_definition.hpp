

#ifndef __BYTECODE_D
#define __BYTECODE_D
#include "./../syntactic/syntactic_analysis.hpp"
#include <map>

namespace Bytecode
{
    typedef unsigned int opcr;
    namespace Opecode
    {
        const opcr d_int = 1;
        const opcr d_str = 2;
        const opcr d_float = 3;
        const opcr d_pointer = 4;   // ポインタ
        const opcr d_reference = 5; // 参照 インスタンス化したものを含む
        const opcr d_html_dom = 6;
        const opcr d_function = 7; // 関数
        const opcr d_class = 8;    // クラスやコンポーネントなど、インスタンス化できるもの。

        inline const std::map<std::string, opcr> type_map = {
            {"int", d_int},
            {"str", d_str},
            {"float", d_float},
            {"pointer", d_pointer},
            {"reference", d_reference},
            {"html_dom", d_html_dom},
            {"function", d_function},
            {"class", d_class}};

        inline opcr resolvOpecrType(std::string type)
        {
            if (type_map.find(type) == type_map.end())
            {
                return 0;
            }

            return type_map.at(type);
        }

        // stack関連
        const opcr push = 0;
        const opcr push_int = push + d_int;
        const opcr push_str = push + d_str;
        const opcr push_float = push + d_float;
        const opcr push_pointer = push + d_pointer;
        const opcr push_reference = push + d_reference;
        const opcr push_html_dom = push + d_html_dom;
        const opcr push_function = push + d_function;
        const opcr push_class = push + d_class;

        const opcr pop = 10;
        const opcr pop_int = pop + d_int;
        const opcr pop_str = pop + d_str;
        const opcr pop_float = pop + d_float;
        const opcr pop_pointer = pop + d_pointer;
        const opcr pop_reference = pop + d_reference;
        const opcr pop_html_dom = pop + d_html_dom;
        const opcr pop_function = pop + d_function;
        const opcr pop_class = pop + d_class;

        const opcr s_invokevirtual = 20;             // 関数呼び出し
        const opcr s_invokevirtual_constructor = 21; // コンストラクタ呼び出し
        const opcr s_instance = 22;                  // インスタンス化したものをスタックに積む

        // ローカル変数関連

        // オペランドスタック → ローカル変数
        // 第3引数はローカル変数のインデックス (格納先)
        const opcr s_store = 30;
        const opcr s_store_int = s_store + d_int;
        const opcr s_store_float = s_store + d_float;
        const opcr s_store_str = s_store + d_str;
        const opcr s_store_pointer = s_store + d_pointer;
        const opcr s_store_reference = s_store + d_reference;
        const opcr s_store_html_dom = s_store + d_html_dom;
        const opcr s_store_function = s_store + d_function;
        const opcr s_store_class = s_store + d_class;

        // ローカル変数 → オペランドスタック
        // 第3引数はローカル変数のインデックス
        const opcr s_load = 40;
        const opcr s_load_int = s_load + d_int;
        const opcr s_load_float = s_load + d_float;
        const opcr s_load_str = s_load + d_str;
        const opcr s_load_pointer = s_load + d_pointer;
        const opcr s_load_reference = s_load + d_reference;
        const opcr s_load_html_dom = s_load + d_html_dom;
        const opcr s_load_function = s_load + d_function;
        const opcr s_load_class = s_load + d_class;

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