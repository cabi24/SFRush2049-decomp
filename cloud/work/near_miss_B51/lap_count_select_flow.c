/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed short s16;
typedef unsigned short u16;
typedef signed char s8;
typedef struct TrackRecord {
    u16 prefix;
    s16 anchor;
    unsigned char opaque[42];
    s16 total;
    s16 first[4];
    s16 second[12];
} TrackRecord;
typedef struct Descriptor {
    unsigned char prefix;
    s8 previous;
    u16 previous_end;
    s8 parent;
    unsigned char gap;
    u16 end;
    unsigned char gap2[2];
    u16 delta;
    unsigned char tail[4];
} Descriptor;
typedef struct Header {
    u16 total;
    unsigned char opaque[10];
    Descriptor *nodes;
} Header;
typedef struct Count {u16 total;unsigned char opaque[6];} Count;
extern TrackRecord D_80151CE8[];
extern Header D_801407F0;
extern Count D_8012E5E8[];
void lap_count_select(int mode,int segment,int distance,int increment,int *out_segment,int *out_distance) {
 if(mode==0 || segment<0) {
  *out_segment=segment;
  *out_distance=distance+increment;
  if(mode) {
   while(*out_distance>=D_801407F0.total)
    *out_distance=D_80151CE8[D_80151CE8[0].anchor].total+*out_distance-D_801407F0.total;
  } else {
   while(*out_distance>=D_8012E5E8[segment].total)
    *out_distance=D_80151CE8[D_80151CE8[0].anchor].first[segment]+*out_distance-D_8012E5E8[segment].total;
  }
 } else if(distance+increment<D_801407F0.nodes[segment].delta) {
  *out_segment=segment;
  *out_distance=distance+increment;
 } else {
  lap_count_select(mode,D_801407F0.nodes[segment].parent,D_801407F0.nodes[segment].end,increment-D_801407F0.nodes[segment].delta+distance,out_segment,out_distance);
 }
}
