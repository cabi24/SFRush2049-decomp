/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct tPalette {            /* anon_name_start_18 */
    char name[16];
    u8 start, end;
    u16 format;
    char *palBase;
} tPalette;
typedef struct _tBlit_2 {
    const char *name;
    tPalette *palette;
    void *texture;
    u16 texIndex;
    s16 x, y;
    u16 z;
    s16 width, height;
    u8 alpha, flip;
    char hide, init;
    s16 top, bot, left, right, color;
    int (*animFunc)(struct _tBlit_2 *);
    u32 animID;
    int animDTA;
    u16 blidx;
    void *data;
    struct _tBlit_2 *child;
} _tBlit_2;
typedef struct tBlitInstance {       /* anon_palette_texture_20, N64 form */
    tPalette *palette;
    void *texture;
    u16 texIndex;
    s16 x, y;
    u16 z;
    s16 width, height;
    u8 alpha, flags;
    u8 pad16[10];
} tBlitInstance;
typedef struct tCarData {
    u8 unknown000[0x386];
    s16 health;                      /* battle.health */
    u8 unknown388[0x30];
} tCarData;

extern s16 active_player_count, D_80151AD0;
extern tCarData player_array[];
extern tBlitInstance pad_config[];
extern s32 D_80394150[4][4][2];
extern u8 D_80394280[4][2][4];
extern tPalette D_80395060[4];
#define gNumPlayers active_player_count
#define gNumWindows D_80151AD0
#define gGameCar player_array
#define gBlitInstanceList pad_config
#define healthPos D_80394150
#define healthColor D_80394280
#define healthPal D_80395060

extern int input_new_data_wrapper(_tBlit_2 *, int);   /* Hidden */
extern void Input_ApplyPadConfig(_tBlit_2 *);          /* UpdateBlit */
#define Hidden input_new_data_wrapper
#define UpdateBlit Input_ApplyPadConfig

int func_80392FE4(_tBlit_2 *blt)
{
    int slot = (blt->animID & 0xf0) >> 4;
    int type = blt->animID & 15;
    float healthPct;
    /*@{*//*@|u8 (*color)[4];@}*/
    if (slot >= gNumPlayers) {
        blt->animFunc = 0;
        return Hidden(blt, 1);
    }
    healthPct = gGameCar[slot].health / 800.0f;
    blt->x = healthPos[gNumWindows-1][slot][0] - blt->width / 2;
    blt->y = healthPos[gNumWindows-1][slot][1] - blt->height / 2;
    if (/*@{*/type == 0/*@|!type@}*/) {
        gBlitInstanceList[blt->blidx].flags |= 1;
    } else {
        /*@{*/blt->x += 8;
        blt->left = 8;
        blt->right = 8.0f + (blt->width - 17) * healthPct;/*@|blt->x += 8; blt->left = 8; blt->right = 8.0f + (blt->width - 17) * healthPct;@|blt->x = blt->x + 8;
        blt->left = 8;
        blt->right = 8.0f + (blt->width - 17) * healthPct;@}*/
        /*@{*/if (healthPct > 0.8f) {
            healthColor[slot][0][0] = 0; healthColor[slot][0][1] = 224; healthColor[slot][0][2] = 0;
            healthColor[slot][1][0] = 0; healthColor[slot][1][1] = 112; healthColor[slot][1][2] = 0;
        }/*@|if (healthPct > 0.8f) {
            healthColor[slot][0][0] = 0;
            healthColor[slot][0][1] = 224;
            healthColor[slot][0][2] = 0;
            healthColor[slot][1][0] = 0;
            healthColor[slot][1][1] = 112;
            healthColor[slot][1][2] = 0;
        }@|if (0.8f < healthPct) {
            healthColor[slot][0][0] = 0; healthColor[slot][0][1] = 224; healthColor[slot][0][2] = 0;
            healthColor[slot][1][0] = 0; healthColor[slot][1][1] = 112; healthColor[slot][1][2] = 0;
        }@}*/ else if (healthPct > 0.6f) {
            healthColor[slot][0][0] = 224.0f * (5.0f * (0.8f - healthPct));
            healthColor[slot][0][1] = 224;
            healthColor[slot][0][2] = 0;
            healthColor[slot][1][0] = 112.0f * (5.0f * (0.8f - healthPct));
            healthColor[slot][1][1] = 112;
            healthColor[slot][1][2] = 0;
        } else if (healthPct > 0.4f) {
            healthColor[slot][0][0] = 224; healthColor[slot][0][1] = 224; healthColor[slot][0][2] = 0;
            healthColor[slot][1][0] = 112; healthColor[slot][1][1] = 112; healthColor[slot][1][2] = 0;
        } else if (healthPct > 0.2f) {
            healthColor[slot][0][0] = 224;
            healthColor[slot][0][1] = 224.0f * (5.0f * (healthPct - 0.2f));
            healthColor[slot][0][2] = 0;
            healthColor[slot][1][0] = 112;
            healthColor[slot][1][1] = 112.0f * (5.0f * (healthPct - 0.2f));
            healthColor[slot][1][2] = 0;
        } else {
            healthColor[slot][0][0] = 224; healthColor[slot][0][1] = 0; healthColor[slot][0][2] = 0;
            healthColor[slot][1][0] = 112; healthColor[slot][1][1] = 0; healthColor[slot][1][2] = 0;
        }
        healthPal[slot].palBase = (char *)healthColor[slot];
        blt->palette = &healthPal[slot];
    }
    UpdateBlit(blt);
    return 1;
}
