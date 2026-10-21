/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * ghost_race_setup = GhostEnd@ghost.c (MAT3 GameSource/src/ghost.c GhostEnd, GC 0x8001A1E0, 260 B; DWARF locals
 * `g` then `i`, block-local `ghost`).  N64 440 B.  Base: Astra's research PR #409
 * (dot_mat3_ghostend_20261017: historical_typed.c 16/110, layouts Ghost44/Header88 from GhostStart).
 * N64: gGhostToSave D_80140800, gRace D_8014A110 (kGhost 2), gEndingGhosts D_80114738 (volatile, inherited
 * from #409), gNumPlayers D_8014A108, gGhosts D_80152698, model[i].fastin.modelrun D_8014A250[i] +1820 (stride
 * 2056), gGameCar D_80152818 (stride 952; place_locked +239, score +240), PurgeGhostData = menu_transition,
 * DeallocateMemory = func_800960CC (the unit's real definition: heap lock, audio_reverb_update(p, 0), unlock;
 * calling it inlines with its parameter copy `lw s0,76(s2)` ahead of the lock, as retail).
 * Levers (w36w2): gGameCar[i] indexed directly (a pointer local loses ugen's .noalias region whose `.alias`
 * close after the ready-branch `b` keeps as1 from hoisting the free path into the lb stall: 53 -> 3);
 * disclosed compiled-out `do {} while (0);` after the place_locked if/else = the join block where retail's
 * single gNumPlayers reload sits (without it each arm gets its own reload: 111-112 words).
 * Residual 3 words: retail `move a0,v0; sw zero,0(s4); ... lw t7,0(a0)`; ours `sw zero; move a0,v0; ...
 * lw t7,0(v0)` = as1 copy propagation of the ghost copy into the header load.  Listing oracle (v/o7.s):
 * emitting the gGameCar[i] address (mul/addu into $2) between the modelrun store and the header load
 * blocks the copy propagation and gives 0 rows; no source form found that makes uopt place it there.
 */
typedef signed char s8; typedef unsigned char u8; typedef signed short s16;
typedef signed int s32; typedef unsigned int u32; typedef float f32;
typedef struct OSMesgQueue OSMesgQueue;
extern int osRecvMesg(OSMesgQueue *, void **, int);
extern int osSendMesg(OSMesgQueue *, void *, int);
extern OSMesgQueue D_80152770;
extern void audio_reverb_update(u32 address, s32 tag);
typedef struct GhostHeader {      /* GC __anon_49776; N64 layout 0x58 */
    u32 headerChecksum;           /* 0x00 */
    u8 revision;                  /* 0x04 */
    s8 flag;                      /* 0x05 */
    s8 track;                     /* 0x06 */
    u8 backward;                  /* 0x07 */
    u8 car;                       /* 0x08 GC link car */
    u8 carTrans;                  /* 0x09 */
    s8 settings[12];              /* 0x0A handling, engine, tires, frame, wings, link 2/3/4, rims, horn, front, rear */
    u8 name[13];                  /* 0x16 */
    u8 pad23[0x28 - 0x23];
    u32 playerId[2];              /* 0x28 */
    u32 bestTime;                 /* 0x30 */
    f32 frameTime;                /* 0x34 GC 0x30 gRetraceTime */
    f32 finish;                   /* 0x38 */
    u32 zippedSize;               /* 0x3C */
    u32 unzippedSize;             /* 0x40 */
    u32 zippedChecksum;           /* 0x44 */
    u32 unzippedChecksum;         /* 0x48 */
    u8 *start;                    /* 0x4C */
    u32 offset;                   /* 0x50 */
    s32 size;                     /* 0x54 */
} GhostHeader;

typedef struct Ghost {            /* GC __anon_49a9c; N64 layout 0x2C */
    void *ll;                     /* 0x00 */
    void **fileh;                 /* 0x04 */
    s8 track;                     /* 0x08 */
    u8 backward;                  /* 0x09 */
    u8 name[13];                  /* 0x0A */
    u8 pad17;
    u32 playerId[2];              /* 0x18 */
    f32 finish;                   /* 0x20 */
    u8 **datah;                   /* 0x24 */
    GhostHeader **header;         /* 0x28 */
} Ghost;

typedef struct GhostPlayer { u8 pad0[239]; s8 ready; f32 value; u8 tail[708]; } GhostPlayer;
typedef struct GhostObject { u8 pad0[1820]; s16 reset; u8 tail[234]; } GhostObject;
extern Ghost **D_80152698[];
extern GhostPlayer D_80152818[];
extern GhostObject D_8014A250[];
extern GhostHeader *D_80140800;
extern s16 D_8014A108;
extern s32 D_8014A110;
extern volatile s8 D_80114738;
extern void menu_transition(Ghost **handle);
void func_800960CC(u32 address);

void ghost_race_setup(void)
{
    GhostHeader *g;
    s32 i;

    D_80140800 = 0;
    if (D_8014A110 != 2) {
        return;
    }
    D_80114738 = 1;
    for (i = 0; i < D_8014A108; i++) {
        if (D_80152698[i] != 0) {
            Ghost **ghost = D_80152698[i]; D_80152698[i] = 0;
            D_8014A250[i].reset = 0;
            g = *(*ghost)->header;
            if (g->flag >= 0) {
                if (g->flag > 0) {
                    menu_transition(ghost);
                }
            } else {
                g->flag = 0;
                if (D_80152818[i].ready != 0) {
                    g->finish = D_80152818[i].value;
                    g->size = g->offset;
                    D_80140800 = g;
                } else {
                    func_800960CC((u32)g->start);
                    g->start = 0;
                }
                do {} while (0);
            }
        }
    }
    D_8014A108 = 1;
    D_80114738 = 0;
}
