/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * track_texture_load (0x800F5F90, 1,348 B) = GC highscorescr.c `static void _InitScreen(void)` (high-score screen
 * init; w30r found it from the unlock script). Lane w31d.
 * Whole-program unit: EQUAL (337 words, frame 120/120) with `--internal track_texture_load --internal AudKillSong_TTL
 * --internal AudSetMusicVol_TTL --internal AudPlaySong_TTL --internal SaveFlush_TTL`; `score.py group` MATCH inside
 * frontier_billboard_render (its only caller billboard_render is there; IPA-internal: retail saves none of s0-s8).
 * Inlined helpers (non-static, force_internal, not kept; every inlined call reserves 8 B of frame here):
 *   - AudKillSong_TTL = the body of the locked -O2 single object_type1_create (GC AudKillSong; audio msg cmd 1),
 *     which retail inlines (msg kept in s0); `msg = func_80091B00(); msg->cmd = 1;` on one line (as1 then puts
 *     `move s0,v0` after the store, as retail) and defined AFTER track_texture_load (its .loc lines put `li a2,1`
 *     in the osRecvMesg delay slot, as retail).
 *   - AudSetMusicVol_TTL(vol, rate) -> speed_set(vol, rate, 1, 0): the same body as the LOCKED speed_mode0_wrapper
 *     (codex_vsync_a145; = GC AudSetMusicVol). Calling speed_mode0_wrapper itself is EQUAL in the unit too, but the
 *     group compile then schedules the 0.05f lwc1 into the jal delay slot (4 words); the in-file copy matches both.
 *   - AudPlaySong_TTL(song, loop) -> sync_entry_register(song, loop): HYPOTHESIS (GC AudPlaySong; the locked
 *     sync_entry_register is an -O2 single, retail calls it by jal through this 8-B inline level).
 * SHAPING DEVICES (disclosed): (1) SaveFlush_TTL(0): an EMPTY inlined helper after entity_audio_update (= GC
 *   ClearRushControls; GC then calls SaveServer(1, 0); SaveFlush(0), the N64 has no code there) - identity
 *   unknown, it supplies one 8-B inline area; (2) unused local `j`: exact frame residual (retail 120; without it
 *   112; 1 named slot above stuffUnlocked, +8 with alignment); (3) `(s8)D_80143CE8` in the circuitCode test:
 *   circuitCode/inited are declared u8 so their `= 1` stores share one constant web (retail s3) apart from the
 *   s32 1 arguments, while retail reads circuitCode with lb.
 * Other levers: memcmp (func_800A1910) size parameter u32 (separates the size constants 13/9/5/6 from the s32
 * multipliers kept in s5-s8); a separate counter `i` for the 6-entry ranks loop (IDO unrolls it 2 + 4).
 * GC-derived names: object_type1_create = AudKillSong, sync_entry_register = AudPlaySong, speed_mode0_wrapper =
 * AudSetMusicVol, D_801426A0.. = namePanes+0x90.. copies of gUnlockedTracks/Cars/Circuits/Trans/Engines/Tires/
 * Frames (D_80150DD8/E30/E88/EB8/ED8/F00/F40), track_lighting_setup = RecordGameOver, D_801148A4 = inited,
 * D_80143CE8 = circuitCode, D_801543D4 = gCircuitPlayer, D_80154628 = gCircuitTrack, D_80154640 = gCircuitSize,
 * player_array = gGameCar (+0xEE place, +0xEF finished, +0xF0 time), D_8014A160 = gPlayers[].+0x4C (76-B stride),
 * func_800F45F8 = UpdateCircuit(ranks, time), func_800F42C8 = GetCircuitCode(namePanes+0x190 = D_80143EF0),
 * net_state_validate = CalcUnlockedStuff, func_800A1910 = memcmp, D_80142694 = gStuffUnlocked, D_80143F18 =
 * gDrawName, D_80142528 = gThisTrack, func_800F2888 = _RankScores, entity_audio_update = ClearRushControls,
 * func_800F1114 = _UninitScreen, D_801461A8 = gSavedGhost, D_80146108 = gOptions (+0xC musicVol, +0xE song).
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct {
    u8 pad0[12];
    s8 musicVol;        /* +0x0C */
    u8 pad0D;
    s8 song;            /* +0x0E */
} Options;

typedef struct {
    u8 pad0[0xEE];
    s8 place;           /* +0xEE */
    s8 finished;        /* +0xEF */
    f32 time;           /* +0xF0 */
    u8 padF4[952 - 0xF4];
} GameCar;              /* 952 bytes */

typedef struct {
    s32 pad0[2];
    s32 f8;
} PlayerRec;

typedef struct {
    PlayerRec **rec;    /* gPlayers[i] + 0x4C */
    u8 pad[72];
} PlayerTail;           /* 76 bytes */

extern void *D_801461A8;            /* gSavedGhost */
extern Options D_80146108;          /* gOptions */
extern u8 D_801426A0[];
extern u8 D_80150DD8[];
extern u8 D_801426F0[];
extern u8 D_80150E30[];
extern u8 D_80142750[];
extern u8 D_80150E88[];
extern u8 D_80142780[];
extern u8 D_80150EB8[];
extern u8 D_80142AD8[];
extern u8 D_80150ED8[];
extern u8 D_80142D78[];
extern u8 D_80150F00[];
extern u8 D_80142DC0[];
extern u8 D_80150F40[];
extern s32 gHighScoreState;
extern u8 D_801148A4;               /* inited */
extern u8 D_80143CE8;               /* circuitCode */
extern s32 gameplay_mode;           /* gRace */
extern u8 D_801543D4;               /* gCircuitPlayer */
extern GameCar player_array[];      /* gGameCar */
extern s8 D_80154628;               /* gCircuitTrack */
extern PlayerTail D_8014A160[];
extern s8 D_80154640;               /* gCircuitSize */
extern u8 D_80143EF0[];
extern s16 active_player_count;     /* gNumPlayers */
extern s8 D_80142694[];             /* gStuffUnlocked */
extern s8 D_80143F18[];             /* gDrawName */
extern s8 D_80152570;               /* gBackwards */
extern s8 D_8014978C;               /* gTrackNum */
extern s16 D_80142528;              /* gThisTrack */

