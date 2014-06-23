// English Essay
// Official Solution - O(N)
// By Alex Li

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <vector>
using namespace std;

const int MOD = 1000000007;

struct item_t {
  bool is_literal;
  string value;

  item_t(const string &s) {
    if (s[0] == '<') {//symbol
      value = s.substr(1, s.size() - 2);
      is_literal = false;
    } else {
      value = s;
      is_literal = true;
    }
  }
};

struct node_t {
  node_t *lchild, *rchild;  
  vector<item_t> items; //only for leaf nodes
  bool is_root;

  node_t(node_t *l = 0, node_t *r = 0):
    lchild(l), rchild(r), is_root(0) {}
};

vector<string> toks;

void parse_input() {
  //read input and tokenize lines
  string s;
  vector<string> tmp;
  while (getline(cin, s)) {
    bool opened = false; //quote opened
    string curr;
    for (int i = 0; i < s.size(); i++) {
      if (opened) {
        if (s[i] == '"') {
          tmp.push_back(curr);
          opened = false;
          curr = "";
        } else {
          curr += s[i];
        }
      } else {
        if (s[i] == ' ') continue;
        if (s[i] == '"') {
          opened = true;
          continue;
        }
        int idx = s.find(' ', i + 1);
        if (idx == string::npos) {
          tmp.push_back(s.substr(i));
          break;
        }
        tmp.push_back(s.substr(i, idx - i));
        i = idx;
      }
    }
  }
  //find all items in quantifier bracket [] {}
  //replace them with an empty string ""
  for (int i = 0; i < tmp.size(); i++) {
    if (tmp[i] == "[" || tmp[i] == "{") {
      int open = 0;
      for (int j = i; j < tmp.size(); j++) {
        if (tmp[j] == "[" || tmp[j] == "{") open++;
        if (tmp[j] == "]" || tmp[j] == "}") open--;
        tmp[j] = "";
        if (open == 0) { i = j; break; }
      }
    }
  }
  //finally, merge all adjacent literals
  //place the results into the toks vector
  for (int i = 0, j; i < tmp.size(); i++) {
    if (!tmp.empty() && (tmp[i][0] == '<' ||
        tmp[i] == "::=" || tmp[i] == "|")) {
      toks.push_back(tmp[i]);
    } else {
      string curr;
      for (j = i; j < tmp.size(); j++) {
        if (!tmp.empty() && (tmp[j][0] == '<' ||
            tmp[j] == "::=" || tmp[j] == "|"))
          break;
        curr += tmp[j];
      }
      toks.push_back(curr);
      i = j - 1;
    }
  }
}

map<string, node_t*> defs;

//last index of an OR sign in [lo, hi)
int find_OR(int lo, int hi) {
  while (--hi >= lo && hi >= 0)
    if (toks[hi] == "|") return hi;
  return -1;
}

//build expression tree using [lo, hi) of the tokens list
node_t* build_tree(int lo, int hi) {
  if (lo >= hi) return NULL;
  int idx = find_OR(lo, hi);
  if (idx == -1) { //base case - leaf node
    node_t *n = new node_t();
    for (int i = lo; i < hi; i++)
      n->items.push_back(item_t(toks[i]));
    return n;
  }
  return new node_t(build_tree(lo, idx),
                    build_tree(idx + 1, hi));
}

void build_definitions() {
  for (int i = 1, hi; i < toks.size(); i++) {
    if (toks[i] == "::=") {
      vector<string>::iterator it;
      it = find(toks.begin() + i + 1, toks.end(), "::=");
      if (it == toks.end()) hi = toks.size();
      else hi = it - toks.begin() - 1;
      node_t *n = build_tree(i + 1, hi);
      n->is_root = true;
      defs[toks[i-1].substr(1, toks[i-1].size() - 2)] = n;
      i = hi;
    }
  }
}

map<node_t*, int> DP;
map<node_t*, int>::iterator it;
set<node_t*> S;

int best(node_t *n) {
  if (n == NULL) return -1;
  if (n->is_root) {
    if ((it = DP.find(n)) != DP.end()) return it->second;
    if (S.count(n)) return -1;
  }
  int ret;
  if (n->lchild != NULL) { //if n is not a leaf
    if (n->is_root) S.insert(n);
    int L = best(n->lchild), R = best(n->rchild);
    if (n->is_root) S.erase(n);
    ret = (L == -1 || R == -1) ? max(L, R) : min(L, R);
  } else {
    ret = 0;
    vector<item_t>::iterator it2 = n->items.begin();
    for (; it2 != n->items.end(); ++it2) {
      if (it2->is_literal) {
        ret += it2->value.length();
      } else {
        if (it2->value == "EOL") {
          ret++;
          continue;
        }
        if (n->is_root) S.insert(n);
        int val = best(defs[it2->value]);
        if (n->is_root) S.erase(n);
        if (val == -1) { ret = -1; break; }
        ret = (ret + val) % MOD;
      }
    }
  }
  if (ret >= MOD) ret %= MOD;
  if (n->is_root) DP[n] = ret;
  return ret;
}

int main() {
  //freopen("testdata/19.in",  "r", stdin);
  //freopen("testdata/19.out", "w", stdout);
  parse_input();
  build_definitions();
  cout << best(defs["essay"]) << endl;
  return 0;
}
