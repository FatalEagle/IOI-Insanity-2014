def LNDS(A):
    A=map(int, str(A))
    A=[0]+A
    dp=[0]*len(A)
    for i in xrange(1, len(A)):
        for j in xrange(i):
            if A[j]<=A[i]:
                v=dp[j]+1
                if v>dp[i]:
                    dp[i]=v
    return dp[len(A)-1]
A=input()
B=input()
X=6
ans=0
i=A
while i<=B:
    if LNDS(i)>=X:
        ans+=1
    i+=1
print ans
