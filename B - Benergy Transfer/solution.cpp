// Benergy Transfer
// Official Solution - O(N log N)
// By Alex Li

#include <cmath>
#include <cstdio>

const int MAXN = 100000;
const double EPS = 1E-7;
const double MAX = 1E10;

int N, A[MAXN], B[MAXN], vA[MAXN], vB[MAXN];

//squared distance between A and B at time t
double eval(double t) {
  double ret = 0.0, x;
  for (int i = 0; i < N; i++) {
    // (B[i] + t*vB[i]) - (A[i] + t*vA[i])
    x = (B[i] - A[i]) + t*(vB[i] - vA[i]);
    ret += x * x;
  }
  return ret;
}

double ternary_search(double L, double R) {
  double lthird, rthird;
  while (fabs(R - L) >= EPS) {
    lthird = (2*L + R)/3;
    rthird = (L + 2*R)/3;
    if (eval(lthird) >= eval(rthird))
      L = lthird;
    else
      R = rthird;
  }
  return (L + R)/2;
}

int main() {
  scanf("%d", &N);
  for (int i = 0; i < N; i++) scanf("%d", A + i);
  for (int i = 0; i < N; i++) scanf("%d", B + i);
  for (int i = 0; i < N; i++) scanf("%d", vA + i);
  for (int i = 0; i < N; i++) scanf("%d", vB + i);

  if (eval(0) <= eval(EPS)) { //not moving closer
    printf("0\n");
  } else {
    printf("%.8lf\n", ternary_search(0.0, MAX));
  }
  return 0;
}
