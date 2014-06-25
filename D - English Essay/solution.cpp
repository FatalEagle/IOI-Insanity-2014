// English Essay
// Official Solution - O(N)
// By Alex Li

#define map unordered_map
#define set unordered_set

#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <vector>
using namespace std;

const int MOD = 1000000007;

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
definition_t defs[10001]; //definitions

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

int DP[10001];
bool done[10001], S[10001];

int best(int id) {
  if (done[id]) return DP[id];
  if (S[id]) return -1;
  int ret = -1;
  definition_t::iterator it = defs[id].begin();
  for (; it != defs[id].end(); it++) { //for all defs
    int curr = 0;
    branch_t::iterator it2 = it->begin();
    for (; it2 != it->end(); it2++) { //for all branches
      if (it2->second) { //if isLiteral
        curr += it2->first; //+= length
      } else {
        S[id] = true;
        int x = best(it2->first);
        S[id] = false;
        if (x == -1) { curr = -1; break; }
        curr = (curr + x) % MOD;
      }
    }
    if (curr == -1) continue;
    if (ret == -1 || curr < ret) ret = curr;
  }
  done[id] = true;
  return DP[id] = ret;
}

int main() {
  cin.sync_with_stdio(0); cin.tie(0);
  tokenize_input();
  add_definitions();
  cout << best(ID["<essay>"]) << endl;
  return 0;
}
