/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Image B, 0x80392FE4..0x80393518 (1332 bytes): animHealth, the battle-mode
 * health bar blit callback (animID bits 4-7 = player slot, bits 0-3 = part).
 * Hides itself for absent players; part 0 marks the frame instance, other
 * parts size the bar from battle health (/800) and pick a green-yellow-red
 * two-row palette.
 * Donor: MAT3 GameCube decomp, GameSource/src/hudextra.c animHealth
 * (GC 0x80023754, size 0x334); typed port, MAT3 names and statement order,
 * N64 offsets. N64 differences: bar inset 8/17 (GC 16/33), unhalved blit
 * size, slot decoded as (animID & 0xf0) >> 4, N64 instance flags byte +0x15.
 * The block-local `inst` pointer for the frame part is MAT3's own form and is
 * load-bearing: indexing gBlitInstanceList directly opens a .noalias region
 * whose `.alias` after the arm's `b` stops as1's cross-block hoisting into the
 * > 0.8 arm (listing oracle: deleting that one directive closed the last 14
 * words). Earlier research: Astra's lean packet
 * cloud/work/lean/runtime_b_health_bar_20261006 (14/333), credited for the
 * record layouts. Historical callee names: input_new_data_wrapper = Hidden,
 * Input_ApplyPadConfig = UpdateBlit; pad_config = gBlitInstanceList,
 * player_array = gGameCar (health = battle.health at +0x386).
 * Own .rodata: 0.8f, 0.6f, 0.4f, 0.2f (retail 0x80394F54..0x80394F64, checked).
 */
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

int func_80392FE4(_tBlit_2 *blt) /* animHealth */
{
    int slot = (blt->animID & 0xf0) >> 4;
    int type = blt->animID & 15;
    float healthPct;
    if (slot >= gNumPlayers) {
        blt->animFunc = 0;
        return Hidden(blt, 1);
    }
    healthPct = gGameCar[slot].health / 800.0f;
    blt->x = healthPos[gNumWindows-1][slot][0] - blt->width / 2;
    blt->y = healthPos[gNumWindows-1][slot][1] - blt->height / 2;
    if (type == 0) {
        { tBlitInstance *inst = &gBlitInstanceList[blt->blidx]; inst->flags |= 1; }
    } else {
        blt->x += 8;
        blt->left = 8;
        blt->right = 8.0f + (blt->width - 17) * healthPct;
        if (healthPct > 0.8f) {
            healthColor[slot][0][0] = 0; healthColor[slot][0][1] = 224; healthColor[slot][0][2] = 0;
            healthColor[slot][1][0] = 0; healthColor[slot][1][1] = 112; healthColor[slot][1][2] = 0;
        } else if (healthPct > 0.6f) {
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
