// English Essay
// Official Solution
// By Alex Li

#if __cplusplus > 199711L
  #define map unordered_map
  #define set unordered_set
#endif

#include <algorithm>
#include <cstring>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <vector>
using namespace std;

const int MAXN = 10000 + 1;
const int MOD = 1000000007;
const int INF = 0x3f3f3f3f;

vector<string> toks; //input tokens

//parse input into tokens, placing the results in toks
void tokenize_input() {
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

//each concat_t is a pair<value, isLiteral>
//if isLiteral: value is the length of the literal
//   otherwise: value is the ID of the symbol
typedef pair<int, bool> concat_t;
typedef vector<concat_t> branch_t;
typedef vector<branch_t> definition_t;

int ndefs = 0;
definition_t defs[MAXN]; //definitions

map<string, int> ID; //IDs of symbol name

inline int getID(const string &s) {
  static map<string, int>::iterator ID_it;
  if ((ID_it = ID.find(s)) == ID.end())
    return ID[s] = ndefs++;
  return ID_it->second;
}

//creates definition from [lo, hi) in the toks vector
void add_definition(definition_t &def, int lo, int hi) {
  def.push_back(branch_t());
  for (int i = lo; i < hi; i++) {
    if (toks[i] == "|") {
      def.push_back(branch_t());
    } else if (toks[i] == "<EOL>") {
      def.back().push_back(make_pair(1, true));
    } else if (toks[i][0] == '<') { //symbol
      def.back().push_back(
        make_pair(getID(toks[i]), false));
    } else { //literal
      def.back().push_back(
        make_pair(toks[i].length(), true));
    }
  }
}

void add_definitions() {
  for (int i = 1, hi; i < toks.size(); i++) {
    if (toks[i] == "::=") {
      vector<string>::iterator it;
      it = find(toks.begin() + i + 1, toks.end(), "::=");
      if (it == toks.end()) hi = toks.size();
      else hi = it - toks.begin() - 1;
      add_definition(defs[getID(toks[i - 1])], i + 1, hi);
      i = hi;
    }
  }
}

//<a> ::= <b> "x" | <b> <c> "xx" yields edges:
//  <b> --> (a, 1 + len(<b>))
//  <b> --> (a, 2 + len(<b>) + len(<c>))
//  <c> --> (a, 2 + len(<b>) + len(<c>))
//We store the distance as the number second part as IDs 

struct edge_t {
  int n, d_val; //node, and known part of weight
  vector<int> d_ids; //ids of unknown distances
} edge, edges[1000000];

int nedges = 0; //total edges
set<int> adj[MAXN + 1]; //set<edgeID> adj[MAXN]

void build_adj_list() {
  for (int i = 0; i < ndefs; i++) { //for all defs
    //for all branches
    definition_t::iterator it = defs[i].begin();
    for (; it != defs[i].end(); it++) {
      edges[nedges].n = i;
      edges[nedges].d_val = 0;
      //for all concatenated values
      branch_t::iterator it2 = it->begin();
      for (; it2 != it->end(); it2++) {
        if (it2->second) { //if isLiteral
          edges[nedges].d_val += it2->first;
        } else {
          edges[nedges].d_ids.push_back(it2->first);
        }
      }
      //for all concatenated values, again
      it2 = it->begin();
      for (; it2 != it->end(); it2++) {
        if (!it2->second) //if it's a symbol
          adj[it2->first].insert(nedges); //add edge
      }
      nedges++;
    }
  }
}

/************ Dijkstra's Algorithm ************/

int dist[MAXN + 1];

void update_dist(edge_t &e) {
  //In the current edge distance, check all dependent d_ids,
  //  if they've already been solved by dijkstra's in dist[]
  //  remove the node from d_ids and reupdate the d_val
  for (int i = 0; i < e.d_ids.size(); i++) {
    if (dist[e.d_ids[i]] != INF) {
      e.d_val += dist[e.d_ids[i]];
      e.d_ids[i] = -1; //mark it as -1 for removal
    }
  }
  remove(e.d_ids.begin(), e.d_ids.end(), -1);
}

struct dijkstra_comp {
  bool operator () (edge_t &a, edge_t &b) {
    update_dist(a);
    update_dist(b);
    //compare by first by whether they have dependencies
    //then compare by their shortest distance
    if (a.d_ids.empty() != b.d_ids.empty())
      //prioritize edges which has fully solved distances
      return a.d_ids.empty() > b.d_ids.empty();
    return a.d_val > b.d_val;
  }
};

int dijkstra(int dest) {
  memset(dist, INF, sizeof dist);
  priority_queue<edge_t, vector<edge_t>, dijkstra_comp> pq;
  //start by pushing edges with no dependencies for their distances
  //i.e. symbols that can be represented as strictly literals
  for (int e = 0; e < nedges; e++)
    if (edges[e].d_ids.empty()) {
      dist[edges[e].n] = edges[e].d_val;
      pq.push(edges[e]);
    }
  int a, b; edge_t temp;
  while (!pq.empty()) {
    edge = pq.top();
    pq.pop();
    if (!edge.d_ids.empty()) break; //out of computable distances
    if (edge.n == dest) return edge.d_val; //found answer
    a = edge.n; temp = edge;
    set<int>::iterator it = adj[edge.n].begin();
    for (; it != adj[edge.n].end(); ++it) { //for all neighbours
      if (dist[b = edges[*it].n] != INF) continue;
      if (edges[*it].d_ids.empty()) {
        if (dist[b] > dist[a] + edges[*it].d_val) {
          edge.d_val = dist[b] = 
            dist[a] + edges[*it].d_val;
        }
      }
      edge.n = b;
      update_dist(edge);
      pq.push(edge);
      edge = temp;
    }
  }
  return dist[dest] == INF ? -1 : dist[dest];
}

int main() {
  cin.sync_with_stdio(0); cin.tie(0);
  tokenize_input();
  add_definitions();
  build_adj_list();
  cout << dijkstra(ID["<essay>"]) << endl;
  return 0;
}
