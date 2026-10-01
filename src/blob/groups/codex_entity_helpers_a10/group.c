/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * Entity handle lookup and the small message/camera-slot helpers built on it.
 * Hand-written from the assembly (round 5).
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct CamSlot {
    /* 0x00 */ u8 pad0[12];
    /* 0x0C */ f32 x, y, z;
    /* 0x18 */ u8 pad18[4];
    /* 0x1C */ f32 blend;
    /* 0x20 */ u8 pad20[4];
    /* 0x24 */ f32 extra;
} CamSlot;

typedef struct Entity {
    /* 0x00 */ u8 pad0[12];
    /* 0x0C */ s32 id;
    /* 0x10 */ s32 state;
    /* 0x14 */ u8 pad14[6];
    /* 0x1A */ u8 msgCount;
    /* 0x1B */ u8 pad1B[0x3C - 0x1B];
    /* 0x3C */ s32 link;
    /* 0x40 */ CamSlot *cam;
} Entity; /* 0x44 */

typedef struct Msg {
    /* 0x00 */ s16 id;
    /* 0x02 */ s8 type;
    /* 0x03 */ s8 used;
    /* 0x04 */ Entity *ent;
    /* 0x08 */ u8 pad8[16];
} Msg; /* 0x18 */

extern Entity *D_80110244;
extern s32 D_80146104;
extern Msg D_80142DD8[128];
extern s32 D_80142728;
extern s32 D_801427A8;

s32 osRecvMesg(void *, void *, s32);
s32 osJamMesg(void *, void *, s32);
s32 entity_state_check(s32);

Entity *func_80091BA8(s32 h);
void func_800BF01C(CamSlot *c);
Msg *func_80091B00(void);

Entity *func_80091BA8(s32 h)
{
    if (h == -1) {
        return 0;
    }
    if (D_80110244[h & D_80146104].id != h) {
        return 0;
    }
    return &D_80110244[h & D_80146104];
}

void func_800BF01C(CamSlot *c)
{
    if(c) {}
    if (0) {
        switch (D_80146104) {
        case 0:
            D_80146104 = 1;
            break;
        case 1:
            D_80146104 = 2;
            break;
        case 2:
            D_80146104 = 3;
            break;
        case 3:
            D_80146104 = 4;
            break;
        }
    }
}

Msg *func_80091B00(void)
{
    s32 i;

    for (i = 0; i < 128; i++) {
        if (D_80142DD8[i].used == 0) {
            D_80142DD8[i].used = 1;
            D_80142DD8[i].id = -1;
            return &D_80142DD8[i];
        }
    }
    return 0;
}

void results_screen_update(s32 h);
s32 results_time_display(s32 h);
void scheduler_recv(s32 h);

void results_screen_update(s32 h)
{
    Entity *e;

    osRecvMesg(&D_80142728, 0, 1);
    e = func_80091BA8(h);
    if (e == 0) {
        osJamMesg(&D_80142728, 0, 0);
        return;
    }
    func_800BF01C(e->cam);
    osJamMesg(&D_80142728, 0, 0);
    scheduler_recv(h);
}

s32 leaderboard_update(s32 h)
{
    Entity *e;

    osRecvMesg(&D_80142728, 0, 1);
    e = func_80091BA8(h);
    if (e == 0) {
        osJamMesg(&D_80142728, 0, 0);
        return 0;
    }
    func_800BF01C(e->cam);
    osJamMesg(&D_80142728, 0, 0);
    return results_time_display(h);
}

s32 results_time_display(s32 h)
{
    s32 r;
    Entity *e;

    osRecvMesg(&D_80142728, 0, 1);
    e = func_80091BA8(h);
    if (e != 0) {
        if (e->state == 1 || e->state == 3) {
            r = 1;
        } else if (e->state != 2) {
            r = 0;
        } else {
            r = entity_state_check(e->link);
        }
    } else {
        r = 0;
    }
    osJamMesg(&D_80142728, 0, 0);
    return r;
}

void camera_clip_planes(s32 h, f32 *p, s32 unused, f32 a, f32 b)
{
    Entity *e;
    CamSlot *c;
    f32 value;

    osRecvMesg(&D_80142728, 0, 1);
    e = func_80091BA8(h);
    if (e != 0) {
        c = e->cam;
        func_800BF01C(c);
        c->x = p[0];
        c->y = p[1];
        c->z = p[2];
        if (a < 0.0f) {
            c->blend = 0.0f;
        } else {
            if (a > 1.0f) value = 1.0f;
            else value = a;
            c->blend = value;
        }
        c->extra = b;
    }
    osJamMesg(&D_80142728, 0, 0);
}

void scheduler_recv(s32 h)
{
    Msg *m = 0;
    Entity *e;

    osRecvMesg(&D_80142728, 0, 1);
    e = func_80091BA8(h);
    if (e != 0) {
        m = func_80091B00();
        m->type = 6;
        m->ent = e;
        e->msgCount++;
    }
    osJamMesg(&D_80142728, 0, 0);
    if (m != 0) {
        osJamMesg(&D_801427A8, m, 0);
    }
}

