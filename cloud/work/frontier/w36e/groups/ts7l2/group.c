/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Image A track-select screen: the whole N64 trackscr.c IPA unit, root updateTrackScreen (func_8038E5DC,
 * kept; called by the game code). Donor: MAT3 GameSource trackscr.c (GC 0x80087abc..0x8008d30c), direct
 * typed ports, N64 offsets/addresses from the retail words. Identities (call graph + MAT3 statement order):
 *   func_8038E5DC updateTrackScreen   func_8038DFEC _InitScreen       func_8038B940 _UninitScreen
 *   func_8038C5EC showTrackSelect     func_8038B834 TrackValid        func_8038ACB0 OptionValid
 *   func_8038A634 SetFogIntensity     func_8038AE50 unloadTrackScreen func_8038AEB0 buildTrackMap
 *   func_8038B388 clearTrackMap       func_8038B3EC animTrackMap      func_8038CD14 buildTrackMaps
 *   func_8038A820 animTrackMaps       func_8038C93C initButtons       func_8038BBB4 updateButtons
 * Inlined statics (8-byte deleted stubs): buildRace/Stunt/Battle/ObstacleTrackMaps, loadPathData,
 * unloadPathData (6 stubs 0x8038CCE4..0x8038CD0C before buildTrackMaps), loadTrackScreen, BlowFog,
 * setCamera, uninitButtons, clearTrackMaps.
 * Every function except the kept root has the private IPA ABI (no callee saves; the root saves s0-s8 and
 * $f20-$f30), so members match only in this unit.
 * w36d owns E5DC, DFEC, BBB4, CD14; the other bodies are w36e's region and are present here as context
 * (stand-ins or w36e/prior research bodies, see each header). func_8038C93C is the installed member of
 * dot_runtime_a_c93c_closure_20261012, unchanged.
 */
typedef signed char s8; typedef unsigned char u8;
typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef float f32;
typedef struct Blit Blit;
typedef struct MultiBlit {
    char *name;s16 x,y,width,height,top,bottom,left,right;
    u32 z,alpha;s32 (*callback)(Blit *);u32 descriptor;
} MultiBlit;
typedef struct MenuObjectSlot {s32 other,handle;f32 angle,matrix[3][3],position[3];u8 alpha,pad[3];} MenuObjectSlot;
typedef union Color {u32 word;u8 channel[4];} Color;
typedef struct Texture Texture;
typedef struct FileInfo {u8 unknown00[16],controller;} FileInfo;
typedef struct Profile Profile;
struct Profile {Profile **next,**previous;FileInfo **file;u32 id0,id1;u8 name[24];u8 **stats;};
typedef struct Player {u32 held,pressed,levels,repeated;u8 unknown10[56];Profile **save;} Player;
typedef struct CircuitTrack {u8 trackNum,unknown1,mirror,fog,wind;} CircuitTrack;
typedef struct TrackSlot {s8 gTrackNum;f32 pos[3];f32 targetPos;s8 slot;} TrackSlot;   /* anon_gTrackNum_pos_18 */

/* game globals */
extern u32 D_801174B4;             /* gGameState */
extern u32 D_801174BC;             /* gSkipToGameState */
extern s16 D_8014A108;             /* gNumPlayers */
extern s32 D_8014A110;             /* gRace (0 OneRace 1 Practice 2 Ghost 3 Circuit 4 Stunt 5 Obstacle 6 Battle) */
extern Player D_8014A118[];        /* gPlayers */
extern s8 D_8014978C;              /* gTrackNum */
extern s8 D_80146108[];            /* gOptions */
extern s8 D_80157244;              /* gSoftReset */
extern u32 D_8015694C;             /* gActiveEdges */
extern u32 D_80156944;             /* gActiveLevels */
extern u32 D_80149784;             /* gActiveAutoEdges */
extern s8 D_8013F1D9;              /* gMirrorControls */
extern s8 D_80154628;              /* gCircuitTrack */
extern u8 D_801543D4;              /* gCircuitPlayer */
extern CircuitTrack D_801543D8[];  /* gCircuitTracks */
extern s8 D_80156994;              /* (N64) all-tracks flag: track 5 may be started */
extern s8 D_80142726;              /* gWind */
extern volatile f32 D_8002EB94;    /* gFrameSeconds */
extern f32 D_8011418C[12];         /* unitmatrix */
extern volatile u8 D_80140BDC;     /* gNumModels */

/* trackscr data */
extern MenuObjectSlot D_803B6B14[48];  /* buttonInfo */
extern s32 D_803B7714,D_803B7718;      /* buttonsInited, buttonsJustInited */
extern f32 D_803B771C;                 /* gButtonScale */
extern f32 D_803B7720;                 /* gButtonSpace */
extern f32 D_803B7724;                 /* buttonX */
extern f32 D_803B7728;                 /* maxY */
extern f32 D_803B772C;                 /* minY */
extern f32 D_803B7730;                 /* buttonZ */
extern f32 D_803B7734;                 /* cursorZ */
extern Color D_803B8394;               /* initButtons buttonColor template */
extern Color D_803B8398;               /* updateButtons buttonColor template */
extern char *D_803B6AF0[];             /* buttonNames */
extern char D_803B9264[],D_803B9274[],D_803B927C[],D_803B9280[];  /* "BUTTON_TITLE" "CURSOR" "A1" "B1" */
extern char D_803B9284[],D_803B9294[],D_803B929C[],D_803B92AC[];  /* "BUTTON_SELECT" "BUTTON" x2 */
extern f32 D_803B7A20[];               /* yFog */
extern MultiBlit D_803B7A6C[];         /* mblit (17 entries on N64) */
extern Blit *D_803B7CD0;               /* blit */
extern s8 D_803B7CD4;                  /* inited */
extern f32 D_803B7CE0;                 /* pathPos */
extern s16 D_803BA84E;                 /* oldTrack */
extern s16 D_803BA852;                 /* gNumTracks */
extern s16 D_803BA856;                 /* gTrack */
extern s16 D_803BA85A;                 /* gActiveOption */
extern s16 D_803BA878;                 /* gFirstOption */
extern s16 D_803BA898;                 /* gNumOptions */
extern s8 D_803BA908;                  /* gActiveOptionEnabled */
extern s8 D_803BA910;                  /* gActiveZ */
extern s16 D_803BA8B0[21];             /* gOpts */
extern TrackSlot D_803BA918[19];       /* gTracks */
extern f32 D_803BA85C;                 /* updateButtons static cursorSpeed */
extern f32 D_803BA87C;                 /* updateButtons static scrollSpeed */
extern s32 D_803BAC78;                 /* currentTrackMap */
extern s32 D_803BAC90;                 /* desiredTrackMap */
extern f32 D_803BACCC;                 /* mapTransition */
extern f32 D_803BAE10[3];              /* _InitScreen static fogPos */
extern u16 D_803BA220,D_803BA222;      /* BlowFog static fogTileSize.uls/.ult */

/* game functions */
extern void *memcpy(void *,const void *,u32);
extern f32 fabsf(f32);
#pragma intrinsic (fabsf)
extern void func_800B5570(s32);                   /* setGameState */
extern s8 func_800A361C(s32);                     /* (N64) controller-pak removed test */
extern void resource_type_select(u32);            /* PlayStartABSound */
extern void audio_doppler(u32);                   /* PlayLRSound */
extern s32 entity_flags_apply(u32,u32,u32,u8);    /* AudPlayEffect */
extern void audio_bus_mix(Profile **,u8,s8);      /* SavePlayerOption */
extern s8 reverb_setup(Profile **,u8);            /* GetPlayerOption */
extern void menu_text_input(Profile **,s32,u8);   /* SavePlayerTrack */
extern u8 func_800CDC3C(Profile **,s32);          /* GetPlayerTrack */
extern void init_state_begin(void);               /* SetOptions */
extern void particle_lifetime_set(void);          /* createBG */
extern Blit *sound_control(s16,s16,const MultiBlit *,s16);  /* NewMultiBlit */
extern void particle_position_set(s16);           /* ZeroCamera */
extern void Effects_UpdateEmitters(void);         /* UpdateCameras */
extern void entity_audio_update(void);            /* ClearRushControls */
extern s32 func_800F7644(s32);                    /* IsTrackUnlocked */
extern s32 sign_extend_call(u32,u32,s32,u32);     /* MBOX_NewObject */
extern s32 string_copy_format(char *,s8,s8,s8);   /* MBOX_FindObject_WORK */
extern Texture *func_800B24EC(char *,s16 *,s8,s8,s32);  /* MBOX_FindTexture_WORK */
extern void func_8008D870(s16,s32,s32);           /* ZOID_SetTexture */
extern void func_8008E06C(s16,s32 *);             /* ZOID_SetColor */
extern void func_8008B32C(f32 [3][3],f32 [3][3],f32);  /* ScaleUV */
extern void func_800B5898(f32,f32 [][3]);         /* WPitchUV */
extern void func_800B5940(f32,f32 [][3]);         /* WRollUV */
extern void particle_velocity_set(void);          /* ZeroCameras */
extern void model_data_load(s32,s32,s32);         /* MBOX_HideObject */
extern void model_transform_setup(s32,s32,s32);   /* MBOX_ShowObject */

