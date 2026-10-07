typedef unsigned char u8; typedef signed char s8; typedef signed int s32; typedef unsigned int u32; typedef float f32;
#define NULL ((void*)0)
#define M2C_FIELD(expr,type,offset) (*(type)((u8 *)(expr)+(offset)))
typedef struct Obj Obj;
typedef Obj **Handle;
struct Obj {
    Handle next;      /* 0 */
    Handle slot;      /* 4 */
    s8 index;         /* 8 */
    u8 variant;       /* 9 */
    char name[14];    /* 10 */
    s32 key0;         /* 24 */
    s32 key1;         /* 28 */
    f32 order;        /* 32 */
    void **h36;       /* 36 */
    void **h40;       /* 40 */
};
typedef struct Slot {
    u8 pad0[8];
    void *update;     /* 8 */
    void *render;     /* 12 */
    u8 id;            /* 16 */
    u8 seat;          /* 17 */
    u8 pad18[46];
    s32 catchup;      /* 64 */
    u8 pad68[4];
    void **data;      /* 72 */
    s32 f76;          /* 76 */
} Slot;
extern s32 draw_ui_element(void*,s32,s32);
extern void menu_back(Handle),menu_transition(Handle),drone_set_catchup(void*,s32,s32),menu_item_select(void*,s32),func_800CB9D0(void*);
s32 func_800CBF2C(Handle h, s32 back, s32 select) {
    Obj *o = *h;
    void **data;

    if (o->h40 != NULL) {
        if (o->slot == NULL) {
            if (back) {
                menu_back(h);
            }
            return 1;
        }
        menu_transition(h);
    }
    drone_set_catchup(o->slot, 0, ((Slot *)*o->slot)->catchup);
    data = ((Slot *)*o->slot)->data;
    if (data == NULL) {
        return 0;
    }
    o->h40 = data;
    ((Slot *)*data)->f76 = 0;
    if (draw_ui_element(h, 0, 1) && !draw_ui_element(h, 1, 1)) {
        return 0;
    }
    if (back) {
        menu_back(h);
    }
    if (select) {
        menu_item_select(o->h40, 88);
        func_800CB9D0(o->h40);
        func_800CB9D0(o->h36);
        ((Slot *)*o->h40)->f76 = *(s32 *)*o->h36;
    }
    return 1;
}
