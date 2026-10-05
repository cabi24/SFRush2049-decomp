/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct {
    /* 0x00 */ s16 total;
    /* 0x02 */ s16 loop;
    /* 0x04 */ s16 last;
    /* 0x06 */ s16 first;
    /* 0x08 */ s16 num;
    /* 0x0A */ char padA[0x20];
    /* 0x2A */ s16 time[2];
    /* 0x2E */ s16 pad2E;
    /* 0x30 */ s16 start[16];
} Sect; /* 0x50 */

typedef struct {
    /* 0x00 */ s16 pos[3];
    /* 0x06 */ u8 speed;
    /* 0x07 */ u8 unk7;
} PathPt; /* 0x08 */


extern Sect D_80151CE8[];
typedef struct {
    /* 0x00 */ u16 num;
    /* 0x04 */ PathPt *pts;
} PathSet; /* 0x08 */

extern PathSet D_8012E5E8[];

s16 func_800B9338(s16, s16);
f32 sqrtf(f32);
#pragma intrinsic (sqrtf)

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
        done = (D_80151CE8->first == 0 || p != 0);
        if (p == 0) {
            cur = D_80151CE8->first;
        } else {
            cur = D_80151CE8->last;
        }
        for (;;) {
            if (cur + 1 == D_80151CE8->num) {
                i = D_80151CE8->loop;
            } else {
                i = cur + 1;
            }
            next = i;
            if (i < cur) {
                end = D_8012E5E8[path].num - 1;
            } else {
                end = D_80151CE8[i].start[path] - 1;
            }
            t = 0.0f;
            for (i = D_80151CE8[cur].start[path]; i < end; i++) {
                d[0] = D_8012E5E8[path].pts[i + 1].pos[0] - D_8012E5E8[path].pts[i].pos[0];
                d[1] = D_8012E5E8[path].pts[i + 1].pos[1] - D_8012E5E8[path].pts[i].pos[1];
                d[2] = D_8012E5E8[path].pts[i + 1].pos[2] - D_8012E5E8[path].pts[i].pos[2];
                t += sqrtf(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]) / ((u32) D_8012E5E8[path].pts[i].speed * 1.4666667f);
            }
            d[0] = D_8012E5E8[path].pts[func_800B9338(i, path)].pos[0] - D_8012E5E8[path].pts[i].pos[0];
            d[1] = D_8012E5E8[path].pts[func_800B9338(i, path)].pos[1] - D_8012E5E8[path].pts[i].pos[1];
            d[2] = D_8012E5E8[path].pts[func_800B9338(i, path)].pos[2] - D_8012E5E8[path].pts[i].pos[2];
            t += sqrtf(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]) / ((u32) D_8012E5E8[path].pts[i].speed * 1.4666667f);
            D_80151CE8[cur].time[p] = t;
            cur = next;
            if (next == D_80151CE8->last) {
                if (done) {
                    break;
                }
                done = 1;
            }
        }
    }
    D_80151CE8->total = D_80151CE8->time[0] + 10;
    cur = D_80151CE8->last;
    t = 0.0f;
    do {
        t += D_80151CE8[cur].time[1];
        if (cur == D_80151CE8->num - 1) {
            i = D_80151CE8->loop;
        } else {
            i = cur + 1;
        }
        cur = i;
    } while (cur != D_80151CE8->last);
    D_80151CE8->total = D_80151CE8->total + t / 30.0f;
    D_80151CE8[D_80151CE8->last].time[1] = D_80151CE8[D_80151CE8->last].time[1] + t / 30.0f;
}