/* unit functions */
void func_8038DFEC(void);
void func_8038B940(void);
void func_8038C5EC(void);
s32 func_8038B834(s32);
s32 func_8038ACB0(s32);
void func_8038A634(s32);
void func_8038AE50(void);
void func_8038AEB0(void);
void func_8038B388(void);
void func_8038B3EC(void);
void func_8038CD14(void);
void func_8038A820(void);
void func_8038C93C(void);
void func_8038BBB4(void);
void trk_BlowFog(void); void trk_setCamera(void);
/*@@BODIES@@*/
/* ---- context (w36e region): OptionValid, TrackValid, SetFogIntensity, unloadTrackScreen, clearTrackMap ---- */
extern s32 D_803B77D4[][21];   /* validOption */
extern s8 D_803B8314[19];      /* TrackValid static trackReady */
extern s32 D_803AF980;

/* OptionValid: body of the installed group dot_runtime_a_c93c_closure_20261012 (context there too). */
s32 func_8038ACB0(s32 opt)
{
    s32 valid;
    valid = D_803B77D4[D_8014A110][opt];
    if (D_8014A110 == 0 && opt == 17) valid = D_8014A108 < 3;
    if (D_8014A110 == 4) {
        if (opt == 5) valid = D_80146108[32] == 1;
        if (opt == 6) valid = D_80146108[32] == 0;
        if (opt == 7) valid = D_80146108[32] == 2;
    }
    if (D_8014A110 == 6) {
        if (opt == 9) valid = D_80146108[37] == 0;
        if (opt == 11) valid = D_80146108[37] == 0;
        if (opt == 10) valid = D_80146108[37] == 1;
    }
    if (opt == 19 && D_8014A108 == 1) valid = 0;
    if (opt == 12) valid = D_8014978C >= 0 && D_8014978C <= 5;
    if (!D_80156994 && D_8014978C == 5 && opt != 0) valid = 0;
    if (!D_803AF980 && opt == 1) valid = 0;
    return valid;
}

/* TrackValid: body of the installed group dot_runtime_a_c93c_closure_20261012 (context there too). */
s32 func_8038B834(s32 track)
{
    if (!D_803B8314[track]) return 0;
    if (!func_800F7644(track)) return 0;
    if (D_8014A110 == 4) return track >= 14 && track <= 17;
    if (D_8014A110 == 5) return track >= 18 && track <= 18;
    if (D_8014A110 == 6) return track >= 6 && track <= 13;
    return track >= 0 && track <= 5;
}

/* SetFogIntensity: research body cloud/work/frontier/dot_runtime_a_color_smoothing_20261006/candidate.c */
struct Blit {const char *Name;void *Image;void *Info;u16 TexIndex;s16 X,Y;u16 Z;s16 Width,Height;u8 Alpha,Flip;s8 Hide,Init;
    u8 other28[24];u16 resource;};
typedef struct RGB { u8 r, g, b; } RGB;
extern s8 D_801427A0;
extern s8 D_8013F1D8;
extern RGB D_80114658[];
extern u8 D_803BA1E0[8];
extern u8 D_803BA1E8[];
extern u8 *D_803BA1FC;
extern Blit *D_803B8288,*D_803B828C;
extern void Input_ApplyPadConfig(Blit *);

void func_8038A634(s32 initialize)
{
    u8 color[3];
    if (D_801427A0 < 19)
        D_801427A0 = D_8014978C;
    color[0] = D_80114658[D_801427A0].r;
    color[1] = D_80114658[D_801427A0].g;
    color[2] = D_80114658[D_801427A0].b;
    if (initialize) {
        D_803BA1E0[0] = color[0];
        D_803BA1E0[1] = color[1];
        D_803BA1E0[2] = color[2];
    } else {
        D_803BA1E0[0] += (color[0] - D_803BA1E0[0]) / 10;
        D_803BA1E0[1] += (color[1] - D_803BA1E0[1]) / 10;
        D_803BA1E0[2] += (color[2] - D_803BA1E0[2]) / 10;
    }
    D_803BA1E0[3] = D_8013F1D8 * 192 / 3;
    if (D_803B828C) {
        D_803BA1FC = D_803BA1E0;
        D_803BA1E0[4] = 0;
        D_803BA1E0[5] = 0;
        D_803BA1E0[6] = 0;
        D_803BA1E0[7] = 0;
        D_803B828C->Z = 0x7E00;
        D_803B828C->Alpha = D_803BA1E0[3];
        D_803B828C->Image = D_803BA1E8;
        Input_ApplyPadConfig(D_803B828C);
    }
}

/* BlowFog (inlined into _InitScreen) */
void trk_BlowFog(void)
{
    s32 s;
    s32 speed;
    speed = D_80142726 ? D_80142726 * 5 : 1;
    s = D_803BA220 + speed;
    if (s < 0) s += 2048;
    if (s >= 2048) s -= 2048;
    D_803BA220 = s;
    D_803BA222 = 0;
}

/* setCamera (inlined into _InitScreen and updateTrackScreen) */
void trk_setCamera(void)
{
    particle_position_set(1);
    Effects_UpdateEmitters();
}

extern volatile s16 D_8002EB70;   /* gRenderInProgress */
extern s32 D_803B8284;            /* trackMapObj */
extern void sound_stop(Blit *);   /* RemoveBlit (ZOID_RemovePoly on N64: blits) */
extern void sound_call_minimal(s16);  /* ZOID_RemoveObject */
extern void ambient_sounds_clear(void);  /* removeBG */
extern void func_800A5B3C(void);  /* ZOID_Reset */

/* unloadTrackScreen: option_parent research body */
void func_8038AE50(void)
{
    Blit *b;
    if ((b = D_803B8288)) { sound_stop(b); D_803B8288 = 0; }
    if ((b = D_803B828C)) { sound_stop(b); D_803B828C = 0; }
}

/* clearTrackMap: option_parent research body */
void func_8038B388(void)
{
    func_8038AE50();
    if (D_803B8284 != -1) { sound_call_minimal((s16)D_803B8284); D_803B8284 = -1; }
    while (D_8002EB70) {}
}

/* ---- context (w36e region): _UninitScreen, buildTrackMap, animTrackMap: option_parent research bodies ---- */
typedef struct ResourceQuad {u32 words[4];} ResourceQuad;
typedef struct ModelResource {u8 other[24];ResourceQuad quad;u8 tail[48];} ModelResource;
typedef struct ModelBank {ModelResource *models;u32 other;} ModelBank;
typedef struct Slot72 {s32 handle;f32 offset_x,offset_y,angle,scale_from,scale_to;f32 matrix[3][3],position[3];} Slot72;   /* trackMap: anon_objIdx_cityX_48 */
extern Slot72 D_803B7CE4[20];
extern s8 D_803BAAE0;            /* trackMapsBuilt */
extern ResourceQuad D_803BAB58[7],D_803BAAE8[7];
extern void *D_803BABC8[7],*D_803BABE8[7];
extern ModelBank D_801161F4[];   /* gRomObjDefPtrs */
extern char *D_803B7738[];       /* trackMapObjNames */
extern void func_800960D4(void *);  /* DeallocateMemory */

