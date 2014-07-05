// Campfire Cooking
// Official Solution
// By Timothy Li

#include <algorithm>
#include <cstdio>
using namespace std;

const int MAX_N = 200000;
const int MAX_M = 200000;
const int LOG_N = 18;
const int MAX_CHANGES = 25000;
const int MAX_SZ = MAX_N + 1;
const int LOG_SZ = LOG_N + 1;

enum dir { L, R };

struct node {
  int sum;
  node *link[2];
} pool[MAX_SZ*2 + 2*(MAX_SZ*LOG_N*2 +
       MAX_CHANGES*LOG_N*LOG_N*2*2)],
  *nextpool = pool, *empty_tree;

int bit[2][MAX_SZ + 1];

void bit_add(int t, int x, int v) {
  for (; x <= MAX_SZ; x += x & -x) bit[t][x] += v;
}

int bit_sum(int t, int x) {
  int sum = 0;
  for (; x > 0; x -= x & -x) sum += bit[t][x];
  return sum;
}

int bit_query(int t, int l, int r) {
  return bit_sum(t, r) - bit_sum(t, l - 1);
}

inline node* alloc(node *old) {
  *nextpool = *old;
  return nextpool++;
}

node* build(int begin, int end) {
  node *root = nextpool++;
  root->sum = 0;
  if (begin != end) {
    int mid = begin + (end - begin)/2;
    root->link[L] = build(begin, mid);
    root->link[R] = build(mid+1, end);
  }
  return root;
}

node* update_node(node *root, int begin, int end, int x, int v) {
  if (x < begin || end < x) return root;
  root = alloc(root);
  if (begin == end) {
    root->sum += v;
  } else {
    int mid = begin + (end - begin)/2;
    root->link[L] = update_node(root->link[L], begin, mid, x, v);
    root->link[R] = update_node(root->link[R], mid+1, end, x, v);
    root->sum = root->link[L]->sum + root->link[R]->sum;
  }
  return root;
}

struct chairman_tree {
  
  node *orig[MAX_SZ], *tree[MAX_SZ];
  node *active[LOG_SZ][2][LOG_SZ];
  int level, nactive[2];

  void init() {
    orig[0] = empty_tree;
    for (int i = 0; i <= MAX_SZ; i++)
      tree[i] = empty_tree;
  }

  void init_update(int x, int pos, int v) {
    orig[x] = update_node(orig[x-1], 1, MAX_SZ, pos, v);
  }

  void update(int x, int pos, int v) {
    for (; x <= MAX_SZ; x += x & -x)
      tree[x] = update_node(tree[x], 1, MAX_SZ, pos, v);
  }

  void query_init(int t, int x) {
    active[level][t][0] = orig[x];
    nactive[t] = 1;
    for (; x > 0; x -= x & -x)
      active[level][t][nactive[t]++] = tree[x];
  }

  int get_sum(int t) {
    int sum = 0;
    for (int i = 0; i < nactive[t]; i++)
      sum += active[level][t][i]->sum;
    return sum;
  }

  int get_lsum(int t) {
    int sum = 0;
    for (int i = 0; i < nactive[t]; i++)
      sum += active[level][t][i]->link[L]->sum;
    return sum;
  }

  void change_direction(int d) {
    level++;
    for (int t = 0; t <= 1; t++)
      for (int i = 0; i < nactive[t]; i++)
        active[level][t][i] = active[level-1][t][i]->link[d];
  }
  
  int _query(int begin, int end, int x) {
    if (end <= x) return get_sum(1) - get_sum(0);
    int ret = 0, mid = begin + (end - begin)/2;
    change_direction(L);
    ret += _query(begin, mid, x);
    level--;
    if (mid + 1 <= x) {
      change_direction(R);
      ret += _query(mid + 1, end, x);
      level--;
    }
    return ret;
  }

  int query(int l, int h, int x) {
    if (x == 0) return 0;
    level = 0;
    query_init(0, l - 1);
    query_init(1, h);
    return _query(1, MAX_SZ, x);
  }

  template<bool complement>
  int _kth(int begin, int end, int pos) {
    if (begin == end) return begin;
    int lsum = get_lsum(1) - get_lsum(0);
    int mid = begin + (end - begin)/2;
    if (complement)
      lsum = bit_query(0, begin, mid) - lsum;
    if (pos <= lsum) {
      change_direction(L);
      return _kth<complement>(begin, mid, pos);
    }
    change_direction(R);
    return _kth<complement>(mid+1, end, pos-lsum);
  }

  template<bool complement>
  int kth(int l, int h, int k) {
    level = 0;
    query_init(0, l - 1);
    query_init(1, h);
    return _kth<complement>(1, MAX_SZ, k);
  }

} cmt_lower, cmt_upper;


int N, M, T, X, Y, Z, C[MAX_SZ][2];

inline void change_temp(int F) {
  T = (T*X + F*Y) % Z + 1;
}

int GetFood(int i, int j, int l, int h) {
  int lower = max(1, T - l);
  int upper = min(MAX_SZ, T + h);
  int ret;
  if (i <= j) {
    ret = cmt_lower.query(i, j, upper) - 
          cmt_upper.query(i, j, lower - 1);
  } else {
    ret = bit_sum(0, upper) - bit_sum(1, lower - 1);
    if (j + 1 <= i - 1) {
      ret -= cmt_lower.query(j + 1, i - 1, upper) -
             cmt_upper.query(j + 1, i - 1, lower - 1);
    }
  }
  change_temp(ret);
  return ret;
}

void NewFood(int i, int l, int h) {
  bit_add(0, C[i][0], -1);
  bit_add(1, C[i][1], -1);
  cmt_lower.update(i, C[i][0], -1);
  cmt_upper.update(i, C[i][1], -1);
  C[i][0] = l;
  C[i][1] = h;
  bit_add(0, C[i][0], 1);
  bit_add(1, C[i][1], 1);
  cmt_lower.update(i, C[i][0], 1);
  cmt_upper.update(i, C[i][1], 1);
}

int KthFood(int i, int j, int k) {
  int ret;
  if (i <= j) {
    ret = cmt_lower.kth<false>(i, j, k);
  } else {
    ret = cmt_lower.kth<true>(j + 1, i - 1, k);
  }
  if (ret == MAX_SZ) ret = -1;
  return ret;
}

int main() {
  empty_tree = build(1, MAX_SZ);
  cmt_lower.init();
  cmt_upper.init();
  scanf("%d%d%d%d%d%d", &N, &M, &T, &X, &Y, &Z);
  for (int i = 1; i <= N; i++) {
    scanf("%d%d", C[i], C[i] + 1);
    bit_add(0, C[i][0], 1);
    bit_add(1, C[i][1], 1);
    cmt_lower.init_update(i, C[i][0], 1);
    cmt_upper.init_update(i, C[i][1], 1);
  }
  int a, b, c, d, e;
  for (int i = 0; i < M; i++) {
    scanf("%d", &a);
    if (a == 1) {
      scanf("%d%d%d%d", &b, &c, &d, &e);
      printf("%d\n", GetFood(b, c, d, e));
    } else if (a == 2) {
      scanf("%d%d%d", &b, &c, &d);
      NewFood(b, c, d);
    } else {
      scanf("%d%d%d", &b, &c, &d);
      printf("%d\n", KthFood(b, c, d));
    }
  }
  return 0;
}
