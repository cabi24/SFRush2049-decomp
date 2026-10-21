/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Image A track-select screen: the whole N64 trackscr.c IPA unit, root updateTrackScreen (func_8038E5DC,
 * kept; called by the game code). Donor: MAT3 GameSource trackscr.c (GC 0x80087abc..0x8008d30c), direct
 * typed ports, N64 offsets/addresses from the retail words. Identities (call graph + MAT3 statement order):
 *   func_8038E5DC updateTrackScreen   func_8038DFEC _InitScreen       func_8038B940 _UninitScreen
 *   func_8038C5EC showTrackSelect     func_8038B834 TrackValid        func_8038ACB0 OptionValid
 *   func_8038A634 SetFogIntensity     func_8038AE50 unloadTrackScreen func_8038AEB0 buildTrackMap
 *   func_8038B388 clearTrackMap       func_8038B3EC animTrackMap      func_8038CD14 buildTrackMaps
 *   func_8038A820 animTrackMaps       func_8038C93C initButtons       func_8038BBB4 updateButtons
 * Inlined statics, named by their retail 8-byte deleted stubs (the compiled object emits them at those places):
 *   func_8038A62C setCamera, func_8038C934 BlowFog, func_8038AEA8 loadTrackScreen, func_8038CCE4 unloadPathData,
 *   func_8038CCEC loadPathData, func_8038CCF4 buildRaceTrackMaps, func_8038CCFC buildObstacleTrackMaps,
 *   func_8038CD04 buildBattleTrackMaps, func_8038CD0C buildStuntTrackMaps.
 * Every function except the kept root has the private IPA ABI (no callee saves; the root saves s0-s8 and
 * $f20-$f30), so members match only in this unit. Function order is MAT3's file order (as1 breaks scheduling
 * ties by source line, and the inlined helpers' lines must follow their callers).
 * Bodies: w36d (E5DC, DFEC, BBB4, CD14, ACB0, B834 ports); w36e (AE50, B388, B940, AEA8, AEB0; cloud/work/frontier/
 * w36e/groups/best_ts); dot_runtime_a_color_smoothing_20261006 (A634); C93C = the installed member of
 * dot_runtime_a_c93c_closure_20261012, text unchanged. C5EC/B3EC/A820 are stand-ins (context only).
 * Strict in this unit: C93C, BBB4, DFEC, ACB0, B834, A634, AE50, B388, B940. E5DC is one instruction off (case 14's
 * gOptions[29] = 3 store is absolute in retail); CD14/AEB0/B3EC/C5EC/A820 are context.
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
void func_8038C934(void); void func_8038A62C(void); void func_8038AEA8(void); void func_8038B938(void); void func_8038B930(void);
void func_8038CCF4(void); void func_8038CD0C(void); void func_8038CD04(void); void func_8038CCFC(void);
void func_8038CCEC(void); void func_8038CCE4(void);
void func_8038C934(void); void func_8038A62C(void); void func_8038AEA8(void); void func_8038B938(void); void func_8038B930(void);
void func_8038CCF4(void); void func_8038CD0C(void); void func_8038CD04(void); void func_8038CCFC(void);
void func_8038CCEC(void); void func_8038CCE4(void);
void func_8038C934(void); void func_8038A62C(void); void func_8038AEA8(void); void func_8038B938(void); void func_8038B930(void);
void func_8038CCF4(void); void func_8038CD0C(void); void func_8038CD04(void); void func_8038CCFC(void);
void func_8038CCEC(void); void func_8038CCE4(void);
void func_8038C934(void); void func_8038A62C(void); void func_8038AEA8(void); void func_8038B938(void); void func_8038B930(void);
void func_8038CCF4(void); void func_8038CD0C(void); void func_8038CD04(void); void func_8038CCFC(void);
void func_8038CCEC(void); void func_8038CCE4(void);
void func_8038C934(void); void func_8038A62C(void); void func_8038AEA8(void); void func_8038B938(void); void func_8038B930(void);
void func_8038CCF4(void); void func_8038CD0C(void); void func_8038CD04(void); void func_8038CCFC(void);
void func_8038CCEC(void); void func_8038CCE4(void);
void func_8038C934(void); void func_8038A62C(void); void func_8038AEA8(void); void func_8038B938(void); void func_8038B930(void);
void func_8038CCF4(void); void func_8038CD0C(void); void func_8038CD04(void); void func_8038CCFC(void);
void func_8038CCEC(void); void func_8038CCE4(void);
void func_8038C934(void); void func_8038A62C(void); void func_8038AEA8(void); void func_8038B938(void); void func_8038B930(void);
void func_8038CCF4(void); void func_8038CD0C(void); void func_8038CD04(void); void func_8038CCFC(void);
void func_8038CCEC(void); void func_8038CCE4(void);
void func_8038C934(void); void func_8038A62C(void); void func_8038AEA8(void); void func_8038B938(void); void func_8038B930(void);
void func_8038CCF4(void); void func_8038CD0C(void); void func_8038CD04(void); void func_8038CCFC(void);
void func_8038CCEC(void); void func_8038CCE4(void);
void func_8038C934(void); void func_8038A62C(void); void func_8038AEA8(void); void func_8038B938(void); void func_8038B930(void);
void func_8038CCF4(void); void func_8038CD0C(void); void func_8038CD04(void); void func_8038CCFC(void);
void func_8038CCEC(void); void func_8038CCE4(void);
void func_8038C934(void); void func_8038A62C(void); void func_8038AEA8(void); void func_8038B938(void); void func_8038B930(void);
void func_8038CCF4(void); void func_8038CD0C(void); void func_8038CD04(void); void func_8038CCFC(void);
void func_8038CCEC(void); void func_8038CCE4(void);
void func_8038C934(void); void func_8038A62C(void); void func_8038AEA8(void); void func_8038B938(void); void func_8038B930(void);
void func_8038CCF4(void); void func_8038CD0C(void); void func_8038CD04(void); void func_8038CCFC(void);
void func_8038CCEC(void); void func_8038CCE4(void);
void func_8038C934(void); void func_8038A62C(void); void func_8038AEA8(void); void func_8038B938(void); void func_8038B930(void);
void func_8038CCF4(void); void func_8038CD0C(void); void func_8038CD04(void); void func_8038CCFC(void);
void func_8038CCEC(void); void func_8038CCE4(void);
void func_8038C934(void); void func_8038A62C(void); void func_8038AEA8(void); void func_8038B938(void); void func_8038B930(void);
void func_8038CCF4(void); void func_8038CD0C(void); void func_8038CD04(void); void func_8038CCFC(void);
void func_8038CCEC(void); void func_8038CCE4(void);
