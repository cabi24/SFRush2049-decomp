typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef float f32;
#define NULL ((void *)0)
extern s8 D_80146115,D_8010FFC0;
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
    osRecvMesg(&D_80142728,0,1);
    p = (Packet *)D_80146170.head;
    func_8009211C(&D_80146170, &p->link);
    p->type = 0;
    return p;
}

static void packet_post(Packet *p) {
    func_80091FBC(&D_80146188, &p->link, D_80146188.head);
    p->ready = 1;
}

Packet *func_800D63EC(Vec3 *a,Vec3 *b,Vec3 *c,Vec3 *d) {
    Packet *packet;
    if(!D_8011025C) return (Packet *)-1;
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
    osJamMesg(&D_80142728,0,0);
    return packet;
}
