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
void di_setButtonColor(s16 objnum,Color *col) { D_8012E700[objnum].color=col->rgba; }
typedef struct OSMesgQueue OSMesgQueue;
typedef struct Msg { s16 id; s8 type, used; u8 payload[20]; } Msg;
extern s32 D_801427A8;
Msg *func_80091B00(void);
void func_80091AF0(void);
void func_80091AF8(void);
s32 osSendMesg(OSMesgQueue *, void *, s32);
void di_ProcessCommand(Msg *task)
{
    osSendMesg((OSMesgQueue *) &D_801427A8, task, 0);
}
void di_post_type1(void)
{
    Msg *task;
    func_80091AF0();
    task = func_80091B00(); task->type = 1;
    func_80091AF8();
    di_ProcessCommand(task);
}
extern void func_800B8810(void);
void drone_pathfind_main(void)
{
    s32 u0, u1;

    func_800B8810();
    drone_collision_avoid();
    if (state_word_a & 0x7C03FFFE) {
        particle_lifetime_set();
        {
        Color buttonColor = {255, 255, 255, 255};
        TexDef *tex;
        unsigned short texIdx;
        s32 buttonIdx;
        for (buttonIdx = 0; buttonIdx < 12; buttonIdx++) {
            if (D_801102B4[buttonIdx].handle == -1) {
                memcpy(D_801102B4[buttonIdx].mat, D_8011418C, 36);
                D_801102B4[buttonIdx].handle = sign_extend_call(
                    string_copy_format(D_80110290[D_801102B4[buttonIdx].type], 0,
                                       (s8)(gNumModels - 1), 1),
                    &D_801102B4[buttonIdx].mat, -1,
                    (D_801102B4[buttonIdx].type == 0 ||
                     D_801102B4[buttonIdx].type == 1 ||
                     D_801102B4[buttonIdx].type == 2) ? 0x42000 : 0);
                di_setButtonColor(D_801102B4[buttonIdx].handle, &buttonColor);
            }
        }
        tex = func_800B24EC(D_8011F6F0, &texIdx, 0, (s8)(gNumModels - 1), 1);
        func_8008D870(D_801102B4[8].handle, tex, -1);
        tex = func_800B24EC(D_8011F6F8, &texIdx, 0, (s8)(gNumModels - 1), 1);
        func_8008D870(D_801102B4[9].handle, tex, -1);
        memcpy(D_801102B4[9].mat, D_8011418C, 36);
        func_800B5898(-1.5707964f, D_801102B4[9].mat);
        D_801102B4[9].pos[0] = -75.1f;
        D_801102B4[9].pos[1] = 55.2f;
        D_801102B4[9].pos[2] = 100.0f;
        func_8008B32C(D_801102B4[9].mat, D_801102B4[9].mat, 0.5f);
        memcpy(D_801102B4[10].mat, D_8011418C, 36);
        func_800B5898(-1.5707964f, D_801102B4[10].mat);
        D_801102B4[10].pos[0] = -87.1f;
        D_801102B4[10].pos[1] = 55.2f;
        D_801102B4[10].pos[2] = 100.0f;
        tex = func_800B24EC(D_8011F6FC, &texIdx, 0, (s8)(gNumModels - 1), 1);
        func_8008D870(D_801102B4[11].handle, tex, -1);
        do {} while (0);
        memcpy(D_801102B4[11].mat, D_8011418C, 36);
        func_800B5898(-1.5707964f, D_801102B4[11].mat);
        D_801102B4[11].pos[0] = 0.0f;
        D_801102B4[11].pos[1] = 115.0f;
        D_801102B4[11].pos[2] = 200.0f;
        D_801105B4 = 1;
        D_801105B8 = 1;
        particle_velocity_set();
}
        D_80110630 = sound_control(0,0,&D_801105C0,1);
    } else D_80110630 = sound_control(0,0,&D_801105E4,2);
    entity_audio_update();
    D_8011063C = 0;
    D_80110640 = D_80146108.track;
    if (D_80110638 == 1 || D_80110638 == 2) {
        di_post_type1();
        speed_mode0_wrapper(D_80146108.music / 10.0f, 0.05f);
        if (D_80110640 < 12) sync_entry_register(D_80110640,1);
        else if (D_80110640 == 12) {
            if (state_word_a & 0x7C03FFFE) sync_entry_register(6,1);
            else physics_init_mode0();
        }
    }
    D_8011062C = 1;
}


typedef struct TextState { void *word0; char **single; void *word8; u16 *tab; char **taba; } TextState;
typedef struct AudioMenuRect { s32 x,w,h,cx; } AudioMenuRect;
extern TextState countdown_state;extern AudioMenuRect D_80110650;
extern char *D_80110030[];
extern OSMesgQueue D_801461D0;
s32 osRecvMesg(OSMesgQueue *, void *, s32);
s32 slot_state_setup(s32);u32 object_manager_update(void *,s16);s16 object_bytes_sum_global(void);void *func_800BE6A4(void *,void *);
s8 func_800D6E74();
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
    du_SetFont(10);
    optionWidth=0;
    valueWidth=0;
    menuCount=0;
    for(menuIdx=0;menuIdx<4;menuIdx++) {
        optionWidth = object_manager_update(countdown_state.taba[countdown_state.tab[39]+menuIdx],-1)>optionWidth ? object_manager_update(countdown_state.taba[countdown_state.tab[39]+menuIdx],-1):optionWidth;
        if(func_800D6E74(menuIdx)) {
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
