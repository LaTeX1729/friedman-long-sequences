#include "core.h"
// Stage-structured search for long M_2(n) sequences (heuristic; every sequence it reports is fully checked).
//
// Observed shape of long sequences:   <start> c^r1 S1 c^r2 S2 c^r3 ...   with r_{t+1} ~ 2 r_t and
// short "separator" words S_t that begin and end with the other letter.
//  * positions < PF ("free phase"): every run length 1..R is tried.
//  * afterwards ("stage phase"): a long run takes r = R - delta (0<=delta<=D); then a separator S
//    (|S|<=LM) is accepted only if the following c-run can reach ALPHA*r.  For each separator the
//    smallest delta is used.  Children are tried longest separator first.
//  * at every stage node a small generic run-length DFS looks for a good ending.
// usage: macro n PF D LM ALPHA stage_budget seconds [prefixbits]
// env ASC=1: enumerate free-phase run lengths ascending.  env SPECIAL=1: only stage structures whose long
// runs are 1s, so the result converts to a Friedman special sequence (see to_special.py).
#define LMAX 32
#define MAXCH 4096
static int PF,D,LM,ASC=0,SPECIAL=0,recok=1; static double ALPHA,t0,limit; static long long mbudget,mused,work,worklim; static int best=0,gbest=0,timeout=0;
typedef struct { int delta,sl; unsigned char s[LMAX]; } Child;
static void record(void){ if(!recok) return; if(len>best) best=len; if(len>gbest){ gbest=len; FILE*f=fopen("macro_best.txt","w"); for(int i=1;i<=len;i++) fputc('0'+x[i],f); fclose(f); fprintf(stderr,"[%.0fs] best %d\n",now()-t0,gbest); } }
static inline int pushok(int c){ push(c); work++; if(len%2==0&&len/2>N0&&!ok_block(len/2)){ len--; return 0; } return 1; }
static int walk(int c){ int R=0; while(len<MAXL-2&&pushok(c)) R++; return R; }
static void generic(int c){ if(work>worklim) return; int P=len,R=walk(c); record(); if(R==0) return;
  int lo=R-8; if(lo<1)lo=1; for(int r=R;r>=lo;r--){ len=P+r; generic(1-c); } for(int r=(lo-1<6?lo-1:6);r>=1;r--){ len=P+r; generic(1-c); } len=P; }
static void sepdfs(int c,int need,int delta,unsigned char*s,int sl,Child*ch,int*n){
  if(s[sl-1]==1-c){ int P=len,ok=1; for(int k=0;k<need;k++) if(!pushok(c)){ ok=0; break; } len=P;
    if(ok){ int dup=0; for(int i=0;i<*n&&!dup;i++) if(ch[i].sl==sl&&!memcmp(ch[i].s,s,sl)) dup=1;
      if(!dup&&*n<MAXCH){ ch[*n].delta=delta; ch[*n].sl=sl; memcpy(ch[*n].s,s,sl); (*n)++; } } }
  if(sl>=LM) return;
  for(int b=0;b<2;b++) if(pushok(b)){ s[sl]=b; sepdfs(c,need,delta,s,sl+1,ch,n); len--; } }
static int cmpch(const void*a,const void*b){ const Child*p=a,*q=b; if(p->sl!=q->sl) return q->sl-p->sl; return p->delta-q->delta; }
static void macro(int c,int wasfree){
  if(timeout) return; if(now()-t0>limit){ timeout=1; return; }
  int P=len; int R=walk(c); record(); if(R==0) return;
  if(P<PF){ if(ASC) for(int r=1;r<=R;r++){ len=P; for(int k=0;k<r;k++) push(c); macro(1-c,1); }
            else for(int r=R;r>=1;r--){ len=P+r; macro(1-c,1); }
            len=P; return; }
  if(wasfree==1){ for(int r=R;r>=1;r--){ len=P+r; macro(1-c,2); } len=P; for(int k=0;k<R;k++) push(c); } // also allow the stage letter to be the other one
  if(SPECIAL&&wasfree&&c!=1){ len=P; return; }   // special-compatible: long runs must be 1s (y4=0 is the separator letter)
  if(wasfree){ mused=0; best=0; recok=1; }
  if(mused<mbudget){ mused++;
    Child*ch=malloc(sizeof(Child)*MAXCH); int n=0; unsigned char s[LMAX];
    for(int delta=0;delta<=D&&delta<R;delta++){ len=P+R-delta; int need=(int)(ALPHA*(R-delta))+1; if(pushok(1-c)){ s[0]=1-c; sepdfs(c,need,delta,s,1,ch,&n); len--; } }
    qsort(ch,n,sizeof(Child),cmpch);
    for(int i=0;i<n&&mused<mbudget&&!timeout;i++){ len=P; for(int k=0;k<R-ch[i].delta;k++) push(c); for(int k=0;k<ch[i].sl;k++) push(ch[i].s[k]); macro(c,0); }
    free(ch); }
  len=P; worklim=work+40LL*P+200000; generic(c); len=P;
  if(wasfree){ printf("%d %lld ",best,mused); for(int i=1;i<=P;i++) putchar('0'+x[i]); printf("\n"); fflush(stdout); if(SPECIAL) recok=0; }
}
int main(int argc,char**argv){ N0=atoi(argv[1]); PF=atoi(argv[2]); D=atoi(argv[3]); LM=atoi(argv[4]); ALPHA=atof(argv[5]); mbudget=atoll(argv[6]); limit=atof(argv[7]);
  core_init(); t0=now(); if(getenv("ASC")) ASC=1; if(getenv("SPECIAL")){ SPECIAL=1; recok=0; }
  if(argc>8){ for(const char*p=argv[8];*p;p++) push(*p-'0'); macro(1-x[len],1); } else { for(int i=1;i<N0;i++) push(0); macro(0,1); }
  fprintf(stderr,"done complete=%d best=%d time=%.0f\n",!timeout,gbest,now()-t0); return 0; }
