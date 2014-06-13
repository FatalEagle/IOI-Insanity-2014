#include <iostream>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <vector>

#define MAXN 100

using namespace std;

struct edge
{
    int v, f, c;
    unsigned int o;
};

const int src=MAXN+1, snk=MAXN+2;
int N, M, A;
int P[MAXN+3];
int V[MAXN+3];
vector<int> voltage;
bool seen[MAXN+3];
vector<edge> adj[MAXN+3];
bool bad[MAXN+3];
int level[MAXN+3];
int Q[MAXN+3], head, tail;

void reset(int max_voltage)
{
    for(int i=1; i<=N; i++)
        for(size_t j=0; j<adj[i].size(); j++)
            adj[i][j].f=V[adj[i][j].v]<=max_voltage?adj[i][j].c:0;
    for(size_t i=0; i<adj[src].size(); i++)
        adj[src][i].f=V[adj[src][i].v]<=max_voltage?adj[src][i].c:0;
}

void add_edge(int u, int v, int f)
{
    adj[u].push_back((edge){v, f, f, adj[v].size()});
    adj[v].push_back((edge){u, 0, 0, adj[u].size()-1});
}

bool bfs()
{
    memset(bad, 0, sizeof bad);
    memset(level, 0x3f, sizeof level);
    level[src]=0;
    Q[head=0]=src;
    tail=1;
    while(head!=tail)
    {
        int u=Q[head++];
        for(size_t i=0; i<adj[u].size(); i++)
        {
            int v=adj[u][i].v;
            int f=adj[u][i].f;
            if(f>0 && level[v]==0x3f3f3f3f)
            {
                level[v]=level[u]+1;
                if(v==snk)
                    return true;
                Q[tail++]=v;
            }
        }
    }
    return false;
}

int dfs(int u, int c)
{
    if(u==snk)
        return c;
    int ret=0;
    for(size_t i=0; i<adj[u].size(); i++)
    {
        int v=adj[u][i].v;
        int& f=adj[u][i].f;
        if(f>0 && level[v]==level[u]+1 && !bad[v])
        {
            int o=adj[u][i].o;
            int s=dfs(v, min(c, f));
            f-=s;
            adj[v][o].f+=s;
            c-=s;
            ret+=s;
            if(c==0)
                break;
        }
    }
    bad[u]=ret==0;
    return ret;
}

int maxflow()
{
    int ret=0;
    while(bfs())
        ret+=dfs(src, 0x3f3f3f3f);
    return ret;
}

int main()
{
    scanf("%d%d%d", &N, &M, &A);
    for(int i=1; i<=N; i++)
    {
        scanf("%d%d", P+i, V+i);
        if(P[i]>0)
            add_edge(src, i, P[i]);
        else
            add_edge(i, snk, -P[i]);
    }
    int a, b;
    for(int i=0; i<M; i++)
    {
        scanf("%d%d", &a, &b);
        add_edge(a, b, 0x3f3f3f3f);
    }
    for(int i=1; i<=N; i++)
    {
        memset(seen, 0, sizeof seen);
        seen[i]=true;
        Q[head=0]=i;
        tail=1;
        while(head!=tail)
        {
            int u=Q[head++];
            for(size_t j=0; j<adj[u].size(); j++)
            {
                int v=adj[u][j].v;
                if(adj[u][j].c==0x3f3f3f3f && !seen[v])
                {
                    V[i]=max(V[i], V[v]);
                    seen[v]=true;
                    Q[tail++]=v;
                }
            }
        }
        voltage.push_back(V[i]);
    }
    sort(voltage.begin(), voltage.end());
    voltage.resize(unique(voltage.begin(), voltage.end())-voltage.begin());
    int ans=0;
    for(size_t i=0; i<voltage.size(); i++)
    {
        reset(voltage[i]);
        int suma=0;
        for(int j=1; j<=N; j++)
            if(V[j]<=voltage[i] && P[j]>0)
                suma+=P[j];
        ans=max(ans, suma-maxflow()-A*voltage[i]);
    }
    printf("%d\n", ans);
    return 0;
}
