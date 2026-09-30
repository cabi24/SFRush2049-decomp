typedef unsigned char u8;
typedef int s32;
typedef struct { u8 pad[71]; u8 mode; u8 pad2[5]; u8 b77; u8 pad3[19]; u8 b97; u8 pad4[15]; u8 b113; u8 pad5[19]; u8 b133; } Inner;
typedef struct { u8 pad[44]; Inner **inner; } Mid;
typedef struct { Mid *mid; } Outer;
u8 func_800CDE88(Outer *a0) {
    Inner *v = *a0->mid->inner;
    u8 r;
    if (v->mode == 4) r = v->b113;
    else if (v->mode == 5) r = v->b133;
    else if (v->mode == 6) r = v->b97;
    else r = v->b77;
    return r;
}