typedef struct { u32 opaque[6]; } OSMesgQueue;
typedef struct { u8 pad0[2]; s8 cmd; } AudMsg;
extern OSMesgQueue D_80142728;
extern OSMesgQueue D_801427A8;
extern s32 osRecvMesg(OSMesgQueue *, void **, s32);
extern s32 osSendMesg(OSMesgQueue *, void *, s32);
extern AudMsg *func_80091B00(void);

void AudKillSong_TTL(void);
extern void sync_entry_register(s32 song, s32 loop);
extern void speed_set(f32 blend, f32 amount, s32 first, s32 second);
extern void *memcpy(void *dst, const void *src, s32 n);
extern s32 track_lighting_setup();
extern void func_800F45F8(u8 *ranks, f32 time);
extern void func_800F42C8(u8 *code);
extern void net_state_validate(void);
extern s32 func_800A1910(void *a, void *b, u32 n);
extern void func_800F2888(void);
extern void entity_audio_update(void);
extern void SetHighScoreState(s32);
extern void func_800F1114(void);

/* GC AudPlaySong(song, loop): N64 wrapper over sync_entry_register (hypothesis) */
void AudPlaySong_TTL(s32 song, s32 loop)
{
    sync_entry_register(song, loop);
}

/* GC AudSetMusicVol(vol, rate): N64 wrapper over speed_set (hypothesis) */
void AudSetMusicVol_TTL(f32 vol, f32 rate)
{
    speed_set(vol, rate, 1, 0);
}

/* GC SaveFlush(0) after ClearRushControls: empty on the N64? (frame hypothesis) */
void SaveFlush_TTL(s32 x)
{
}

void track_texture_load(void)
{
    s32 p;
    s32 i;
    s32 j;
    s32 stuffUnlocked;
    s32 oldCircuitTrack;
    u8 ranks[6];

    stuffUnlocked = 0;
    D_801461A8 = 0;
    AudKillSong_TTL();
    if (D_80146108.song < 13) {
        AudPlaySong_TTL(5, 1);
        AudSetMusicVol_TTL(D_80146108.musicVol / 10.0f, 0.05f);
    }
    memcpy(D_801426A0, D_80150DD8, 76);
    memcpy(D_801426F0, D_80150E30, 52);
    memcpy(D_80142750, D_80150E88, 16);
    memcpy(D_80142780, D_80150EB8, 32);
    memcpy(D_80142AD8, D_80150ED8, 36);
    memcpy(D_80142D78, D_80150F00, 20);
    memcpy(D_80142DC0, D_80150F40, 24);
    track_lighting_setup();
    gHighScoreState = 0;
    D_801148A4 = 1;
    D_80143CE8 = 0;
    if (gameplay_mode == 3 && player_array[D_801543D4].finished) {
        for (i = 0; i < 6; i++) {
            ranks[i] = player_array[i].place;
        }
        oldCircuitTrack = D_80154628;
        func_800F45F8(ranks, player_array[D_801543D4].time);
        D_80154628 = oldCircuitTrack;
        if ((*D_8014A160[D_801543D4].rec)->f8 == 0 && D_80154628 < D_80154640 - 1) {
            D_80143CE8 = 1;
            func_800F42C8(D_80143EF0);
        }
    }
    net_state_validate();
    for (p = 0; p < active_player_count; p++) {
        D_80142694[p] = func_800A1910(&D_801426A0[p * 19], &D_80150DD8[p * 19], 19) != 0
                     || func_800A1910(&D_801426F0[p * 13], &D_80150E30[p * 13], 13) != 0
                     || func_800A1910(&D_80142750[p * 4], &D_80150E88[p * 4], 4) != 0
                     || func_800A1910(&D_80142780[p * 8], &D_80150EB8[p * 8], 8) != 0
                     || func_800A1910(&D_80142AD8[p * 9], &D_80150ED8[p * 9], 9) != 0
                     || func_800A1910(&D_80142D78[p * 5], &D_80150F00[p * 5], 5) != 0
                     || func_800A1910(&D_80142DC0[p * 6], &D_80150F40[p * 6], 6) != 0;
        if (D_80142694[p]) {
            stuffUnlocked = 1;
        }
        D_80143F18[p] = 0;
    }
    D_80142528 = (D_80152570 ? 6 : 0) + D_8014978C;
    if (gameplay_mode == 0 || gameplay_mode == 2 || gameplay_mode == 3) {
        func_800F2888();
    }
    entity_audio_update();
    SaveFlush_TTL(0);
    if (stuffUnlocked) {
        SetHighScoreState(1);
    } else if ((s8)D_80143CE8) {
        SetHighScoreState(2);
    } else if (gameplay_mode == 0 || gameplay_mode == 3 || gameplay_mode == 2) {
        SetHighScoreState(3);
    } else {
        func_800F1114();
    }
}

/* = object_type1_create (GC AudKillSong), inlined by umerge in retail (its locked copy is an -O2 single) */
void AudKillSong_TTL(void)
{
    AudMsg *msg;

    osRecvMesg(&D_80142728, (void **)0, 1);
    msg = func_80091B00(); msg->cmd = 1;
    osSendMesg(&D_80142728, (void *)0, 0);
    osSendMesg(&D_801427A8, msg, 0);
}
