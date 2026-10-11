/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
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

extern OSScTask D_80155238;
extern OSMesgQueue D_80152750;
extern OSMesgQueue D_8002E960;
extern OSMesgQueue D_8002E928;
extern void *memcpy(void *, const void *, unsigned int);
extern int osJamMesg(OSMesgQueue *, OSMesg, int);

extern void *D_80155238_next;
extern u32 D_80155240;
extern OSTask D_80155248;
extern OSMesgQueue *D_80155288;
extern OSMesg D_8015528C;
void func_8010FBE0(OSTask *task)
{
    D_80155238.next = 0;
    D_80155238.msgQ = &D_80152750;
    D_80155238.msg = 0;
    D_80155240 = 2;
    memcpy(&D_80155248, task, sizeof(OSTask));
    osJamMesg(&D_8002E960, (OSMesg)&D_80155238, 1);
    osJamMesg(&D_8002E928, (OSMesg)670, 1);
}
