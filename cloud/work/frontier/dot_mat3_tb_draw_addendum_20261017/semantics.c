#include <stdint.h>
#include <stdio.h>
#include "root.c"
#include "position.c"
Just D_80118E20;s16 D_80149D92,D_80149D9E;
static u32 width;static s16 height;static unsigned width_calls,height_calls,draw_calls,bad;
static unsigned char text[2]={65,0};
u32 object_manager_update(unsigned char*p,s16 count){width_calls++;if(p!=text||count!=-1)bad=1;return width;}
s16 object_bytes23_sum(void){height_calls++;return height;}
void menu_input_process(void*p,s16 count){draw_calls++;if(p!=text||count!=-1)bad=1;}
static int signed16(uint32_t v){v&=65535;return v<32768?(int)v:(int)v-65536;}
int main(void){
 static const uint32_t widths[]={0,1,2,255,65535,0xffffffffu};
 static const short heights[]={0,1,2,255,256,510};
 static const int vertical[]={0,1,3,4};uint64_t cases=0;
 for(int coord=-32768;coord<32768;coord++)for(unsigned wi=0;wi<6;wi++)for(unsigned hi=0;hi<6;hi++)for(unsigned h=0;h<4;h++)for(unsigned v=0;v<4;v++){
  int y=-1-coord;int expected_x=coord,expected_y=y;
  width=widths[wi];height=heights[hi];D_80118E20.h=(JustEnum)h;D_80118E20.v=(JustEnum)vertical[v];
  if(h==1)expected_x=signed16((uint32_t)coord-(width>>1));else if(h==2)expected_x=signed16((uint32_t)coord-width);
  if(vertical[v]==1)expected_y=signed16((uint32_t)(y-height/2));else if(vertical[v]==4)expected_y=signed16((uint32_t)(y-height));
  if(expected_x==-32768)expected_x=1234;if(expected_y==-32768)expected_y=-2345;
  D_80149D92=1234;D_80149D9E=-2345;width_calls=height_calls=draw_calls=bad=0;
  state_utility((s16)coord,(s16)y,text);cases++;
  if(bad||D_80149D92!=expected_x||D_80149D9E!=expected_y||width_calls!=(h==1||h==2)||height_calls!=(vertical[v]==1||vertical[v]==4)||draw_calls!=1){fprintf(stderr,"text placement differential failure at %llu\n",(unsigned long long)cases);return 3;}
 }
 printf("%llu bounded text placement fixtures passed\n",(unsigned long long)cases);return 0;
}
