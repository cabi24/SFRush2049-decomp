/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned int u32;
typedef int s32;
typedef struct OSMesgQueue OSMesgQueue;
extern int osRecvMesg(OSMesgQueue *, void **, int);
extern int osJamMesg(OSMesgQueue *, void *, int);
typedef struct Handle Handle;
typedef struct Object {
    Handle *next;
    u8 pad4[4];
    void (*func)(Handle *);
    u8 padC[4];
    u8 list;
    u8 pad11[0x48 - 0x11];
    u32 allocation;
} Object;
struct Handle { Object *object; };
typedef struct List { u8 indirect, doubly, pad[2]; u32 count; Handle *head, *tail; } List;
extern List D_80144D60[];
extern List D_801460E0;
extern OSMesgQueue D_80152770;
extern void audio_reverb_update(u32 address, s32 tag);
extern void synced_model_render(u32);
extern void func_8009211C(List *, Handle *);
extern void func_80091FBC(List *, Handle *, Handle *);

void func_800A3640(s32 index)
{
    Handle *handle;
    Handle *next;
    Object *object;

    handle = D_80144D60[index].head;
    while (handle != 0) {
        object = handle->object;
        next = object->next;
        if (object->func != 0) {
            object->func(handle);
        }
        if (object->allocation != 0) {
            synced_model_render(object->allocation);
            object->allocation = 0;
        }
        func_8009211C(&D_80144D60[handle->object->list], handle);
        func_80091FBC(&D_801460E0, handle, D_801460E0.head);
        handle = next;
    }
}

extern s32 w8b_sink(void);
void w8b_standin(s32 a)
{
    s32 x0 = w8b_sink(), x1 = w8b_sink(), x2 = w8b_sink(), x3 = w8b_sink(), x4 = w8b_sink();
    s32 x5 = w8b_sink(), x6 = w8b_sink(), x7 = w8b_sink(), x8 = w8b_sink();
    func_800A3640(a);
    func_800A3640(x0 + x1 + x2 + x3 + x4 + x5 + x6 + x7 + x8);
    func_800A3640(a + 1);
}
