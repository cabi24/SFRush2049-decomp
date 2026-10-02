/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed short s16;typedef unsigned short u16;typedef unsigned char u8;typedef float f32;
typedef struct Point6 {s16 x,y,z;} Point6;
typedef struct Route16 {u8 mode,start;u16 first;u8 end,gap;u16 last;u8 state,gap2;u16 count;Point6 *points;} Route16;
typedef struct Routes16 {u8 prefix[8];u8 count;u8 gap[3];Route16 *routes;} Routes16;
typedef struct Track80 {u8 prefix[12];f32 x;u8 gap[4];f32 z,dx;u8 gap2[4];f32 dz;int squared_range;u8 tail[40];} Track80;
extern Routes16 D_801407F0;
extern Track80 D_80151CE8[];
int audio_priority_find(s16 route_index,s16 player_index) {
 f32 direction[2],delta[2];
 s16 i,previous;
 int first=1,sign;
 Route16 *route;
 Track80 *track;
 if(route_index>=D_801407F0.count)return -1;
 route=&D_801407F0.routes[route_index];
 if(route->mode==2) {
  if(player_index==0)return 0;
  return -1;
 }
 track=&D_80151CE8[player_index];
 direction[0]=track->dx;
 direction[1]=track->dz;
 for(i=0;i<route->count;i++) {
  delta[0]=(f32)route->points[i].x-track->x;
  delta[1]=(f32)route->points[i].z-track->z;
  if(delta[0]*delta[0]+delta[1]*delta[1]<=(f32)track->squared_range) {
   sign=1;
   if(direction[0]*delta[0]+direction[1]*delta[1]<0.0f)sign=-1;
   if(first) {
    previous=sign;
    first=0;
   } else if(previous!=sign)return i;
  }
 }
 return -1;
}
