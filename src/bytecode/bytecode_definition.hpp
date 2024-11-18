

#ifndef __BYTECODE_D
#define __BYTECODE_D
#include "./../syntactic/syntactic_analysis.hpp"

namespace Bytecode
{
    typedef unsigned char opcr;
    namespace Opecode
    {
        const opcr push = 00;
        const opcr push_int = 001;
        const opcr push_str = 002;
        const opcr push_float = 003;
        const opcr pop = 010;
        const opcr pop_int = 011;
        const opcr pop_str = 012;
        const opcr pop_float = 013;
        const opcr s_goto = 020;
        const opcr s_save = 030;
        const opcr s_save_int = 031;
        const opcr s_save_float = 032;

        const opcr c_add = 100;
        const opcr c_subtraction = 101;
        const opcr c_multiplication = 102;
        const opcr c_division = 103;

        const opcr s_if_acmpeq = 041; //== (オブジェクト参照)
        const opcr s_if_acmpne = 042; //!= (オブジェクト参照)
        const opcr s_if_icmpeq = 043; // ==
        const opcr s_if_icmpge = 044; // >=
        const opcr s_if_icmpgt = 045; // >
        const opcr s_if_icmple = 046; // <=
        const opcr s_if_icmplt = 047; // <

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