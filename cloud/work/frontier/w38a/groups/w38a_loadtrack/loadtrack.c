/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * LoadTrack (0x800BB9B0, 2,104 B) = MAT3/GC/DC tracks.c LoadTrack(track, background, reloading), N64 version:
 *   load + wait on the track / world / collision / path waves (audio_frame_sync = LoadWave, player_state_get =
 *   WaveInstanceReady, slot_value_get = GetWaveInstanceAddr; N64 wave numbers track+101/+120/+139, path 158/177),
 *   world wad setup (func_80096C28 = MBSetupWad, lookup_with_output = MBGetFromWad: WDHR / GDAT / GTLD), the
 *   "other" waves 61..81 by race/track/window count, then once: initTargetStructs (camera_lerp_position), SkyInit
 *   (camera_reset), NewWorld (wheel_torque_apply, locked, inlined), InitEnvs, InitTrackDynamicObjs, LoadCars,
 *   InitVisuals, mode_flags_clear (locked: clears D_80110680[0..1] and calls InitCheckpointData; inlined),
 *   InitPathData, InitDemoPathPoint, InitSkids, InitSmoke, InitBlast and the image-B call func_8038A8CC (GC
 *   initWorldDamage) when D_80156994 or track >= 6.
 * Base: w29d/w30d/w31d draft (39/526 words). w38a levers (39 -> 0):
 *   - the Dreamcast tracks.c keeps three empty gGameState & 8 checks after the gRace tests (`if (x & 8) {} else
 *     if (x & 8) {}` + `if (x & 8) {}`); they compile to nothing but supply the missing t6-t9 ring pop
 *     (workbench L65 class) -> 12 words;
 *   - NewWorld is the locked wheel_torque_apply (its out-parameter store gives retail's separate lui at / lui a0
 *     WorldBase store and reload) and the D_80110680 clear is the locked mode_flags_clear, both called and
 *     umerge-inlined, instead of the draft's private NewWorld/FindWorld/clear copies -> 0;
 *   - `track >= 6` for the final condition (uopt turns it into retail's &gWorldNames[track] compare).
 * No shaping devices: the empty checks are the Dreamcast decomp's source, every helper is a real locked function.
 */
typedef signed char s8;typedef unsigned char u8;typedef short s16;typedef unsigned short u16;
typedef signed int s32;typedef unsigned int u32;typedef float f32;
#define NULL ((void *)0)

typedef struct LoadState {     /* GC tracks.c __anon_10faf9 */
    s8 loading;                /* 0x00 */
    s8 loaded;                 /* 0x01 */
    s8 inited;                 /* 0x02 */
    s8 ready;                  /* 0x03 */
    s32 idx;                   /* 0x04 */
} LoadState;
typedef struct TrackLoadInfo { /* GC tracks.c __anon_10fbbb (gTrackLoadInfo), 0x2C */
    s8 trackInited;            /* 0x00 */
    LoadState track;           /* 0x04 */
    LoadState world;           /* 0x0C */
    LoadState collision;       /* 0x14 */
    LoadState path;            /* 0x1C */
    LoadState other;           /* 0x24 */
} TrackLoadInfo;
typedef struct WorldHdr {      /* GC world.c __anon_10fd25 */
    u32 woff;                  /* 0x00 */
    char wname[16];            /* 0x04 */
} WorldHdr;
typedef struct Wad {           /* GC mb_wad.c __anon_87656 */
    u32 *base;
    s32 TOCentries;
    void *TOC;
} Wad;

extern TrackLoadInfo D_80143A10;   /* gTrackLoadInfo */
extern s32 *D_801497FC;            /* gWorldData */
extern WorldHdr *D_80149818;       /* WorldTable */
extern void *D_80149770;           /* gTargetGroups */
extern s32 D_80149790;             /* numgTargetGroups */
extern void *D_801497C0;           /* gTargetGroupList */
extern s32 D_801497EC;             /* numgTargetGroupList */
extern void *D_801526DC;           /* gRoadDat */
extern void *D_80153F20;           /* gPathWave */
extern s8 D_80152570;              /* gBackwards */
extern u32 D_801174B4;             /* gGameState */
extern s32 D_8014A110;             /* gRace */
extern s8 D_80156994;
extern s8 D_8014978C;              /* gTrackNum */
extern s16 gNumWindows;             /* gNumWindows */
extern char *D_8011A924[];         /* gWorldNames (N64: indexed by track) */

s32 audio_frame_sync(s32 wave, s32 a1, s32 background, s32 a3, s32 a4);   /* GC LoadWave */
s8 player_state_get(s32 idx);                                            /* GC WaveInstanceReady */
void *slot_value_get(s32 idx);                                           /* GC GetWaveInstanceAddr */
void func_80096C28(Wad *wad, void *ptr);                                 /* GC MBSetupWad */
void *lookup_with_output(Wad *wad, s32 tag, s32 *count);                 /* GC MBGetFromWad */
void camera_lerp_position(s16 track);                                    /* GC initTargetStructs */
void camera_reset(void);                                                 /* GC SkyInit */
void func_800B2BDC(void);                                                /* GC InitEnvs */
void tire_sound_update(void);                                            /* GC InitTrackDynamicObjs */
void audio_interrupt_handler(s32 x);                                     /* GC LoadCars */
void sfx_position_3d(s32 x);                                             /* GC InitVisuals */
void audio_effect_remove(void);                                          /* GC InitPathData */
void func_800B9284(void);                                                /* GC InitDemoPathPoint */
void func_800B0580(void);                                                /* GC InitSkids */
void physics_collision_test(void);                                       /* GC InitSmoke */
void camera_smooth_lerp(void);                                           /* GC InitBlast */
void func_8038A8CC(void);                                                /* overlay call (GC initWorldDamage?) */

s16 wheel_torque_apply(u8 *World, s16 Parent);                        /* GC NewWorld (world.c, locked) */
void mode_flags_clear(void);                                            /* N64 InitCheckpointData wrapper (locked) */

s32 LoadTrack(s32 track, s32 background, s32 reloading) {
    Wad wad;
    WorldHdr *whdr;
    u8 *worldptr;
    s32 i;

    if (!D_80143A10.track.loading) {
        D_80143A10.track.idx = audio_frame_sync(track + 101, 0, background, 0, 0);
        D_80143A10.track.loading = 1;
    }
    if (!player_state_get(D_80143A10.track.idx)) {
        return 0;
    }
    D_80143A10.track.loaded = 1;
    D_80143A10.track.inited = 1;
    D_80143A10.track.ready = 1;
    if (!D_80143A10.world.loading) {
        D_80143A10.world.idx = audio_frame_sync(track + 120, 0, background, 0, 0);
        D_80143A10.world.loading = 1;
    }
    if (!player_state_get(D_80143A10.world.idx)) {
        return 0;
    }
    D_80143A10.world.loaded = 1;
    if (!D_80143A10.world.ready) {
        worldptr = slot_value_get(D_80143A10.world.idx);
        func_80096C28(&wad, worldptr);
        D_801497FC = lookup_with_output(&wad, 0x57484452, NULL);
        D_80149818 = (WorldHdr *)(D_801497FC + 1);
        if (!D_80143A10.world.inited) {
            for (i = 0, whdr = D_80149818; i < *D_801497FC; i++, whdr++) {
                whdr->woff += (u32)worldptr;
            }
            D_80143A10.world.inited = 1;
        }
        D_80149770 = lookup_with_output(&wad, 0x47444154, &D_80149790);
        D_801497C0 = lookup_with_output(&wad, 0x47544C44, &D_801497EC);
        D_80143A10.world.ready = 1;
    }
    if (!D_80143A10.collision.loading) {
        D_80143A10.collision.idx = audio_frame_sync(track + 139, 0, background, 0, 0);
        D_80143A10.collision.loading = 1;
    }
    if (!player_state_get(D_80143A10.collision.idx)) {
        return 0;
    }
    D_80143A10.collision.loaded = 1;
    D_80143A10.collision.inited = 1;
    if (!D_80143A10.collision.ready) {
        D_801526DC = slot_value_get(D_80143A10.collision.idx);
        D_80143A10.collision.ready = 1;
    }
    if (!D_80143A10.path.loading) {
        D_80143A10.path.idx = audio_frame_sync((D_80152570 ? 177 : 158) + track, 0, background, 0, 0);
        D_80143A10.path.loading = 1;
    }
    if (!player_state_get(D_80143A10.path.idx)) {
        return 0;
    }
    D_80143A10.path.loaded = 1;
    D_80143A10.path.inited = 1;
    if (!D_80143A10.path.ready) {
        D_80153F20 = slot_value_get(D_80143A10.path.idx);
        D_80143A10.path.ready = 1;
    }
    if (!D_80143A10.other.loading) {
        D_80143A10.other.loading = 1;
        if (!(D_801174B4 & 0x100)) {
            if (D_8014A110 == 6 || (track >= 6 && track < 14)) {
                audio_frame_sync(76, 0, 0, 0, 0);
            }
            if (!(D_801174B4 & 8) || ((D_801174B4 & 8) && D_80156994)) {
                audio_frame_sync(68, 0, 0, 0, 0);
                if (track == 18) {
                    audio_frame_sync(78, 0, 0, 0, 0);
                } else if (track >= 0 && track < 6) {
                    if (D_80156994) {
                        audio_frame_sync(78, 0, 0, 0, 0);
                        audio_frame_sync(D_8014978C + 82, 0, 0, 0, 0);
                    } else {
                        audio_frame_sync(79, 0, 0, 0, 0);
                    }
                }
                audio_frame_sync(77, 0, 0, 0, 0);
            }
            audio_frame_sync(62, 0, 0, 0, 0);
            if (D_80156994 || D_8014978C >= 6) {
                audio_frame_sync(80, 0, 0, 0, 0);
                audio_frame_sync(61, 0, 0, 0, 0);
            } else {
                audio_frame_sync(81, 0, 0, 0, 0);
            }
            audio_frame_sync(64, 0, 0, 0, 0);
            if (gNumWindows >= 2) {
                audio_frame_sync(66, 0, 0, 0, 0);
                if (D_80156994 || D_8014978C >= 6) {
                    audio_frame_sync(65, 0, 0, 0, 0);
                }
            }
            if (D_8014A110 == 6) {
                audio_frame_sync(63, 0, 0, 0, 0);
            } else if (D_8014A110 == 4) {
                audio_frame_sync(67, 0, 0, 0, 0);
            }
            if (D_801174B4 & 8) {
            } else if (D_801174B4 & 8) {
            }
            if (D_801174B4 & 8) {
            }
        }
        D_80143A10.other.loaded = 1;
        D_80143A10.other.ready = 1;
        D_80143A10.other.inited = 1;
    }
    if (D_80143A10.trackInited) {
        return 1;
    }
    D_80143A10.trackInited = 1;
    camera_lerp_position(track);
    camera_reset();
    wheel_torque_apply((u8 *)D_8011A924[track], -1);
    func_800B2BDC();
    tire_sound_update();
    if (!(D_801174B4 & 0x100) && !reloading) {
        audio_interrupt_handler(0);
    }
    sfx_position_3d(!(D_801174B4 & 0x100));
    mode_flags_clear();
    audio_effect_remove();
    if (D_8014978C >= 0 && D_8014978C < 6) {
        func_800B9284();
    }
    func_800B0580();
    physics_collision_test();
    camera_smooth_lerp();
    if (D_80156994 || track >= 6) {
        func_8038A8CC();
    }
    return 1;
}
