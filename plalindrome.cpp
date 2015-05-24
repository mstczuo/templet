#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
#define MAXN 5005000
#define palin(x, id, c) (str[id - 1 - l[x]] - 'a' == c)

char str[MAXN], ans[MAXN];
struct Trie
{
    int sz, odd_, even, last;
    int next[MAXN][2], rt[MAXN], l[MAXN];

    void Init()
    {
        sz = 0;
        next[sz][0] = next[sz][1] = 0;
        l[sz]= -2, rt[sz] = 0;
        odd_ = ++sz, l[odd_] = -1, rt[odd_] = odd_;
        even = ++sz, l[even] =  0, rt[even] = odd_;
        last = odd_;
    }

    void Add(int c, int id)
    {
        while(!palin(last, id, c)) last = rt[last];

        if (next[last][c]) last = next[last][c];
        else
        {
            int x = last;
            ++sz;
            next[x][c] = sz, l[sz] = l[x] + 2;
            if (x == odd_) rt[sz] = even;
            else
            {
                x = rt[x];
                while(x != odd_ && (!next[x][c] || !palin(x, id, c))) x = rt[x];
                if (next[x][c] && palin(x, id, c)) rt[sz] = next[x][c];
                else rt[sz] = odd_;
            }
            last = sz;
        }
    }
}pt;

int main()
{
    gets(str+1);
    pt.Init();
    int len = strlen(str + 1);
    for (int i = 1; i <= len; i++)
    {
        int tmp = pt.sz;
        pt.Add(str[i] - 'a', i);
        if (tmp != pt.sz) ans[i] = '1';
        else ans[i] = '0';
    }
    ans[len + 1] = 0;
    puts(ans + 1);
    return 0;
}
