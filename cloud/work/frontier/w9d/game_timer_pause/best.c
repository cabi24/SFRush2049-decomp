/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * game_timer_pause (0x800FE7A4; historical label): under the D_80142728 event-queue lock, look up
 * the timed event with handle h in the event table D_80110270 (slot = h & D_80146104, valid only if
 * the slot's handle still equals h; h == -1 is "none") and set its flag at +0x0E. Event layout as in
 * src/blob/car_stats_display.c (0x3C bytes, handle @8).
 *
 * The lookup is a deleted inlined helper: the caller-less `jr ra; nop` stub func_800FE79C right before
 * this function (same two-return shape as func_80091BA8 in src/blob/groups/entity_lookup). Written
 * static here so the standalone scorer inlines it; in the whole-program unit the non-static form named
 * func_800FE79C also scores EQUAL with `blob_unit score game_timer_pause --internal func_800FE79C`.
 * The two separate `D_80110270[h & D_80146104]` address computations (test vs return) are retail's.
 */
typedef float f32;
typedef unsigned char u8;
typedef unsigned int u32;
typedef int s32;

typedef struct Event {
    u32 pad0;
    u32 pad4;
    u32 handle;     /* 8 */
    u8 busy;        /* 12 */
    u8 active;      /* 13 */
    u8 flag14;      /* 14 */
    u8 pad15;
    f32 start;      /* 16 */
    f32 delay;      /* 20 */
    u8 flag24;      /* 24 */
    u8 pad25[3];
    int args[4];    /* 28 */
    int data;       /* 44 */
    int func;       /* 48 */
    u8 type;        /* 52 */
    u8 pad53[3];
    int link;       /* 56 */
} Event;

extern char D_80142728[];
extern Event *D_80110270;
extern u32 D_80146104;
int osRecvMesg(void *, void *, int);
int osJamMesg(void *, void *, int);

static Event *func_800FE79C(s32 h)
{
    if (h == -1) {
        return 0;
    }
    if (D_80110270[h & D_80146104].handle != h) {
        return 0;
    }
    return &D_80110270[h & D_80146104];
}

void game_timer_pause(s32 h)
{
    Event *e;

    osRecvMesg(D_80142728, 0, 1);
    e = func_800FE79C(h);
    if (e != 0) {
        e->flag14 = 1;
    }
    osJamMesg(D_80142728, 0, 0);
}