void func_8038B940(void)
{
    s32 i,model;
    ModelResource *def;
    for (i = 0; i < 48; i++) {
        if (D_803B6B14[i].handle != -1) {
            if (D_801174B4 & 0x7C03FFFE) func_80090254((s16)D_803B6B14[i].handle);
            D_803B6B14[i].handle = -1;
        }
    }
    D_803B7714 = 0;
    if (D_803B7CD0) { sound_stop(D_803B7CD0); D_803B7CD0 = 0; }
    ambient_sounds_clear();
    func_8038B388();
    if (D_803BAAE0) {
        for (i = 0; i < 20; i++) {
            if (D_803B7CE4[i].handle != -1) {
                func_80090254((s16)D_803B7CE4[i].handle);
                D_803B7CE4[i].handle = -1;
            }
        }
        while (D_8002EB70) {}
        for (i = 0; i < 7; i++) {
            model = string_copy_format(D_803B7738[i < 6 ? i : 19],0,D_80140BDC - 1,1);
            def = &D_801161F4[model >> 10].models[model & 0x3FF];
            def->quad = D_803BAB58[i];
            if (D_803BABC8[i]) func_800960D4(D_803BABC8[i]);
            D_803BABC8[i] = 0;
            if (D_803BABE8[i]) func_800960D4(D_803BABE8[i]);
            D_803BABE8[i] = 0;
            D_803BAAE8[i].words[2] = 0;
            D_803BAAE8[i].words[3] = 0;
        }
        D_803BAAE0 = 0;
    }
    func_800A5B3C();
    D_803B7CD4 = 0;
}

typedef struct ImageMeta {u8 other0[21];u8 flags;u8 other22[10];} ImageMeta;
typedef struct DisplayCommand {u32 w0,w1;} DisplayCommand;
extern ImageMeta D_80140BF0[];
extern char *D_803B7788[];
extern char D_803B9260[];
extern Color D_803B8328;
extern s32 D_803B7CD8;          /* kNumMapSects */
extern f32 D_803B8254[12],D_803BAC30[12],D_803B832C[3];
extern Blit *func_800B3704(char *,s16,s16,s16);
extern void func_80094EC8(Blit *);
#define DISPLAY_WORDS(p,a,b) do {DisplayCommand *command=(p)++; command->w0=(a);command->w1=(b);} while(0)
#define DISPLAY_TEXTURE(p,on) DISPLAY_WORDS(p,0xD7000000u|((on)<<1),0xFFFFFFFFu)
#define DISPLAY_VERTEX(p,v,n,first) DISPLAY_WORDS(p,0x01000000u|((n)<<12)|(((first)+(n))<<1),(u32)(v))
#define DISPLAY_TRIANGLES(p,a,b,c,d,e,f) DISPLAY_WORDS(p,0x06000000u|((a)*2<<16)|((b)*2<<8)|((c)*2),((d)*2<<16)|((e)*2<<8)|((f)*2))

/* loadTrackScreen (MAT3 trackscr.c, GC 0x8008a634): inlined into buildTrackMap (deleted stub 0x8038AEA8).
 * The GC poly locals stay declared (shared GameSource declarations); the N64 body makes blits. */
void func_8038AEA8(void)
{
    f32 polyVerts[4][3];
    u16 texIdx;
    Color backColor = D_803B8328;
    func_8038AE50();
    if (!((D_8014978C >= 0 && D_8014978C <= 5) ||
        (D_8014978C >= 6 && D_8014978C <= 13) ||
        (D_8014978C >= 14 && D_8014978C <= 17) ||
        (D_8014978C >= 18 && D_8014978C <= 18))) return;
    D_803B8288 = func_800B3704(D_803B7788[D_8014978C],176,32,0);
    D_803B8288->Z = 0x7F00;
    func_80094EC8(D_803B8288);
    D_80140BF0[D_803B8288->resource].flags |= 1;
    if (func_8038ACB0(14) == 1) {
        D_803B828C = func_800B3704(D_803B9260,176,32,0);
        D_803B828C->Z = 0x7E00;
        func_80094EC8(D_803B828C);
    }
}

typedef struct { u32 w0; u32 w1; } Gwords;
typedef union { Gwords words; long long int force_structure_alignment; } Gfx;
#define _SHIFTL(v, s, w) ((u32)(((u32)(v) & ((0x01 << (w)) - 1)) << (s)))
#define gSPTexture(pkt, s, t, level, tile, on) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (_SHIFTL(0xD7, 24, 8) | _SHIFTL(0, 16, 8) | _SHIFTL((level), 11, 3) | _SHIFTL((tile), 8, 3) | _SHIFTL((on), 1, 7)); \
    _g->words.w1 = (_SHIFTL((s), 16, 16) | _SHIFTL((t), 0, 16)); \
}
#define gDPNoParam(pkt, cmd) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = _SHIFTL(cmd, 24, 8); \
    _g->words.w1 = 0; \
}
#define gDPPipeSync(pkt) gDPNoParam(pkt, 0xE7)
#define gSPEndDisplayList(pkt) gDPNoParam(pkt, 0xDF)
#define gSPSetOtherMode(pkt, cmd, sft, len, data) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (_SHIFTL(cmd, 24, 8) | _SHIFTL(32 - (sft) - (len), 8, 8) | _SHIFTL((len) - 1, 0, 8)); \
    _g->words.w1 = (u32)(data); \
}
#define gDPSetRenderMode(pkt, c0, c1) gSPSetOtherMode(pkt, 0xE2, 3, 29, (c0) | (c1))
#define gDPSetCombine(pkt, muxs0, muxs1) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = _SHIFTL(0xFC, 24, 8) | _SHIFTL(muxs0, 0, 24); \
    _g->words.w1 = (u32)(muxs1); \
}
#define gSPVertex(pkt, v, n, v0) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = _SHIFTL(0x01, 24, 8) | _SHIFTL((n), 12, 8) | _SHIFTL((v0) + (n), 1, 7); \
    _g->words.w1 = (u32)(v); \
}
#define __gsSP1Triangle_w1(v0, v1, v2) (_SHIFTL((v0) * 2, 16, 8) | _SHIFTL((v1) * 2, 8, 8) | _SHIFTL((v2) * 2, 0, 8))
#define gSP2Triangles(pkt, v00, v01, v02, flag0, v10, v11, v12, flag1) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (_SHIFTL(0x06, 24, 8) | __gsSP1Triangle_w1(v00, v01, v02)); \
    _g->words.w1 = __gsSP1Triangle_w1(v10, v11, v12); \
}
typedef struct { s16 ob[3]; u16 flag; s16 tc[2]; u8 cn[4]; } Vtx;
extern Gfx *D_803BAB50;            /* mirrored-map display-list insert point */

