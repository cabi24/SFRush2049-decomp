typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16; typedef int s32; typedef unsigned int u32;
typedef struct TextState { void *word0; char **single; void *word8; u16 *tab; char **taba; } TextState;
typedef struct AudioMenuRect { s32 x,w,h,cx; } AudioMenuRect;
extern TextState countdown_state;extern AudioMenuRect D_80110650;
extern char *D_80110030[];
typedef struct OSMesgQueue OSMesgQueue;
extern OSMesgQueue D_801461D0;
s32 osRecvMesg(OSMesgQueue *, void *, s32);
s32 osSendMesg(OSMesgQueue *, void *, s32);
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
