typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32;

typedef struct {
    void *p0;   /* 0x00 */
    void *p4;   /* 0x04 */
    s16 id;     /* 0x08 */
    s16 a;      /* 0x0A */
    s16 b;      /* 0x0C */
    s16 e;      /* 0x0E */
    s16 w;      /* 0x10 */
    s16 h;      /* 0x12 */
    u8 alpha;   /* 0x14 */
    u8 f15;     /* 0x15 */
    s8 state;   /* 0x16 */
    s8 pad17;
    s16 x18;    /* 0x18 */
    s16 x1A;    /* 0x1A */
    s16 hm1;    /* 0x1C */
    s16 wm1;    /* 0x1E */
} Slot;

extern Slot D_80140BF0[];
extern s32 D_801613AC;
extern s32 D_8013C234;

s32 func_800A79F4(s32 id, void *p4, void *p0, s32 a, s32 b, s32 w, s32 h)
{
    Slot *s;
    s32 i;
    s32 n;

    n = D_801613AC;
    for (i = 0; i < n; i++) {
        if (D_80140BF0[i].state == 2) {
            break;
        }
    }
    if (i >= 200) {
        return -1;
    }
    if (i >= n) {
        D_801613AC = ++n;
    }
    if (D_8013C234 < n) {
        D_8013C234 = n;
    }
    D_80140BF0[i].p4 = p4;
    D_80140BF0[i].p0 = p0;
    D_80140BF0[i].id = id;
    D_80140BF0[i].a = a;
    D_80140BF0[i].e = 0;
    D_80140BF0[i].alpha = 255;
    D_80140BF0[i].x18 = 0;
    D_80140BF0[i].x1A = 0;
    D_80140BF0[i].hm1 = h - 1;
    D_80140BF0[i].wm1 = w - 1;
    D_80140BF0[i].state = 0;
    D_80140BF0[i].f15 = 0;
    D_80140BF0[i].w = w;
    D_80140BF0[i].h = h;
    D_80140BF0[i].b = b;
    return i;
}
