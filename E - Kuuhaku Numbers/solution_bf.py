# Kuuhaku Numbers
# Brute Force Solution - O(???)
# By Timothy Li

def LNDS(A):
  A = map(int, str(A))
  A = [0] + A
  DP = [0]*len(A)
  for i in xrange(1, len(A)):
    for j in xrange(i):
      if A[j] <= A[i]:
        v = DP[j] + 1
        if v > DP[i]:
          DP[i] = v
  return DP[len(A) - 1]

A = int(raw_input())
B = int(raw_input())
X = 6
ans = 0
i = A
while i <= B:
  if LNDS(i) >= X:
    ans += 1
  i += 1
print ans
