// Power Grid
// Official Solution - O(???)
// By Timothy Li

#include <algorithm> 
#include <cstdio> 
#include <cstring>
#include <vector> 
using namespace std;

const int MAXN = 100;
const int src = MAXN + 1;
const int snk = MAXN + 2;
const int INF = 0x3f3f3f3f;

struct edge {
  int v, f, c;
  unsigned int o;
};

int N, M, A;
int P[MAXN + 3], V[MAXN + 3];
vector<int> vltg; //voltage
vector<edge> adj[MAXN + 3];
bool bad[MAXN + 3];
bool seen[MAXN + 3];
int level[MAXN + 3];
int Q[MAXN + 3], head, tail;

void reset(int max_voltage) {
  for (int i = 1; i <= N; i++)
    for (int j = 0; j < adj[i].size(); j++)
      if (V[adj[i][j].v] <= max_voltage)
        adj[i][j].f = adj[i][j].c;
      else
        adj[i][j].f = 0;
  for (int i = 0; i < adj[src].size(); i++)
    if (V[adj[src][i].v] <= max_voltage)
      adj[src][i].f = adj[src][i].c;
    else
      adj[src][i].f = 0;
}

void add_edge(int u, int v, int f) {
  adj[u].push_back((edge){v, f, f, adj[v].size()});
  adj[v].push_back((edge){u, 0, 0, adj[u].size() - 1});
}

bool BFS() {
  memset(bad, 0, sizeof bad);
  memset(level, 0x3f, sizeof level);
  level[src] = 0;
  Q[head = 0] = src;
  tail = 1;
  while (head != tail) {
    int u = Q[head++];
    for (int i = 0; i < adj[u].size(); i++) {
      int v = adj[u][i].v;
      if (adj[u][i].f > 0 && level[v] == INF) {
        level[v] = level[u] + 1;
        if (v == snk) return true;
        Q[tail++] = v;
      }
    }
  }
  return false;
}

int DFS(int u, int c) {
  if (u == snk) return c;
  int ret = 0;
  for (int i = 0; i < adj[u].size(); i++) {
    int v = adj[u][i].v, &f = adj[u][i].f;
    if (f > 0 && level[v] == level[u] + 1 && !bad[v]) {
      int o = adj[u][i].o, s = DFS(v, min(c, f));
      f -= s;
      adj[v][o].f += s;
      c -= s;
      ret += s;
      if (c == 0) break;
    }
  }
  bad[u] = (ret == 0);
  return ret;
}

int max_flow() {
  int ret = 0;
  while (BFS()) ret += DFS(src, INF);
  return ret;
}

int main() {
  scanf("%d%d%d", &N, &M, &A);
  for (int i = 1; i <= N; i++) {
    scanf("%d%d", P + i, V + i);
    if (P[i] > 0)
      add_edge(src, i, P[i]);
    else
      add_edge(i, snk, -P[i]);
  }
  int a, b;
  for (int i = 0; i < M; i++) {
    scanf("%d%d", &a, &b);
    add_edge(a, b, INF);
  }
  for (int i = 1; i <= N; i++) {
    memset(seen, 0, sizeof seen);
    seen[i] = true;
    Q[head = 0] = i;
    tail = 1;
    while (head != tail) {
      int u = Q[head++];
      for (int j = 0; j < adj[u].size(); j++) {
        int v = adj[u][j].v;
        if (adj[u][j].c == INF && !seen[v]) {
          V[i] = max(V[i], V[v]);
          seen[v] = true;
          Q[tail++] = v;
        }
      }
    }
    vltg.push_back(V[i]);
  }
  sort(vltg.begin(), vltg.end());
  vltg.resize(unique(vltg.begin(), vltg.end()) - vltg.begin());
  int ans = 0;
  for (int i = 0; i < vltg.size(); i++) {
    reset(vltg[i]);
    int suma = 0;
    for (int j = 1; j <= N; j++)
      if (V[j] <= vltg[i] && P[j] > 0)
        suma += P[j];
    ans = max(ans, suma - max_flow() - A*vltg[i]);
  }
  printf("%d\n", ans);
  return 0;
}
