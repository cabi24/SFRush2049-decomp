/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef unsigned int u32; typedef float f32;

typedef union tRGBA { unsigned char c[4]; unsigned int rgba; } tRGBA;
typedef struct CarParts {
    int frame;                  /* 0x00 */
    int extraparts[3];          /* 0x04 */
    int hood;                   /* 0x10 */
    int sheen;                  /* 0x14 */
    int weap;                   /* 0x18 */
    int tires[4];               /* 0x1C */
    int lfSpark;                /* 0x2C */
    int lrSpark;                /* 0x30 */
    int rfSpark;                /* 0x34 */
    int rrSpark;                /* 0x38 */
    int bSpark;                 /* 0x3C */
} CarParts;
typedef struct ObjInstance {
    unsigned int flags; char pad4[0x38]; unsigned int color; unsigned int envcolor;
} ObjInstance;
typedef struct tTexDef { u8 pad[36]; } tTexDef;
typedef struct tTexDefPtr { tTexDef *texDefs; int pad; } tTexDefPtr;
typedef struct tLink { u8 pad0; u8 car; u8 pad2[5]; u8 owner; } tLink;
typedef struct tCarData { u8 pad0[0x34C]; tRGBA IRclr; tRGBA prevIRclr; u8 pad354[0x360 - 0x354]; int sheenAlpha; u8 pad364[952 - 0x364]; } tCarData;
typedef struct tModelData { u8 pad0[0x7CD]; u8 a; u8 b; u8 c; u8 pad7D0[0x808 - 0x7D0]; } tModelData;

extern s16 active_player_count;         /* gNumPlayers */
extern u16 D_801427C0[];                /* gObjList */
extern CarParts D_80139320[];           /* gCarParts */
extern u16 D_8016137A;                   /* gTexList[11] */
extern tTexDefPtr gTexDefPtrs[];
extern ObjInstance D_8012E700[];        /* ObjInstanceList */
extern tRGBA D_8011ADC0[][2];           /* SheenColors */
extern tRGBA D_8011AE58[];              /* Track1Colors */
extern tRGBA *D_8011AF90[];             /* gTrackColors */
extern s8 D_8014978C;                   /* gTrackNum */
extern s8 D_80156994;
extern tLink D_80153E88[];              /* gLink */
extern s8 D_8011157C[][13];             /* gTireRims */
extern void *D_80143F74[];              /* gWheelTextures */
extern tCarData player_array[];         /* gGameCar */
extern tModelData D_8014A250[];         /* model */

int save_slot_valid(int objnum, s16 camlink, int parent, u32 flags, s16 slot, s16 carObjNum, int forGame); /* InitCarObj */
void func_8008D870(s16 objnum, void *texdef, int bank);                 /* ZOID_SetTexture */
void sound_bank_unload(s16 slot, u8 car, u8 a, u8 b, u8 c);             /* SetupCarColor */
void func_80092FE0(s16 slot, tRGBA *col);                               /* SetFrameColor */

void func_8008E06C(s16 objnum, tRGBA *col)
{
    D_8012E700[objnum].color = col->rgba;
}

void func_80092BC8(s16 objnum, tRGBA *col)
{
    ObjInstance *obj;
    obj = &D_8012E700[objnum];
    obj->envcolor = col->rgba;
}

void func_80092BF4(s16 slot, tRGBA *col, tRGBA *env)
{
    CarParts *car;
    car = &D_80139320[slot];
    func_8008E06C(car->sheen, col);
    func_80092BC8(car->sheen, env);
}

static void ZOID_SetTexture(s16 objnum, void *tex, int bank)
{
    func_8008D870(objnum, tex, bank);
}

void music_seq_load(s16 slot, s16 camlink, int for_game)
{
    CarParts *car;
    s16 offset;
    unsigned int idx;
    unsigned int flags;
    unsigned int flags2;
    int u0, u1, u2, u3;
    idx = slot % 13;
    flags = (active_player_count == 1 || !for_game) ? 0 : 5;
    offset = slot * 3;
    car = &D_80139320[slot];
    flags2 = !for_game ? 0x40004 : 0;
    car->frame = save_slot_valid(D_801427C0[0 + offset], camlink, -1, flags2 | 0x2000 | 0x10 | 0x8, slot, 0, for_game);
    car->hood = save_slot_valid(D_801427C0[1 + offset], camlink, car->frame, flags2 | 0x2000 | 0x80, slot, -1, for_game);
    car->sheen = save_slot_valid(D_801427C0[2 + offset], camlink, car->frame, flags2 | 0x2000 | 0x4000 | 0x200000 | 0x80000 | 0x80, slot, -1, for_game);
    ZOID_SetTexture(D_80139320[slot].sheen, &gTexDefPtrs[D_8016137A >> 10].texDefs[D_8016137A & 1023], -1);
    if (!for_game) func_80092BF4(slot, &D_8011AE58[0], &D_8011AE58[0]);
    else func_80092BF4(slot, &D_8011ADC0[D_8014978C][0], &D_8011ADC0[D_8014978C][1]);
    if (!for_game) flags2 = flags | 0x40000 | 0x2000 | 0x4;
    else flags2 = flags | (active_player_count == 1 ? 0 : 5);
    car->tires[0] = save_slot_valid(D_801427C0[156], camlink, car->frame, flags2 | 0x2000, slot, 3, for_game);
    car->tires[1] = save_slot_valid(D_801427C0[156], camlink, car->frame, flags2 | 0x2000, slot, 4, for_game);
    car->tires[2] = save_slot_valid(D_801427C0[156], camlink, car->frame, flags2 | 0x2000, slot, 5, for_game);
    car->tires[3] = save_slot_valid(D_801427C0[156], camlink, car->frame, flags2 | 0x2000, slot, 6, for_game);
    if (D_80156994 || D_8014978C >= 6) {
        ZOID_SetTexture(car->tires[0], D_80143F74[D_8011157C[D_80153E88[idx].owner == 6 ? idx + 1 : 0][idx]], -1);
    } else ZOID_SetTexture(car->tires[0], D_80143F74[13], -1);
    if (for_game) {
        car->lfSpark = save_slot_valid(D_801427C0[194], camlink, car->frame, 0xf00, slot, 7, for_game);
        car->lrSpark = save_slot_valid(D_801427C0[194], camlink, car->frame, 0xf00, slot, 8, for_game);
        car->rfSpark = save_slot_valid(D_801427C0[194], camlink, car->frame, 0xf00, slot, 9, for_game);
        car->rrSpark = save_slot_valid(D_801427C0[194], camlink, car->frame, 0xf00, slot, 10, for_game);
        car->bSpark = save_slot_valid(D_801427C0[194], camlink, car->frame, 0xf00, slot, 11, for_game);
    } else {
        car->lfSpark = -1;
        car->lrSpark = -1;
        car->rfSpark = -1;
        car->rrSpark = -1;
        car->bSpark = -1;
    }
    if (for_game) {
        player_array[slot].sheenAlpha = 255;
        player_array[slot].IRclr.rgba = D_8011AF90[D_8014978C][0].rgba;
        player_array[slot].prevIRclr.rgba = D_8011AF90[D_8014978C][0].rgba;
        sound_bank_unload(slot, D_80153E88[slot].car, D_8014A250[slot].a, D_8014A250[slot].b, D_8014A250[slot].c);
    }
    func_80092FE0(slot, &D_8011AE58[0]);
}
