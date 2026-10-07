/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Image B health/status bar callback. Four float thresholds come from the authenticated image B asset. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef struct Blit {
    char *Name; void *Image; void *Info;
    s16 TexIndex, X, Y; u16 State; s16 Width, Height;
    u8 Alpha, Flip; s8 Hide, Init;
    s16 Top, Bot, Left, Right, Color; u16 reserved26;
    s32 (*AnimFunc)(struct Blit *); u32 AnimID;
    s32 AnimDTA; u16 BLIdx;
} Blit;
typedef struct Player {
    u8 unknown000[0x386];
    s16 health;
    u8 unknown388[0x30];
} Player;
typedef struct TextureState { u8 unknown00[21]; u8 flags; u8 unknown16[10]; } TextureState;
typedef struct ColorPair { u8 bright[4], dark[4]; } ColorPair;
typedef struct ColorDescriptor { u8 unknown00[20]; ColorPair *colors; } ColorDescriptor;
extern s16 D_8014A108, D_80151AD0;
extern s32 D_80394150[4][4][2];
extern Player D_80152818[];
extern TextureState D_80140BF0[];
extern ColorPair D_80394280[4];
extern ColorDescriptor D_80395060[4];

extern s8 input_new_data_wrapper(Blit *, s32);
extern void Input_ApplyPadConfig(Blit *);

s32 func_80392FE4(Blit *blt)
{
    s32 player = (blt->AnimID & 0xf0) >> 4;
    s32 segment = blt->AnimID & 15;
    float health, blend;
    if (player >= D_8014A108) {
        blt->AnimFunc = 0;
        return input_new_data_wrapper(blt, 1);
    }
    health = D_80152818[player].health / 800.0f;
    blt->X = D_80394150[D_80151AD0 - 1][player][0] - blt->Width / 2;
    blt->Y = D_80394150[D_80151AD0 - 1][player][1] - blt->Height / 2;
    if (segment == 0) {
        D_80140BF0[blt->BLIdx].flags |= 1;
    } else {
        blt->X += 8;
        blt->Left = 8;
        blt->Right = (blt->Width - 17) * health + 8.0f;
        if (health > 0.8f) {
            D_80394280[player].bright[0] = 0;
            D_80394280[player].bright[1] = 224;
            D_80394280[player].bright[2] = 0;
            D_80394280[player].dark[0] = 0;
            D_80394280[player].dark[1] = 112;
            D_80394280[player].dark[2] = 0;
        } else if (health > 0.6f) {
            blend = (0.8f - health) * 5.0f;
            D_80394280[player].bright[0] = (u8)(blend * 224.0f);
            D_80394280[player].bright[1] = 224;
            D_80394280[player].bright[2] = 0;
            D_80394280[player].dark[0] = (u8)(blend * 112.0f);
            D_80394280[player].dark[1] = 112;
            D_80394280[player].dark[2] = 0;
        } else if (health > 0.4f) {
            D_80394280[player].bright[0] = 224;
            D_80394280[player].bright[1] = 224;
            D_80394280[player].bright[2] = 0;
            D_80394280[player].dark[0] = 112;
            D_80394280[player].dark[1] = 112;
            D_80394280[player].dark[2] = 0;
        } else if (health > 0.2f) {
            blend = (health - 0.2f) * 5.0f;
            D_80394280[player].bright[0] = 224;
            D_80394280[player].bright[1] = (u8)(blend * 224.0f);
            D_80394280[player].bright[2] = 0;
            D_80394280[player].dark[0] = 112;
            D_80394280[player].dark[1] = (u8)(blend * 112.0f);
            D_80394280[player].dark[2] = 0;
        } else {
            D_80394280[player].bright[0] = 224;
            D_80394280[player].bright[1] = 0;
            D_80394280[player].bright[2] = 0;
            D_80394280[player].dark[0] = 112;
            D_80394280[player].dark[1] = 0;
            D_80394280[player].dark[2] = 0;
        }
        D_80395060[player].colors = &D_80394280[player];
        blt->Image = &D_80395060[player];
    }
    Input_ApplyPadConfig(blt);
    return 1;
}