void func_8038AEB0(void)
{
    Gfx *gfx;
    Vtx *vtx;
    s32 i, vertex, model;
    func_8038AEA8();
    if (D_803BAC90 >= 0 && D_803BAC90 <= 5) {
        if (D_803B7CE4[19].handle != -1) {
            memcpy(D_803BABE8[6], D_803BABE8[D_803BAC90], (D_803B7CD8 + 1) * 4 * sizeof(Vtx));
            memcpy(D_803BABC8[6], D_803BABC8[D_803BAC90], (D_803B7CD8 * 5 + 7) * sizeof(Gfx));
            gfx = D_803BAB50;
            if ((D_8014A110 == 3 && D_801543D8[D_80154628].mirror == 1)
                || (D_8014A110 != 3 && D_80146108[28])) {
                gSPTexture(gfx++, 0xFFFF, 0xFFFF, 0, 0, 0);
                gDPPipeSync(gfx++);
                gDPSetRenderMode(gfx++, 0xC8112230, 0);
                gDPSetCombine(gfx++, 0xFFFFFF, 0xFFFE7C38);
                vertex = 0; for (i = 0; i < D_803B7CD8; i++, vertex += 4) {
                    if (i + 1 == D_803B7CD8) {
                        gSPVertex(gfx++, &((Vtx *)D_803BABE8[6])[vertex], 4, 0);
                        gSPVertex(gfx++, &((Vtx *)D_803BABE8[6])[0], 4, 4);
                    } else {
                        gSPVertex(gfx++, &((Vtx *)D_803BABE8[6])[vertex], 8, 0);
                    }
                    gSP2Triangles(gfx++, 1, 5, 0, 0, 4, 0, 5, 0);
                    gSP2Triangles(gfx++, 0, 4, 3, 0, 7, 3, 4, 0);
                    gSP2Triangles(gfx++, 3, 7, 2, 0, 6, 2, 7, 0);
                    gSP2Triangles(gfx++, 2, 6, 1, 0, 5, 1, 6, 0);
                }
                gSPTexture(gfx++, 0xFFFF, 0xFFFF, 0, 0, 1);
                gSPEndDisplayList(gfx++);
            }
            func_8008B32C((f32 (*)[3])D_8011418C, (f32 (*)[3])D_803B8254, 2.0f);
            func_800B5898(-0.785398185f, (f32 (*)[3])D_803B8254);
            D_803B7CE4[19].position[0] = D_803B832C[0];
            D_803B7CE4[19].position[1] = D_803B832C[1];
            D_803B7CE4[19].position[2] = D_803B832C[2];
        }
    } else if ((D_803BAC90 >= 6 && D_803BAC90 <= 13) || (D_803BAC90 >= 14 && D_803BAC90 <= 17)) {
        if (D_803B8284 == -1) {
            D_803B8284 = sign_extend_call(string_copy_format(D_803B7738[D_803BAC90], 0, D_80140BDC - 1, 1), (u32)D_803BAC30, -1, 0);
            func_8008B32C((f32 (*)[3])D_8011418C, (f32 (*)[3])D_803BAC30, 2.0f);
            func_800B5898(-0.785398185f, (f32 (*)[3])D_803BAC30);
            D_803BAC30[9] = D_803B832C[0];
            D_803BAC30[10] = D_803B832C[1];
            D_803BAC30[11] = D_803B832C[2];
        }
    }
    D_803BAC78 = D_803BAC90;
}

extern f32 D_803B8350,D_803B8338[3],D_803B8344[3];
extern void func_800FD754(f32 (*)[3],f32 (*)[3],f32,f32,f32);   /* ScaleUVXYZ */
extern void gfx_setup_fc(f32,f32 (*)[3]);   /* WYawUV */

void func_8038B3EC(void)
{
    f32 inverse;
    D_803B8350 += D_8002EB94 * 6.28318548f / 10.0f;
    if (D_803B8350 > 6.28318548f) D_803B8350 -= 6.28318548f;
    if (D_803BAC90 != D_803BAC78) {
        D_803BACCC -= D_8002EB94 * 4.0f;
        if (D_803BACCC < 0.0f) {
            D_803BACCC = 0.0f;
            func_8038B388();
            func_8038AEB0();
        }
    } else {
        D_803BACCC += D_8002EB94 * 4.0f;
        if (D_803BACCC > 1.0f) D_803BACCC = 1.0f;
    }
    D_803B7CE4[19].position[0] = D_803B8338[0];
    D_803B7CE4[19].position[1] = D_803B8338[1];
    D_803B7CE4[19].position[2] = D_803B8338[2];
    D_803B7CE4[19].position[0] += D_803B7CE4[D_803BAC78].offset_x;
    D_803B7CE4[19].position[1] += D_803B7CE4[D_803BAC78].offset_y;
    inverse = 1.0f - D_803BACCC;
    D_803B7CE4[19].position[0] *= inverse;
    D_803B7CE4[19].position[1] *= inverse;
    D_803B7CE4[19].position[2] *= inverse;
    D_803B7CE4[19].position[0] += D_803B8344[0] * D_803BACCC;
    D_803B7CE4[19].position[1] += D_803B8344[1] * D_803BACCC;
    D_803B7CE4[19].position[2] += D_803B8344[2] * D_803BACCC;
    D_803BAC30[9] = D_803B7CE4[19].position[0];
    D_803BAC30[10] = D_803B7CE4[19].position[1];
    D_803BAC30[11] = D_803B7CE4[19].position[2];
    func_8008B32C((f32 (*)[3])D_8011418C,(f32 (*)[3])D_803BAC30,
        D_803B7CE4[D_803BAC78].scale_from * inverse + D_803BACCC * D_803B7CE4[D_803BAC78].scale_to);
    func_8008B32C((f32 (*)[3])D_8011418C,(f32 (*)[3])D_803B8254,
        D_803B7CE4[D_803BAC78].scale_from * (1.0f - D_803BACCC) + D_803BACCC * D_803B7CE4[D_803BAC78].scale_to);
    if ((D_8014A110 == 3 && D_801543D8[D_80154628].mirror == 1)
        || (D_8014A110 != 3 && D_8014A110 != 4 && D_8014A110 != 6 && D_8014A110 != 5 && D_80146108[28])) {
        func_800FD754((f32 (*)[3])D_803BAC30,(f32 (*)[3])D_803BAC30,-1.0f,1.0f,1.0f);
        func_800FD754((f32 (*)[3])D_803B8254,(f32 (*)[3])D_803B8254,-1.0f,1.0f,1.0f);
    }
    gfx_setup_fc(D_803B7CE4[D_803BAC78].angle * (1.0f - D_803BACCC) + D_803B8350 * D_803BACCC,(f32 (*)[3])D_803BAC30);
    func_800B5898(-0.52359879f * D_803BACCC + (1.0f - D_803BACCC) * -1.57079637f,(f32 (*)[3])D_803BAC30);
    gfx_setup_fc(D_803B7CE4[D_803BAC78].angle * (1.0f - D_803BACCC) + D_803B8350 * D_803BACCC,(f32 (*)[3])D_803B8254);
    func_800B5898(-0.52359879f * D_803BACCC + (1.0f - D_803BACCC) * -1.57079637f,(f32 (*)[3])D_803B8254);
}
/* ---- context (w36e region): showTrackSelect stand-in (MAT3 port), animTrackMaps stand-in ---- */
extern s8 D_80152570;      /* gBackwards */
extern s8 D_80140A04;      /* gMirrorWaves */
extern f32 D_803B8290;     /* showTrackSelect static angle */
extern f32 D_803B9A90[3];  /* showTrackSelect static fogPos */
extern void math_utility(f32 *,f32 (*)[3]);          /* CopyUV */
extern void func_80090E9C(f32,f32 (*)[3]);           /* YawUV */

void func_8038C5EC(void)
{
    f32 uv[3][3];
    f32 speed;
    f32 frames;
    s32 i;
    s32 slot;
    func_8038BBB4();
    D_803BA898 = 0;
    for (i = 0; i < 21; i++) if (func_8038ACB0(i)) D_803BA8B0[D_803BA898++] = i;
    frames = 30.0f * D_8002EB94;
    if ((D_80152570 && !D_80140A04) || (!D_80152570 && D_80140A04)) {
        D_803B8290 -= 0.01745329238474369f * frames;
        if (D_803B8290 < 0.0f) D_803B8290 += 6.2831854820251465f;
    } else {
        D_803B8290 += 0.017453292f * frames;
        if (D_803B8290 >= 6.283185482f) D_803B8290 -= 6.283185482f;
    }
    speed = frames * ((D_803BA918[D_803BA856].targetPos - D_803BA918[D_803BA856].pos[0]) / 10.0f);
    for (i = 0; i < D_803BA852; i++) {
        slot = i - D_803BA856;
        if (slot < 0) slot += D_803BA852;
        if (slot != D_803BA918[i].slot) {
            if (D_803BA918[i].slot == (slot-1 < 0 ? D_803BA852-1 : slot-1)) D_803BA918[i].targetPos -= 100.0f;
            else D_803BA918[i].targetPos += 100.0f;
            D_803BA918[i].slot = slot;
        }
        if (speed != 0.0f) {
            D_803BA918[i].pos[0] += speed;
            if (D_803BA918[i].pos[0] < -(D_803BA852*100/2)) {
                D_803BA918[i].pos[0] += D_803BA852*100;
                D_803BA918[i].targetPos += D_803BA852*100;
            } else if (D_803BA918[i].pos[0] > D_803BA852*100/2) {
                D_803BA918[i].pos[0] -= D_803BA852*100;
                D_803BA918[i].targetPos -= D_803BA852*100;
            }
        }
    }
    D_803B9A90[1] += frames * ((D_803B7A20[D_803BA918[D_803BA856].gTrackNum] - 5.0f - D_803B9A90[1]) / 10.0f);
    math_utility(D_8011418C, uv);
    func_80090E9C(D_803B8290, uv);
    func_800FD754(uv, uv, 2.0f, 1.0f, 2.5f);
}

