/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * Schedule a timed event (historical label car_stats_display; N64 code, no
 * arcade ancestor found). Under the D_80142728 message-queue lock
 * (osRecvMesg ... osJamMesg), take a node from the free pool (func_800D18D8,
 * which also stamps a fresh handle), fill type, start time (D_80152748, the
 * elapsed-time clock), delay, callback, data and up to four int varargs
 * (missing ones -1), link -1, insert it at the head of the active list
 * D_80149860 (func_80091FBC), mark it active and return its handle.
 *
 * Variadic: (int type, f32 delay, int func, int data, int nargs, ...);
 * va_arg uses the align-then-advance spelling ((ap + 3) & -4, +4, [-1]).
 * The start time is read through an inlined getter: in the whole-program
 * unit this is the kept func_80095CE8() (never called by jal anywhere in
 * retail: umerge inlines it at every site) -- blob_unit score is EQUAL with
 * `e->start = func_80095CE8();` (cloud/work/frontier/w8c/car_stats_display/unit_call.c).
 * Standalone, the static get_time() below reproduces it (f0 return web).
 * Declaration order e, ap, i, handle sets the spill homes. -O3 only.
 */
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
    Event *e;
    va_list ap;
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
