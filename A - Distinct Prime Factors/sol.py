MAXN=1000001
sieve=[0]*MAXN
for i in xrange(2, MAXN):
    if not sieve[i]:
        for j in xrange(i, MAXN, i):
            sieve[j]+=1
X=input()
Y=input()
for i in xrange(X, Y+1):
    print sieve[i]