extern f32 D_803B8388[3];   /* animTrackMaps static mapPos */

/* animTrackMaps stand-in: the GC transform loop and pathPos advance (the N64 vertex-colour fade is omitted). */
void func_8038A820(void)
{
    s32 trackIdx;
    if (D_803BAAE0) {
        for (trackIdx = 0; trackIdx <= 19; trackIdx++) {
            if (D_803B7CE4[trackIdx].handle != -1) {
                if (trackIdx < 19) {
                    func_8008B32C((f32 (*)[3])D_8011418C, D_803B7CE4[trackIdx].matrix, D_803B7CE4[trackIdx].scale_from);
                    func_800B5898(-1.5707963705062866f, D_803B7CE4[trackIdx].matrix);
                    func_800B5940(-D_803B7CE4[trackIdx].angle, D_803B7CE4[trackIdx].matrix);
                    D_803B7CE4[trackIdx].position[2] = D_803B8388[2];
                    D_803B7CE4[trackIdx].position[0] = D_803B8388[0] + D_803B7CE4[trackIdx].offset_x;
                    D_803B7CE4[trackIdx].position[1] = D_803B8388[1] + D_803B7CE4[trackIdx].offset_y;
                    if (trackIdx == D_8014978C) {
                        func_8008B32C(D_803B7CE4[trackIdx].matrix, D_803B7CE4[trackIdx].matrix, 0.8f);
                        D_803B7CE4[trackIdx].position[0] *= 0.8f;
                        D_803B7CE4[trackIdx].position[1] *= 0.8f;
                        D_803B7CE4[trackIdx].position[2] *= 0.8f;
                    }
                }
            }
        }
        D_803B7CE0 += D_8002EB94 / 5.0f;
        while (D_803B7CE0 > 1.0f) D_803B7CE0 -= 1.0f;
    }
}

/* ---- buildTrackMaps (stand-in, to be replaced by the full port) ---- */
extern f32 D_803B8370[3],D_803B837C[3];  /* buildStunt/BattleTrackMaps static mapPos */

void func_8038CCE4(void) {}
void func_8038CCEC(void)
{
    s32 saveTrackNum;
    saveTrackNum = D_8014978C;
    for (D_8014978C = 14; D_8014978C <= 17; D_8014978C++) {
        if (func_800F7644(D_8014978C) && D_803B7CE4[D_8014978C].handle == -1) {
            D_803B7CE4[D_8014978C].handle = sign_extend_call(string_copy_format(D_803B7738[D_8014978C], 0, D_80140BDC - 1, 1), (u32)D_803B7CE4[D_8014978C].matrix, -1, 0);
            func_8008B32C((f32 (*)[3])D_8011418C, D_803B7CE4[D_8014978C].matrix, 1.0f);
            func_800B5898(-1.5707963705062866f, D_803B7CE4[D_8014978C].matrix);
            D_803B7CE4[D_8014978C].position[0] = D_803B8370[0];
            D_803B7CE4[D_8014978C].position[1] = D_803B8370[1];
            D_803B7CE4[D_8014978C].position[2] = D_803B8370[2];
        }
    }
    D_8014978C = saveTrackNum;
}
void func_8038CCF4(void)
{
    s32 saveTrackNum;
    saveTrackNum = D_8014978C;
    for (D_8014978C = 6; D_8014978C <= 13; D_8014978C++) {
        if (func_800F7644(D_8014978C) && D_803B7CE4[D_8014978C].handle == -1) {
            D_803B7CE4[D_8014978C].handle = sign_extend_call(string_copy_format(D_803B7738[D_8014978C], 0, D_80140BDC - 1, 1), (u32)D_803B7CE4[D_8014978C].matrix, -1, 0);
            func_8008B32C((f32 (*)[3])D_8011418C, D_803B7CE4[D_8014978C].matrix, 1.0f);
            func_800B5898(-1.5707963705062866f, D_803B7CE4[D_8014978C].matrix);
            D_803B7CE4[D_8014978C].position[0] = D_803B837C[0];
            D_803B7CE4[D_8014978C].position[1] = D_803B837C[1];
            D_803B7CE4[D_8014978C].position[2] = D_803B837C[2];
        }
    }
    D_8014978C = saveTrackNum;
}
void func_8038CCFC(void) {}

void func_8038CD14(void)
{
    if (D_8014A110 == 4) func_8038CCEC();
    else if (D_8014A110 == 6) func_8038CCF4();
    else if (D_8014A110 == 5) func_8038CCFC();
    else func_8038CCE4();
    D_803BAAE0 = 1;
    D_803B7CE0 = 0.0f;
}

void func_8038C93C(void)
{
    Color color = D_803B8394;
    s32 i;
    s16 texture_index;
    f32 yaw;
    for (i = 0; i < 48; i++) {
        if (D_803B6B14[i].handle == -1) {
            memcpy(D_803B6B14[i].matrix, D_8011418C, 36);
            D_803B6B14[i].handle = sign_extend_call(
                string_copy_format(D_803B6AF0[D_803B6B14[i].other], 0, D_80140BDC - 1, 1),
                (u32)D_803B6B14[i].matrix, -1,
                (D_803B6B14[i].other == 0 || D_803B6B14[i].other == 1 || D_803B6B14[i].other == 2) ? 0x42000 : 0);
            func_8008E06C((s16)D_803B6B14[i].handle, (s32 *)&color);
        }
    }
    func_8008D870((s16)D_803B6B14[42].handle,
        (s32)func_800B24EC(D_803B9264, &texture_index, 0, D_80140BDC - 1, 1), -1);
    memcpy(D_803B6B14[42].matrix, D_8011418C, 36);
    yaw = -1.5707963705062866f;
    func_800B5898(yaw, D_803B6B14[42].matrix);
    D_803B6B14[42].position[0] = 0.0f;
    D_803B6B14[42].position[1] = 115.0f;
    D_803B6B14[42].position[2] = 200.0f;
    func_8008D870((s16)D_803B6B14[43].handle,
        (s32)func_800B24EC(D_803B9274, &texture_index, 0, D_80140BDC - 1, 1), -1);
    func_8008D870((s16)D_803B6B14[44].handle,
        (s32)func_800B24EC(D_803B927C, &texture_index, 0, D_80140BDC - 1, 1), -1);
    memcpy(D_803B6B14[44].matrix, D_8011418C, 36);
    func_800B5898(yaw, D_803B6B14[44].matrix);
    D_803B6B14[44].position[0] = 75.1f;
    D_803B6B14[44].position[1] = 55.2f;
    D_803B6B14[44].position[2] = 100.0f;
    func_8008B32C(D_803B6B14[44].matrix, D_803B6B14[44].matrix, 0.5f);
    memcpy(D_803B6B14[45].matrix, D_8011418C, 36);
    func_800B5898(yaw, D_803B6B14[45].matrix);
    func_800B5940(3.1415927410125732f, D_803B6B14[45].matrix);
    D_803B6B14[45].position[0] = 87.1f;
    D_803B6B14[45].position[1] = 55.2f;
    D_803B6B14[45].position[2] = 100.0f;
    func_8008D870((s16)D_803B6B14[46].handle,
        (s32)func_800B24EC(D_803B9280, &texture_index, 0, D_80140BDC - 1, 1), -1);
    memcpy(D_803B6B14[46].matrix, D_8011418C, 36);
    func_800B5898(yaw, D_803B6B14[46].matrix);
    D_803B6B14[46].position[0] = -75.1f;
    D_803B6B14[46].position[1] = 55.20f;
    D_803B6B14[46].position[2] = 100.0f;
    func_8008B32C(D_803B6B14[46].matrix, D_803B6B14[46].matrix, 0.5f);
    memcpy(D_803B6B14[47].matrix, D_8011418C, 36);
    func_800B5898(yaw, D_803B6B14[47].matrix);
    D_803B6B14[47].position[0] = -87.1f;
    D_803B6B14[47].position[1] = 55.20f;
    D_803B6B14[47].position[2] = 100.0f;
    D_803B7714 = 1;
    D_803B7718 = 1;
    particle_velocity_set();
}
/* updateButtons (MAT3 trackscr.c, GC 0x8008cb54). N64: single-precision speed clamps (60.0f/10.0f),
 * WPitchUV(angle - pi/2) (GC adds pi), and the out-of-range buttons fade over one button space
 * (alpha 255 - d*255/(space*scale)) instead of GC's fixed 80. */
