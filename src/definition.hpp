#ifndef __Definition
#define __Definition

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <algorithm>
#include <fstream>

typedef unsigned char uchar;
typedef std::vector<char> vchar;
typedef std::string string;
typedef std::vector<unsigned char> vuchar;
typedef std::vector<std::string> vstring;
typedef std::vector<std::vector<std::string>> v2string;

typedef std::vector<int> vint;
typedef std::vector<std::vector<int>> v2int;
typedef std::vector<std::vector<std::vector<int>>> v3int;
typedef std::vector<std::vector<std::vector<std::vector<int>>>> v4int;
typedef std::stack<int> sint;
typedef std::queue<int> quint;
typedef std::queue<std::string> qustr;
typedef std::vector<std::queue<int>> vquint;
typedef std::vector<std::map<string, int>> vmap_str_int;
typedef std::vector<std::map<string, vint>> vmap_str_vint;

typedef std::map<std::string, bool> mapstrbool;
// typedef std::map<std::string, int> mapstrint;
typedef std::map<std::string, string> mapstr;
typedef std::map<std::string, int> mp_s_i; // mp を生成
typedef std::map<int, int> mp_i_i;         // mp を生成
typedef std::map<int, string> mp_i_s;
typedef std::map<int, vint> mp_i_vint;
typedef std::map<std::string, vint> mp_s_vint;
typedef std::map<std::string, v2int> mp_s_v2int;
typedef std::map<std::string, vstring> mp_s_vstring;

using namespace std;

const int token_string_arr_size = 10000;      // tokenの最大数
const int token_progression_arr_size = 10000; // 変数の最大数
// const int token_size = 50;           // tokenの最大文字数
// const int variable_size = 50;        // variableの最大文字数

#define code_token_len 50 // BNFトークンの最大長
#define code_token_arr_len 500

// 最大ソースコード容量 1MBまで対応

const int source_code_size = 1000000;

template <typename T>
inline bool hasKeyMap(vector<T> keys, T key)
{
    bool flag = false;
    for (size_t i = 0; i < keys.size(); i++)
    {
        if (keys[i] == key)
        {
            // printf("hasKeyMap %s %s\n", keys[i].c_str(), key.c_str());
            return true;
        }
        /* code */
    }
    return false;
}

template <typename T, typename Y>
inline bool hasMapKey(std::map<Y, T> mp, Y key)
{
    return mp.find(key) != mp.end();
}

template <typename T, typename Y>
inline std::vector<Y> getMapKeyString(std::map<Y, T> mp)
{
    std::vector<Y> keys;
    auto begin = mp.begin(), end = mp.end();
    for (auto iter = begin; iter != end; iter++)
    {
        // first: key, second: value
        // cout << "key = " << iter->first << "\n";
        keys.push_back(iter->first);
    }

    return keys;
}

/* プログラム・ファイルを読み込むための関数 text / filename / size(読み込むサイズ)*/
inline void loadText(char *text, const char *file_name, int size)
{
    FILE *fp = fopen(file_name, "rb");
    int read_size = fread(text, 1, size - 1, fp);
    printf("read_size : %s %d\n", file_name, read_size);

    fclose(fp);
    text[read_size] = '\0'; // 終端マーク 別に配列終端でなくてよい
}

// アルファベットであるか探索
inline int isNumber(char ch)
{

    if ('0' <= ch && ch <= '9')
    {
        return 1;
    }
    return 0;
}

// アルファベットであるか探索
inline int isAlphabetOrNumber(char ch)
{
    if ('A' <= ch && ch <= 'Z')
    {
        return 1;
    }
    if ('a' <= ch && ch <= 'z')
    {
        return 1;
    }
    if ('0' <= ch && ch <= '9')
    {
        return 1;
    }
    return 0;
}

inline bool isAssignExpr(string ts)
{

    if (ts.length() > 1)
    {
        return false;
    }

    const char *token = ts.c_str();
    return (strchr("=", token[0]) != 0);
}

inline bool isExpr(string ts)
{

    if (ts == "<=" || ts == ">=" || ts == "==" || ts == "!=")
    {
        return true;
    }

    if (ts.length() > 1)
    {
        return false;
    }

    const char *token = ts.c_str();
    return (strchr("+-*/%=<>:\'\"", token[0]) != 0);
}

#endif