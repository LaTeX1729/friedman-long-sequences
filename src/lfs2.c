#include "core.h"
// Fast checker / exhaustive DFS for the binary problem M_2(n).
// lfs2 search n seconds [prefixfile] | lfs2 check n file | lfs2 runs n file
int main(int argc,char**argv){
  N0=atoi(argv[2]); core_init();
  if(!strcmp(argv[1],"check")||!strcmp(argv[1],"runs")){
    static unsigned char s[MAXL]; int L=0; FILE*f=fopen(argv[3],"r"); int ch; while((ch=fgetc(f))!=EOF) if(ch=='0'||ch=='1') s[++L]=ch-'0'; fclose(f);
    if(!strcmp(argv[1],"check")){ for(int i=1;i<=L;i++){ push(s[i]); if(len%2==0&&len/2>N0&&!ok_block(len/2)){ printf("FAIL at block %d\n",len/2); return 2; } } printf("length %d OK from index %d\n",L,N0); return 0; }
    // runs: for each run print (letter, r, R = max valid extension)
    int p=1; while(p<=L){ int c=s[p], r=0; while(p+r<=L&&s[p+r]==c) r++; int P=len,R=0;
      for(;;){ if(len>=MAXL-2)break; push(c); if(len%2==0&&len/2>N0&&!ok_block(len/2)){len--;break;} R++; if(R>4*(P+10)+r) {R=-1;break;} }
      printf("%d^%d(R=%d) ",c,r,R); len=P+r; p+=r; } printf("\n"); return 0; }
  double limit=atof(argv[3]),t0=now(); int base;
  if(argc>4){ FILE*f=fopen(argv[4],"r"); int ch; while((ch=fgetc(f))!=EOF) if(ch=='0'||ch=='1') push(ch-'0'); fclose(f);} else for(int i=1;i<=N0;i++) push(0);
  base=len; static signed char nxt[MAXL+2]; long long nodes=0,nbest=0; int best=0,complete=1; nxt[len+1]=0;
  for(;;){ if((nodes&4095)==0&&now()-t0>limit){complete=0;break;}
    int d=len+1; if(nxt[d]>=2){ if(len==base)break; len--; continue; }
    int c=nxt[d]++; push(c); nodes++;
    if(len%2==0&&len/2>N0&&!ok_block(len/2)){ len--; continue; }
    if(len>best){best=len;nbest=1; FILE*f=fopen("best.txt","w"); for(int i=1;i<=len;i++) fputc('0'+x[i],f); fclose(f);} else if(len==best) nbest++;
    nxt[len+1]=0; }
  printf("n=%d complete=%d nodes=%lld max=%d count=%lld time=%.1f\n",N0,complete,nodes,best,nbest,now()-t0); return 0; }
