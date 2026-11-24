import sys, itertools
s=open(sys.argv[1]).read().strip(); L=len(s); n=L//2+1
def sub(a,b):
    it=iter(b); return all(c in it for c in a)
B=lambda i: s[i-1:2*i]
assert L==2*n-2 and s[n-2]=='1' and s[n-1:]=='3'*(n-1), "form u 1 3^(n-1)"
assert all('1' in B(i) for i in range(1,n)), "every block has a 1"
bad=[(i,j) for j in range(2,L//2+1) for i in range(1,j) if sub(B(i),B(j))]
assert not bad, bad[:3]
k=(L-8)//26
print("special: length",L,"n =",n,"-> k =",k,": n(3) > A_%d(%d)"%(k-1,22*k+8))
print("prefix u1 runs:",[(c,len(list(g))) for c,g in itertools.groupby(s[:n-1])])
