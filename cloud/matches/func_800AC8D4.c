/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * Unless bit 3 of the state word at 0x801174B4 is set, initialise the five
 * 0x18-byte callback slots at +0x12C of car record `idx` (0x3B8-byte records
 * at 0x80152818): handler pointer (0x8008BD9C, 0x8008BC94, 0x8008BB8C,
 * 0x8008BA84, 0x8008B964), state = -1, owner = idx, and a 16-bit id taken
 * from the five words at +0x2C of the 0x40-byte record idx at 0x80139320.
 * No arcade ancestor identified.  Also matches at -O2.
 * Shaping: per slot the statement order is func, state, owner, id (the other
 * 23 orders of the four stores give 6..39 differing words); as1 then moves
 * the loads and stores into the retail order.
 */
typedef float f32;
typedef int s32;
typedef unsigned int u32;
typedef short s16;

typedef struct {
    /* 0x00 */ s16 state;
    /* 0x02 */ s16 id;
    /* 0x04 */ s16 owner;
    /* 0x06 */ s16 pad06;
    /* 0x08 */ s32 pad08[2];
    /* 0x10 */ void (*func)();
    /* 0x14 */ s32 pad14;
} CarSlot;

typedef struct {
    char pad0[0x12C];
    CarSlot slot[5];
    char pad1[0x3B8 - 0x12C - 5 * 0x18];
} Car;

typedef struct {
    char pad0[0x2C];
    s32 id[5];
} Rec40;

extern u32 D_801174B4;
extern Car D_80152818[];
extern Rec40 D_80139320[];
extern void func_8008BD9C();
extern void func_8008BC94();
extern void func_8008BB8C();
extern void func_8008BA84();
extern void func_8008B964();

void func_800AC8D4(s16 idx) {
    Car *car;
    Rec40 *r;

    if (D_801174B4 & 8) {
        return;
    }
    car = &D_80152818[idx];
    r = &D_80139320[idx];
    car->slot[0].func = func_8008BD9C;
    car->slot[0].state = -1;
    car->slot[0].owner = idx;
    car->slot[0].id = r->id[0];
    car->slot[1].func = func_8008BC94;
    car->slot[1].state = -1;
    car->slot[1].owner = idx;
    car->slot[1].id = r->id[1];
    car->slot[2].func = func_8008BB8C;
    car->slot[2].state = -1;
    car->slot[2].owner = idx;
    car->slot[2].id = r->id[2];
    car->slot[3].func = func_8008BA84;
    car->slot[3].state = -1;
    car->slot[3].owner = idx;
    car->slot[3].id = r->id[3];
    car->slot[4].func = func_8008B964;
    car->slot[4].state = -1;
    car->slot[4].owner = idx;
    car->slot[4].id = r->id[4];
}
