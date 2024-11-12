

#include "./../lr_table/lr_table.hpp"
#include "./syntactic_analysis.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>

namespace Syntactic
{
    bool isTokenSkepSyntacticAnalysis(string);

    /// @brief シンプルにノード間関係を変更するメソッド
    /// @param tree グラフ(木)
    /// @param parent_node_index 親要素
    /// @param parent_node_child_index 親要素の子要素index 変更するノード
    /// @param move_node_index 移動するノード
    /// @return
    bool deleteChild(vSyntacticTree &, int);

    /// @brief 親要素の組み換え
    /// @param tree
    /// @param parent_node_index 親要素
    /// @param parent_node_child_index 親要素に指定されている子要素リストのindex
    /// @param move_node_index 新しい子要素
    /// @return
    bool moveChild(vSyntacticTree &, int, int, int);

    bool climbChild(vSyntacticTree &, int, int, int);

    /// @brief 自分自身を削除して、親要素と子要素をつなげる
    /// @param tree
    /// @param parent_node_index
    /// @param current_node_index
    /// @param leftright
    /// @return
    bool cutChild(vSyntacticTree &, int, int);

    /// @brief 子要素が複数ある時の、親要素の接続処理
    /// @param tree
    /// @param parent_node_index
    /// @param delete_child_node_index
    /// @param move_child_node_index
    /// @return
    bool cutSelfChildParent(vSyntacticTree &, int, int, int);

    /// @brief 子要素が複数ある時の、孫要素→子要素接続処理
    /// @param tree
    /// @param flow_parent 親候補
    /// @param flow 移動要素
    /// @param leftright 左結合/右結合(巻き上げ要求と逆)
    /// @return
    bool cutSelfChildFlow(vSyntacticTree &, int, int);
}
