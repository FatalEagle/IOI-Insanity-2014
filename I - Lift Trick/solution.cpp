// Lift Trick
// Official Solution
// By Alex Li

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <utility>
#include <vector>
using namespace std;

const int MAX_N = 10001;
const int MAX_F = 1001;

struct passenger {
  int id, t, a, b;
  bool operator < (const passenger &rhs) const {
    return t < rhs.t || (t == rhs.t && a < rhs.a);
  }
} P[MAX_N];

int N, F, S;  double V;
int A[MAX_N], B[MAX_N];

typedef vector< pair<char, int> > solution;

/************* Elevator and passenger interface *************/

int curr_t = 0, curr_f = 1;
solution curr_sol;

//wait[f] = IDs of passengers waiting on floor f
//insi[f] = IDs inside the elevator getting off on floor f
vector<int> wait[MAX_F], insi[MAX_F];
int ptr = 0; //pointer to current passenger in P[]

//time when person i got out of the elevator
int t_exit[MAX_N];

//this must be called before every run-through
void reset() {
  curr_t = 0;
  curr_f = 1;
  curr_sol.clear();
  fill(wait, wait + MAX_F, vector<int>());
  fill(insi, insi + MAX_F, vector<int>());
  fill(t_exit, t_exit + MAX_N, -1);
  ptr = 0;
}

void go(int f) {
  curr_sol.push_back(make_pair('G', f));
  curr_f = f;
  curr_t += ceil(fabs(f - curr_f)/V);
}

void stop(int t) {
  curr_sol.push_back(make_pair('S', t));
  if (t >= S) { //open doors to let people out
    for (int i = 0; i < insi[curr_f].size(); i++) {
      t_exit[insi[curr_f][i]] = curr_t;
    }
    insi[curr_f].clear();
  }
  curr_t += t;
  //move people with A[i] up to curr_t into waiting
  while (ptr < N && P[ptr].t <= curr_t) {
    wait[A[P[ptr].id]].push_back(P[ptr].id);
    ptr++;
  }
  if (t >= S) { //open doors to let people in
    for (int i = 0; i < wait[curr_f].size(); i++) {
      insi[B[wait[curr_f][i]]].push_back(wait[curr_f][i]);
    }
    wait[curr_f].clear();
  }
}

double get_avg() {
  double ret = 0;
  for (int i = 0; i < N; i++) {
#ifndef NDEBUG
    if (t_exit[i] == -1) {
      fprintf(stderr, "Not all passengers served!\n");
      return -1;
    }
#endif
    ret += t_exit[i] - A[i] + 1;
  }
  return ret / N;
}

double solve1() {
  reset();

  //do stuff

  return get_avg();
}

double solve2() {
  reset();
  
  //do stuff

  return get_avg();
}

int main() {
  freopen("lift.in", "r", stdin);

  scanf("%d%d%lf%d", &F, &S, &V, &N);
  for (int i = 0; i < N; i++) {
    P[i].id = i;
    scanf("%d%d%d", &P[i].t, &P[i].a, &P[i].b);
    A[i] = P[i].a;
    B[i] = P[i].b;
  }
  sort(P, P + N);

  double best_avg = 1E30, avg;
  solution best_sol;

  if (best_avg > (avg = solve1())) {
    best_avg = avg;
    best_sol = curr_sol;
  }
  if (best_avg > (avg = solve2())) {
    best_avg = avg;
    best_sol = curr_sol;
  }
  //etc.

  for (int i = 0; i < best_sol.size(); i++) {
    printf("%c %d\n", best_sol[i].first, best_sol[i].second);
  }
  return 0;
}
