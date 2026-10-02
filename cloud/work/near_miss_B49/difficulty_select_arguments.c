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
    int original=segment;
    if (mode==0) {
        segment=D_80151CE8[player].first[original];
        if (segment<distance) {
            return D_8012E5E8[original].total-distance+segment-
                (player>=D_80151CE8[0].anchor?D_80151CE8[D_80151CE8[0].anchor].first[original]:0);
        }
        return segment-distance;
    }
    if (original<0) {
        mode=D_80151CE8[player].total;
        if(mode<distance) {
            return D_801407F0.total-distance+mode-
                (player>=D_80151CE8[0].anchor?D_80151CE8[D_80151CE8[0].anchor].total:0);
        }
        return mode-distance;
    }
    if(D_80151CE8[player].second[original]>=0 && D_80151CE8[player].second[original]>=distance)
        return D_80151CE8[player].second[original]-distance;
    return difficulty_select(mode,D_801407F0.nodes[original].parent,D_801407F0.nodes[original].end,player)+D_801407F0.nodes[original].delta-distance;
}
