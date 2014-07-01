// Distinct Prime Factors
// Brute Force Solution - O(N^2)
// By Alex Li

#include <iostream>
using namespace std;

const int MAXN = 1000000;

int A, B, sieve[MAXN + 1];

int main() {
  for (int i = 2; i <= 1000; i++) {
    if (!sieve[i])
      for (int j = i*i; j <= MAXN; j += i)
        sieve[j] = true;
  }
  cin >> A >> B;
  for (int i = A; i <= B; i++) {
    int cnt = 0;
    for (int j = 2; j <= i; j++)
      if (i % j == 0 && !sieve[j])
        cnt++;
    cout << cnt << '\n';
  }
  return 0;
}