void func_8038BBB4(void)
{
    s32 buttonIdx;
    f32 prevPos[3];
    f32 buttonY;
    f32 selectedPos;
    s8 clearScrollSpeed = 1;
    Texture *tex;
    s16 texIdx;
    Color buttonColor = D_803B8398;
    if (D_803B7714) {
        if (D_803B771C * ((D_803BA898 - 1) * D_803B7720) > D_803B7728 - D_803B772C)
            selectedPos = D_803B7728 - D_803BA85A * (D_803B7728 - D_803B772C) / (D_803BA898 - 1);
        else selectedPos = D_803B7728 - D_803B771C * (D_803BA85A * D_803B7720);
        memcpy(prevPos, D_803B6B14[43].position, 12);
        D_803B6B14[43].angle += 6.2831854820251465f * D_8002EB94;
        if (D_803B6B14[43].angle > 6.2831854820251465f) D_803B6B14[43].angle -= 6.2831854820251465f;
        D_803B6B14[43].position[0] = D_803B7724 * D_803B7734 / D_803B7730 + -180.0f * D_803B771C;
        D_803B6B14[43].position[1] = selectedPos * D_803B7734 / D_803B7730;
        D_803B6B14[43].position[2] = D_803B7734;
        if (!D_803B7718) {
            if (60.0f * fabsf(D_803B6B14[43].position[1] - prevPos[1]) / 10.0f > D_803BA85C) {
                D_803BA85C = 60.0f * fabsf(D_803B6B14[43].position[1] - prevPos[1]) / 10.0f;
                D_803BA85C = D_803BA85C < 540.0f * D_803B771C ? D_803BA85C : 540.0f * D_803B771C;
                D_803BA85C = D_803BA85C > 180.0f * D_803B771C ? D_803BA85C : 180.0f * D_803B771C;
            }
            if (D_803B6B14[43].position[1] > prevPos[1] + D_803BA85C * D_8002EB94)
                D_803B6B14[43].position[1] = prevPos[1] + D_803BA85C * D_8002EB94;
            else if (D_803B6B14[43].position[1] < prevPos[1] - D_803BA85C * D_8002EB94)
                D_803B6B14[43].position[1] = prevPos[1] - D_803BA85C * D_8002EB94;
            else D_803BA85C = 180.0f * D_803B771C;
        }
        func_8008B32C((f32 (*)[3])D_8011418C, D_803B6B14[43].matrix, 0.4f * D_803B771C);
        func_800B5940(D_803B6B14[43].angle, D_803B6B14[43].matrix);
        func_800B5898(1.5707963705062866f, D_803B6B14[43].matrix);
        buttonY = selectedPos + D_803B771C * (D_803BA85A * D_803B7720);
        for (buttonIdx = 0; buttonIdx < 21; buttonIdx++) {
            if (buttonIdx == D_803BA8B0[D_803BA85A]) {
                if (D_803B6B14[buttonIdx].angle < 3.1415927410125732f) D_803B6B14[buttonIdx].angle += 12.566370964050293f * D_8002EB94;
                if (D_803B6B14[buttonIdx].angle > 3.1415927410125732f) D_803B6B14[buttonIdx].angle = 3.1415927410125732f;
                if (D_803B7718) D_803B6B14[buttonIdx].angle = 3.1415927410125732f;
            } else {
                if (D_803B6B14[buttonIdx].angle > 0.0f) D_803B6B14[buttonIdx].angle -= 12.566371f * D_8002EB94;
                if (D_803B6B14[buttonIdx].angle < 0.0f) D_803B6B14[buttonIdx].angle = 0.0f;
                if (D_803B7718) D_803B6B14[buttonIdx].angle = 0.0f;
            }
            if ((D_8014A110 != 3 || (D_80154628 == 0 && buttonIdx == 20)) && (D_8014A110 != 2 || buttonIdx != 2)) D_803B6B14[buttonIdx+21].angle = D_803B6B14[buttonIdx].angle;
            else D_803B6B14[buttonIdx+21].angle = 0.0f;
            memcpy(prevPos, D_803B6B14[buttonIdx].position, 12);
            D_803B6B14[buttonIdx].position[0] = D_803B7724 + -155.0f * D_803B771C;
            D_803B6B14[buttonIdx].position[1] = buttonY;
            D_803B6B14[buttonIdx].position[2] = D_803B7730;
            if (!D_803B7718) {
                if (60.0f * fabsf(D_803B6B14[buttonIdx].position[1] - prevPos[1]) / 10.0f > D_803BA87C) {
                    D_803BA87C = 60.0f * fabsf(D_803B6B14[buttonIdx].position[1] - prevPos[1]) / 10.0f;
                    D_803BA87C = D_803BA87C < 540.0f * D_803B771C ? D_803BA87C : 540.0f * D_803B771C;
                    D_803BA87C = D_803BA87C > 180.0f * D_803B771C ? D_803BA87C : 180.0f * D_803B771C;
                }
                if (D_803B6B14[buttonIdx].position[1] > prevPos[1] + D_803BA87C * D_8002EB94) {
                    D_803B6B14[buttonIdx].position[1] = prevPos[1] + D_803BA87C * D_8002EB94;
                    clearScrollSpeed = 0;
                } else if (D_803B6B14[buttonIdx].position[1] < prevPos[1] - D_803BA87C * D_8002EB94) {
                    D_803B6B14[buttonIdx].position[1] = prevPos[1] - D_803BA87C * D_8002EB94;
                    clearScrollSpeed = 0;
                }
            }
            D_803B6B14[buttonIdx+21].position[0] = D_803B7724 + 50.0f * D_803B771C;
            D_803B6B14[buttonIdx+21].position[1] = D_803B6B14[buttonIdx].position[1];
            D_803B6B14[buttonIdx+21].position[2] = D_803B6B14[buttonIdx].position[2];
            func_8008B32C((f32 (*)[3])D_8011418C, D_803B6B14[buttonIdx].matrix, D_803B771C);
            func_8008B32C((f32 (*)[3])D_8011418C, D_803B6B14[buttonIdx+21].matrix, D_803B771C);
            func_800B5898(D_803B6B14[buttonIdx].angle - 1.5707963705062866f, D_803B6B14[buttonIdx].matrix);
            func_800B5898(D_803B6B14[buttonIdx+21].angle - 1.5707963705062866f, D_803B6B14[buttonIdx+21].matrix);
            if (D_803B6B14[buttonIdx].angle > 1.5707963705062866f) tex = func_800B24EC(D_803B9284, &texIdx, 0, D_80140BDC - 1, 1);
            else tex = func_800B24EC(D_803B9294, &texIdx, 0, D_80140BDC - 1, 1);
            func_8008D870(D_803B6B14[buttonIdx].handle, (s32)tex, -1);
            if (D_803B6B14[buttonIdx+21].angle > 1.5707963705062866f) tex = func_800B24EC(D_803B929C, &texIdx, 0, D_80140BDC - 1, 1);
            else tex = func_800B24EC(D_803B92AC, &texIdx, 0, D_80140BDC - 1, 1);
            func_8008D870(D_803B6B14[buttonIdx+21].handle, (s32)tex, -1);
            if (D_803B6B14[buttonIdx].position[1] > D_803B7728 + D_803B7720 * D_803B771C || D_803B6B14[buttonIdx].position[1] < D_803B772C - D_803B7720 * D_803B771C) D_803B6B14[buttonIdx].alpha = 0;
            else if (D_803B6B14[buttonIdx].position[1] > D_803B7728) D_803B6B14[buttonIdx].alpha = 255.0f - (D_803B6B14[buttonIdx].position[1] - D_803B7728) * 255.0f / (D_803B7720 * D_803B771C);
            else if (D_803B6B14[buttonIdx].position[1] < D_803B772C) D_803B6B14[buttonIdx].alpha = 255.0f - (D_803B772C - D_803B6B14[buttonIdx].position[1]) * 255.0f / (D_803B7720 * D_803B771C);
            else D_803B6B14[buttonIdx].alpha = 255;
            if (!func_8038ACB0(buttonIdx)) D_803B6B14[buttonIdx].alpha = 0;
            D_803B6B14[buttonIdx+21].alpha = D_803B6B14[buttonIdx].alpha;
            buttonColor.channel[3] = D_803B6B14[buttonIdx].alpha;
            func_8008E06C(D_803B6B14[buttonIdx].handle, (s32 *)&buttonColor);
            func_8008E06C(D_803B6B14[buttonIdx+21].handle, (s32 *)&buttonColor);
            if (!func_8038ACB0(buttonIdx)) {
                model_data_load(D_803B6B14[buttonIdx].handle, 0, 15);
                model_data_load(D_803B6B14[buttonIdx+21].handle, 0, 15);
            } else if (D_803B6B14[buttonIdx].alpha == 0) {
                model_data_load(D_803B6B14[buttonIdx].handle, 0, 15);
                model_data_load(D_803B6B14[buttonIdx+21].handle, 0, 15);
                buttonY -= D_803B7720 * D_803B771C;
            } else {
                model_transform_setup(D_803B6B14[buttonIdx].handle, 0, 15);
                model_transform_setup(D_803B6B14[buttonIdx+21].handle, 0, 15);
                buttonY -= D_803B7720 * D_803B771C;
            }
        }
        if (clearScrollSpeed == 1) D_803BA87C = 0.0f;
        D_803B7718 = 0;
    }
}

