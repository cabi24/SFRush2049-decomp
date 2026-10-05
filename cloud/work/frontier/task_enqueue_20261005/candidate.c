/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Research only: this source is not an exact match or a splice candidate.
 * Submits an OSTask through an OSScTask-compatible 88-byte scheduler record.
 * The callback registration path and native layout are documented in README.md.
 * No symbol aliases, storage padding, artificial helpers or register keepers.
 */
typedef unsigned int u32;
typedef int s32;
typedef unsigned long long u64;
typedef void *OSMesg;
typedef struct OSMesgQueue OSMesgQueue;
typedef union OSTask {
    struct {
        u32 type, flags;
        u64 *ucode_boot;
        u32 ucode_boot_size;
        u64 *ucode;
        u32 ucode_size;
        u64 *ucode_data;
        u32 ucode_data_size;
        u64 *dram_stack;
        u32 dram_stack_size;
        u64 *output_buff;
        u64 *output_buff_size;
        u64 *data_ptr;
        u32 data_size;
        u64 *yield_data_ptr;
        u32 yield_data_size;
    } t;
    u64 force_alignment;
} OSTask;
typedef struct OSScTask {
    struct OSScTask *next;
    s32 state;
    u32 flags;
    void *framebuffer;
    OSTask list;
    OSMesgQueue *msgQueue;
    OSMesg msg;
} OSScTask;
extern OSScTask D_80155238;
extern OSMesgQueue D_80152750, D_8002E960, D_8002E928;
void *memcpy(void *, const void *, u32);
s32 osJamMesg(OSMesgQueue *, OSMesg, s32);
void func_8010FBE0(const OSTask *source) {
    D_80155238.next = 0;
    D_80155238.msgQueue = &D_80152750;
    D_80155238.msg = 0;
    D_80155238.flags = 2;
    memcpy(&D_80155238.list, source, sizeof(OSTask));
    osJamMesg(&D_8002E960, &D_80155238, 1);
    osJamMesg(&D_8002E928, (OSMesg)670, 1);
}
