#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "audio_output_setup.c"
struct OSMesgQueue { int placeholder; };
OSMesgQueue D_80152770;
AudioList *D_801527C8;
static AudioList *default_after_acquire;
static int phase;
s32 osRecvMesg(OSMesgQueue *queue, OSMesg *message, s32 blocking) {
 assert(phase==0 && queue==&D_80152770 && message==0 && blocking==1);
 D_801527C8=default_after_acquire;phase=1;return -1;
}
s32 osJamMesg(OSMesgQueue *queue, OSMesg message, s32 blocking) {
 assert(phase==1 && queue==&D_80152770 && message==0 && blocking==0);
 phase=2;return -1;
}
static uint32_t rng=0x8badf00d;
static uint32_t random32(void) {rng^=rng<<13;rng^=rng>>17;rng^=rng<<5;return rng;}
int main(void) {
 AudioEntry entries[20],saved[20];AudioList list,empty;int n,mode,i,trial;unsigned int expected,got;unsigned long cases=0;
 memset(&list,0,sizeof list);memset(&empty,0,sizeof empty);
 for(trial=0;trial<10000;trial++)for(mode=0;mode<2;mode++) {
  n=trial%21;memset(entries,0xa5,sizeof entries);expected=0;
  for(i=0;i<n;i++) {entries[i].next=i+1<n?&entries[i+1]:0;entries[i].count=random32();entries[i].excluded=(signed char)(trial%256);if(i%3==0)entries[i].excluded=0;if(!entries[i].excluded)expected+=entries[i].count;}
  list.head=n?entries:0;empty.head=0;D_801527C8=&empty;default_after_acquire=&list;phase=0;memcpy(saved,entries,sizeof entries);
  got=audio_output_setup(mode?&list:0);assert(got==expected && phase==2);assert(memcmp(saved,entries,sizeof entries)==0);cases++;
 }
 printf("PASS: %lu cases, acquire-before-default-read, call arguments/order, unsigned wrapping, excluded bytes, empty/mixed lists, full node preservation\n",cases);return 0;
}
