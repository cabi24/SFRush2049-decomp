/* Context copy of src/blob/groups/codex_func_800C7200_2/group.c with its SaveServer context draft removed (the real
 * body is in group.c): func_800C7200 (GC _NewGhost) is internal in the unit, SaveServer calls it.
 * w32c (wave 32): func_800CC50C (GC CompressGhost) is a MEMBER now; its body and the helpers ghost_blocks_from_size /
 * ghost_free_start are at the end of this file (see the comments there). */
/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Real C7200 callsites; only the allocator is claimed. */
/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
#define NULL ((void *)0)
#define TRUE 1
#define FALSE 0
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
typedef union {
    struct {
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
extern u8 rspbootTextStart[], rspbootTextEnd[];
extern u8 gspF3DEX2_fifoTextStart[], gspF3DEX2_fifoTextEnd[];
extern u8 gspF3DEX2_fifoDataStart[], gspF3DEX2_fifoDataEnd[];
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
typedef struct {
    OSTask_t t;
} OSTask;
typedef s32 OSPri;
typedef s32 OSId;
typedef struct __OSThreadContext {
    u64 at, v0, v1, a0, a1, a2, a3;
    u64 t0, t1, t2, t3, t4, t5, t6, t7;
    u64 s0, s1, s2, s3, s4, s5, s6, s7;
    u64 t8, t9, gp, sp, s8, ra;
    u64 lo, hi;
    u32 sr, pc, cause, badvaddr, rcp;
    u32 fpcsr;
    f32 fp0, fp2, fp4, fp6, fp8, fp10, fp12, fp14;
    f32 fp16, fp18, fp20, fp22, fp24, fp26, fp28, fp30;
} __OSThreadContext;
typedef struct OSThread_s {
    struct OSThread_s *next;
    s32 priority;
    struct OSThread_s **queue;
    struct OSThread_s *tlnext;
    u16 state;
    u16 flags;
    s32 id;
    s32 fp;
    struct __OSThreadprofile_s *thprof;
    __OSThreadContext context;
} OSThread;
void osCreateThread(OSThread *thread, OSId id, void (*entry)(void *),
                    void *arg, void *sp, OSPri priority);
void osStartThread(OSThread *thread);
void osStopThread(OSThread *thread);
OSPri osViSetMode(OSThread *thread, OSPri priority);
OSPri osGetThreadPri(OSThread *thread);
OSId osGetThreadId(OSThread *thread);
OSThread *__osPopThread(OSThread **queue);
void __osEnqueueThread(OSThread **queue, OSThread *thread);
extern OSThread *__osRunningThread;
extern OSThread **__osActiveQueue;
typedef void *OSMesg;
typedef struct OSMesgQueue_s {
    OSThread *mtqueue;
    OSThread *fullqueue;
    s32 validCount;
    s32 first;
    s32 msgCount;
    OSMesg *msg;
} OSMesgQueue;
void osCreateMesgQueue(OSMesgQueue *mq, OSMesg *msg, s32 count);
s32 osJamMesg(OSMesgQueue *mq, OSMesg msg, s32 flags);
s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flags);
s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flags);
typedef struct OSIoMesgHdr {
    u16 type;
    u8 pri;
    u8 status;
    OSMesgQueue *retQueue;
} OSIoMesgHdr;
typedef struct OSIoMesg {
    OSIoMesgHdr hdr;
    void *dramAddr;
    u32 devAddr;
    u32 size;
    void *piHandle;
} OSIoMesg;
typedef struct OSPiHandle {
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
s32 __osSiRawReadIo(u32 devAddr, u32 *data);
s32 __osPiRawStartDma(OSIoMesg *mb, s32 priority, s32 direction,
                 u32 devAddr, void *dramAddr, u32 size, OSMesgQueue *mq);
void osCreatePiManager(s32 pri, OSMesgQueue *cmdQ, OSMesg *cmdBuf, s32 cmdMsgCnt);
OSPiHandle *osCartRomInit(void);
s32 osAiSetNextBuffer(void *addr, u32 size);
s32 osAiSetFrequency(u32 frequency);
typedef struct OSPfs {
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
typedef struct OSPfsState {
    u32 file_size;
    u32 game_code;
    u16 company_code;
    char ext_name[4];
    char game_name[16];
} OSPfsState;
typedef union __OSInodeUnit {
    struct {
        u8 bank;
        u8 page;
    } inode_t;
    u16 ipage;
} __OSInodeUnit;
typedef struct __OSInode {
    __OSInodeUnit inode_page[128];
} __OSInode;
typedef struct __OSDir {
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
s32 osPfsAllocateFile(OSPfs *pfs, u16 companyCode, u32 gameCode,
                      u8 *gameName, u8 *extName, s32 size, s32 *fileNo);
s32 __osPfsReleasePages(OSPfs *pfs, u16 companyCode, u32 gameCode,
                  u8 *gameName, u8 *extName, s32 *fileNo);
s32 osPfsFileState(OSPfs *pfs, s32 fileNo, OSPfsState *state);
s32 osPfsGetLabel(OSPfs *pfs, u8 *label, s32 *length);
s32 osPfsSetLabel(OSPfs *pfs, u8 *label);
s32 osPfsFreeBlocks(OSPfs *pfs, s32 *freeBytes);
s32 osPfsNumFiles(OSPfs *pfs, s32 *maxFiles, s32 *usedFiles);
s32 __osPfsSelectBank(OSPfs *pfs, u8 bank);
s32 __osPfsRWInode(OSPfs *pfs, __OSInode *inode, u8 flag, u8 bank);
s32 osPfsFindFile(OSPfs *pfs, s32 pages);
typedef struct OSContStatus {
    u16 type;
    u8 status;
    u8 errno;
} OSContStatus;
typedef struct OSContPad {
    u16 button;
    s8 stick_x;
    s8 stick_y;
    u8 errno;
} OSContPad;
typedef struct OSContRamIo {
    void *address;
    u8 databuffer[32];
    u8 addressCrc;
    u8 dataCrc;
    u8 errno;
} OSContRamIo;
s32 osContInit(OSMesgQueue *mq, u8 *pattern, OSContStatus *status);
s32 osContReset(OSMesgQueue *mq, OSContStatus *status);
s32 osContStartQuery(OSMesgQueue *mq);
s32 __osPfsGetStatus(OSMesgQueue *mq);
s32 osContSetCh(u8 num);
void osContGetQuery(OSContStatus *status);
void osContGetReadData(OSContPad *pad);
typedef u64 OSTime;
typedef struct OSTimer {
    struct OSTimer *next;
    struct OSTimer *prev;
    OSTime interval;
    OSTime value;
    OSMesgQueue *mq;
    OSMesg msg;
} OSTimer;
OSTime osGetTime(void);
void osSetTime(OSTime time);
s32 osSetTimer(OSTimer *timer, OSTime countdown, OSTime interval,
               OSMesgQueue *mq, OSMesg msg);
s32 osStopTimer(OSTimer *timer);
void osInvalDCache(void *vaddr, s32 nbytes);
void osWritebackDCacheAll(void *vaddr, s32 nbytes);
void osWritebackDCache(void *vaddr, s32 nbytes);
void rmonPrintf(void);
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
typedef struct {
    u32 ctrl, width, burst, vSync, hSync, leap, hStart, xScale, vCurrent;
} OSViCommonRegs;
typedef struct {
    u32 origin, yScale, vStart, vBurst, vIntr;
} OSViFieldRegs;
typedef struct {
    u8 type;
    OSViCommonRegs comRegs;
    OSViFieldRegs fldRegs[2];
} OSViMode;
typedef struct {
    f32 factor;
    u16 offset;
    u32 scale;
} __OSViScale;
typedef struct {
    u16 state;
    u16 retraceCount;
    void *framep;
    OSViMode *modep;
    u32 control;
    OSMesgQueue *msgq;
    OSMesg msg;
    __OSViScale x;
    __OSViScale y;
} __OSViContext;
extern __OSViContext *__osViContext;
void __osDequeueThread(OSThread **queue, OSThread *thread);
extern s32 __osSiRawStartDma(s32 direction, void *dramAddr);
typedef struct {
    u32 ramarray[15];
    u32 pifstatus;
} OSPifRam;
extern OSPifRam __osSiDmaBuffer;
extern u8 __osPfsBuffer[64];
extern s32 __osContRamWrite(OSMesgQueue *mq, s32 channel, u16 address,
                             u8 *buffer, s32 force);
extern s16 gViewportOffsetX[32];
extern s16 gViewportOffsetY[32];
extern s32 dma_wait(s32 blocking);
extern void dma_signal(void);
extern s32 __osSiDmaRetry;
extern void __osEnqueueAndYield(OSThread **queue);
extern s32 __osContRamRead(OSMesgQueue *mq, s32 channel, u16 address,
                            u8 *buffer);
typedef struct OSScTask_s {
    struct OSScTask_s *next;
    s32 state;
    s32 flags;
    void *framebuffer;
    s32 type;
    u8 pad14[0x38 - 0x14];
    void *unk38;
    s32 *unk3C;
    u8 pad40[0x50 - 0x40];
    OSMesgQueue *msgQueue;
    OSMesg msg;
} OSScTask;
typedef struct OSScClient_s {
    struct OSScClient_s *next;
    OSMesgQueue *msgQueue;
} OSScClient;
typedef struct {
    s16 state;
    u8 pad02[0x20 - 0x02];
    s16 priority;
    u8 pad22[0x40 - 0x22];
    OSMesgQueue cmdQueue;
    OSMesg cmdMsgs[8];
    OSMesgQueue retQueue;
    OSMesg retMsgs[8];
    u8 padB0[0x260 - 0xB0];
    OSScClient *clientList;
    OSScTask *rspTaskHead;
    OSScTask *rspTaskTail;
    OSScTask *rdpTaskHead;
    OSScTask *rdpTaskTail;
    OSScTask *curRSPTask;
    OSScTask *curRDPTask;
    s32 retraceCount;
    s32 audioListPending;
} OSSched;
extern void __scAppendList(OSSched *sc, OSScTask *task);
extern void __scExec(OSSched *sc, OSScTask *rspTask, OSScTask *rdpTask);
extern void osViSwapBuffer(void *mode);
extern void display_mode_tick(void);
extern u8 gInflateBufferA[0x1000];
extern u8 gInflateBufferB[0x1000];
extern s32 osPiStartDma(void *mb, s32 priority, s32 direction,
                              u32 devAddr, void *dramAddr, u32 size,
                              OSMesgQueue *mq);
typedef struct {
    s32 flag;
    u8 pad04[0x8 - 0x4];
    s32 unk8;
    u8 pad0C[0x1C - 0xC];
} __OSPiMgrState;
extern __OSPiMgrState __osPiMgrState;
extern void __osPiCreateAccessQueue(void);
extern void __osPiGetAccess(void);
extern void __osPiRelAccess(void);
extern u32 osRomBase;
extern void __osSetCompare(u32 compare);
extern void __osSpSetStatus(u32 status);
extern void osSpTaskYield(void);
extern void __setfpcsr(u32 value);
extern void game_loop(void);
extern void __osSiGetAccess(void);
extern void __osSiRelAccess(void);
extern void __osPackReadData(void);
typedef struct __OSTimerNode_s {
    struct __OSTimerNode_s *next;
    struct __OSTimerNode_s *prev;
    s32 reload_hi;
    s32 reload_lo;
    s32 delta_hi;
    s32 delta_lo;
    OSMesgQueue *msgQueue;
    OSMesg msg;
} __OSTimerNode;
extern __OSTimerNode *__osTimerList;
extern void osSetThreadPri(OSThread *thread, OSPri priority);
extern void dma_queue_init(void);
extern void osViBlack(s32 mask);
extern void osViSetSpecialFeatures(u32 features);
extern u8 gStackGame[0x2000];
typedef enum GState {
    ATTRACT, TRKSEL, CARSEL, PLAYGAME, ENDGAME, GAMEOVER, HISCORE,
    PREPLAY, PREPLAY2, COUNTDOWN, NUM_GAME_STATES
} GState;
extern u8 gstate;
extern s32 frame_counter;
extern s32 game_state_flags;
extern s32 state_word_a;
extern s32 state_word_b;
extern OSMesgQueue *msgq_ptr;
typedef struct {
    u8 pad00[1];
    u8 active;
    u8 pad02[2];
    s32 unk04;
    s32 unk08;
    s32 unk0C;
    f32 unk10;
    f32 unk14;
    u8 _pad18[0x4D - 0x18];
    u8 unk4D;
} InputRecord;
extern InputRecord input_rec0;
extern InputRecord input_rec1;
extern s32 D_80156978[4];
extern s32 D_80156998[4];
extern s32 D_80143A00[4];
typedef struct {
    f32 unk0;
    f32 unk4;
} D_80156958_Entry;
extern D_80156958_Entry D_80156958[4];
typedef struct {
    u8 pad0000[0x9CC0];
    OSMesgQueue unk9CC0;
} SegmentHeader;
typedef struct {
    u8 pad00[0x58];
    SegmentHeader *unk58;
    u8 pad5C[0x7C - 0x5C];
    void *unk7C;
} SegmentTableEntry;
extern SegmentTableEntry D_80156BE0[];
typedef struct {
    u8 pad0[8];
    s32 unk8;
} D_8014A160_Target;
extern D_8014A160_Target **D_8014A160;
typedef struct D_8012E6E0_Node {
    struct D_8012E6E0_Node *next;
} D_8012E6E0_Node;
extern D_8012E6E0_Node *D_8012E6E0;
typedef struct {
    u8 pad000[0xE8];
    s32 unkE8;
    u8 pad0EC[0x35B - 0xEC];
    u8 unk35B;
    u8 pad35C[0x380 - 0x35C];
    u8 unk380;
    u8 pad381[0x3A3 - 0x381];
    u8 unk3A3;
    u8 pad3A4[0x3B8 - 0x3A4];
} GameCar;
extern GameCar player_array[8];
typedef struct {
    u8 pad00[0x39];
    u8 unk39;
    u8 unk3A;
    u8 unk3B;
} PlaygameSettings;
extern PlaygameSettings playgame_settings;
typedef struct {
    u8 pad000[0x1F0];
    s32 unk1F0;
    s32 unk1F4;
    s32 unk1F8;
    s32 unk1FC;
    s32 unk200;
} CountdownObject;
typedef struct {
    u8 pad000[0x19C];
    void *unk19C;
    u8 pad1A0[0x200 - 0x1A0];
    void *unk200;
    void *unk204;
    void *unk208;
} CountdownDetail;
typedef struct {
    void *unk0;
    CountdownDetail *unk4;
    void *unk8;
    void *unkC;
    void **unk10;
} CountdownState;
extern CountdownState countdown_state;
extern CountdownObject *countdown_object;
typedef struct PadConfig_s {
    struct PadConfig_s *unk0;
    struct PadConfig_s *unk4;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s16 unk12;
    u8 unk14;
    u8 unk15;
    u8 unk16;
    u8 pad17;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
} PadConfig;
extern PadConfig pad_config;
typedef struct {
    PadConfig *unk0;
    s32 unk4;
} D_80138670_Entry;
extern D_80138670_Entry D_80138670[];
extern s32 game_loop_tick;
extern s16 active_player_count;
extern s32 gameplay_mode;
extern f32 D_8002AFB4, D_8002AFB8;
extern s32 D_8002AFC0, D_8002AFC4, D_8002EBB0;
extern u16 D_8002EB70;
extern u8 D_80035470, D_80035471, D_80035472;
extern s32 D_80111958;
extern u8 D_80114650, D_80114654, D_801146F0;
extern s32 D_801146F8, D_801170FC;
extern u8 D_80117350, D_80117354;
extern s32 D_801174BC;
extern u8 D_8011ED0B;
extern u16 D_8011ED0C[];
extern f32 D_80123FB4, D_80123FB8, D_80123FBC, D_801242A8;
extern u8 D_80124F84;
extern s32 D_80124FC8;
extern u8 D_8012E67C, D_8013FECB;
extern s32 D_80140008;
extern u16 D_80140618;
extern s32 D_801406B8, D_801407BC, D_80140804, D_80140A00;
extern s32 D_80140AD8, D_80140B08, D_80140BD8;
extern u8 D_80140C26;
extern s32 D_80140D70, D_80141428, D_80142510;
extern u8 D_80142690, D_80142699, D_80142760;
extern s32 D_80143F10;
extern f32 D_8014401C;
extern u8 D_801461F8, D_80146204, D_80146205, D_80149414;
extern s32 D_80149438;
extern u8 D_80149774, D_80149794, D_801497C4;
extern s32 D_801497F4, D_80149D98;
extern u8 D_8014B240, D_80150EFC, D_80150F14;
extern s32 D_80150000;
extern u16 D_80151AD0;
extern u8 D_80151AD8, D_8015256C, D_80152744, D_80152F29;
extern s32 D_8015204C, D_801520C4, D_80153308;
extern f32 D_801525F4, D_801543CC;
extern u16 D_80152734;
extern s32 D_8015698C;
extern u8 D_80156994, D_80156CF0, D_80157244, D_8015F72D;
extern s32 D_8015B250, D_8015B260, D_8015F738;
extern s32 D_80161380, D_80161398, D_801613A4, D_801613AC;
extern s32 D_801613B0, D_80161434, D_8017A4B0, D_8017A508;
extern s32 D_8017A638;
typedef struct {
    u16 unk00;
    u16 unk02;
    u8 pad04[0x60 - 0x04];
    s32 unk60;
    s32 unk64;
} D_80143FD8_Record;
extern D_80143FD8_Record *D_80143FD8;
typedef struct {
    u8 pad000[0x7C6];
    u16 unk7C6;
    u8 pad7C8[0x7E8 - 0x7C8];
    u8 unk7E8;
} D_8014A250_Record;
extern D_8014A250_Record D_8014A250;
typedef struct {
    u8 pad00[0x0C];
    u8 unk0C;
    u8 unk0D;
    u8 unk0E;
} D_80146108_Record;
extern D_80146108_Record D_80146108;
typedef struct {
    F32 start_time[8];
    F32 end_time[8];
    S16 loop_chkpnt;
    S16 finish_line;
    S16 before_finish;
    S16 number_of_laps;
} Track_Data;
typedef struct SoundClearRecord {
    s32 unk0;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s32 unk14;
    s32 unk18;
    s32 (*unk1C)(void *);
    s32 unk20;
} SoundClearRecord;
typedef struct SoundState {
    u8 _pad0[0x4];
    struct SoundState *unk4;
    s32 (*unk8)(void *);
    u8 _padC[0x12 - 0xC];
    s16 unk12;
    s16 unk14;
    s16 unk16;
    s8 unk18;
    u8 _pad19[0x1C - 0x19];
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    u8 _pad24[0x28 - 0x24];
    s32 (*unk28)(void *);
    s32 unk2C;
    u8 _pad30[0x3C - 0x30];
    struct SoundState *unk3C;
} SoundState;
extern SoundState *func_800b3704(s32, s32, s32, s32);
extern SoundState *sound_control(s16 arg0, s16 arg1, SoundClearRecord *arg2, s16 arg3);
extern void game_loop(void);
extern void game_mode_handler(void);
extern void gfx_frame_begin(void);
extern void process_inputs(void);
extern void playgame_state_change(void);
extern void RaceStateMachine_Update(void);
extern void countdown(void);
extern void countdown_handler(void);
extern void Input_ProcessGameplayPad(s32 pad);
extern void Effects_UpdateEmitters(void);
   extern s32 PhysicsObjectList_Update(void);
extern void UpdateActiveObjects(void);
extern void input_aux_handler(void);
extern void sound_stop(s32 sound_id);
extern s32 input_init_flag_get(void);
extern void viUpdateTime(void);
extern void sound_init(void);
extern s32 wheel_render_full(s32, s32, s32, s32);
extern void world_trigger_check(void);
extern void controller_poll(void);
extern void Input_ApplyPadConfig(void *);
extern void InitMaxPath(void);
extern s32 audio_frame_sync(s32, s32, s32, s32, s32);
extern void display_enable(s32);
extern void func_800a3424(s32, s32, s32);
extern void func_800a7480(s32, s32, u8, u8, s32, s32, s32);
extern void func_800c813c(s32, s32);
extern void func_800c885c(void);
extern void func_800c9480(void);
extern void hud_setup(s32, s32, s32, s32, s32, f32, f32, s32);
extern void hud_speed_display(s32, s32, s32, s32, s32);
extern void init_state_begin(void);
extern s32 object_create(s32);
extern s32 object_render_cleanup(void **);
extern void player_cleanup_slots(void);
extern void player_mode_set(s32, s32);
extern void player_state_set(s32, s32);
extern void resource_slots_clear_multiple(void);
extern void scene_cleanup_slots(void);
extern void speed_set(void);
extern void state_change_preprocess(void);
extern void sync_entry_register(s32, s32);
extern void tire_compound_set(void);
extern void visual_objects_update(s32);
extern void billboard_render(void);
extern void camera_race_setup(void);
extern void cpak_read(s8);
extern s32 display_list_flush(s32, s32);
extern s32 entity_audio_update(s32);
extern void finish_state_alt(void);
extern void func_800ab18c(s32, s32);
extern void func_800b61a8(s32, s32, s32, s32);
extern void func_800d5374(void);
extern void func_800d5828(s16);
extern void func_800d6160(void);
extern void func_800e762c(s32);
extern void func_800f7f3c(void);
extern void func_800fbe30(void);
extern void func_800fbe60(void);
extern void func_800fbf2c(void);
extern void ghost_race_setup(void);
extern void init_state_continue(void);
extern void players_frame_update(void);
extern void race_init_helper(void);
extern s32 race_setup_1(void);
extern void race_setup_2(s16);
extern void records_screen(void);
extern void render_viewport_init(void);
extern void *memset(void *s, s32 c, u32 n);
extern void viScheduleTick(f32);
extern void dispatch_handler(s32);
extern void func_800a4770(void *, s32);
extern s32 object_manager_update(void *, s32);
extern s32 slot_state_setup(void);
extern void state_utility(s16, s32, void *);
extern void sprintf(s8 *buf, s8 *fmt, ...);
extern void func_8008705c(s32, u8, s32);
extern void func_80087110(s32, s16, s32, s32, s32, s32);
extern void func_800878e0(s32, u8, s32);
extern void func_8008a148(s32, u8, u8, s32);
extern void func_8008a38c(u8);
extern void func_8008a3e4(s16, s16, s32, s32);
extern void func_8008a46c(s16, s16, s32, s32, void *);
extern void func_8008a644(u16);
extern void func_8009f058(s32);
extern void object_render(s32, u8, u8, u16, s32, s32, s32, s32, s32, s32, s32);

/*
 * This header contains macros emitted by m2c in "valid syntax" mode,
 * which can be enabled by passing `--valid-syntax` on the command line.
 *
 * In this mode, unhandled types and expressions are emitted as macros so
 * that the output is compilable without human intervention.
 */

#ifndef M2C_MACROS_H
#define M2C_MACROS_H

/* Unknown types */
typedef s32 M2C_UNK;
typedef s8  M2C_UNK8;
typedef s16 M2C_UNK16;
typedef s32 M2C_UNK32;
typedef s64 M2C_UNK64;

/* Unknown field access, like `*(type_ptr) &expr->unk_offset` */
#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))

/* Bitwise (reinterpret) cast */
#define M2C_BITWISE(type, expr) ((type)(expr))

/* Unaligned reads */
#define M2C_LWL(expr) (expr)
#define M2C_FIRST3BYTES(expr) (expr)
#define M2C_UNALIGNED32(expr) (expr)

/* Unhandled instructions */
#define M2C_ERROR(desc) (0)
#define M2C_TRAP_IF(cond) (0)
#define M2C_BREAK() (0)
#define M2C_SYNC() (0)

#define GLUE_F64(a, b) (0.0)
#define MULT_HI(a, b) (0)
#define MULTU_HI(a, b) (0)
#define DMULT_HI(a, b) (0)
#define DMULTU_HI(a, b) (0)
#define CLZ(x) (0)
#define REVERSE_BITS(x) (0)
#define ROTATE_RIGHT(x, shift) (0)
#define ARM_RRX(x, carry) (0)
#define BSWAP32(x) (0)
#define BSWAP16(x) (0)
#define BSWAP16X2(x) (0)

/* Carry/overflow bits from partially-implemented instructions */
#define M2C_CARRY 0
#define M2C_OVERFLOW(a) (0)

/* Memcpy patterns */
#define M2C_MEMCPY_ALIGNED memcpy
#define M2C_MEMCPY_UNALIGNED memcpy
#define M2C_STRUCT_COPY memcpy

#endif

float fabsf(float);
float sqrtf(float);
#pragma intrinsic (fabsf)
#pragma intrinsic (sqrtf)

/* referenced data (types guessed from access width) */
extern f32 D_80111754[];   /* GC ghostRaceTimes */
extern s8 D_80117428;
extern s8 D_8011742C;
extern s8 D_80117430;
extern s8 D_80117434;
extern M2C_UNK D_801211BC;
extern M2C_UNK D_801211C8;
extern D_8012E6E0_Node *D_8012E6D8;
extern M2C_UNK D_8013C128;
extern M2C_UNK D_8013E5D8;
extern M2C_UNK D_8013F1F8;
extern M2C_UNK D_8013F390;
extern s16 D_8013FEC8;
extern M2C_UNK D_80144030;
extern M2C_UNK D_80144D68;
extern s8 D_8014978C;
extern M2C_UNK D_80152020;
extern void **D_80152028;
extern M2C_UNK D_80152770;

/* callees outside the group */
void pak_delete_file_retry();
void AdjustSteer();
void *audio_buffer_sync(u32 address);                               /* GC GetPtrSize */
void audio_reverb_update(u32 address, s32 tag);
void **audio_task_complete(void *heap, u32 size);                   /* GC AllocateHandleHeap */
u32 car_angular_velocity_clamp(u8 *next_in, u32 avail_in, u8 *next_out, u32 avail_out,
                               s32 level, s32 windowBits, s32 memLevel);   /* GC zip */
s32 draw_speedometer(void *ipa_fp);
void draw_text();
void drone_set_catchup();
u32 format_string_parse(u8 *, u32);                                 /* GC _CalcChecksum */
s32 func_8008AD04();
void func_80091FBC();
s32 func_800950AC();
void *func_800A47C0(void *dst, const void *src, u32 n);            /* GC memmove */
void func_800A510C();
s32 reverb_setup();
void *sound_play_menu(void *heap, u32 size);                        /* GC AllocateMemoryHeapHigh */
void track_render_process();

/* group functions */
void **func_800C7200(void);
void SaveServer(s32 forceUpdate, s32 rumbleOnly);
struct GhostHeader;
void **func_800CC50C(struct GhostHeader *header, s8 *blocks);
s32 draw_ui_element(void **ipa_t0, s32 ipa_t1, s32 ipa_t2);
extern void **func_800C7EC8(void **);


void **func_800C7200(void) {
    void **var_a0;
    s32 var_v1;

    var_v1 = 0; var_a0 = (void **)&D_8013F1F8;
loop_1:
    if (M2C_FIELD(var_a0, void **, 0) == NULL) {
        M2C_FIELD(var_a0, void **, 0) = (void *) ((var_v1 * 0x2C) + (u8 *) &D_8013F390);
        return var_a0;
    }
    if (M2C_FIELD(var_a0, void **, 4) == NULL) {
        M2C_FIELD(var_a0, void **, 4) = (void *) ((var_v1 * 0x2C) + 0x2C + (u8 *) &D_8013F390);
        return (void **)((u8 *)var_a0 + 4);
    }
    if (M2C_FIELD(var_a0, void **, 8) == NULL) {
        M2C_FIELD(var_a0, void **, 8) = (void *) ((var_v1 * 0x2C) + 0x58 + (u8 *) &D_8013F390);
        return (void **)((u8 *)var_a0 + 8);
    }
    if (M2C_FIELD(var_a0, void **, 0xC) == NULL) {
        M2C_FIELD(var_a0, void **, 0xC) = (void *) ((var_v1 * 0x2C) + 0x84 + (u8 *) &D_8013F390);
        return (void **)((u8 *)var_a0 + 0xC);
    }
    var_v1 += 4;
    var_a0 = (void **)((u8 *) var_a0 + 0x10);
    if (var_v1 == 0x40) {
        goto exhausted;
    }
    goto loop_1;
exhausted:;
}


/* SaveServer: defined in group.c (w30c) */


/* ===== func_800CC50C = GC save.c CompressGhost(header, blocks), N64 form (w31c draft, closed by w32c) =====
 * GC transcription: memmove = func_800A47C0, _CalcChecksum = format_string_parse, AllocateMemoryHeapHigh =
 * sound_play_menu, zip = car_angular_velocity_clamp, _NewGhost = func_800C7200, AllocateHandleHeap =
 * audio_task_complete, GetPtrSize = audio_buffer_sync, ghostRaceTimes = D_80111754, gRetraceRate = D_8002AFB4,
 * gTrackNum = D_8014978C; PS_GetBlocks inlined as ghost_blocks_from_size; GC `DeallocateMemory(header->start)` as
 * the inlined helper ghost_free_start (defined AFTER this function, see there).
 * Shaping (w32c): `gh` (= ghost->header) is a named local assigned twice -- once for track/backward/name, again after
 * the data memcpy for playerId/finish/GetPtrSize (retail: first web in a3, second in a0; a single assignment gives
 * the first web v1). No unused locals; no compiled-out reads. */
typedef struct GhostHeader {      /* GC __anon_49776; N64 layout 0x58 (w29d, from the retail words) */
    u32 headerChecksum;           /* 0x00 */
    u8 revision;                  /* 0x04 */
    s8 flag;                      /* 0x05 */
    s8 track;                     /* 0x06 */
    u8 backward;                  /* 0x07 */
    u8 pad08[0x16 - 0x08];
    u8 name[13];                  /* 0x16 */
    u8 pad23[0x28 - 0x23];
    u32 playerId[2];              /* 0x28 */
    u8 pad30[0x38 - 0x30];
    f32 finish;                   /* 0x38 */
    u32 zippedSize;               /* 0x3C */
    u32 unzippedSize;             /* 0x40 */
    u32 zippedChecksum;           /* 0x44 */
    u32 unzippedChecksum;         /* 0x48 */
    u8 *start;                    /* 0x4C */
    u32 offset;                   /* 0x50 */
    u32 size;                     /* 0x54 */
} GhostHeader;

typedef struct Ghost {            /* GC __anon_49a9c; N64 layout 0x2C */
    void *ll;                     /* 0x00 */
    void **fileh;                 /* 0x04 */
    s8 track;                     /* 0x08 */
    u8 backward;                  /* 0x09 */
    u8 name[13];                  /* 0x0A */
    u8 pad17;
    u32 playerId[2];              /* 0x18 */
    f32 finish;                   /* 0x20 */
    u8 **datah;                   /* 0x24 */
    GhostHeader **header;         /* 0x28 */
} Ghost;

void *memcpy(void *, const void *, u32);
void ghost_free_start(GhostHeader *header);

s32 ghost_blocks_from_size(s32 size) {   /* GC PS_GetBlocks */
    return (size + 255) >> 8;
}

void **func_800CC50C(GhostHeader *header, s8 *blocks) {
    Ghost *ghost;
    void **ghosth;
    s32 maxsize;
    u8 *sorted;
    u8 *gdata;
    u32 dataSize;
    u32 num;
    GhostHeader **gh;
    u32 unzippedChecksum;

    sorted = header->start;
    maxsize = (f32)(D_80111754[D_8014978C] + 10.0) * D_8002AFB4;
    func_800A47C0(header->start + header->size, header->start + maxsize, header->size);
    func_800A47C0(header->start + header->size * 2, header->start + maxsize * 2, header->size);
    num = header->size * 3;
    unzippedChecksum = format_string_parse(sorted, num);
    *(s16 *)((u8 *)header + 36) = 0;
    gdata = sound_play_menu(NULL, num);
    dataSize = car_angular_velocity_clamp(sorted, num, gdata, num, 9, 10, 4);
    ghost_free_start(header);
    header->start = NULL;
    ghosth = func_800C7200();
    ghost = *ghosth;
    memset(ghost, 0, 44);
    ghost->header = (GhostHeader **)audio_task_complete(NULL, dataSize + 88);
    memcpy(*ghost->header, header, 88);
    (*ghost->header)->revision = 17;
    (*ghost->header)->start = NULL;
    (*ghost->header)->offset = 0;
    (*ghost->header)->size = 0;
    (*ghost->header)->zippedSize = dataSize;
    (*ghost->header)->unzippedSize = num;
    (*ghost->header)->headerChecksum = format_string_parse(&(*ghost->header)->revision,
        (u8 *)&(*ghost->header)->zippedChecksum - (u8 *)*ghost->header - 4);
    (*ghost->header)->zippedChecksum = format_string_parse(gdata, dataSize);
    (*ghost->header)->unzippedChecksum = unzippedChecksum;
    memcpy((u8 *)*ghost->header + 88, gdata, dataSize);
    osRecvMesg((OSMesgQueue *)&D_80152770, NULL, 1);
    audio_reverb_update((u32)gdata, 0);
    osSendMesg((OSMesgQueue *)&D_80152770, NULL, 0);
    gh = ghost->header;
    ghost->track = (*gh)->track;
    ghost->backward = (*gh)->backward;
    memcpy(ghost->name, (*gh)->name, 13);
    gh = ghost->header;
    ghost->playerId[0] = (*gh)->playerId[0];
    ghost->playerId[1] = (*gh)->playerId[1];
    ghost->finish = (*gh)->finish;
    *blocks = ghost_blocks_from_size((s32)audio_buffer_sync((u32)gh));
    return ghosth;
}

/* GC DeallocateMemory(header->start) for CompressGhost's first free, written as an inlined helper that reads the
 * pointer itself, on ONE line, defined AFTER func_800CC50C (w32c, as1 evidence): as1 breaks scheduling ties by
 * source line; retail schedules `lw s2,76(s3)` between the lock's `la a0` and `move a1` with `li a2,1` in the delay
 * slot, which needs the pointer load on the lock's line and that line after the caller's `dataSize = ...` line.
 * Identity hypothesis (disclosed): the GC code calls DeallocateMemory(header->start) directly. Non-static, internal. */
void ghost_free_start(GhostHeader *header) { u8 *p = header->start; osRecvMesg((OSMesgQueue *)&D_80152770, NULL, 1); audio_reverb_update((u32)p, 0); osSendMesg((OSMesgQueue *)&D_80152770, NULL, 0); }
