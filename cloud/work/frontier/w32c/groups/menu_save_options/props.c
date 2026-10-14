/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* props_render (0x800F6AB8, 545 words) = GC ghost.c GhostStart, N64 form (w32c, wave 32; label is historical:
 * the function sets up the ghost players at race start). Fresh draft from the GC disassembly
 * (reference/gc/disasm/ghost.c/GhostStart.s) + the N64 words. EQUAL in the whole-program unit
 * (blob_unit --tag w32c score props_render --with THIS --internal ReadGhostData --neighbours: 0 locked differ).
 * GC-derived names (source GhostStart unless noted): gRace = gameplay_mode, gInited = D_80114734,
 *   gRestartGame = D_80114650, gNumCars = D_801543CA (volatile, as in func_80108DA8/func_800EC190),
 *   gNumPlayers = active_player_count, num_humans = D_80153FD2 (volatile), gGhostPlayers = D_801409F0
 *   (player records D_80140A18, 0x30 each) / D_80142500 (header pointers to D_80142530, 0x58 each) /
 *   D_80140AF8 (ghost slots to D_80140B28 ghosts, 0x2C each), gGhosts = D_80152698, gPlayers = input_rec0
 *   (0x4C each: node 0, slot 1, carTrans 0x42, handle 0x48), gLink = D_80153E88 (8 B each), model = D_8014A250
 *   (0x808 each: frameTime 0x718, state 0x7CC), gTrackNum = D_8014978C, gBackwards = D_80152570, gRetraceTime =
 *   D_8002AFB8, gRetraceRate = D_8002AFB4, ghostRaceTimes = D_80111754, gCarHandling/Engines/Tires/Frame/Wings/
 *   TireRims/Horn = D_8011103C/D_80111080/D_8011119C/D_80111230/D_8011128C/D_8011157C/D_801115C0 ([5][13] s8),
 *   gFrontTireScale/gRearTireScale = D_801112DC/D_801113E0 ([5][13] f32), GetPlayerCarTrans = func_800CDDE8,
 *   AllocateMemoryHeap = audio_dma_sync, _ReadGhostData = func_800CBF2C (IPA ghosth s1, decode s0, select s3).
 * N64 differences from GC: the slot count n = D_80156994 ? 4 : 2 (GC 4); the ghost header's best time comes
 *   from the player data's TrackRec table at +0xE0 (96 B per entry, +6 entries when backwards); gLink has three
 *   more header bytes (settings[5..7]); the per-player car settings are 12 bytes from data + 0x744 + car*16.
 * Helper: ReadGhostData = GC save.c ReadGhostData(ghosth) { return _ReadGhostData(ghosth, 1, 1); } (GC passes
 *   the same 1, 1), non-static, inlined, not on keep (its parameter is retail's v0 copy before `move s1,v0`).
 * Source-form evidence (no devices, no unused locals): `bk`/`size` are named locals (retail frame 184 and the
 *   named-slot offsets n 160 / np 168 need g, i, j, np, car, n, ghost, bk, size in this order); the car-settings
 *   source is indexed as CarCfg[car] so ugen evaluates the data chain before car*16; the final chained store
 *   `D_80153FD2 = active_player_count = np` sets the temp-ring registers of the tail. */
typedef signed char s8;typedef unsigned char u8;typedef short s16;typedef unsigned short u16;
typedef signed int s32;typedef unsigned int u32;typedef float f32;
#define NULL ((void *)0)

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

typedef struct PlayerRec {        /* GC player record (0x4C); N64 0x30 */
    u8 pad0[12];
    u32 playerId[2];              /* 0x0C */
    u8 name[13];                  /* 0x14 */
    u8 pad21[44 - 33];
    u8 **datah;                   /* 0x2C */
} PlayerRec;

typedef struct Player {           /* GC gPlayers (0x50); N64 0x4C */
    u8 node;                      /* 0x00 */
    u8 slot;                      /* 0x01 */
    u8 pad2[66 - 2];
    u8 carTrans;                  /* 0x42 */
    u8 pad43[72 - 67];
    PlayerRec **handle;           /* 0x48 */
} Player;

typedef struct Link {             /* GC gLink (0x0A); N64 0x08 */
    s8 track;
    u8 car;
    s8 b2, b3, b4;
    u8 np;
    u8 b6, b7;
} Link;

typedef struct Model { u8 pad[0x718]; f32 frameTime; u8 pad71C[0x7CC - 0x71C]; u8 state; u8 pad7CD[0x808 - 0x7CD]; } Model;
typedef struct CarCfg { u8 s[16]; } CarCfg;
typedef struct TrackRec { u16 time; u8 pad[94]; } TrackRec;

void *memset(void *, s32, u32);
void *memcpy(void *, const void *, u32);
u8 func_800CDDE8(PlayerRec **handle);                       /* GC GetPlayerCarTrans */
void *audio_dma_sync(void *heap, u32 size);                 /* GC AllocateMemoryHeap */
s32 func_800CBF2C(Ghost **ghosth, s32 decode, s32 select);  /* GC _ReadGhostData (IPA s1, s0, s3) */
s32 ReadGhostData(Ghost **ghosth) {                        /* GC save.c ReadGhostData */
    return func_800CBF2C(ghosth, 1, 1);
}

extern s8 D_80156994;
extern s32 gameplay_mode;                                   /* GC gRace */
extern s8 D_80114734;                                       /* GC gInited */
extern s8 D_80114650;                                       /* GC gRestartGame */
extern volatile s16 D_801543CA;                                      /* GC gNumCars */
extern s16 active_player_count;                             /* GC gNumPlayers */
extern volatile s16 D_80153FD2;                                      /* GC num_humans */
extern PlayerRec *D_801409F0[];                             /* GC gGhostPlayers[0..3] */
extern PlayerRec D_80140A18[];
extern GhostHeader *D_80142500[];                           /* GC gGhostPlayers + 0x140 */
extern GhostHeader D_80142530[];
extern Ghost *D_80140AF8[];                                 /* GC gGhostPlayers + 0x360 */
extern Ghost D_80140B28[];                                  /* GC gGhostPlayers + 0x2A0 */
extern Ghost **D_80152698[];                                /* GC gGhosts */
extern Player input_rec0[];                                 /* GC gPlayers */
extern Link D_80153E88[];                                   /* GC gLink */
extern Model D_8014A250[];                                  /* GC model */
extern s8 D_8014978C;                                       /* GC gTrackNum */
extern s8 D_80152570;                                       /* GC gBackwards */
extern f32 D_8002AFB8;                                      /* GC gRetraceTime */
extern f32 D_8002AFB4;                                      /* GC gRetraceRate */
extern f32 D_80111754[];                                    /* GC ghostRaceTimes */
extern s8 D_8011103C[][13];                                 /* GC gCarHandling */
extern s8 D_80111080[][13];                                 /* GC gCarEngines */
extern s8 D_8011119C[][13];                                 /* GC gCarTires */
extern s8 D_80111230[][13];                                 /* GC gCarFrame */
extern s8 D_8011128C[][13];                                 /* GC gCarWings */
extern s8 D_8011157C[][13];                                 /* GC gTireRims */
extern s8 D_801115C0[][13];                                 /* GC gCarHorn */
extern f32 D_801112DC[][13];                                /* GC gFrontTireScale */
extern f32 D_801113E0[][13];                                /* GC gRearTireScale */

void props_render(void) {
    GhostHeader *g;
    s32 i;
    s32 j;
    s32 np;
    s32 car;
    s32 n;
    Ghost *ghost;
    s32 bk;
    s32 size;

    n = D_80156994 ? 4 : 2;
    if (gameplay_mode != 2) {
        return;
    }
    if (!D_80114734) {
        for (i = 0; i < n; i++) {
            D_801409F0[i] = &D_80140A18[i];
            D_80142500[i] = &D_80142530[i];
        }
        D_80114734 = 1;
    }
    if (D_80114650) {
        D_801543CA = 1;
        active_player_count = 1;
    }
    np = active_player_count;
    for (i = 0; i < active_player_count; i++) {
        D_80140AF8[i] = &D_80140B28[i];
        D_80152698[i] = &D_80140AF8[i];
        g = D_80142500[i];
        D_80140B28[i].header = &D_80142500[i];
        g->flag = -1;
        bk = D_80152570 ? 6 : 0;
        g->bestTime = ((TrackRec *)(*(*input_rec0[i].handle)->datah + 224) + bk)[D_8014978C].time;
        g->track = D_8014978C;
        g->backward = D_80152570;
        memcpy(g->name, (*input_rec0[i].handle)->name, 13);
        memcpy(g->playerId, (*input_rec0[i].handle)->playerId, 8);
        g->carTrans = func_800CDDE8(input_rec0[i].handle);
        memcpy(g->settings, ((CarCfg *)(*(*input_rec0[i].handle)->datah + 1860))[D_80153E88[i].car].s, 12);
        g->car = D_80153E88[i].car;
        g->frameTime = D_8002AFB8;
        if (!D_80114650) {
            g->size = (f32)(D_80111754[D_8014978C] + 10.0) * D_8002AFB4;
            g->start = audio_dma_sync(NULL, g->size * 3);
        }
        g->offset = 0;
    }
    for (; i < n; i++) {
        if (D_80152698[i] == NULL) {
            break;
        }
        if (ReadGhostData(D_80152698[i]) == 0) {
            for (j = i; j < 3; j++) {
                D_80152698[j] = D_80152698[j + 1];
            }
            continue;
        }
        ghost = *D_80152698[i];
        (*ghost->header)->start = *ghost->datah;
        g = *(*D_80152698[i])->header;
        g->offset = 0;
        car = g->car;
        memset(&input_rec0[i], 0, 76);
        input_rec0[i].handle = &D_801409F0[i];
        memset(D_801409F0[i], 0, 48);
        memcpy((*input_rec0[i].handle)->name, g->name, 13);
        input_rec0[i].node = i;
        input_rec0[i].slot = i;
        input_rec0[i].carTrans = g->carTrans;
        D_8011103C[i + 1][car] = g->settings[0];
        D_80111080[i + 1][car] = g->settings[1];
        D_8011119C[i + 1][car] = g->settings[2];
        D_80111230[i + 1][car] = g->settings[3];
        D_8011128C[i + 1][car] = g->settings[4];
        D_8011157C[i + 1][car] = g->settings[8];
        D_801115C0[i + 1][car] = g->settings[9];
        D_801112DC[i + 1][car] = g->settings[10] / 100.0f + 0.75f;
        D_801113E0[i + 1][car] = g->settings[11] / 100.0f + 0.75f;
        D_80153E88[np].track = g->track;
        D_80153E88[np].car = car;
        D_80153E88[np].b2 = g->settings[5];
        D_80153E88[np].b3 = g->settings[6];
        D_80153E88[np].b4 = g->settings[7];
        D_80153E88[np].np = np;
        D_80153E88[np].b6 = 176;
        D_80153E88[np].b7 = 6;
        D_8014A250[np].state = 2;
        D_8014A250[np].frameTime = g->frameTime;
        np++;
        D_801543CA = np;
    }
    D_80153FD2 = active_player_count = np;
}
