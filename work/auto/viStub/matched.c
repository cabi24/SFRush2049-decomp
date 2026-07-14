
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef float f32;
typedef double f64;
typedef u32 uintptr_t;
typedef s32 intptr_t;
typedef volatile u8 vu8;
typedef volatile u16 vu16;
typedef volatile u32 vu32;
typedef volatile u64 vu64;
typedef volatile s8 vs8;
typedef volatile s16 vs16;
typedef volatile s32 vs32;
typedef volatile s64 vs64;
typedef union 
{
  struct 
  {
    u32 w0;
    u32 w1;
  } words;
  u64 force_structure_alignment;
} Gfx;
typedef u32 Mtx[4][4];
typedef f32 F32;
typedef s32 S32;
typedef s16 S16;
typedef s8 S08;
typedef u32 U32;
typedef u16 U16;
typedef u8 U08;
typedef s32 BOOL;
typedef f32 Vec3f[3];
typedef s16 Vec3s[3];
typedef s32 Vec3i[3];
typedef f32 Mat3f[3][3];
typedef f32 Mat4f[4][4];
typedef f32 MtxF[4][4];
struct OSPfs;
extern u8 rspbootTextStart[];
extern u8 rspbootTextEnd[];
extern u8 gspF3DEX2_fifoTextStart[];
extern u8 gspF3DEX2_fifoTextEnd[];
extern u8 gspF3DEX2_fifoDataStart[];
extern u8 gspF3DEX2_fifoDataEnd[];
typedef struct 
{
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
typedef struct 
{
  OSTask_t t;
} OSTask;
typedef s32 OSPri;
typedef s32 OSId;
typedef struct __OSThreadContext
{
  u64 at;
  u64 v0;
  u64 v1;
  u64 a0;
  u64 a1;
  u64 a2;
  u64 a3;
  u64 t0;
  u64 t1;
  u64 t2;
  u64 t3;
  u64 t4;
  u64 t5;
  u64 t6;
  u64 t7;
  u64 s0;
  u64 s1;
  u64 s2;
  u64 s3;
  u64 s4;
  u64 s5;
  u64 s6;
  u64 s7;
  u64 t8;
  u64 t9;
  u64 gp;
  u64 sp;
  u64 s8;
  u64 ra;
  u64 lo;
  u64 hi;
  u32 sr;
  u32 pc;
  u32 cause;
  u32 badvaddr;
  u32 rcp;
  u32 fpcsr;
  f32 fp0;
  f32 fp2;
  f32 fp4;
  f32 fp6;
  f32 fp8;
  f32 fp10;
  f32 fp12;
  f32 fp14;
  f32 fp16;
  f32 fp18;
  f32 fp20;
  f32 fp22;
  f32 fp24;
  f32 fp26;
  f32 fp28;
  f32 fp30;
} __OSThreadContext;
typedef struct OSThread_s
{
  struct OSThread_s *next;
  s32 priority;
  struct OSThread_s **queue;
  struct OSThread_s *tlnext;
  u16 state;
  u16 flags;
  s32 id;
  s32 fp;
  __OSThreadContext context;
} OSThread;
void osCreateThread(OSThread *thread, OSId id, void (*entry)(void *), void *arg, void *sp, OSPri priority);
void osStartThread(OSThread *thread);
void osStopThread(OSThread *thread);
OSPri osSetThreadPri(OSThread *thread, OSPri priority);
OSPri osGetThreadPri(OSThread *thread);
OSId osGetThreadId(OSThread *thread);
OSThread *__osPopThread(OSThread **queue);
void __osEnqueueThread(OSThread **queue, OSThread *thread);
extern OSThread *__osRunningThread;
extern OSThread **__osActiveQueue;
typedef void *OSMesg;
typedef struct OSMesgQueue_s
{
  OSThread *mtqueue;
  OSThread *fullqueue;
  s32 validCount;
  s32 first;
  s32 msgCount;
  OSMesg *msg;
} OSMesgQueue;
void osCreateMesgQueue(OSMesgQueue *mq, OSMesg *msg, s32 count);
s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flags);
s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flags);
s32 osJamMesg(OSMesgQueue *mq, OSMesg msg, s32 flags);
typedef struct OSIoMesgHdr
{
  u16 type;
  u8 pri;
  u8 status;
  OSMesgQueue *retQueue;
} OSIoMesgHdr;
typedef struct OSIoMesg
{
  OSIoMesgHdr hdr;
  void *dramAddr;
  u32 devAddr;
  u32 size;
  void *piHandle;
} OSIoMesg;
typedef struct OSPiHandle
{
  struct OSPiHandle *next;
  u8 type;
  u8 latency;
  u8 pageSize;
  u8 relDuration;
  u8 pulse;
  u8 domain;
  u32 baseAddress;
  u32 speed;
} OSPiHandle;
u32 osPiGetStatus(void);
s32 osPiWriteIo(u32 devAddr, u32 data);
s32 osPiReadIo(u32 devAddr, u32 *data);
s32 osPiStartDma(OSIoMesg *mb, s32 priority, s32 direction, u32 devAddr, void *dramAddr, u32 size, OSMesgQueue *mq);
void osCreatePiManager(s32 pri, OSMesgQueue *cmdQ, OSMesg *cmdBuf, s32 cmdMsgCnt);
OSPiHandle *osCartRomInit(void);
s32 osAiSetNextBuffer(void *addr, u32 size);
s32 osAiSetFrequency(u32 frequency);
typedef struct OSPfs
{
  s32 status;
  OSMesgQueue *queue;
  s32 channel;
  u8 id[32];
  u8 label[32];
  s32 version;
  s32 dir_size;
  s32 inode_table;
  s32 minode_table;
  s32 dir_table;
  s32 inode_start_page;
  u8 banks;
  u8 activebank;
} OSPfs;
typedef struct OSPfsState
{
  u32 file_size;
  u32 game_code;
  u16 company_code;
  char ext_name[4];
  char game_name[16];
} OSPfsState;
typedef union __OSInodeUnit
{
  struct 
  {
    u8 bank;
    u8 page;
  } inode_t;
  u16 ipage;
} __OSInodeUnit;
typedef struct __OSInode
{
  __OSInodeUnit inode_page[128];
} __OSInode;
typedef struct __OSDir
{
  u32 game_code;
  u16 company_code;
  __OSInodeUnit start_page;
  u8 status;
  u8 reserved;
  char game_name[16];
  char ext_name[4];
  u16 data_sum;
} __OSDir;
s32 osPfsInitPak(OSMesgQueue *queue, OSPfs *pfs, s32 channel);
s32 osPfsChecker(OSPfs *pfs);
s32 osPfsRepairId(OSPfs *pfs);
s32 osPfsAllocateFile(OSPfs *pfs, u16 companyCode, u32 gameCode, u8 *gameName, u8 *extName, s32 size, s32 *fileNo);
s32 osPfsFindFile(OSPfs *pfs, u16 companyCode, u32 gameCode, u8 *gameName, u8 *extName, s32 *fileNo);
s32 osPfsDeleteFile(OSPfs *pfs, u16 companyCode, u32 gameCode, u8 *gameName, u8 *extName);
s32 osPfsReadWriteFile(OSPfs *pfs, s32 fileNo, u8 flag, s32 offset, s32 size, u8 *data);
s32 osPfsFileState(OSPfs *pfs, s32 fileNo, OSPfsState *state);
s32 osPfsGetLabel(OSPfs *pfs, u8 *label, s32 *length);
s32 osPfsSetLabel(OSPfs *pfs, u8 *label);
s32 osPfsFreeBlocks(OSPfs *pfs, s32 *freeBytes);
s32 osPfsNumFiles(OSPfs *pfs, s32 *maxFiles, s32 *usedFiles);
s32 __osPfsSelectBank(OSPfs *pfs, u8 bank);
s32 __osPfsRWInode(OSPfs *pfs, __OSInode *inode, u8 flag, u8 bank);
s32 osPfsAllocate(OSPfs *pfs, s32 pages);
s32 osPfsReAllocate(OSPfs *pfs, s32 pages);
typedef struct OSContStatus
{
  u16 type;
  u8 status;
  u8 errno;
} OSContStatus;
typedef struct OSContPad
{
  u16 button;
  s8 stick_x;
  s8 stick_y;
  u8 errno;
} OSContPad;
typedef struct OSContRamIo
{
  void *address;
  u8 databuffer[32];
  u8 addressCrc;
  u8 dataCrc;
  u8 errno;
} OSContRamIo;
s32 osContInit(OSMesgQueue *mq, u8 *pattern, OSContStatus *status);
s32 osContReset(OSMesgQueue *mq, OSContStatus *status);
s32 osContStartQuery(OSMesgQueue *mq);
s32 osContStartReadData(OSMesgQueue *mq);
s32 osContSetCh(u8 num);
void osContGetQuery(OSContStatus *status);
void osContGetReadData(OSContPad *pad);
typedef u64 OSTime;
typedef struct OSTimer
{
  struct OSTimer *next;
  struct OSTimer *prev;
  OSTime interval;
  OSTime value;
  OSMesgQueue *mq;
  OSMesg msg;
} OSTimer;
OSTime osGetTime(void);
void osSetTime(OSTime time);
s32 osSetTimer(OSTimer *timer, OSTime countdown, OSTime interval, OSMesgQueue *mq, OSMesg msg);
s32 osStopTimer(OSTimer *timer);
void osInvalDCache(void *vaddr, s32 nbytes);
void osInvalICache(void *vaddr, s32 nbytes);
void osWritebackDCache(void *vaddr, s32 nbytes);
void osWritebackDCacheAll(void);
void bzero(void *s, s32 n);
void bcopy(const void *src, void *dst, s32 len);
u32 osVirtualToPhysical(void *vaddr);
void *osPhysicalToVirtual(u32 paddr);
u32 __osDisableInt(void);
void __osRestoreInt(u32 flags);
void osSetTime(u64 time);
u64 osGetTime(void);
s32 osDpIsBusy(void);
void osDpSetNextBuffer(void *dramAddr, u32 size);
void viStub(void)
{
  int new_var2;
  unsigned short new_var;
 new_var2 = 1; new_var2 = 0; new_var = new_var2; if (new_var & (0xFFFF ^ new_var2)) { } if (new_var) { } if (new_var) { } if (new_var) { }
}
