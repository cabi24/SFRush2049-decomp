/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16; typedef int s32; typedef unsigned int u32; typedef float f32;
typedef struct TexDef TexDef; typedef struct Blit Blit; typedef struct MultiBlit MultiBlit;
typedef struct { s32 type, handle; f32 ang; f32 mat[3][3]; f32 pos[3]; u8 pad3C[4]; } Button;
typedef struct Ent {
    /* 0x00 */ s32 w0;
    /* 0x04 */ s32 w4;
    /* 0x08 */ void *owner;
    /* 0x0C */ f32 scaleA;
    /* 0x10 */ f32 scaleB;
    /* 0x14 */ u16 id;          /* 0xFFFF = free */
    /* 0x16 */ s16 link[3];     /* -1 = none */
    /* 0x1C */ s32 w28[8];
    u32 color;
    /* 0x40 */ s32 w64;
} Ent; /* size 0x44 */


typedef union Color { u8 c[4]; u32 rgba; } Color;
typedef struct { u8 pad0[12]; s8 music; s8 sfx; s8 track; s8 mono; } AudioCfg;
extern AudioCfg D_80146108;
extern Button D_801102B4[12]; extern Ent D_8012E700[];
extern char *D_80110290[]; extern u8 gNumModels; extern f32 D_8011418C[3][3];
extern char D_8011F6F0[],D_8011F6F8[],D_8011F6FC[];
extern const MultiBlit D_801105C0,D_801105E4;
extern Blit *D_80110630;
extern s32 D_801105B4,D_801105B8,D_80110638,D_8011063C,D_80110640;
extern s8 D_80117428,D_8011062C; extern u32 state_word_a;
void *memcpy(void *,const void *,u32);
s32 string_copy_format(char *,s8,s8,s32);
s16 func_8008E26C(s32,void *,s16,s32);
TexDef *func_800B24EC(char *,u16 *,s8,s8,s32);
void func_8008D870(s16,TexDef *,s32);
void func_800B5898(f32,f32 [3][3]); void func_8008B32C(f32 [3][3],f32 [3][3],f32);
void particle_lifetime_set(void); void particle_velocity_set(void); void entity_audio_update(void);
Blit *sound_control(s16,s16,const MultiBlit *,s16);
void drone_collision_avoid(void);void object_type1_create(void);void speed_mode0_wrapper(f32,f32);void sync_entry_register(s32,s32);void physics_init_mode0(void);
s16 sign_extend_call(s32,void *,s32,u32);void func_800B8810(void);
typedef struct OSMesgQueue OSMesgQueue;
s32 osSendMesg(OSMesgQueue *, void *, s32);

typedef struct TextState { void *word0; char **single; void *word8; u16 *tab; char **taba; } TextState;
typedef struct AudioMenuRect { s32 x,w,h,cx; } AudioMenuRect;
extern TextState countdown_state;extern AudioMenuRect D_80110650;
extern char *D_80110030[];
extern OSMesgQueue D_801461D0;
s32 osRecvMesg(OSMesgQueue *, void *, s32);
s32 slot_state_setup(s32);u32 object_manager_update(void *,s16);s16 object_bytes_sum_global(void);void *func_800BE6A4(void *,void *);
s8 du_validOption(s32 option) { s8 valid = 1; return valid; }
void du_gfx_lock(void) { osRecvMesg(&D_801461D0, 0, 1); }
void du_gfx_unlock(void) { osSendMesg(&D_801461D0, 0, 0); }
s32 du_font_set(s32 font)
{
    s32 old;
    du_gfx_lock();
    old = slot_state_setup(font);
    du_gfx_unlock();
    return old;
}
void du_SetFont(s32 font)
{
    du_font_set(font);
}
void drone_collision_avoid(void)
{
    s32 menuIdx,menuCount,optionWidth,valueWidth;
    u8 str[40];
    u8 *charPtr;
    s32 songIdx;
    du_font_set(10);
    optionWidth=0;
    valueWidth=0;
    menuCount=0;
    for(menuIdx=0;menuIdx<4;menuIdx++) {
        optionWidth = object_manager_update(countdown_state.taba[countdown_state.tab[39]+menuIdx],-1)>optionWidth ? object_manager_update(countdown_state.taba[countdown_state.tab[39]+menuIdx],-1):optionWidth;
        if(du_validOption(menuIdx)) {
            switch(menuIdx) {
            case 0: case 1:
                valueWidth=object_manager_update("10",-1)>valueWidth ? object_manager_update("10",-1):valueWidth;
                break;
            case 2:
                for(songIdx=0;songIdx<14;songIdx++) {
                    if(songIdx<12) func_800BE6A4(str,D_80110030[songIdx]);
                    else if(songIdx==12) func_800BE6A4(str,countdown_state.single[34]);
                    else if(songIdx==13) func_800BE6A4(str,countdown_state.taba[countdown_state.tab[16]]);
                    for(charPtr=str;*charPtr != 0;charPtr++)
                        *charPtr=(*charPtr>='a' && *charPtr<='z') ? *charPtr^0x20:*charPtr;
                    valueWidth=object_manager_update(str,-1)>valueWidth ? object_manager_update(str,-1):valueWidth;
                }
                break;
            case 3:
                valueWidth=object_manager_update(countdown_state.taba[countdown_state.tab[40]],-1)>valueWidth ? object_manager_update(countdown_state.taba[countdown_state.tab[40]],-1):valueWidth;
                valueWidth=object_manager_update(countdown_state.taba[countdown_state.tab[40]+1],-1)>valueWidth ? object_manager_update(countdown_state.taba[countdown_state.tab[40]+1],-1):valueWidth;
                break;
            }
            menuCount++;
        }
    }
    D_80110650.w=optionWidth+valueWidth+10;
    D_80110650.h=menuCount*object_bytes_sum_global();
    D_80110650.x=160-D_80110650.w/2;
    D_80110650.cx=D_80110650.x+optionWidth+5;
}
