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
void split_time_display(int segment,int distance,int amount,int *out_segment,int *out_distance) {
 if(segment<0) {
  *out_segment=segment;
  *out_distance=distance-amount;
  if(distance<D_80151CE8[D_80151CE8[0].anchor].total) {
   if(*out_distance<0)*out_distance=0;
  } else {
   while(*out_distance<D_80151CE8[D_80151CE8[0].anchor].total)
    *out_distance=*out_distance+D_801407F0.total-D_80151CE8[D_80151CE8[0].anchor].total;
  }
 } else if(distance>=amount) {
  *out_segment=segment;
  *out_distance=distance-amount;
 } else {
  split_time_display(D_801407F0.nodes[segment].previous,D_801407F0.nodes[segment].previous_end,amount-distance,out_segment,out_distance);
 }
}
