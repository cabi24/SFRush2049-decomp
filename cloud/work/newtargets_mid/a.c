typedef unsigned char u8;
typedef int s32;
typedef struct { u8 pad[71]; u8 mode; u8 pad2[5]; u8 b77; u8 pad3[19]; u8 b97; u8 pad4[15]; u8 b113; u8 pad5[19]; u8 b133; } Inner;
typedef struct { u8 pad[44]; Inner **inner; } Mid;
typedef struct { Mid *mid; } Outer;
u8 func_800CDE88(Outer *a0) {
    Inner *v = *a0->mid->inner;
    switch (v->mode) {
    case 4: return v->b113;
    case 5: return v->b133;
    case 6: return v->b97;
    }
    return v->b77;
}
