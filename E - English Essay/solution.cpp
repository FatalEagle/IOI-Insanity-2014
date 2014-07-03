// English Essay
// Official Solution ~ O(E log N)
// By Alex Li

#if __cplusplus > 199711L
  #define map unordered_map
#endif

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>
using namespace std;

const size_t MAX_N = 10005;
const size_t MAX_SZ = 5500005;
const size_t MOD = 1000000007;

struct bigint { /* simple unsigned version */
  static const size_t base = 1000000000;
  static const size_t base_digits = 9; /* ceil(log10(base)) */

  std::vector<int> a; /* a[0] is the rightmost base-digit */

  bigint(unsigned long long v = 0) { *this = v; }

  void operator = (const bigint &v) { a = v.a; }

  void operator = (unsigned long long v) {
    for (; v > 0; v = v / base)
      a.push_back(v % base);
  }

  bigint operator + (const bigint &v) const {
    bigint res = v;
    for (int i = 0, carry = 0; i < 
     std::max(a.size(), v.a.size()) || carry; ++i) {
      if (i == res.a.size()) res.a.push_back(0);
      res.a[i] += carry + (i < a.size() ? a[i] : 0);
      carry = res.a[i] >= base;
      if (carry) res.a[i] -= base;
    }
    return res;
  }

  unsigned int operator % (unsigned int v) const {
    int m = 0;
    for (int i = a.size() - 1; i >= 0; --i)
      m = (a[i] + m * (long long)base) % v;
    return m;
  }

  bool operator < (const bigint &v) const {
    if (a.size() != v.a.size())
      return a.size() < v.a.size();
    for (int i = a.size() - 1; i >= 0; i--)
      if (a[i] != v.a[i]) return a[i] < v.a[i];
    return false;
  }

  friend std::ostream& operator << (std::ostream &out,
                                    const bigint &v) {
    out << (v.a.empty() ? 0 : v.a.back());
    for (int i = v.a.size() - 2; i >= 0; --i)
      out << std::setw(base_digits) <<
             std::setfill('0') << v.a[i];
    return out;
  }
};

struct node_t;

struct edge_t {
  node_t *n;
  int len1, counter;
  bigint len2;

  edge_t(node_t *n) {
    this->n = n;
    len1 = counter = 0;
  }
};

struct node_t {
  bigint *best;
  bool seen;
  vector<edge_t*> depend;

  node_t() {
    best = NULL;
    seen = false;
  }

  bool push(const bigint &x) {
    if (best == NULL || x < *best) {
      best = new bigint(x);
      return true;
    }
    return false;
  }
};

struct qnode_t {
  node_t *n;
  bigint *v;

  qnode_t(node_t *n) {
    this->n = n;
    this->v = n->best;
  }

  //compare descending for priority queue
  bool operator < (const qnode_t &rhs) const {
    return *(rhs.v) < *(this->v);
  }
};

int main() {
  //read the entire file...
  char file[MAX_SZ]; size_t fileSize;
  fseek(stdin, 0L, SEEK_END);
  fileSize = ftell(stdin); rewind(stdin);
  fread(file, 1, fileSize, stdin);
  file[fileSize] = '\0';

  //remove expressions in quantifier brackets
  char str[MAX_SZ]; size_t s_len = 0;
  bool opened = false; int depth = 0;
  for (char *c = file; *c != '\0'; c++) {
    if (*c == '\n' || *c == '\r') *c = ' ';
    if (*c == '[' || *c == '{') depth++;
    else if (*c == ']' || *c == '}') depth--;
    else if (depth == 0) {
      if (*c == '"') {
        opened = !opened;
      } else {
        if (*c == ' ' && opened)
          str[s_len++] = 'x';
        else
          str[s_len++] = *c;
      }
    }
  }
  str[s_len] = '\0';

  //tokenize definitions
  vector<string> tokens;
  char *tokptr = strtok(str, " ");
  while (tokptr != NULL) {
    tokens.push_back(tokptr);
    tokptr = strtok(NULL, " ");
  }

  //build graph
  node_t nodes[MAX_N]; size_t nnodes = 0;
  vector<int> def_idx;
  map<string, node_t*> m;
  for (int i = 0; i < tokens.size(); i++)
    if (tokens[i] == "::=") {
      def_idx.push_back(i);
      nodes[nnodes] = node_t();
      m[tokens[i - 1]] = &nodes[nnodes++];
    }
  for (int i = 0, hi; i < def_idx.size(); i++) {
    if (i == def_idx.size() - 1) {
      hi = tokens.size();
    } else {
      hi = def_idx[i + 1] - 1;
    }
    edge_t *e = new edge_t(nodes + i);
    for (int j = def_idx[i] + 1; j < hi; j++) {
      if (tokens[j] == "|") {
        if (e->counter == 0)
          nodes[i].push(bigint(e->len1));
        e = new edge_t(nodes + i);
      } else {
        map<string, node_t*>::iterator it;
        it = m.find(tokens[j]);
        if (it != m.end()) {
          e->counter++;
          it->second->depend.push_back(e);
        } else if (tokens[j] == "<EOL>") {
          e->len1++;
        } else {
          e->len1 += tokens[j].length();
        }
      }
    }
    if (e->counter == 0)
      nodes[i].push(bigint(e->len1));
  }

  //Dijkstra's algorithm
  priority_queue<qnode_t> pq;
  for (int i = 0; i < nnodes; i++)
    if (nodes[i].best != NULL)
      pq.push(qnode_t(nodes + i));
  node_t *n; edge_t *e;
  while (!pq.empty()) {
    qnode_t qn = pq.top();
    pq.pop();
    if ((n = qn.n)->seen) continue;
    n->seen = true;
    vector<edge_t*>::iterator it;
    it = n->depend.begin();
    for (; it != n->depend.end(); ++it) {
      e = *it;
      e->counter--;
      e->len2 = e->len2 + *n->best;
      if (e->counter == 0) {
        if (e->n->push(e->len2 + bigint(e->len1)))
          pq.push(qnode_t(e->n));
      }
    }
  }

  //output answer
  bigint *x = m["<essay>"]->best;
  if (x == NULL) {
    cout << -1 << endl;
  } else {
    cout << (*x % MOD) << endl;
  }
  return 0;
}