/* _InitScreen (MAT3 trackscr.c, GC 0x800887ac). N64: 17-entry multiblit; BlowFog and setCamera inlined. */
void func_8038DFEC(void)
{
    s32 i;
    s32 j;
    f32 pos;
    D_803BA85A = 0;
    D_803BA878 = 0;
    D_803BA910 = 0;
    D_803BAC78 = -1;
    D_803BA898 = 0;
    for (i = 0; i < 21; i++) if (func_8038ACB0(i)) D_803BA8B0[D_803BA898++] = i;
    func_8038CD14();
    if (D_8014A110 == 3) D_803BA84E = D_8014978C = D_801543D8[D_80154628].trackNum;
    else {
        if (D_8014A110 == 4) D_803BA84E = D_8014978C = func_800CDC3C(D_8014A118[0].save, D_8014A110) + 14;
        else if (D_8014A110 == 5) D_803BA84E = D_8014978C = func_800CDC3C(D_8014A118[0].save, D_8014A110) + 18;
        else if (D_8014A110 == 6) D_803BA84E = D_8014978C = func_800CDC3C(D_8014A118[0].save, D_8014A110) + 6;
        else D_803BA84E = D_8014978C = func_800CDC3C(D_8014A118[0].save, D_8014A110) + 0;
        if (!func_8038B834(D_8014978C)) {
            if (D_8014A110 == 4) D_803BA84E = D_8014978C = 14;
            else if (D_8014A110 == 5) D_803BA84E = D_8014978C = 18;
            else if (D_8014A110 == 6) D_803BA84E = D_8014978C = 6;
            else D_803BA84E = D_8014978C = 0;
        }
        for (i = 0; i < D_8014A108; i++) {
            if (D_8014A110 == 4) menu_text_input(D_8014A118[i].save, D_8014A110, D_8014978C - 14);
            else if (D_8014A110 == 5) menu_text_input(D_8014A118[i].save, D_8014A110, D_8014978C - 18);
            else if (D_8014A110 == 6) menu_text_input(D_8014A118[i].save, D_8014A110, D_8014978C - 6);
            else menu_text_input(D_8014A118[i].save, D_8014A110, D_8014978C);
        }
        for (j = 21; j < 41; j++) D_80146108[j] = reverb_setup(D_8014A118[0].save, j);
    }
    init_state_begin();
    particle_lifetime_set();
    func_8038C93C();
    D_803B7CD0 = sound_control(0, 0, D_803B7A6C, 17);
    { s32 j;
    i = 0;
    for (j = 0; j < 19; j++) {
        if (func_8038B834(j)) {
            if (j == D_8014978C) { D_803BA856 = i; break; }
            i++;
        }
    }
    }
    D_803BA852 = 19;
    i = 0;
    pos = 0.0f;
    for (j = 0; j < 19; j++) {
        if (func_8038B834(j)) {
            D_803BA918[i].gTrackNum = j;
            D_803BA918[i].slot = i - D_803BA856;
            if (D_803BA918[i].slot < 0) D_803BA918[i].slot += D_803BA852;
            if (D_803BA856 == i) D_803BA918[i].targetPos = 0.0f;
            else if (D_803BA856 >= D_803BA852/2) D_803BA918[i].targetPos = pos - (D_803BA852 - D_803BA856)*100;
            else D_803BA918[i].targetPos = pos + D_803BA856*100;
            D_803BA918[i].pos[0] = D_803BA918[i].targetPos;
            D_803BA918[i].pos[1] = -5.0f;
            D_803BA918[i].pos[2] = 0.0f;
            pos -= 100.0f;
            if (pos < -(D_803BA852*100/2)) pos += D_803BA852*100;
            i++;
        }
    }
    D_803BAE10[0] = 0.0f;
    D_803BAE10[1] = D_803B7A20[D_803BA918[D_803BA856].gTrackNum] - 5.0f;
    D_803BAE10[2] = 0.0f;
    func_8038A634(1);
    trk_BlowFog();
    trk_setCamera();
    entity_audio_update();
    D_803B7CD4 = 1;
    D_803BAC78 = -1;
    D_803BAC90 = D_8014978C;
    D_803BACCC = 0.0f;
}

/* updateTrackScreen (MAT3 trackscr.c, GC 0x80087abc). N64 differences read from the retail words: the
 * _InitScreen/showTrackSelect calls come before the player loop, which tests a removed controller pak
 * instead of walking gPlayerFiles; A/B start needs D_80156994 or a track other than 5 (the same gate guards
 * the up/down option moves); effect ids 38/40/39; case 0 sets buttonsJustInited when !D_80156994;
 * options are saved for player 0 after SetOptions; ZeroCamera/UpdateCameras = inlined setCamera. */
