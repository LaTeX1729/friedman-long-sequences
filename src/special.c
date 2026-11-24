// Search for Friedman "special" sequences over {1,2,3}:  a = u 1 3^(n-1), |a| = 2n-2,
// property * holds, and every block a[i..2i] (i<=n-1) contains a 1.
// DFS over the prefix w = u1; each prefix ending in 1 is tested by padding with 3^(n-1).
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAXL 2000000
#define K 3
static unsigned char x[MAXL+2]; static int len; static int *cnt[K],*pos[K];
static inline void push(int c){ ++len; x[len]=c; for(int d=0;d<K;d++) cnt[d][len]=cnt[d][len-1]+(d==c); pos[c][cnt[c][len]]=len; }
static inline int nextdiff(int p){ int c=x[p],b=len+1; for(int d=0;d<K;d++) if(d!=c){int k=cnt[d][p]+1; if(k<=cnt[d][len]){int q=pos[d][k]; if(q<b)b=q;}} return b; }
static inline int embeds(int i,int j){
  for(int d=0;d<K;d++) if(cnt[d][2*i]-cnt[d][i-1]>cnt[d][2*j]-cnt[d][j-1]) return 0;
  int q=j,p=i,e=2*i; while(p<=e){ int c=x[p]; int nd=nextdiff(p); if(nd>e+1) nd=e+1; int k=cnt[c][q-1]+(nd-p); if(k>cnt[c][2*j]) return 0; q=pos[c][k]+1; p=nd; } return 1; }
static inline int ok_block(int j){ for(int i=j-1;i>=1;i--) if(embeds(i,j)) return 0; return 1; }
static double now(void){ struct timespec t; clock_gettime(CLOCK_MONOTONIC,&t); return t.tv_sec+1e-9*t.tv_nsec; }
static int best=0; static double t0;
static void try_complete(void){ // len = n-1, x[len]==0 (letter '1')
  int n=len+1, P=len; if(2*n-2<=best) { /* still must test? no: only improvements matter */ return; }
  int ok=1; for(int t=0;t<n-1 && ok;t++){ push(2); if(len%2==0 && !ok_block(len/2)) ok=0; }
  if(ok){ best=len; FILE*f=fopen("sbest.txt","w"); for(int i=1;i<=len;i++) fputc('1'+x[i],f); fclose(f); fprintf(stderr,"[%.0fs] special length %d\n",now()-t0,best); }
  len=P; }
int main(int argc,char**argv){
  double limit=atof(argv[1]); const char*order=argc>2?argv[2]:"123";
  for(int d=0;d<K;d++){ cnt[d]=calloc(MAXL+2,sizeof(int)); pos[d]=calloc(MAXL+2,sizeof(int)); }
  static signed char nxt[MAXL+2]; long long nodes=0; int complete=1; t0=now(); nxt[1]=0;
  for(;;){
    if((nodes&1023)==0 && now()-t0>limit){ complete=0; break; }
    int d=len+1; if(nxt[d]>=K){ if(len==0) break; len--; continue; }
    int c=order[nxt[d]++]-'1'; push(c); nodes++;
    int good=1; if(len%2==0){ int j=len/2; if(cnt[0][2*j]-cnt[0][j-1]==0) good=0; else good=ok_block(j); }
    if(!good){ len--; continue; }
    if(c==0) try_complete();
    nxt[len+1]=0; }
  printf("complete=%d nodes=%lld best special length=%d time=%.1f\n",complete,nodes,best,now()-t0); return 0; }
