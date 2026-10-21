/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_8010FBE0 (0x8010FBE0, 128 B), w36w3 (wave 36): N64-only scheduler submission of an extra RSP task (no arcade
 * or MAT3 ancestor; caller-less in the image, reached through a pointer). The caller's OSTask is copied into a
 * function-local static OSScTask (0x80155238: next 0, flags 2, msgQ = done queue D_80152750, msg 0), which is sent
 * to the scheduler's task queue D_8002E960 (OS_MESG_BLOCK); then message 670 goes to its command queue D_8002E928.
 * Shaping: the OSScTask is a FUNCTION-LOCAL static (only this function references 0x80155238..0x8015528F). As an
 * extern or file-level definition uopt keeps its address in a register over the four field stores (an lda web:
 * w15d/w16e/w20e, 6 rows); a local static is a direct variable reference, so every store gets its own `lui at`
 * (as1 shares one only between consecutive stores), as retail.
 */
typedef unsigned int u32;
typedef unsigned long long u64;
typedef void *OSMesg;
typedef struct OSMesgQueue OSMesgQueue;

typedef struct {
    u32 type;
    u32 flags;
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
} OSTask_t;

typedef union {
    OSTask_t t;
    long long force_structure_alignment;
} OSTask;

typedef struct OSScTask_s {
    struct OSScTask_s *next;
    u32 state;
    u32 flags;
    void *framebuffer;
    OSTask list;
    OSMesgQueue *msgQ;
    OSMesg msg;
} OSScTask;

extern OSMesgQueue D_80152750;
extern OSMesgQueue D_8002E960;
extern OSMesgQueue D_8002E928;
extern void *memcpy(void *, const void *, unsigned int);
extern int osSendMesg(OSMesgQueue *, OSMesg, int);

void func_8010FBE0(OSTask *task)
{
    static OSScTask scTask;   /* 0x80155238 */

    scTask.next = 0;
    scTask.msgQ = &D_80152750;
    scTask.msg = 0;
    scTask.flags = 2;
    memcpy(&scTask.list, task, sizeof(OSTask));
    osSendMesg(&D_8002E960, (OSMesg)&scTask, 1);
    osSendMesg(&D_8002E928, (OSMesg)670, 1);
}
