typedef float f32;
typedef unsigned char u8;
typedef unsigned int u32;

typedef char *va_list;
#define va_start(list, parmN) (list = ((char *)&parmN + sizeof(parmN)))
#define va_arg(list, mode) ((mode *)(list = \
    (char *)((sizeof(mode) > 4 ? ((int)list + 7) & -8 \
                               : ((int)list + 3) & -4) + sizeof(mode))))[-1]
#define va_end(list) (void)0

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

typedef struct EventList {
    u8 indirect, doubly, pad2[2];
    u32 count;
    Event *head, *tail;
} EventList;

extern char D_80142728[];
extern EventList D_80149860;
extern f32 D_80152748;
int osRecvMesg(void *, void *, int);
int osJamMesg(void *, void *, int);
Event *func_800D18D8(void);
void func_80091FBC(EventList *, Event *, Event *);

static f32 get_time(void)
{
    return D_80152748;
}

u32 car_stats_display(int type, f32 delay, int func, int data, int nargs, ...)
{
    va_list ap;
    Event *e;
    int i;
    u32 handle;

    osRecvMesg(D_80142728, 0, 1);
    e = func_800D18D8();
    e->type = type;
    e->start = get_time();
    e->delay = delay;
    e->func = func;
    e->data = data;
    e->flag14 = 0;
    e->flag24 = 0;
    va_start(ap, nargs);
    for (i = 0; i < nargs; i++) {
        e->args[i] = va_arg(ap, int);
    }
    for (; i < 4; i++) {
        e->args[i] = -1;
    }
    va_end(ap);
    e->link = -1;
    func_80091FBC(&D_80149860, e, D_80149860.head);
    e->active = 1;
    handle = e->handle;
    osJamMesg(D_80142728, 0, 0);
    return handle;
}
