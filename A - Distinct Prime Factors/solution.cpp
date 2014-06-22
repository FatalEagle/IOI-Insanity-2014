// Distinct Prime Factors
// Official Solution - O(N log log N)
// By Alex Li

#include <iostream>
using namespace std;

const int MAXN = 1000001;

int X, Y, sieve[MAXN];

int main() {
  cin >> X >> Y;
  for (int i = 2; i < MAXN; i++) {
    if (!sieve[i])
      for (int j = i; j < MAXN; j += i)
        sieve[j]++;
  }
  for (int i = X; i <= Y; i++)
    cout << sieve[i] << "\n";
  return 0;
}
