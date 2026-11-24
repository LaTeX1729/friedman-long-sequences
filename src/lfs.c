// Friedman "Long Finite Sequences": property * checker and exhaustive DFS.
// Sequence x[1..p] over letters 0..K-1.  Block B_i = x[i..2i].
// Property *(n): no n <= i < j <= p/2 with B_i a subsequence of B_j.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAXL 4000000
#define MAXK 3
static int K, N0;
static unsigned char x[MAXL+2];
static int len;
static int *cnt[MAXK];   // cnt[c][p] = #c in x[1..p]
static int *pos[MAXK];   // pos[c][k] = position of k-th c
static void push(int c){ ++len; x[len]=c; for(int d=0;d<K;d++) cnt[d][len]=cnt[d][len-1]+(d==c); pos[c][cnt[c][len]]=len; }
static void pop(void){ --len; }
// first position > p holding a letter != x[p], or len+1
static inline int nextdiff(int p){ int c=x[p], best=len+1; for(int d=0;d<K;d++) if(d!=c){ int k=cnt[d][p]+1; if(k<=cnt[d][len]){int q=pos[d][k]; if(q<best)best=q;} } return best; }
// is B_i a subsequence of x[j..2j]?  requires 2j<=len
static inline int embeds(int i,int j){
  for(int d=0;d<K;d++) if(cnt[d][2*i]-cnt[d][i-1] > cnt[d][2*j]-cnt[d][j-1]) return 0;
  int q=j, p=i, e=2*i;
  while(p<=e){ int c=x[p]; int nd=nextdiff(p); if(nd>e+1) nd=e+1; int L=nd-p;
    int k=cnt[c][q-1]+L; if(k>cnt[c][2*j]) return 0; q=pos[c][k]+1; p=nd; }
  return 1;
}
// does block j (just completed, len>=2j) avoid all earlier blocks?
static inline int ok_block(int j){ for(int i=j-1;i>=N0;i--) if(embeds(i,j)) return 0; return 1; }
static double now(void){ struct timespec t; clock_gettime(CLOCK_MONOTONIC,&t); return t.tv_sec+1e-9*t.tv_nsec; }
int main(int argc,char**argv){
  if(argc<4){ fprintf(stderr,"usage: lfs check K n file | lfs search K n seconds [prefixfile]\n"); return 1; }
  K=atoi(argv[2]); N0=atoi(argv[3]);
  for(int d=0;d<K;d++){ cnt[d]=calloc(MAXL+2,sizeof(int)); pos[d]=calloc(MAXL+2,sizeof(int)); }
  if(!strcmp(argv[1],"check")){
    FILE*f=fopen(argv[4],"r"); int ch; while((ch=fgetc(f))!=EOF) if(ch>='0'&&ch<'0'+K) push(ch-'0'); fclose(f);
    printf("length %d\n",len); double t0=now();
    for(int j=N0+1;2*j<=len;j++) for(int i=N0;i<j;i++) if(embeds(i,j)){ printf("FAIL: block %d embeds in block %d\n",i,j); return 2; }
    printf("OK: property * holds from index %d (%.1fs)\n",N0,now()-t0); return 0;
  }
  // exhaustive DFS. Symmetry: x[1..n-1] never lie in a block start... they are outside all blocks -> fix 0.
  double limit=atof(argv[4]), t0=now(); int base=0;
  if(argc>5){ FILE*f=fopen(argv[5],"r"); int ch; while((ch=fgetc(f))!=EOF) if(ch>='0'&&ch<'0'+K) push(ch-'0'); fclose(f); base=len; }
  else { for(int i=1;i<N0;i++) push(0); push(0); base=len; } // x[n]=0 by letter symmetry (K=2 exact; K=3 up to relabel)
  static signed char nxt[MAXL+2]; long long nodes=0; int best=0; long long nbest=0; static unsigned char bestx[MAXL+2];
  int depth=base; nxt[depth+1]=0; int complete=1;
  for(;;){
    if((nodes&1023)==0 && now()-t0>limit){ complete=0; break; }
    int d=len+1;
    if(nxt[d]>=K){ if(len==base) break; pop(); continue; }
    int c=nxt[d]++; push(c); nodes++;
    int good=1; if(len%2==0 && len/2>N0) good=ok_block(len/2);
    if(!good){ pop(); continue; }
    if(len>best){ best=len; nbest=1; memcpy(bestx,x,len+1);} else if(len==best) nbest++;
    nxt[len+1]=0;
  }
  // max valid length is odd unless search starts odd; report as found
  printf("K=%d n=%d complete=%d nodes=%lld time=%.1fs\n",K,N0,complete,nodes,now()-t0);
  printf("max length %d, #sequences at that length (with fixed prefix) %lld\n",best,nbest);
  if(best<=400){ for(int i=1;i<=best;i++) putchar('0'+bestx[i]); putchar('\n'); }
  FILE*f=fopen("best.txt","w"); for(int i=1;i<=best;i++) fputc('0'+bestx[i],f); fclose(f);
  return 0;
}
