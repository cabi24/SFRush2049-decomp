/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
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
    unsigned char prefix[4];
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
int difficulty_select(int mode,int segment,int distance,int player) {
    int current,base,index;
    if (mode==0) {
        current=D_80151CE8[player].first[segment];
        if (current<distance) {
            index=D_80151CE8[0].anchor;
            if(player>=index) base=D_80151CE8[index].first[segment];
            else base=0;
            return D_8012E5E8[segment].total-distance+current-base;
        }
        return current-distance;
    }
    if (segment<0) {
        current=D_80151CE8[player].total;
        if(current<distance) {
            index=D_80151CE8[0].anchor;
            if(player>=index) base=D_80151CE8[index].total;
            else base=0;
            return D_801407F0.total-distance+current-base;
        }
        return current-distance;
    }
    current=D_80151CE8[player].second[segment];
    if(current>=0 && current>=distance) return current-distance;
    return difficulty_select(mode,D_801407F0.nodes[segment].parent,D_801407F0.nodes[segment].end,player)+D_801407F0.nodes[segment].delta-distance;
}
