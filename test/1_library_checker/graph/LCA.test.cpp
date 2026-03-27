#define PROBLEM "https://judge.yosupo.jp/problem/lca"
#include "graph/LCA.hpp"
#include <bits/stdc++.h>
using namespace std;
int N,Q;
void solve()
{
        cin>>N>>Q;
        LCA graph(N);
        for(int i=1;i<N;i++)
        {
                int p;cin>>p;
                graph.add_edge(p,i);
        }
        graph.build();
        while(Q--)
        {
                int u,v;cin>>u>>v;
                cout<<graph.lca(u,v)<<'\n';
        }
}
int main()
{
        ios::sync_with_stdio(false);cin.tie(NULL);
        int tc=1;
        // cin>>tc;
        while(tc--)solve();
        return 0;
}
