typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef float f32;
#define NULL ((void *)0)
typedef struct OSMesgQueue OSMesgQueue;
s32 osRecvMesg(OSMesgQueue *, void **, s32);
s32 osJamMesg(OSMesgQueue *, void *, s32);
void audio_reverb_update(u32 address, s32 tag);
extern OSMesgQueue D_80152770;
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
    u8 pad17[47];
    s32 catchup;      /* 64 */
    u8 pad68[4];
    void **data;      /* 72 */
    s32 f76;          /* 76 */
} Slot;
void AdjustSteer(void *);

void menu_transition(Handle h) {
    Obj *o = *h;

    if (o->h36 != NULL) {
        void **old = o->h36;
        osRecvMesg(&D_80152770, NULL, 1);
        audio_reverb_update((u32)old, 0);
        osJamMesg(&D_80152770, NULL, 0);
        o->h36 = NULL;
        ((Slot *)*o->h40)->f76 = 0;
        if (o->slot != NULL) {
            AdjustSteer(o->slot);
            o->h40 = NULL;
        }
    }
}
