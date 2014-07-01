// Benergy Transfer
// Golden Section Search - O(N log N)
// By Timothy Li

#include <algorithm>
#include <cmath>
#include <cstdio>

const int MAXN = 100000;
const double EPS = 1E-7;
const double MAX = 1E10;
const double phi2 = 2.0 - (1.0 + sqrt(5.0)) / 2.0;

int N;
int A[MAXN], vA[MAXN];
int B[MAXN], vB[MAXN];
int D[MAXN], vD[MAXN]; //for constant optimization

//squared distance between A and B at time t
double eval(double t) {
  double ret = 0.0, x;
  for (int i = 0; i < N; i++) {
    x = D[i] + t*vD[i];
    ret += x * x;
  }
  return ret;
}

double golden(double x1, double x2, double m1, double v1) {
  double m2;
  if (x2 - m1 > m1 - x1)
    m2 = m1 + phi2*(x2 - m1);
  else
    m2 = m1 - phi2*(m1 - x1);
  if (fabs(x2 - x1) < EPS) return x1;
  double v2 = eval(m2);
  if (m1 > m2) {
    std::swap(m1, m2);
    std::swap(v1, v2);
  }
  if (v1 > v2) return golden(m1, x2, m2, v2);
  return golden(x1, m2, m1, v1);
}

int main() {
  scanf("%d", &N);
  for (int i = 0; i < N; i++) scanf("%d", A + i);
  for (int i = 0; i < N; i++) scanf("%d", B + i);
  for (int i = 0; i < N; i++) scanf("%d", vA + i);
  for (int i = 0; i < N; i++) scanf("%d", vB + i);
  for (int i = 0; i < N; i++) {
    D[i] = A[i] - B[i];
    vD[i] = vA[i] - vB[i];
  }
  printf("%lf\n", golden(0.0, MAX, phi2*MAX, eval(phi2*MAX)));
  return 0;
}
