/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned int u32;
typedef struct Link {struct Link *next, *prev;} Link;
typedef struct List {u8 indirect, doubly, pad[2];u32 count;Link *head, *tail;} List;
void func_80091FBC(List *list, Link *object, Link *before);
typedef signed char s8;
typedef int s32;
typedef struct Controller { u8 pad0[5]; s8 saved; u8 rest[766]; } Controller;
typedef struct Record { Link link; u8 rest[68]; } Record;
extern Controller D_80144030[];
extern Record D_80144DC0[], D_801460C0;
extern Link *D_80144C60[];
extern List D_801460E0, D_80144D60[];
extern s32 D_80144008;
extern s8 D_8011EAE0;
extern u8 D_800349F0[], D_800329F0[];
extern void *memset(void *,s32,u32);
extern void audio_queue_process(void *);
extern void osCreateThread(void *,s32,void (*)(void *),void *,void *,s32);
extern void osStartThread(void *);
extern void func_800A43FC(void);
void func_800A3724(u8 port) {
    Controller *controller = &D_80144030[port];
    s8 saved = controller->saved;
    memset(controller,0,sizeof(Controller));
    controller->saved = saved;
}
void cl_InitLL(List *list, u8 handle, u8 dbl)
{
    list->indirect = handle;
    list->doubly = dbl;
    list->count = 0;
    list->tail = 0;
    list->head = 0;
}

void car_lod_select(s32 mode) {
    /*@{D*/Record *record;
    Link **slot;/*@| Link **slot;
    Record *record; @}*/
    List *list;
    Controller *controller;
    s32 port;
    D_80144008 = mode;
     list = &D_801460E0;
    list->indirect = 1;
    list->doubly = 1;
    list->count = 0;
    list->tail = 0;
    list->head = 0; 
    /*@{I*/record = D_80144DC0;
    slot = D_80144C60;/*@| slot = D_80144C60;
    record = D_80144DC0; @}*/
    do {
        *slot = (Link *)record;
        func_80091FBC( &D_801460E0 ,(Link *)slot, D_801460E0.head );
        /*@{P*/record++;
        slot++;/*@| slot++;
        record++; @}*/
    } while (/*@{W*/record < &D_801460C0/*@| &D_801460C0 > record @}*/);
    list = D_80144D60;
    controller = D_80144030;
    for (port=0;port<4;port++) {
        list->indirect=1;
        list->doubly=1;
        list->head=0;
        list->tail=0;
        list->count=0;
        controller->saved=0;
        func_800A3724((u8)port);
        list++;
        controller++;
    }
    osCreateThread(D_800349F0,2,audio_queue_process,0,D_800329F0,6);
    osStartThread(D_800349F0);
    D_8011EAE0=1;
    func_800A43FC();
}

/* DISCLOSED stand-in caller (not retail): retail's second caller of func_800A3724 is the unmatched
 * track_render_process (PS_PakServer); it keeps the reset out of line as in retail. */
void __standin_func_800A3724(void)
{
    func_800A3724(0);
}
