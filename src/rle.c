#include "core.h"
// Restricted run-length DFS for M_2(n).  A run of maximal extension R may take length r with
// r<=S, or r>=R-D, or any r if R<=RS.   usage: rle3 n S D RS seconds [tag]
static int S,D,RS; static double t0,limit; static int best=0,timeout=0; static long long rnodes=0; static const char*tag="rbest";
static void save(void){ best=len; char fn[64]; sprintf(fn,"%s.txt",tag); FILE*f=fopen(fn,"w"); for(int i=1;i<=len;i++) fputc('0'+x[i],f); fclose(f); fprintf(stderr,"[%.0fs] best %d (rle nodes %lld)\n",now()-t0,best,rnodes); }
static void rec(int c){
  if(timeout) return; if((rnodes&255)==0&&now()-t0>limit){ timeout=1; return; }
  rnodes++; int P=len,R=0;
  for(;;){ if(len>=MAXL-2)break; push(c); if(len%2==0&&len/2>N0&&!ok_block(len/2)){len--;break;} R++; }
  if(len>best) save(); if(R==0) return;
  int lo=(R<=RS)?1:R-D; if(lo<1)lo=1;
  for(int r=R;r>=lo;r--){ len=P+r; rec(1-c); }
  for(int r=(lo-1<S?lo-1:S);r>=1;r--){ len=P+r; rec(1-c); }
  len=P; }
int main(int argc,char**argv){ N0=atoi(argv[1]); S=atoi(argv[2]); D=atoi(argv[3]); RS=atoi(argv[4]); limit=atof(argv[5]); if(argc>6) tag=argv[6];
  core_init(); for(int i=1;i<N0;i++) push(0); t0=now(); rec(0);
  printf("n=%d S=%d D=%d RS=%d complete=%d rle_nodes=%lld best=%d time=%.1f\n",N0,S,D,RS,!timeout,rnodes,best,now()-t0); return 0; }
