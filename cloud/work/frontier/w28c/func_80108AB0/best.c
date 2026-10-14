/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_80108AB0 (760 B) DRAFT = GC hud.c animWrongWay by behaviour (gRace D_8014A110, wasStrayed static
 * D_8013F2F8, wrongWayTime D_801543B8, gSeconds D_8002EB90, wrongWayPos D_80115F28, TB_SetJust = D_80118E20
 * pair, HAL_SetTextTranslucency = func_800ED66C, TB_GetFlashTextCycle = D_8017A630). Unit 184/190, ops 46, frame
 * 200/128; EQUIVALENT 4,832 calls. Residual: ours hoists the FP constants into $f20-$f26, retail reloads them
 * each iteration; retail frame has 72 B more (unknown). gc_named.c is the GC-structured variant (frame 208, ops
 * 60).
 */
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
    s32 a, b;
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
extern s16 gNumWindows;
extern Car D_80152818[];
extern f32 D_801543B8[];
extern s8 D_8013F2F8[];
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

s32 animWrongWay(void *item)
{
    s32 i;
    s32 x;
    s32 y;
    s32 previous;

    if (D_8014A110 == 5) {
        return 1;
    }
    render_helper(0.0f);
    D_80118E20.a = 1;
    D_80118E20.b = 1;
    for (i = 0; i < gNumWindows; i++) {
        if ((-1.5707964f <= D_80152818[i].heading && D_80152818[i].heading <= 1.5707964f) ||
            !(D_801174B4 & 0x400000) || D_80152818[i].f239 == 1 || !D_801613A0 || D_8014A110 == 1) {
            D_8013F2F8[i] = 0;
            D_801543B8[i] = D_8002EB90;
            continue;
        }
        D_8013F2F8[i] = 1;
        if (D_8002EB90 - D_801543B8[i] < 2.0f) {
            continue;
        }
        if (gNumWindows == 1) {
            osRecvMesg(&D_801461D0, (void *)0, 1);
            previous = slot_state_setup(10);
            osSendMesg(&D_801461D0, (void *)0, 0);
        } else {
            osRecvMesg(&D_801461D0, (void *)0, 1);
            previous = slot_state_setup(11);
            osSendMesg(&D_801461D0, (void *)0, 0);
        }
        func_800ED66C(D_8017A630 * 255.0f);
        x = D_80115F28[gNumWindows - 1][i].x;
        y = D_80115F28[gNumWindows - 1][i].y;
        dispatch_handler(0);
        state_utility(x + 1, y + 1, D_8017A4E0.hdr->text);
        dispatch_handler(14);
        state_utility(x, y, D_8017A4E0.hdr->text);
    }
    D_80118E20.b = 3;
    D_80118E20.a = 0;
    func_800ED66C(-1.0f);
    render_helper(-1.0f);
    return 1;
}
