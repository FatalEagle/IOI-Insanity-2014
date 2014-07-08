// Lift Control
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
int T[MAX_N], A[MAX_N], B[MAX_N];

typedef vector< pair<char, int> > solution;

/************* Elevator and passenger interface *************/

int curr_t = 0, curr_f = 1;
solution curr_sol;

//wait[f] = IDs of passengers waiting on floor f
//insi[f] = IDs inside the elevator getting off on floor f
vector<int> wait[MAX_F], insi[MAX_F];
int ptr = 0; //pointer to current passenger in P[]

//time when person i got out of the elevator
int done = 0, t_exit[MAX_N];

//this must be called before every run-through
void reset() {
  curr_t = 0;
  curr_f = 1;
  curr_sol.clear();
  fill(wait, wait + MAX_F, vector<int>());
  fill(insi, insi + MAX_F, vector<int>());
  fill(t_exit, t_exit + MAX_N, -1);
  done = 0;
  ptr = 0;
}

void go(int f) {
  curr_sol.push_back(make_pair('G', f));
  curr_t += ceil(fabs(f - curr_f)/V);
  curr_f = f;
}

void stop(int t) {
  curr_sol.push_back(make_pair('S', t));
  if (t >= S) { //open doors to let people out
    for (int i = 0; i < insi[curr_f].size(); i++) {
      t_exit[insi[curr_f][i]] = curr_t;
      done++;
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
  double ret = 0; int cnt = 0;
  for (int i = 0; i < N; i++) {
    if (t_exit[i] != -1) {
      ret += t_exit[i] - T[i] + 1;
      cnt++;
    }
#if 0
    else {
      fprintf(stderr, "%d - Not all passengers served!\n");
      return 1E30;
    }
    printf("%d [%d, %d]\n", i + 1, T[i], t_exit[i]);
#endif
  }
  return (cnt == 0) ? 1E30 : (ret / cnt);
}

/************* Implement Solutions Below *************/

/* Heuristic Solution 1 - Back and Forth

   Sweep up to process all tasks, then sweep down
   If nothing has been processed, go to the floor where
   the next passenger will be waiting, then stop until
   they arrive to pick them up.
*/
double solve1() {
  reset();
  //Just in case TLE
  for (int steps = 0; steps < 100000; steps++) {
    bool action = false;
    //sweep down, then up
    for (int df = -1; df <= 1; df += 2) {
      int lim = (df == -1 ? 0 : F + 1);
      for (int f = curr_f; f != lim; f += df) {
        if (!insi[f].empty()) { //drop off
          go(f); stop(S);
          action = true;
        }
        if (!wait[f].empty()) { //pick up
          go(f); stop(S);
          action = true;
        }
      }
    }
    if (!action) {
      if (ptr == N) break; //done
      go(P[ptr].a); //go to the next passenger's floor
      //stop just long enough so they can get in
      stop(max(S, P[ptr].t - curr_t + 1));
    }
  }
  return get_avg();
}


/* Heuristic Solution 2 - Greedy

   Tend to the floor that has the largest waiting average
*/

double solve2() {
  reset();
  return get_avg();
  for (int steps = 0; steps < 100000; steps++) {
    int max_time = -1, max_floor;
    for (int f = 1; f <= F; f++) {
      if (wait[f].empty()) continue;
      double floor_avg = 0;
      for (int i = 0; i < wait[f].size(); i++) {
        floor_avg += curr_t - A[wait[f][i]];
      }
      floor_avg /= wait[f].size();
      if (floor_avg > max_time) {
        max_time = floor_avg;
        max_floor = f;
      }
    }
  }

  return get_avg();
}

int main() {
#define NUM "6"
  freopen("testdata/" NUM ".in", "r", stdin);
  freopen("testdata/" NUM ".out", "w", stdout);

  scanf("%d%d%lf%d", &F, &S, &V, &N);
  for (int i = 0; i < N; i++) {
    scanf("%d%d%d", T + i, A + i, B + i);
    P[i] = (passenger){i, T[i], A[i], B[i]};
  }
  sort(P, P + N);

  double best_avg = 1E30, avg;
  solution best_sol;

  if (best_avg > (avg = solve1())) {
    best_avg = avg;
    best_sol = curr_sol;
  }
/*
  if (best_avg > (avg = solve2())) {
    best_avg = avg;
    best_sol = curr_sol;
  }
  //etc...
*/
  printf("%.8lf\n", best_avg);
  for (int i = 0; i < best_sol.size(); i++) {
    printf("%c %d\n", best_sol[i].first, best_sol[i].second);
  }
  return 0;
}
