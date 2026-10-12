/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * music_seq_load (0x800B15B4, 1428 B) -- DRAFT, not matching. N64 CreateCar-like car-object setup
 * (arcade svisuals.c CreateCar/SetupCar shape): body/hood/brake objects via save_slot_valid (an
 * object-create call), tint words in D_8012E700[obj] (+0x3C/+0x40), 4 tyre objects, 5 extra parts
 * (for_game only), game_car fields 0x34C/0x350/0x360, sound_bank_unload, func_80092FE0.
 * State: unit 42 ops rows (words 189), frame 192/192. The static init_obj wrapper (3 inline sites,
 * 7 params) is a FRAME HYPOTHESIS only: 3 x 32 B reproduces the 96 B retail frame gap; without it the
 * frame is 96 with the same 42 ops rows. Residual: (1) retail reads the tint object handle with lw +
 * two separate (s16) conversions (an s32 value narrowed per use, not lh 22(s0)); (2) the body flags
 * variable lives in a3 / is reloaded from 64(sp) per call (`move a3,v1; ori t5,a3,0x18; move a3,t5`);
 * (3) tyre flags: retail ORs refl inside each arm of the for_game test.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct {
    s32 frame;      /* 0x00 */
    s32 unk4[3];    /* 0x04 */
    s32 hood;       /* 0x10 */
    s32 brakes;     /* 0x14 */
    s32 unk18;      /* 0x18 */
    s32 tires[4];   /* 0x1C */
    s32 sparks[5];  /* 0x2C */
} CarParts;

typedef struct {
    u8 pad0[0x34C];
    s32 unk34C;
    s32 unk350;
    u8 pad354[0x360 - 0x354];
    s32 unk360;
    u8 pad364[0x3B8 - 0x364];
} CAR_DATA;

typedef struct {
    u8 pad0[0x7CD];
    u8 unk7CD;
    u8 unk7CE;
    u8 unk7CF;
    u8 pad7D0[0x808 - 0x7D0];
} MODELDAT;

typedef struct {
    u8 pad0[0x3C];
    s32 unk3C;
    s32 unk40;
} ObjInfo;

typedef struct {
    s32 *ptr;
    s32 pad;
} TexBank;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2[5];
    u8 unk7;
} NodeInfo;

extern s16 D_8014A108;
extern u16 D_801427C0[];
extern CarParts D_80139220[];
extern u16 D_8016137A;
extern TexBank D_80151AE8[];
extern s32 D_8011AE58;
extern ObjInfo D_8012E700[];
extern s8 D_8014978C;
extern s32 D_8011ADC0[][2];
extern u16 D_801428F8;
extern s8 D_80156994;
extern NodeInfo D_80153E88[];
extern s8 D_8011157C[][13];
extern s32 D_80143F74[];
extern s32 D_80143FA8;
extern u16 D_80142944;
extern CAR_DATA D_80152818[];
extern MODELDAT D_8014A250[];
extern s32 *D_8011AF70[];

s32 save_slot_valid(s32 id, s16 a1, s32 parent, s32 flags, s16 slot, s16 part, s32 for_game);
void func_8008D870(s16 obj, s32 tex, s32 arg2);
void sound_bank_unload(s16 slot, s16 arg1, u8 arg2, u8 arg3, u8 arg4);
void func_80092FE0(s16 slot, s32 *arg1);

static s32 init_obj(s32 id, s16 a1, s32 parent, s32 flags, s16 slot, s16 part, s32 for_game) {
    return save_slot_valid(id, a1, parent, flags, slot, part, for_game);
}

void music_seq_load(s16 slot, s16 arg1, s32 for_game) {
    CarParts *car;
    u16 *ids;
    s32 refl;
    s32 flags;
    s16 i;
    s16 offset;

    refl = (D_8014A108 == 1 || !for_game) ? 0 : 5;
    flags = (!for_game ? 0x40004 : 0) | 0x2000;
    offset = slot * 3;
    ids = &D_801427C0[offset];
    car = &D_80139220[slot];
    car->frame = init_obj(ids[0], arg1, -1, flags | 0x18, slot, 0, for_game);
    car->hood = init_obj(ids[1], arg1, car->frame, flags | 0x80, slot, -1, for_game);
    car->brakes = init_obj(ids[2], arg1, car->frame, flags | 0x4000 | 0x200000 | 0x80000 | 0x80, slot, -1, for_game);
    func_8008D870(car->brakes, (s32)D_80151AE8[D_8016137A >> 10].ptr + (D_8016137A & 0x3FF) * 36, -1);
    if (!for_game) {
        D_8012E700[(s16)car->brakes].unk3C = D_8011AE58;
        D_8012E700[(s16)car->brakes].unk40 = D_8011AE58;
    } else {
        D_8012E700[(s16)car->brakes].unk3C = D_8011ADC0[D_8014978C][0];
        D_8012E700[(s16)car->brakes].unk40 = D_8011ADC0[D_8014978C][1];
    }
    if (!for_game) {
        flags = refl | 0x40004;
    } else {
        flags = ((D_8014A108 == 1) ? 0 : 5) | refl;
    }
    flags |= 0x2000;
    car->tires[0] = save_slot_valid(D_801428F8, arg1, car->frame, flags, slot, 3, for_game);
    car->tires[1] = save_slot_valid(D_801428F8, arg1, car->frame, flags, slot, 4, for_game);
    car->tires[2] = save_slot_valid(D_801428F8, arg1, car->frame, flags, slot, 5, for_game);
    car->tires[3] = save_slot_valid(D_801428F8, arg1, car->frame, flags, slot, 6, for_game);
    if (D_80156994 || D_8014978C >= 6) {
        i = (D_80153E88[slot % 13].unk7 == 6) ? slot % 13 + 1 : 0;
        func_8008D870(car->tires[0], D_80143F74[D_8011157C[i][slot % 13]], -1);
    } else {
        func_8008D870(car->tires[0], D_80143FA8, -1);
    }
    if (for_game) {
        car->sparks[0] = save_slot_valid(D_80142944, arg1, car->frame, 0xF00, slot, 7, for_game);
        car->sparks[1] = save_slot_valid(D_80142944, arg1, car->frame, 0xF00, slot, 8, for_game);
        car->sparks[2] = save_slot_valid(D_80142944, arg1, car->frame, 0xF00, slot, 9, for_game);
        car->sparks[3] = save_slot_valid(D_80142944, arg1, car->frame, 0xF00, slot, 10, for_game);
        car->sparks[4] = save_slot_valid(D_80142944, arg1, car->frame, 0xF00, slot, 11, for_game);
    } else {
        car->sparks[0] = car->sparks[1] = car->sparks[2] = car->sparks[3] = car->sparks[4] = -1;
    }
    if (for_game) {
        D_80152818[slot].unk360 = 255;
        D_80152818[slot].unk34C = *D_8011AF70[D_8014978C];
        D_80152818[slot].unk350 = *D_8011AF70[D_8014978C];
        sound_bank_unload(slot, D_80153E88[slot].unk1, D_8014A250[slot].unk7CD, D_8014A250[slot].unk7CE,
                          D_8014A250[slot].unk7CF);
    }
    func_80092FE0(slot, &D_8011AE58);
}
