/* Complete native road-distance helper, historical audio_channel_alloc.
 * Research only; true six-byte points and actual three-float scratch array.
 * Flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul.
 */
typedef signed char s8; typedef unsigned char u8;
typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef float f32;
typedef s16 RoadPoint[3];
typedef struct RoadGraph {
 u16 count, unknown2; RoadPoint *points; u8 branch_count, unknown9[3]; void *branches;
} RoadGraph;
typedef struct RoadSection {
 u8 unknown0[8]; f32 length; u8 unknownC[34]; s16 first_point; u8 unknown30[32];
} RoadSection;
extern RoadGraph D_801407F0;
extern RoadSection D_80151CE8[];
extern void func_800B9F60(s32, s32, s32 *, s32 *);
extern f32 sqrtf(f32);
#pragma intrinsic(sqrtf)
f32 audio_channel_alloc(s16 first, s32 last) {
 s32 next;
 f32 delta[3];
 f32 total;
 s32 i;
 u16 end;
 RoadPoint *point, *next_point;
 if (last<first && last<D_80151CE8[*(s16 *)((u8 *)D_80151CE8+2)].first_point) return -1.0f;
 if (last<first) end=D_801407F0.count;
 else end=last;
 total=0.0f;
 for (i=first;i<end;i++) {
  func_800B9F60(-1,i,0,&next);
  point=&D_801407F0.points[i]; next_point=&D_801407F0.points[next];
  delta[0]=(f32)((*next_point)[0]-(*point)[0]);
  delta[1]=(f32)((*next_point)[1]-(*point)[1]);
  delta[1]=0.0f;
  delta[2]=(f32)((*next_point)[2]-(*point)[2]);
  total+=sqrtf(delta[2]*delta[2]+(delta[0]*delta[0]+delta[1]*delta[1]));
 }
 if (end!=last) {
  for (i=D_80151CE8[*(s16 *)((u8 *)D_80151CE8+2)].first_point;i<last;i++) {
   func_800B9F60(-1,i,0,&next);
   point=&D_801407F0.points[i]; next_point=&D_801407F0.points[next];
   delta[0]=(f32)((*next_point)[0]-(*point)[0]);
   delta[1]=(f32)((*next_point)[1]-(*point)[1]);
   delta[1]=0.0f;
   delta[2]=(f32)((*next_point)[2]-(*point)[2]);
   total+=sqrtf(delta[2]*delta[2]+(delta[0]*delta[0]+delta[1]*delta[1]));
  }
 }
 return total;
}
