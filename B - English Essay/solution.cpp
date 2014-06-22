//English Essay
//Official Solution - O(N)
//by Alex Li

#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <vector>
using namespace std;

const int INF = 0x3f3f3f3f;

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

//range [lo, hi) in the tokens vector to build from
node_t* build_tree(int lo, int hi) {
  if (lo >= hi) return NULL; //???
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

map<node_t*, int> memo;
map<node_t*, int>::iterator it;
set<node_t*> S;

int best(node_t *n) {
  if (n == NULL) return INF;
  if (n->is_root) {
    if ((it = memo.find(n)) != memo.end()) return it->second;
    if (S.count(n)) return INF;
  }
  int ret;
  if (n->lchild != NULL) { //if n is not a leaf
    if (n->is_root) S.insert(n);
    ret = min(best(n->lchild), best(n->rchild));
    if (n->is_root) S.erase(n);
  } else {
    ret = 0;
    vector<item_t>::iterator it2 = n->items.begin();
    for (; it2 != n->items.end(); it2++) {
      if (it2->is_literal) {
        ret += it2->value.length();
      } else {
        if (it2->value == "EOL") {
          ret++;
          continue;
        }
        if (n->is_root) S.insert(n);
        ret += best(defs[it2->value]);
        if (n->is_root) S.erase(n);
        if (ret >= INF) break;
      }
    }
  }
  if (n->is_root) memo[n] = ret;
  return ret;
}

int main() {
  parse_input();
  build_definitions();
  int ans = best(defs["essay"]);
  cout << (ans < INF ? ans : -1) << endl;
  return 0;
}
