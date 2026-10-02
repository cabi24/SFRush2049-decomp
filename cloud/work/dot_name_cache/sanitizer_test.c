/* Bounded cache strings and defined eviction states only. */
#include <assert.h>
#include <stdio.h>
#include "host_bridge.c"
static unsigned rng=0xF1D04;
static unsigned next_random(void) {rng=rng*1664525u+1013904223u;return rng;}
int main(void) {
 unsigned char names[260],outnames[260],name[13];
 unsigned short ages[20],outages[20];int refs[180],outrefs[180];
 int t,i,result;
 for(t=0;t<20000;t++) {
  for(i=0;i<20;i++) {memset(names+i*13,0,13);sprintf((char *)names+i*13,"slot%02d",i);ages[i]=(unsigned short)next_random();}
  for(i=0;i<180;i++)refs[i]=(int)(next_random()%24)-2;
  memset(name,0,13);strcpy((char *)name,"new");
  if(t%4==0)sprintf((char *)name,"slot%02d",t%20);
  if(t%4==1)names[(t%20)*13]=0;
  if(t%4==2)for(i=0;i<180;i++)refs[i]=i%20;
  /* At least one incremented positive age establishes best in full eviction. */
  ages[0]=1;
  result=run_case(names,ages,refs,name,outnames,outages,outrefs);
  assert(result>=0 && result<20 && outages[result]==0);
  assert(strcmp((char *)outnames+13*result,(char *)name)==0);
 }
 puts("PASS 20000 bounded ASan/UBSan cases");return 0;
}
