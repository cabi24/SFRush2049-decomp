/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* menu_back = _DecodeGhost@save.c (MAT3 GameSource/src/save.c _DecodeGhost, GC 0x8003ab30). Base: installed
 * context body src/blob/groups/menu_save_options/ctx_slot.c (w36w notes: 3 words). */
typedef signed char s8;typedef unsigned char u8;typedef short s16;typedef unsigned short u16;
typedef signed int s32;typedef unsigned int u32;typedef float f32;
#define NULL ((void *)0)
typedef struct Obj Obj;
typedef Obj **Handle;
struct Obj {
    Handle next; Handle slot; s8 index; u8 variant; char name[14]; s32 key0; s32 key1; f32 order;
    void **h36;       /* 36 */
    void **h40;       /* 40 */
};
typedef struct MenuData {
    u8 pad0[5];
    s8 loaded;        /* 5 */
    s8 index;         /* 6 */
    u8 variant;       /* 7 */
    u8 pad8[14];
    char name[18];    /* 22 */
    s32 key0;         /* 40 */
    s32 key1;         /* 44 */
    u8 pad48[12];
    s32 size;         /* 60 */
    s32 tag;          /* 64 */
    u8 pad68[8];
    s32 buffer;       /* 76 */
    u8 pad80[4];
    s32 count;        /* 84 */
    u8 data[4];       /* 88 */
} MenuData;
s32 audio_task_complete(void *, s32);
s32 inflate_entry_alt(void *, s32, s32);
s32 format_string_parse(s32, s32);
void menu_back(Handle h) {
    s32 n;
    MenuData *m = *(MenuData **)(*h)->h40;
    if (m->buffer == 0) {
        (*h)->h36 = (void **)audio_task_complete(NULL, m->tag);
        m->buffer = *(s32 *)(*h)->h36;
        n = inflate_entry_alt(m->data, m->size, m->buffer);
        m->count = n / 3; format_string_parse(m->buffer, n);
        m->loaded = 1;
    }
}
