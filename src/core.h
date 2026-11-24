// Shared core: binary sequence x[1..len], run-indexed counts, and a fast block test.
// Block B_i = x[i..2i].  Property *(n): no n<=i<j with B_i a subsequence of B_j.
//
// Fast test for a new block j.  Let E(i) = max e such that x[i..e] embeds in B_j.
// E is nondecreasing in i, and B_i embeds in B_j iff E(i) >= 2i.  So scanning i downward,
// if E(i)=e<2i then any i'<i with B_i' in B_j needs 2i' <= E(i') <= e: jump to i'=e/2.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifndef MAXL
#define MAXL 6000000
#endif
static int N0; static unsigned char x[MAXL+2]; static int len;
static int *cnt[2], *pos[2];
static void core_init(void){ for(int d=0;d<2;d++){ cnt[d]=calloc(MAXL+2,sizeof(int)); pos[d]=calloc(MAXL+2,sizeof(int)); } }
static inline void push(int c){ ++len; x[len]=c; cnt[0][len]=cnt[0][len-1]+(c==0); cnt[1][len]=cnt[1][len-1]+(c==1); pos[c][cnt[c][len]]=len; }
static inline int nextdiff(int p){ int d=1-x[p]; int k=cnt[d][p]+1; return k<=cnt[d][len]?pos[d][k]:len+1; }
// E(i) for target block j (needs len>=2j); stops early once e>=2i
static inline int Eval(int i,int j){
  int q=j,p=i,J=2*j,lim=2*i;
  while(p<=lim){ int c=x[p]; int nd=nextdiff(p); int L=nd-p; int avail=cnt[c][J]-cnt[c][q-1];
    if(avail<L) return p+avail-1; q=pos[c][cnt[c][q-1]+L]+1; p=nd; }
  return p-1; }
static inline int ok_block(int j){ int i=j-1; while(i>=N0){ int e=Eval(i,j); if(e>=2*i) return 0; int ni=e/2; i=(ni<i-1)?ni:i-1; } return 1; }
static double now(void){ struct timespec t; clock_gettime(CLOCK_MONOTONIC,&t); return t.tv_sec+1e-9*t.tv_nsec; }
