/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * physics_response @ 0x800B93A8 (920 B) -- PROVISIONAL: EQUAL (score.py group: MATCH) only with its real but
 * still-unmatched caller audio_effect_apply (0x800BAAA0) as context; retail writes s0-s6/$f20-$f24 without
 * saving them, so the body cannot be scored outside a group containing that caller.
 *
 * Not physics (historical label): N64 checkpoint-section timing, the N64 descendant of arcade
 * game/checkpoint.c InitCPS's "optimal time" walk (get_next_checkpoint ring walk, per-checkpoint time, total).
 * For two passes (time[0], time[1]) walk the section ring of D_80151CE8 (80-byte records; header fields
 * total@0, first@2 (ring wrap section), last@4, start@6 (first section), count@8 live in record 0) and store into time[pass] the travel time of each section:
 * sum over its path points of segment length / (speed byte * 1.4666667 (mph -> ft/s)); the closing segment goes
 * through func_800B9338 (next path point with loop wrap). Then total = time[0] of record 0 + 10 and the sum of
 * time[1] over the ring / 30 is added to total and to the last section.
 *
 * func_800B930C is the locked kept get_next_checkpoint, defined here verbatim (one line, same text as
 * src/blob/func_800B930C.c, against a struct `Route` with the same offsets, as world_physics_tick.c does) BEFORE
 * physics_response: umerge inlines it. Placement matters: the same helper after physics_response gives 2 words,
 * in a separate file (byte-identical locked copy as group context) 16 words (as1 order of `move s4,v0` and the
 * `cur*80` address). In arcade game/checkpoint.c get_next_checkpoint and InitCPS are also in one file, above it;
 * retail 0x800B930C / 0x800B9338 / 0x800B93A8 are consecutive. Section records are addressed as
 * (&D_80151CE8)[i] (D_80151CE8 is record 0; its header fields live in it).
 *
 * SHAPING DEVICES (disclosed): two compiled-out `do {} while (0);` block ends (compiled-out debug hooks are the
 * likely original). Traced (w17h RESULTS.md): the one before `cur = next` keeps cur's definition out of the
 * block where the hoisted path-point pointer (v1) dies, so cur can share v1 as in retail (`move v1,s4` before
 * the `next == last` test); the one after `next = ...` adds one block to next's live range so the priorities
 * order as in retail (ctrace: &D_80151CE8 743/10 = 74.3 -> s3 above next 300/5 = 60 -> s4; without it next is
 * 300/4 = 75 and the two swap s3/s4, 22 words).
 * `path` is a local equal to 0 (w2a: a literal 0 changes what uopt hoists).
 */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct Route {
    /* 0x00 */ s16 total;     /* record 0: lap time */
    /* 0x02 */ s16 first;     /* record 0: section the ring wraps to */
    /* 0x04 */ s16 last;      /* record 0: last (finish) section */
    /* 0x06 */ s16 start;     /* record 0: first section of the race */
    /* 0x08 */ s16 count;     /* record 0: number of sections */
    /* 0x0A */ char padA[0x20];
    /* 0x2A */ s16 time[2];
    /* 0x2E */ s16 pad2E;
    /* 0x30 */ s16 pt_start[16];
} Route; /* 0x50 */

typedef struct {
    /* 0x00 */ s16 pos[3];
    /* 0x06 */ u8 speed;
    /* 0x07 */ u8 unk7;
} PathPt; /* 0x08 */


extern Route D_80151CE8;
typedef struct {
    /* 0x00 */ u16 num;
    /* 0x04 */ PathPt *pts;
} PathSet; /* 0x08 */

extern PathSet D_8012E5E8[];

s16 func_800B9338(s16, s16);
f32 sqrtf(f32);
#pragma intrinsic (sqrtf)

s32 func_800B930C(s32 index) {if(index+1 == D_80151CE8.count)return D_80151CE8.first;return index+1;}

void physics_response(void) {
    s32 p;
    s32 path;
    s32 cur;
    s32 next;
    s32 end;
    s32 i;
    s32 done;
    f32 d[3];
    f32 t;

    path = 0;
    for (p = 0; p < 2; p++) {
        done = (D_80151CE8.start == 0 || p != 0);
        if (p == 0) {
            cur = D_80151CE8.start;
        } else {
            cur = D_80151CE8.last;
        }
        for (;;) {
            next = func_800B930C(cur);
            do {} while (0);
            if (next < cur) {
                end = D_8012E5E8[path].num - 1;
            } else {
                end = (&D_80151CE8)[next].pt_start[path] - 1;
            }
            t = 0.0f;
            for (i = (&D_80151CE8)[cur].pt_start[path]; i < end; i++) {
                d[0] = D_8012E5E8[path].pts[i + 1].pos[0] - D_8012E5E8[path].pts[i].pos[0];
                d[1] = D_8012E5E8[path].pts[i + 1].pos[1] - D_8012E5E8[path].pts[i].pos[1];
                d[2] = D_8012E5E8[path].pts[i + 1].pos[2] - D_8012E5E8[path].pts[i].pos[2];
                t += sqrtf(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]) / ((u32) D_8012E5E8[path].pts[i].speed * 1.4666667f);
            }
            d[0] = D_8012E5E8[path].pts[func_800B9338(i, path)].pos[0] - D_8012E5E8[path].pts[i].pos[0];
            d[1] = D_8012E5E8[path].pts[func_800B9338(i, path)].pos[1] - D_8012E5E8[path].pts[i].pos[1];
            d[2] = D_8012E5E8[path].pts[func_800B9338(i, path)].pos[2] - D_8012E5E8[path].pts[i].pos[2];
            t += sqrtf(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]) / ((u32) D_8012E5E8[path].pts[i].speed * 1.4666667f);
            (&D_80151CE8)[cur].time[p] = t;
            do {} while (0);
            cur = next;
            if (next == D_80151CE8.last) {
                if (done) {
                    break;
                }
                done = 1;
            }
        }
    }
    D_80151CE8.total = D_80151CE8.time[0] + 10;
    cur = D_80151CE8.last;
    t = 0.0f;
    do {
        t += (&D_80151CE8)[cur].time[1];
        if (cur == D_80151CE8.count - 1) {
            i = D_80151CE8.first;
        } else {
            i = cur + 1;
        }
        cur = i;
    } while (cur != D_80151CE8.last);
    D_80151CE8.total = D_80151CE8.total + t / 30.0f;
    (&D_80151CE8)[D_80151CE8.last].time[1] = (&D_80151CE8)[D_80151CE8.last].time[1] + t / 30.0f;
}
