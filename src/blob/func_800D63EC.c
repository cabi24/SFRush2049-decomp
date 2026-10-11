/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_800D63EC: allocate and post a four-vector packet (60-byte records of the pool D_8011025C
 * that hud_speed_display builds; free list D_80146170, posted list D_80146188).
 *   if the pool does not exist return -1; take the lock (osRecvMesg on D_80142728, blocking);
 *   p = free.head, unlink it (func_8009211C), p->type (+8) = 0; copy the four Vec3 a..d into
 *   p+12/24/36/48; insert at the head of the posted list (func_80091FBC(list, p, list->head));
 *   p->ready (+9) = 1; release the lock (osJamMesg); return p.
 * Shape (w7c reconstruction, w7c sweep sw1/P0_h re-scored by w21g):
 *   - allocate, post, lock and unlock are four static helpers inlined by umerge (frame 48 and the
 *     separate homes 28/24/44 are their inlined locals); no retail stubs remain for them.
 *   - SHAPING DEVICE (disclosed): one compiled-out read `if (D_80146170.head) {}` right after the
 *     lock (probably a compiled-out empty-list assertion). It splits the block so the four
 *     parameter webs are reloaded and re-spilled around func_8009211C and the head load / home
 *     store land where retail has them (without it: 10 of 81 words, as1 order of one spill store).
 * Whole-program unit: EQUAL (blob_unit --neighbours, 0 locked bodies differ). No own .rodata.
 */
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef float f32;
#define NULL ((void *)0)
typedef unsigned char u8;
typedef unsigned int u32;
typedef struct Link {struct Link *next,*prev;} Link;
typedef struct List {u8 indirect,doubly,pad[2];u32 count;Link *head,*tail;} List;
typedef struct Vec3 {float x,y,z;} Vec3;
typedef struct Packet {Link link;u8 type,ready,opaque[2];Vec3 a,b,c,d;} Packet;
typedef struct OSMesgQueue OSMesgQueue;
extern OSMesgQueue D_80142728;
extern List D_80146170,D_80146188;
extern void *D_8011025C;
extern int osRecvMesg(OSMesgQueue *,void **,int);
extern int osJamMesg(OSMesgQueue *,void *,int);
extern void func_8009211C(List *,Link *);
extern void func_80091FBC(List *,Link *,Link *);
static Packet *packet_alloc(void) {
    Packet *p;
    p = (Packet *)D_80146170.head;
    func_8009211C(&D_80146170, &p->link);
    p->type = 0;
    return p;
}

static void packet_post(Packet *p) {
    func_80091FBC(&D_80146188, &p->link, D_80146188.head);
    p->ready = 1;
}

static void packet_lock(void) {
    osRecvMesg(&D_80142728,0,1);
}

static void packet_unlock(void) {
    osJamMesg(&D_80142728,0,0);
}

Packet *func_800D63EC(Vec3 *a,Vec3 *b,Vec3 *c,Vec3 *d) {
    Packet *packet;
    if(!D_8011025C) return (Packet *)-1;
    packet_lock();
    if (D_80146170.head) {}
    packet = packet_alloc();
    packet->a.x=a->x;
    packet->a.y=a->y;
    packet->a.z=a->z;
    packet->b.x=b->x;
    packet->b.y=b->y;
    packet->b.z=b->z;
    packet->c.x=c->x;
    packet->c.y=c->y;
    packet->c.z=c->z;
    packet->d.x=d->x;
    packet->d.y=d->y;
    packet->d.z=d->z;
    packet_post(packet);
    packet_unlock();
    return packet;
}
