// English Essay
// Official Solution ~ O(E log N)
// By Alex Li

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

const size_t MAX_SZ = 5000005;
const size_t MOD = 1000000007;
const size_t base = 1000000000; /* for bigint */
const size_t base_digits = 9; /* ceil(log10(base)) */

struct bigint {
  std::vector<int> a; /* a[0] is the rightmost base-digit */
  int sign;

  bigint() : sign(1) {}
  bigint(long long v) { *this = v; }
  //bigint(const std::string &s) { read(s); }
  //bigint(const char *s) { read(std::string(s)); }
  
  bigint(const bigint &v) {
    sign = v.sign;
    a = v.a;
  }

  void operator = (const bigint &v) {
    sign = v.sign;
    a = v.a;
  }

  void operator = (long long v) {
    sign = 1;
    if (v < 0) sign = -1, v = -v;
    for (; v > 0; v = v / base)
      a.push_back(v % base);
  }

  bigint operator + (const bigint &v) const {
    if (sign == v.sign) {
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
    //return *this - (-v);
  }

  int operator % (int v) const {
    if (v < 0) v = -v;
    int m = 0;
    for (int i = a.size() - 1; i >= 0; --i)
      m = (a[i] + m * (long long)base) % v;
    return m * sign;
  }

  bool operator < (const bigint &v) const {
    if (sign != v.sign) return sign < v.sign;
    if (a.size() != v.a.size())
      return a.size() * sign < v.a.size() * v.sign;
    for (int i = a.size() - 1; i >= 0; i--)
      if (a[i] != v.a[i])
        return a[i] * sign < v.a[i] * sign;
    return false;
  }

  friend std::ostream& operator << (std::ostream &out,
                                    const bigint &v) {
    if (v.sign == -1) out << '-';
    out << (v.a.empty() ? 0 : v.a.back());
    for (int i = v.a.size() - 2; i >= 0; --i)
      out << std::setw(base_digits) <<
             std::setfill('0') << v.a[i];
    return out;
  }
};

struct node_t;

struct edge_t {
  node_t *target;
  int litlen, counter;
  bigint tlen;

  edge_t(node_t &n) {
    target = &n;
    litlen = counter = 0;
  }
};

struct node_t {
  string name;
  bigint *best;
  bool seen;
  vector<edge_t*> dependents;

  node_t(const string &s = "") {
    name = s;
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

  qnode_t(node_t &n) {
    this->n = &n;
    this->v = n.best;
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
    if (*c == '\n' || *c == '\r') *c == ' ';
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
  char *tokptr = strtok(str, " \r\n");
  while (tokptr != NULL) {
    tokens.push_back(tokptr);
    tokptr = strtok(NULL, " \r\n");
  }

  //build graph
  vector<int> def_idx;
  vector<node_t> nodes;
  map<string, node_t> m;
  for (int i = 0; i < def_idx.size(); i++)
    if (tokens[i] == "::=") {
      def_idx.push_back(i);
      nodes.push_back(node_t(tokens[i - 1]));
      m[tokens[i - 1]] = nodes.back();
    }
  for (int i = 0, hi; i < def_idx.size(); i++) {
    if (i == def_idx.size() - 1) {
      hi = tokens.size();
    } else {
      hi = def_idx[i + 1] - 1;
    }
    edge_t *e = new edge_t(nodes[i]);
    for (int j = def_idx[i] + 1; j < hi; j++) {
      if (tokens[j] == "|") {
        if (e->counter == 0)
          nodes[i].push(bigint(e->litlen));
        e = new edge_t(nodes[i]);
      } else {
        map<string, node_t>::iterator it;
        it = m.find(tokens[j]);
        if (it != m.end()) {
          e->counter++;
          it->second.dependents.push_back(e);
        } else if (tokens[j] == "<EOL>") {
          e->litlen++;
        } else {
          e->litlen += tokens[j].length();
        }
      }
    }
    if (e->counter == 0)
      nodes[i].push(bigint(e->litlen));
  }

  //Dijkstra's algorithm
  priority_queue<qnode_t> pq;
  for (int i = 0; i < nodes.size(); i++)
    if (nodes[i].best != NULL)
      pq.push(qnode_t(nodes[i]));
  node_t *n;
  while (!pq.empty()) {
    qnode_t qn = pq.top();
    pq.pop();
    if ((n = qn.n)->seen) continue;
    n->seen = true;
    vector<edge_t*>::iterator it;
    it = n->dependents.begin();
    for (; it != n->dependents.end(); ++it) {
      edge_t *e = *it;
      e->counter--;
      e->tlen = e->tlen + *n->best;
      if (e->counter == 0) {
        node_t *o = e->target;
        if (o->push(e->tlen + bigint(e->litlen))) {
          pq.push(qnode_t(*(e->target)));
        }
      }
    }
  }

  bigint *x = m["<essay>"].best;
  if (x == NULL) {
    cout << -1 << endl;
  } else {
    cout << (*x % MOD) << endl;
  }
  return 0;
}
