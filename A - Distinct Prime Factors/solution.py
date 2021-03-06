# Distinct Prime Factors
# Official Solution - O(N log N)
# By Timothy Li

MAXN = 1000001
sieve = [0]*MAXN
for i in xrange(2, MAXN):
  if not sieve[i]:
    for j in xrange(i, MAXN, i):
      sieve[j] += 1
A = int(raw_input())
B = int(raw_input())
for i in xrange(A, B + 1):
  print sieve[i]
