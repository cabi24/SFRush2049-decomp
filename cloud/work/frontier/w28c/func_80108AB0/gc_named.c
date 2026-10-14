/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct { u32 opaque[6]; } OSMesgQueue;

typedef struct TblHdr {
    u8 pad0[0x68];
    char *text;
} TblHdr;

typedef struct StrTable {
    u8 pad0[4];
    TblHdr *hdr;
} StrTable;

typedef struct Pos {
    s32 x, y;
} Pos;

typedef struct Pair {
    s32 h, v;
} Pair;

typedef struct Car {
    u8 pad0[239];
    s8 f239;
    u8 padF0[868 - 240];
    f32 heading;
    u8 pad368[952 - 872];
} Car;

extern OSMesgQueue D_801461D0;
extern u32 D_801174B4;
extern s32 D_8014A110;
extern s8 D_801613A0;
extern Pair D_80118E20;
extern StrTable D_8017A4E0;
extern s16 D_80151AD0;
extern Car D_80152818[];
extern f32 D_801543B8[];
extern f32 D_8002EB90;
extern f32 D_8017A630;
extern Pos D_80115F28[][4];

s32 osRecvMesg(OSMesgQueue *, void **, s32);
s32 osSendMesg(OSMesgQueue *, void *, s32);
s32 slot_state_setup(s32 selection);
void render_helper(f32);
void func_800ED66C(f32);
void dispatch_handler(s32);
void state_utility(s16, s16, void *);

static void gfx_lock(void)
{
    osRecvMesg(&D_801461D0, (void *)0, 1);
}

static void gfx_unlock(void)
{
    osSendMesg(&D_801461D0, (void *)0, 0);
}

static s32 font_set(s32 font)
{
    s32 old;

    gfx_lock();
    old = slot_state_setup(font);
    gfx_unlock();
    return old;
}

static void TB_SetJust(s32 h, s32 v)
{
    D_80118E20.h = h;
    D_80118E20.v = v;
}

static f32 TB_GetFlashTextCycle(void)
{
    return D_8017A630;
}

s32 animWrongWay(void *item)
{
    s32 x;
    s32 y;
    s32 slot;
    static s8 D_8013F2F8[4];

    if (D_8014A110 == 5) {
        return 1;
    }
    render_helper(0.0f);
    TB_SetJust(1, 1);
    for (slot = 0; slot < D_80151AD0; slot++) {
        if ((-1.5707964f <= D_80152818[slot].heading && D_80152818[slot].heading <= 1.5707964f) ||
            !(D_801174B4 & 0x400000) || D_80152818[slot].f239 == 1 || !D_801613A0 || D_8014A110 == 1) {
            D_8013F2F8[slot] = 0;
            D_801543B8[slot] = D_8002EB90;
            continue;
        }
        D_8013F2F8[slot] = 1;
        if (D_8002EB90 - D_801543B8[slot] < 2.0f) {
            continue;
        }
        if (D_80151AD0 == 1) {
            font_set(10);
        } else {
            font_set(11);
        }
        func_800ED66C(255.0f * TB_GetFlashTextCycle());
        x = D_80115F28[D_80151AD0 - 1][slot].x;
        y = D_80115F28[D_80151AD0 - 1][slot].y;
        dispatch_handler(0);
        state_utility(x + 1, y + 1, D_8017A4E0.hdr->text);
        dispatch_handler(14);
        state_utility(x, y, D_8017A4E0.hdr->text);
    }
    TB_SetJust(0, 3);
    func_800ED66C(-1.0f);
    render_helper(-1.0f);
    return 1;
}
