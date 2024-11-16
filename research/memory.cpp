
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

struct A
{
    int c = 1000;
    std::string s = "aiueo";
};

int main()
{
    // int m = 1410065408;
    int m = 200000;
    std::vector<A> a = {};

    for (int i = 0; i < m; i++)
    {
        A p;
        a.push_back(p);
    }

    std::cout << a.size() << std::endl;
}