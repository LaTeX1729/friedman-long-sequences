#!/usr/bin/env python3
"""Turn a binary M_2(4)-type sequence y into a Friedman 'special' sequence over {1,2,3} and verify it.

a = 1 2 2 f(y4) f(y5) ...  where f maps the separator letter to 1 and the long-run letter to 3,
cut to the form u 1 3^(n-1) (length 2n-2).  Verification: form, 'every block a[i..2i], i<=n-1,
contains a 1' (here), and property * from index 1 (by the brute-force C checker ./lfs)."""
import sys, subprocess, os
LFS=os.path.join(os.path.dirname(os.path.abspath(__file__)),'..','bin','lfs')
y=open(sys.argv[1]).read().strip(); out=sys.argv[2]
best=''
for sep in (sys.argv[3] if len(sys.argv)>3 else '01'):
    f={sep:'1', ('1' if sep=='0' else '0'):'3'}
    t='122'+''.join(f[c] for c in y[3:])
    ones=[i+1 for i,c in enumerate(t) if c=='1']
    for p in reversed(ones[-12:]):                     # p = n-1 : position of the last 1
        n=p+1; a=t[:p]+'3'*(n-1)
        if len(a)<=len(best): break
        pref=[0]
        for c in a: pref.append(pref[-1]+(c=='1'))
        if not all(pref[2*i]-pref[i-1]>0 for i in range(1,n)): continue
        open('tmp012.txt','w').write(a.translate(str.maketrans('123','012')))
        r=subprocess.run([LFS,'check','3','1','tmp012.txt'],capture_output=True,text=True).stdout
        print("separator letter",sep,"n-1 =",p,"length",len(a),"*:", "OK" if "OK" in r else "FAIL", flush=True)
        if "OK" in r: best=a; break
L=len(best); open(out,'w').write(best); k=(L-8)//26
print("special sequence length",L,"-> Lemma 4.3 with k =",k,": n(3) > A_%d(%d)"%(k-1,22*k+8))
