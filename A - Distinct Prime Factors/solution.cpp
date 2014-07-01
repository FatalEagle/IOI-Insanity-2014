// Distinct Prime Factors
// Official Solution - O(N log log N)
// By Alex Li

#include <iostream>
using namespace std;

const int MAXN = 1000001;

int A, B, sieve[MAXN];

int main() {
  cin >> A >> B;
  for (int i = 2; i < MAXN; i++) {
    if (!sieve[i])
      for (int j = i; j < MAXN; j += i)
        sieve[j]++;
  }
  for (int i = A; i <= B; i++)
    cout << sieve[i] << "\n";
  return 0;
}
