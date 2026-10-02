/* Host semantics supplement only. This file is never compiled into the match. */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "func_800F7EB0.c"
#define CAPACITY 32770
short D_8014A108;
float D_80144DA8[CAPACITY];
unsigned char D_80144018[CAPACITY];
signed char D_80151AC0[3][5];
float D_80124618;
static unsigned long cases;
static void check_records(void)
{
 int r,c;
 for(r=0;r<3;r++) for(c=0;c<5;c++) assert(D_80151AC0[r][c] == -1);
}
static void run_case(int count, unsigned int bits)
{
 int i;
 unsigned int actual;
 D_8014A108=(short)count;
 memcpy(&D_80124618,&bits,sizeof bits);
 memset(D_80144DA8,0x55,sizeof D_80144DA8);
 memset(D_80144018,0xA5,sizeof D_80144018);
 memset(D_80151AC0,0x23,sizeof D_80151AC0);
 func_800F7EB0();
 for(i=0;i<CAPACITY;i++) {
  memcpy(&actual,&D_80144DA8[i],sizeof actual);
  assert(actual == (i<count ? bits : 0x55555555U));
  assert(D_80144018[i] == (i<count ? 0 : 0xA5));
 }
 check_records();
 assert(D_8014A108==count);
 memcpy(&actual,&D_80124618,sizeof actual); assert(actual==bits);
 cases++;
}
int main(void)
{
 static const unsigned int bits[]={0,0x80000000U,0x3F800000U,0xC1200000U,0x7F800000U,0xFF800000U,0x7FC12345U};
 static const int large[]={255,256,1024,32767};
 int n,k;
 assert(sizeof(float)==4 && sizeof(unsigned int)==4 && SHRT_MIN==-32768 && SHRT_MAX==32767);
 for(n=-32768;n<=0;n++) {
  D_8014A108=(short)n;
  D_80144DA8[0]=123.0f;D_80144018[0]=0xA5;
  memset(D_80151AC0,0,sizeof D_80151AC0);
  func_800F7EB0();
  assert(D_80144DA8[0]==123.0f && D_80144018[0]==0xA5);
  check_records(); cases++;
 }
 for(k=0;k<7;k++) {
  for(n=1;n<=128;n++) run_case(n,bits[k]);
  for(n=0;n<4;n++) run_case(large[n],bits[k]);
 }
 printf("PASS: %lu cases; all nonpositive signed-half counts, positive boundaries, full-array guards, float bit patterns and all15 record bytes\n",cases);
 return 0;
}
