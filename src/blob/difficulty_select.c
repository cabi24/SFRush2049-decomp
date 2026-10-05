/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * difficulty_select (0x800D2A74, 412 bytes; historical label, also aliased lap_counter_display):
 * N64-only path-distance helper, no arcade ancestor found.  Returns how many path points lie
 * ahead of `point` up to section `sec`'s marker on path `path`, wrapping by the path length:
 *   mode == 0          main path `path`: marker D_80151CE8[sec].first[path], wrap length
 *                      D_8012E5E8[path].total minus the loop section's marker (record 0 .anchor).
 *   mode != 0, path<0  same on the main loop: .total, wrap D_801407F0.total.
 *   mode != 0, path>=0 branch `path`: marker .second[path]; if it is unset (<0) or behind
 *                      `point`, recurse into the parent branch (nodes[path].parent) from the
 *                      branch's join point (nodes[path].end) and add the branch length (.delta).
 * Types (from loads; consistent with func_800B9740 and frontier w2a notes):
 *   D_80151CE8[] 80-byte section records (record 0 +2 = loop/anchor section);
 *   D_801407F0 path graph header {u16 total; ...; u8 nbranches @8; Branch *branches @12},
 *   Branch 16 bytes {u8 kind; .. s8 parent @4; u16 end @6; .. u16 count @10; Vertex *pts @12}.
 * Shaping: NO named locals.  Every named local costs a frame slot (retail frame is 32 bytes with
 * only the call-spill slots), so each table read is written out in full and CSE'd by uopt.  The
 * recursive term is written `delta + recursion` (retail adds the call result first).
 * Does not match at -O2 (s0/s1 saved instead of the caller-home spill).
 */
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

int difficulty_select(int mode, int path, int point, int sec) {
    if (mode == 0) {
        if (D_80151CE8[sec].first[path] < point) {
            return D_8012E5E8[path].total - point + D_80151CE8[sec].first[path]
                - ((sec >= D_80151CE8[0].anchor) ? D_80151CE8[D_80151CE8[0].anchor].first[path] : 0);
        }
        return D_80151CE8[sec].first[path] - point;
    }
    if (path < 0) {
        if (D_80151CE8[sec].total < point) {
            return D_801407F0.total - point + D_80151CE8[sec].total
                - ((sec >= D_80151CE8[0].anchor) ? D_80151CE8[D_80151CE8[0].anchor].total : 0);
        }
        return D_80151CE8[sec].total - point;
    }
    if (D_80151CE8[sec].second[path] >= 0 && D_80151CE8[sec].second[path] >= point)
        return D_80151CE8[sec].second[path] - point;
    return D_801407F0.nodes[path].delta
        + difficulty_select(mode, D_801407F0.nodes[path].parent, D_801407F0.nodes[path].end, sec)
        - point;
}
