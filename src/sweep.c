#include "core.h"
// Prefix sweep for M_2(n): the first T runs take every valid length; below each such prefix a
// restricted run-length DFS (r<=S or r>=R-D) runs with a node budget.  Prints the best length per prefix.
// usage: sweep n T S D budget [prefixbits]   (prefix must itself be valid and end a run)
static int S,D,T; static long long budget,used; static int best,gbest=0; static int runs[64];
static void rec(int c){
  if(used>=budget) return; used++; int P=len,R=0;
  for(;;){ if(len>=MAXL-2)break; push(c); if(len%2==0&&len/2>N0&&!ok_block(len/2)){len--;break;} R++; }
  if(len>best){ best=len; if(best>gbest){ gbest=best; FILE*f=fopen("sweep_best.txt","w"); for(int i=1;i<=len;i++) fputc('0'+x[i],f); fclose(f);} }
  if(R==0) return; int lo=R-D; if(lo<1)lo=1;
  for(int r=R;r>=lo;r--){ len=P+r; rec(1-c); }
  for(int r=(lo-1<S?lo-1:S);r>=1;r--){ len=P+r; rec(1-c); }
  len=P; }
static void top(int c,int t){
  if(t>T){ best=0; used=0; rec(c); printf("%d %lld ",best,used); for(int i=1;i<=T;i++) printf("%d,",runs[i]); printf("\n"); return; }
  int P=len,R=0; for(;;){ push(c); if(len%2==0&&len/2>N0&&!ok_block(len/2)){len--;break;} R++; if(R>200)break; }
  for(int r=R;r>=1;r--){ len=P+r; runs[t]=r; top(1-c,t+1); } len=P; } // descending: x[P+1..P+r] must still hold c
int main(int argc,char**argv){ N0=atoi(argv[1]); T=atoi(argv[2]); S=atoi(argv[3]); D=atoi(argv[4]); budget=atoll(argv[5]); core_init(); if(argc>6){ for(const char*p=argv[6];*p;p++) push(*p-'0'); top(1-x[len],1);} else { for(int i=1;i<N0;i++) push(0); top(0,1);}  return 0; }
