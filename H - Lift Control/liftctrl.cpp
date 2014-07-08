// Lift Control
// Grader by Alex Li <alextrovert@gmail.com>

#include <algorithm>
#include <cstdio>
#include <cmath>
#include <cstdlib>
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

int curr_t, curr_f;

//wait[f] = IDs of passengers waiting on floor f
//insi[f] = IDs inside the elevator getting off on floor f
vector<int> wait[MAX_F], insi[MAX_F];
int ptr; //pointer to current passenger in P[]

//time when person i got out of the elevator
int t_exit[MAX_N];

//this must be called before every run-through
void reset() {
  curr_t = 0;
  curr_f = 1;
  fill(wait, wait + MAX_F, vector<int>());
  fill(insi, insi + MAX_F, vector<int>());
  fill(t_exit, t_exit + MAX_N, -1);
  ptr = 0;
}

void go(int f) {
  curr_t += ceil(fabs(f - curr_f)/V);
  curr_f = f;
}

void stop(int t) {
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
    if (t_exit[i] == -1) exit(1); //WA
    ret += t_exit[i] - T[i] + 1;
  }
  return ret / N;
}

int main(int argc, char** argv) {
  FILE *fans = fopen(argv[1], "r");
  FILE *fout = fopen(argv[2], "r");
  FILE *fin  = fopen(argv[3], "r");

  fscanf(fin, "%d%d%lf%d", &F, &S, &V, &N);
  for (int i = 0; i < N; i++) {
    fscanf(fin, "%d%d%d", T + i, A + i, B + i);
    P[i] = (passenger){i, T[i], A[i], B[i]};
  }
  sort(P, P + N);

  reset();
  char cmd; int val;
  while (fscanf(fout, " %c%d", &cmd, &val) == 2) {
    if (cmd == 'G') {
      if (val < 1 || val > F) return 1;
      go(val);
    } else if (cmd == 'S') {
      if (val < 0 || val > 1000000) return 1;
      stop(val);
    }
  }

  double X = get_avg(), Y;
  fscanf(fans, "%lf", &Y);
  double score = 10 + (int)(90 * (Y / X) + 0.5);
  printf("%d\n100\n", score);
  return 0;
}
