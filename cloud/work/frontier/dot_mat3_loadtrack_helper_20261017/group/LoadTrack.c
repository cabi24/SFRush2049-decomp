/* Research-only LoadTrack with the genuine locked N64 checkpoint/view-flag wrapper.
 * 39/526 residual; no matching claim. See README.md for inherited world-base limits. */
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
extern s16 D_80149D90;             /* Worlddlidx */
extern volatile u32 D_80149B80;    /* WorldBase */
extern char *D_8011A924[];         /* gWorldNames (N64: indexed by track) */
extern s8 D_80110680[];

s32 audio_frame_sync(s32 wave, s32 a1, s32 background, s32 a3, s32 a4);   /* GC LoadWave */
s8 player_state_get(s32 idx);                                            /* GC WaveInstanceReady */
void *slot_value_get(s32 idx);                                           /* GC GetWaveInstanceAddr */
void func_80096C28(Wad *wad, void *ptr);                                 /* GC MBSetupWad */
void *lookup_with_output(Wad *wad, s32 tag, s32 *count);                 /* GC MBGetFromWad */
void camera_lerp_position(s16 track);                                    /* GC initTargetStructs */
void camera_reset(void);                                                 /* GC SkyInit */
void func_800A473C(char *dst, char *src);                                /* strcpy */
void *entity_name_copy(void *key, void *base, s32 n, s32 size, s32 (*cmp)(const void *, const void *)); /* bsearch */
s32 func_8008AD48(const void *a, const void *b);                         /* GC worldcmp */
void differential_output(u32 base, s32 parent);                         /* GC NewWorldObject */
void func_800AB638(void);                                                /* GC InitSTree */
void func_800B2BDC(void);                                                /* GC InitEnvs */
void tire_sound_update(void);                                            /* GC InitTrackDynamicObjs */
void audio_interrupt_handler(s32 x);                                     /* GC LoadCars */
void sfx_position_3d(s32 x);                                             /* GC InitVisuals */
void audio_dsp_process(void);                                            /* GC InitCheckpointData */
void audio_effect_remove(void);                                          /* GC InitPathData */
void func_800B9284(void);                                                /* GC InitDemoPathPoint */
void func_800B0580(void);                                                /* GC InitSkids */
void physics_collision_test(void);                                       /* GC InitSmoke */
void camera_smooth_lerp(void);                                           /* GC InitBlast */
void func_8038A8CC(void);                                                /* overlay call (GC initWorldDamage?) */

/* GC world.c FindWorld(wstr) (static there), inlined */
WorldHdr *w29_FindWorld(char *wstr) {
    WorldHdr *ptr;
    WorldHdr whdr;

    func_800A473C(whdr.wname, wstr);
    ptr = entity_name_copy(&whdr, D_80149818, *D_801497FC, 20, func_8008AD48);
    return ptr;
}

/* GC world.c NewWorld(World, Parent), inlined */
s16 w29_NewWorld(char *World, s16 Parent) {
    WorldHdr *whdr;

    D_80149D90 = -1;
    whdr = w29_FindWorld(World);
    D_80149B80 = whdr->woff;
    differential_output(D_80149B80, Parent);
    func_800AB638();
    return D_80149D90;
}

void mode_flags_clear(void);

s32 LoadTrack(s32 track, s32 background, s32 reloading) {
    Wad wad;
    WorldHdr *whdr;
    s32 *worldptr;
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
    w29_NewWorld(D_8011A924[track], -1);
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
    if (D_80156994 || &D_8011A924[track] >= &D_8011A924[6]) {
        func_8038A8CC();
    }
    return 1;
}
