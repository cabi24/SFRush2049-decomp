/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_80109A60 -- minimap dot Blit AnimFunc, N64 port of arcade game/hud.c AnimateDot (Hidden is the
 * arcade hud.c helper; umerge inlines it, so it is a static copy here -- state_update_global.c defines
 * the kept Hidden).
 * Picks the player slot from AnimID, hides the dot when the race has 5+ players / no slot / map off,
 * scales the car position into the minimap (map size D_801161C4 - 8, aspect from the track bounds
 * D_801407B4/D_801407D4, mirrored when D_80140A04 is clear), flashes the dot (alpha 64/255 on clock
 * bit 3) when the car is dead/crashed/not collidable/hidden/targeted, and draws it via stat_race_update.
 *
 * Shaping (each measured in the whole-program unit, w7b -> w10b -> w11a):
 *  - D_801543CA and the clock struct D_8002E8E8 are volatile (retail `lui; addiu; lh 0()` / `la; lw 636()`).
 *  - u32 id = blt->AnimID: retail keeps AnimID in a coloured web (v1).
 *  - the map size `D_801161C4 - 8` is written twice per branch: uopt's CSE temp (a0) is copied into
 *    map_width/map_height and pushes blt off a0 into t0, as in retail.
 *  - the product is `(s32)(world_height * (u32)(D_801161C4 - 8))`: an unsigned conversion is a cfe node
 *    that demotes its operand to the right-hand side, giving retail's `multu a3,a0` (w11a).
 *  - `if (size) { }` (compiled out) after the render-flag store: one more use in size's web raises its
 *    priority (traced: save 8/7 = 1.14 over 7 blocks) above the D_80151AD0 PRE web (save 1.0), so size
 *    takes t3 and the count t4 as in retail (w11a). It also replaces w7b's artificial `if (flash) { }` (read of an uninitialised local).
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct Blit {
    const char *Name;
    void *image;
    void *Info;
    s16 TexIndex;
    s16 X, Y;
    u16 Z;
    s16 Width, Height;
    u8 Alpha, Flip;
    s8 Hide;
    u8 Init;
    s16 Top, Bot, Left, Right;
    s16 color;
    u16 unknown26;
    s32 (*AnimFunc)(struct Blit *);
    u32 AnimID;
    u32 unk30;
    u16 render_slot;
} Blit;

typedef struct Car {
    u8 pad0[8];
    f32 position[3];
    u8 pad14[856 - 20];
    s8 dead;
    s8 active;
    u8 tail[952 - 858];
} Car;

typedef struct Model {
    u8 pad0[1600];
    s8 crash;
    u8 pad641[1732 - 1601];
    s16 hit_target;
    u8 pad6C6[1996 - 1734];
    s8 mode;
    u8 pad7CD[2015 - 1997];
    s8 hide;
    u8 pad7E0[2027 - 2016];
    s8 collidable;
    u8 tail[2056 - 2028];
} Model;

typedef struct WorldPoint { s16 x, y, z; } WorldPoint;
typedef struct MapOrigin { s32 x, y; } MapOrigin;
typedef struct RenderRec { u8 pad0[21]; u8 flags; u8 tail[10]; } RenderRec;
typedef struct Clock { u8 pad0[636]; u32 tick; } Clock;

extern s8 D_80142DB4[];
extern s16 D_80151AD0;
extern s8 D_80156BDC;
extern WorldPoint D_801407B4, D_801407D4;
extern s32 D_801161C4;
extern MapOrigin D_801160A8[];
extern s8 D_80140A04;
extern Car D_80152818[];
extern Model D_8014A250[];
extern RenderRec D_80140BF0[];
extern volatile s16 D_801543CA;
extern volatile Clock D_8002E8E8;

void Input_ApplyPadConfig(Blit *blt);
void stat_race_update(Blit *blt, s32 index, s32 w, s32 h);

static s32 Hidden(Blit *blt, s32 hide)
{
    if (hide != blt->Hide) {
        blt->Hide = hide;
        Input_ApplyPadConfig(blt);
    }
    return blt->Hide;
}

s32 func_80109A60(Blit *blt)
{
    Model *m;
    Car *car;
    s16 b, flash, slot;
    s16 size;
    u32 id;
    s32 world_width, world_height, map_width, map_height, xo, yo;

    id = blt->AnimID;
    slot = D_80142DB4[id & 0xf];
    if (Hidden(blt, D_80151AD0 >= 5 || slot == -1 || !D_80156BDC))
        return 1;
    m = &D_8014A250[slot];
    car = &D_80152818[slot];

    world_width = D_801407B4.x - D_801407D4.x;
    world_height = D_801407B4.z - D_801407D4.z;
    size = blt->Height;
    if (world_height < world_width) {
        map_width = D_801161C4 - 8;
        map_height = (s32)(world_height * (u32)(D_801161C4 - 8)) / world_width;
    } else {
        map_height = D_801161C4 - 8;
        map_width = (s32)(world_width * (u32)(D_801161C4 - 8)) / world_height;
    }
    xo = (D_801161C4 - map_width - 8) / 2 + 4;
    yo = (D_801161C4 - map_height - 8) / 2 + 4;
    blt->X = D_801160A8[D_80151AD0 - 1].x + xo - (size + D_801161C4) / 2;
    blt->Y = D_801160A8[D_80151AD0 - 1].y + yo - (size + D_801161C4) / 2;
    if (D_80140A04) {
        blt->X += (car->position[0] - D_801407D4.x) * map_width / world_width;
    } else {
        blt->X += ((map_width - 1) - (car->position[0] - D_801407D4.x) * map_width / world_width);
    }
    blt->Y += (car->position[2] - D_801407D4.z) * map_height / world_height;
    D_80140BF0[blt->render_slot].flags |= 1;
    if (size) { }
    flash = car->dead || m->crash || !m->collidable || m->hide || m->hit_target != -1;
    if (car->active > 0)
        flash = 0;
    if (slot < D_801543CA) {
        blt->Alpha = ((D_8002E8E8.tick & 8) && flash) ? 64 : 255;
        if (m->mode == 2)
            stat_race_update(blt, slot, size, size);
        else if (m->mode == 1)
            stat_race_update(blt, 7, size, size);
    } else {
        return Hidden(blt, 1);
    }
    Input_ApplyPadConfig(blt);
    return 1;
}
