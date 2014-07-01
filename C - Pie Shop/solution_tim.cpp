// Pie Shop
// Solution - O(R*(2^C)*Fib(C))
// By Timothy Li

#include <algorithm>
#include <cstdio>
#include <iostream>
#include <vector>
using namespace std;

const int MAXR = 16, MAXC = 16;
const int MOD = 1000000007;

int R, C, T[MAXR][MAXC];
int sum[MAXR][1 << MAXC];
int ngood, good[1 << MAXC];
pair<int, int> DP[MAXR + 1][1 << MAXC];

pair<int, int> rec(int row, int mask) {
  pair<int, int> &ret = DP[row][mask];
  if (ret.first != -1) return ret;
  ret.first = 0;
  if (ret.second = (row == R)) return ret;
  for (int i = 0; i < ngood; i++)
    if (!(good[i] & mask)) {
      pair<int, int> val = rec(row + 1, good[i]);
      if (val.first + sum[row][good[i]] > ret.first) {
        ret.first = val.first + sum[row][good[i]];
        ret.second = val.second;
      } else if (val.first + sum[row][good[i]] == ret.first) {
        ret.second += val.second;
        ret.second %= MOD;
      }
    }
  return ret;
}

int main() {
  for (int i = 0; i < MAXR + 1; i++)
    fill(DP[i], DP[i] + (1 << MAXC), make_pair(-1, -1));
  scanf("%d%d", &R, &C);
  for (int i = 0; i < R; i++)
    for (int j = 0; j < C; j++)
      scanf("%d", T[i] + j);
  for (int cur = 0; cur < (1<<C); cur++) {
    for (int i = 0; i < C - 1; i++)
      if (((cur >> i) & 3) == 3) goto next;
    good[ngood++] = cur;
    for (int i = 0; i < R; i++) {
      sum[i][cur] = 0;
      for (int j = 0; j < C; j++)
        sum[i][cur] += ((cur >> j) & 1)*T[i][j];
    }
    next:;
  }
  printf("%d %d\n", rec(0, 0).first, rec(0, 0).second);
  return 0;
}