void func_8038E5DC(void)
{
    s32 i;
    s32 j;
    s32 add;
    if (D_801174BC != 1) {
        if (D_801174BC != D_801174B4) { func_800B5570(0x100); return; }
        D_801174BC = 1;
    }
    if (!D_803B7CD4) func_8038DFEC();
    func_8038C5EC();
    for (i = 0; i < D_8014A108; i++) {
        if ((*D_8014A118[i].save)->file && func_800A361C((*(*D_8014A118[i].save)->file)->controller)) {
            func_8038B940(); func_800B5570(0x20); return;
        }
    }
    if (D_80157244) { func_8038B940(); func_800B5570(4); return; }
    if ((D_8015694C & 3) && (D_80156994 || D_8014978C != 5)) {
        resource_type_select(D_8015694C); func_800B5570(0x100); func_8038B940();
        for (j = 0; j < D_8014A108; j++)
            for (i = 21; i < 41; i++) audio_bus_mix(D_8014A118[j].save, i, D_80146108[i]);
        return;
    }
    if (D_8015694C & 4) {
        entity_flags_apply(38, 0, 1, 0); func_8038B940();
        if (D_8014A110 == 3) func_800B5570(0x4000);
        else func_800B5570(0x40);
        return;
    }
    if (D_80156944 & 0x3080) {
        add = (D_80149784 & 0x1000) ? -1 : (D_80149784 & 0x2000) ? 1 : 0;
        if (D_8014A110 == 3 && (D_80154628 > 0 || D_803BA8B0[D_803BA85A] != 20)) goto controls_done;
        if (add == 0 && !D_803BA910) goto controls_done;
        if (D_8014A110 == 2 && D_803BA8B0[D_803BA85A] == 2) goto controls_done;
        audio_doppler(D_80149784);
        if (D_8013F1D9 > 1) add = -add;
        switch (D_803BA8B0[D_803BA85A]) {
        case 0:
            if (add) {
                D_803BA856 += add;
                if (D_803BA856 < 0) D_803BA856 = D_803BA852 - 1;
                else if (D_803BA856 >= D_803BA852) D_803BA856 = 0;
                do {
                    D_8014978C += add;
                    if (D_8014978C < 0) D_8014978C = 18;
                    else if (D_8014978C >= 19) D_8014978C = 0;
                } while (!func_8038B834(D_8014978C));
                for (i = 21; i < 41; i++) D_80146108[i] = reverb_setup(D_8014A118[0].save, i);
                if (!D_80156994) D_803B7718 = 1;
            }
            break;
        case 1:
            if (add) {
                D_80146108[25] += add;
                if (D_80146108[25] < 0) D_80146108[25] = 1;
                else if (D_80146108[25] > 1) D_80146108[25] = 0;
            } else D_80146108[25] = 0;
            break;
        case 2:
            if (add) {
                D_80146108[21] += add;
                if (D_80146108[21] < 1) D_80146108[21] = 8;
                else if (D_80146108[21] > 8) D_80146108[21] = 1;
            } else D_80146108[21] = 3;
            break;
        case 3:
            if (add) {
                D_80146108[32] += add;
                if (D_80146108[32] < 0) D_80146108[32] = 2;
                else if (D_80146108[32] > 2) D_80146108[32] = 0;
            } else D_80146108[32] = 0;
            break;
        case 4:
            if (add) {
                if (!D_80146108[33]) D_80146108[33] = 1;
                else D_80146108[33] = 0;
            } else D_80146108[33] = 0;
            break;
        case 5:
            if (add) {
                D_80146108[34] += add;
                if (D_80146108[34] <= 0) D_80146108[34] = 100;
                else if (D_80146108[34] > 100) D_80146108[34] = 1;
            } else D_80146108[34] = 69;
            break;
        case 6:
            if (add) {
                D_80146108[35] += add;
                if (D_80146108[35] <= 0) D_80146108[35] = 20;
                else if (D_80146108[35] > 20) D_80146108[35] = 1;
            } else D_80146108[35] = 5;
            break;
        case 7:
            if (add) {
                D_80146108[36] += add;
                if (D_80146108[36] <= 0) D_80146108[36] = 50;
                else if (D_80146108[36] > 50) D_80146108[36] = 1;
            } else D_80146108[36] = 30;
            break;
        case 8:
            if (add) {
                if (!D_80146108[37]) D_80146108[37] = 1;
                else D_80146108[37] = 0;
            } else D_80146108[37] = 0;
            break;
        case 9:
            if (add) {
                D_80146108[38] += 5*add;
                if (D_80146108[38] < 5) D_80146108[38] = 50;
                else if (D_80146108[38] > 50) D_80146108[38] = 5;
            } else D_80146108[38] = 10;
            break;
        case 10:
            if (add) {
                D_80146108[39] += add;
                if (D_80146108[39] <= 0) D_80146108[39] = 20;
                else if (D_80146108[39] > 20) D_80146108[39] = 1;
            } else D_80146108[39] = 8;
            break;
        case 11:
            if (add) {
                D_80146108[40] ^= 1;
            } else D_80146108[40] = 0;
            break;
        case 12:
            if (add) {
                D_80146108[27] ^= 1;
            } else D_80146108[27] = 0;
            func_8038B388(); func_8038AEB0(); D_803B7CE0 = 1.0f - D_803B7CE0;
            break;
        case 13:
            if (add) {
                if ((D_8014A118[0].levels & 0x3c000) == 0x3c000) D_80146108[28] = 2;
                else D_80146108[28] = !D_80146108[28];
            } else D_80146108[28] = 0;
            func_8038B388(); func_8038AEB0();
            break;
        case 14:
            if (add) {
                D_80146108[29] += add;
                if (D_80146108[29] < 0) D_80146108[29] = 3;
                else if (D_80146108[29] > 3) D_80146108[29] = 0;
            } else D_80146108[29] = 0;
            break;
        case 15:
            if (add) {
                D_80146108[30] += add;
                if (D_80146108[30] < 0) D_80146108[30] = 4;
                else if (D_80146108[30] > 4) D_80146108[30] = 0;
            } else D_80146108[30] = 0;
            break;
        case 16:
            if (add) {
                D_80146108[23] += add;
                if (D_80146108[23] < 0) D_80146108[23] = 3;
                else if (D_80146108[23] > 3) D_80146108[23] = 0;
            } else D_80146108[23] = 1;
            break;
        case 17:
            if (add) {
                D_80146108[22] += add;
                if (D_80146108[22] > 5) D_80146108[22] = 0;
                else if (D_80146108[22] < 0) D_80146108[22] = 5;
            } else D_80146108[22] = 5;
            break;
        case 18:
            if (add) {
                D_80146108[24] += add;
                if (D_80146108[24] < 0) D_80146108[24] = 5;
                else if (D_80146108[24] > 5) D_80146108[24] = 0;
            } else D_80146108[24] = 2;
            break;
        case 19:
            if (add) {
                D_80146108[26] += add;
                if (D_80146108[26] < 0) D_80146108[26] = 2;
                else if (D_80146108[26] > 2) D_80146108[26] = 0;
            } else D_80146108[26] = 2;
            break;
        case 20:
            if (add) {
                D_80146108[31] ^= 1;
            } else D_80146108[31] = 0;
            if (D_8014A110 == 3) (*(*D_8014A118[D_801543D4].save)->stats)[1785] = D_80146108[31];
            break;
        }
        init_state_begin();
        for (i = 21; i < 41; i++) audio_bus_mix(D_8014A118[0].save, i, D_80146108[i]);
    } else if (D_80149784 & 0x400) {
        if (D_80156994 || D_8014978C != 5) {
            entity_flags_apply(40, 0, 1, 0);
            if (--D_803BA85A < 0) D_803BA85A = D_803BA898 - 1;
        }
    } else if (D_80149784 & 0x800) {
        if (D_80156994 || D_8014978C != 5) {
            entity_flags_apply(39, 0, 1, 0);
            if (++D_803BA85A > D_803BA898 - 1) D_803BA85A = 0;
        }
    }
    if (D_803BA878 > D_803BA85A - 1) D_803BA878 = D_803BA85A - 1;
    if (D_803BA878 < D_803BA85A - 2) D_803BA878 = D_803BA85A - 2;
    if (D_803BA878 > D_803BA898 - 4) D_803BA878 = D_803BA898 - 4;
    if (D_803BA878 < 0) D_803BA878 = 0;
controls_done:
    D_803BA908 = D_8014A110 != 3 || (D_80154628 == 0 && D_803BA8B0[D_803BA85A] == 20);
    if (D_8014978C != D_803BA84E) {
        D_803BAC90 = D_8014978C;
        D_803BA84E = D_8014978C;
        for (i = 0; i < D_8014A108; i++) {
            switch (D_8014A110) {
            case 0: case 1: case 2:
                if (D_8014978C >= 0 && D_8014978C <= 5) menu_text_input(D_8014A118[i].save, D_8014A110, D_8014978C + 0);
                break;
            case 4:
                if (D_8014978C >= 14 && D_8014978C <= 17) menu_text_input(D_8014A118[i].save, D_8014A110, D_8014978C - 14);
                break;
            case 6:
                if (D_8014978C >= 6 && D_8014978C <= 13) menu_text_input(D_8014A118[i].save, D_8014A110, D_8014978C - 6);
                break;
            }
        }
    }
    func_8038B3EC();
    func_8038A820();
    func_8038A634(0);
    trk_setCamera();
}

