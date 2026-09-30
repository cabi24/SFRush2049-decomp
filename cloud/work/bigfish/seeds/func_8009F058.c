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
OSPri osSetThreadPri(OSThread *thread, OSPri priority);
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
s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flags);
s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flags);
s32 osJamMesg(OSMesgQueue *mq, OSMesg msg, s32 flags);
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
s32 osPiReadIo(u32 devAddr, u32 *data);
s32 osPiStartDma(OSIoMesg *mb, s32 priority, s32 direction,
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
s32 osPfsFindFile(OSPfs *pfs, u16 companyCode, u32 gameCode,
                  u8 *gameName, u8 *extName, s32 *fileNo);
s32 osPfsDeleteFile(OSPfs *pfs, u16 companyCode, u32 gameCode,
                    u8 *gameName, u8 *extName);
s32 osPfsReadWriteFile(OSPfs *pfs, s32 fileNo, u8 flag, s32 offset,
                       s32 size, u8 *data);
s32 osPfsFileState(OSPfs *pfs, s32 fileNo, OSPfsState *state);
s32 osPfsGetLabel(OSPfs *pfs, u8 *label, s32 *length);
s32 osPfsSetLabel(OSPfs *pfs, u8 *label);
s32 osPfsFreeBlocks(OSPfs *pfs, s32 *freeBytes);
s32 osPfsNumFiles(OSPfs *pfs, s32 *maxFiles, s32 *usedFiles);
s32 __osPfsSelectBank(OSPfs *pfs, u8 bank);
s32 __osPfsRWInode(OSPfs *pfs, __OSInode *inode, u8 flag, u8 bank);
s32 osPfsAllocate(OSPfs *pfs, s32 pages);
s32 osPfsReAllocate(OSPfs *pfs, s32 pages);
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
s32 osContStartReadData(OSMesgQueue *mq);
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
extern void __osCleanupThread(OSThread **queue);
void dll_remove(OSThread **queue, OSThread *thread);
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
extern void osViSetMode(void *mode);
extern void display_mode_tick(void);
extern u8 gInflateBufferA[0x1000];
extern u8 gInflateBufferB[0x1000];
extern s32 __osPiRawStartDma(void *mb, s32 priority, s32 direction,
                              u32 devAddr, void *dramAddr, u32 size,
                              OSMesgQueue *mq);
typedef struct {
    s32 flag;
    u8 pad04[0x8 - 0x4];
    s32 unk8;
    u8 pad0C[0x1C - 0xC];
} __OSPiMgrState;
extern __OSPiMgrState __osPiMgrState;
extern void osPiInit(void);
extern void osPiGetAccess(void);
extern void osPiReleaseAccess(void);
extern u32 osRomBase;
extern void __osSetCompare(u32 compare);
extern void __osSpSetStatus(u32 status);
extern void osDpWait(void);
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
extern void osCreateViManager(OSThread *thread, OSPri priority);
extern void dma_queue_init(void);
extern void osSetIntMask(s32 mask);
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
extern void attract_or_transition(void);
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

M2C_UNK func_8009E820(void *, f32 *, void *);       /* extern */
M2C_UNK func_8009E8B4(M2C_UNK, s32);                /* extern */
M2C_UNK func_8009E9D8(M2C_UNK, M2C_UNK, M2C_UNK);   /* extern */
M2C_UNK func_8009EA68(f32, M2C_UNK *);              /* extern */
M2C_UNK guLookAtF(M2C_UNK, s32, s32, s32, f32, f32, f32, f32, f32, f32); /* extern */
M2C_UNK guMtxIdent(s32);                            /* extern */
M2C_UNK guOrtho(M2C_UNK, s32, f32, f32, f32, f32, f32, f32, f32); /* extern */
M2C_UNK guPerspective(s32, u16 *, f32, s32, f32, f32, f32); /* extern */
M2C_UNK math_utility(void *, M2C_UNK *);            /* extern */
M2C_UNK memcpy(void *, void *, M2C_UNK);            /* extern */
M2C_UNK physics_float_calc(s32, M2C_UNK);           /* extern */
M2C_UNK render_display_list(u16, s32);              /* extern */
M2C_UNK track_collision_wall(M2C_UNK, M2C_UNK, s32, s32, s32, s32, s32, s32); /* extern */

void func_8009F058(s32 arg0, s32 arg3) {
    f32 sp124;
    f32 sp134;
    f32 sp128;
    f32 sp130;
    s32 sp178;
    s32 sp174;
    s32 sp170;
    s32 sp16C;
    u16 sp142;
    f32 sp12C;
    f32 sp120;
    M2C_UNK sp114;
    f32 spDC;
    f32 spD8;
    f32 spD4;
    s32 spB4;
    void *sp48;
    f32 *var_a0;
    f32 *var_v0;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f2;
    f32 var_f18;
    f32 var_f6;
    f32 var_f6_2;
    f32 var_f6_3;
    f32 var_f6_4;
    s16 temp_a0_3;
    s16 temp_t1_2;
    s16 temp_v0_55;
    s16 temp_v0_56;
    s32 *temp_a1;
    s32 *temp_t1;
    s32 *temp_v0;
    s32 *temp_v0_10;
    s32 *temp_v0_11;
    s32 *temp_v0_13;
    s32 *temp_v0_14;
    s32 *temp_v0_15;
    s32 *temp_v0_16;
    s32 *temp_v0_17;
    s32 *temp_v0_18;
    s32 *temp_v0_19;
    s32 *temp_v0_20;
    s32 *temp_v0_21;
    s32 *temp_v0_23;
    s32 *temp_v0_24;
    s32 *temp_v0_25;
    s32 *temp_v0_26;
    s32 *temp_v0_27;
    s32 *temp_v0_28;
    s32 *temp_v0_29;
    s32 *temp_v0_2;
    s32 *temp_v0_31;
    s32 *temp_v0_32;
    s32 *temp_v0_33;
    s32 *temp_v0_34;
    s32 *temp_v0_35;
    s32 *temp_v0_36;
    s32 *temp_v0_37;
    s32 *temp_v0_38;
    s32 *temp_v0_39;
    s32 *temp_v0_3;
    s32 *temp_v0_40;
    s32 *temp_v0_41;
    s32 *temp_v0_42;
    s32 *temp_v0_43;
    s32 *temp_v0_44;
    s32 *temp_v0_45;
    s32 *temp_v0_46;
    s32 *temp_v0_47;
    s32 *temp_v0_48;
    s32 *temp_v0_49;
    s32 *temp_v0_4;
    s32 *temp_v0_50;
    s32 *temp_v0_51;
    s32 *temp_v0_52;
    s32 *temp_v0_53;
    s32 *temp_v0_54;
    s32 *temp_v0_5;
    s32 *temp_v0_6;
    s32 *temp_v0_7;
    s32 *temp_v0_8;
    s32 *temp_v0_9;
    s32 temp_s1_2;
    s32 temp_s3;
    s32 temp_t0;
    s32 temp_t7;
    s32 temp_t8;
    s32 temp_t8_2;
    s32 temp_t9;
    s32 temp_v0_12;
    s32 temp_v0_22;
    s32 var_a2;
    s32 var_s0;
    s32 var_s1;
    s32 var_s3;
    s32 var_t7;
    s32 var_t9;
    s32 var_v1;
    u16 temp_a0;
    u16 temp_a0_2;
    u16 temp_a2;
    u16 temp_a3;
    u16 temp_a3_2;
    u16 temp_t2;
    u16 temp_t3;
    u16 temp_v0_30;
    void *temp_s0;
    void *temp_s1;
    void *temp_t0_2;
    void *temp_t0_3;
    void *temp_t0_4;
    void *temp_v1;
    void *var_s1_2;
    void *var_s2;
    void *var_s6;
    void *var_t0;

    temp_s3 = arg0 * 0x10;
    temp_s1 = (u8 *)*(void **)0x8015B250 + temp_s3;
    physics_float_calc(arg3 + 0x24, 0);
    temp_t8 = arg0 * 0x48;
    temp_s0 = temp_t8 + 0x8017A510;
    memcpy(temp_s1, M2C_FIELD(temp_t8, void **, 0x8017A510), 0x10);
    M2C_FIELD(temp_s1, s16 *, 0) = (s16) ((s32) M2C_FIELD(temp_s0, f32 *, 0x24) * 2);
    M2C_FIELD(temp_s1, s16 *, 2) = (s16) ((s32) M2C_FIELD(temp_s0, f32 *, 0x28) * 2);
    M2C_FIELD(temp_s1, s16 *, 8) = (s16) ((s32) M2C_FIELD(temp_s0, f32 *, 0x2C) * 4);
    M2C_FIELD(temp_s1, s16 *, 0xA) = (s16) ((s32) M2C_FIELD(temp_s0, f32 *, 0x30) * 4);
    temp_v1 = M2C_FIELD(temp_s0, void **, 4);
    temp_v0 = *(s32 **)0x801497C8;
    temp_a2 = M2C_FIELD(temp_v1, u16 *, 0);
    temp_a3 = M2C_FIELD(temp_v1, u16 *, 2);
    temp_t2 = M2C_FIELD(temp_v1, u16 *, 4);
    temp_t3 = M2C_FIELD(temp_v1, u16 *, 6);
    *(s32 **)0x801497C8 = temp_v0 + 2;
    M2C_FIELD(temp_v0, s32 *, 0) = 0xDC080008;
    M2C_FIELD(temp_v0, s32 *, 4) = (s32) ((u8 *)*(void **)0x8015B250 + (arg0 * 0x10));
    temp_a1 = *(s32 **)0x801497C8;
    var_f6 = (f32) temp_a2;
    *(s32 **)0x801497C8 = temp_a1 + 2;
    M2C_FIELD(temp_a1, s32 *, 4) = 0;
    M2C_FIELD(temp_a1, s32 *, 0) = 0xE7000000;
    temp_t1 = *(s32 **)0x801497C8;
    *(s32 **)0x801497C8 = temp_t1 + 2;
    if ((s32) temp_a2 < 0) {
        var_f6 += 4294967296.0f;
    }
    var_f6_2 = (f32) temp_a3;
    if ((s32) temp_a3 < 0) {
        var_f6_2 += 4294967296.0f;
    }
    var_f6_3 = (f32) temp_t2;
    M2C_FIELD(temp_t1, s32 *, 0) = (((s32) (var_f6 * 4.0f) & 0xFFF) << 0xC) | 0xED000000 | ((s32) (var_f6_2 * 4.0f) & 0xFFF);
    if ((s32) temp_t2 < 0) {
        var_f6_3 += 4294967296.0f;
    }
    var_f6_4 = (f32) temp_t3;
    if ((s32) temp_t3 < 0) {
        var_f6_4 += 4294967296.0f;
    }
    M2C_FIELD(temp_t1, s32 *, 4) = (s32) ((((s32) (var_f6_3 * 4.0f) & 0xFFF) << 0xC) | ((s32) (var_f6_4 * 4.0f) & 0xFFF));
    temp_v0_2 = *(s32 **)0x801497C8;
    *(s32 **)0x801497C8 = temp_v0_2 + 2;
    M2C_FIELD(temp_v0_2, s32 *, 0) = 0xF8000000;
    M2C_FIELD(temp_v0_2, s32 *, 4) = (s32) (M2C_FIELD(temp_s0, u8 *, 0x47) | (M2C_FIELD(temp_s0, u8 *, 0x44) << 0x18) | (M2C_FIELD(temp_s0, u8 *, 0x45) << 0x10) | (M2C_FIELD(temp_s0, u8 *, 0x46) << 8));
    temp_v0_3 = *(s32 **)0x801497C8;
    *(s32 **)0x801497C8 = temp_v0_3 + 2;
    M2C_FIELD(temp_v0_3, s32 *, 0) = 0xDB080000;
    temp_a3_2 = M2C_FIELD(temp_s0, u16 *, 0x40);
    temp_t0 = M2C_FIELD(temp_s0, u16 *, 0x42) - temp_a3_2;
    M2C_FIELD(temp_v0_3, s32 *, 4) = (s32) (((0x1F400 / temp_t0) << 0x10) | (((s32) (((s32) temp_a3_2 * -0x100) + 0x1F400) / temp_t0) & 0xFFFF));
    temp_v0_4 = *(s32 **)0x801497C8;
    *(s32 **)0x801497C8 = temp_v0_4 + 2;
    M2C_FIELD(temp_v0_4, s32 *, 4) = 0x100000;
    M2C_FIELD(temp_v0_4, s32 *, 0) = 0xE3000A01;
    temp_v0_5 = *(s32 **)0x801497C8;
    *(s32 **)0x801497C8 = temp_v0_5 + 2;
    M2C_FIELD(temp_v0_5, s32 *, 4) = 0x10000;
    M2C_FIELD(temp_v0_5, s32 *, 0) = 0xD9FFFFFF;
    temp_v0_6 = *(s32 **)0x801497C8;
    sp170 = -1;
    *(s32 **)0x801497C8 = temp_v0_6 + 2;
    M2C_FIELD(temp_v0_6, s32 *, 0) = 0xE2001D00;
    M2C_FIELD(temp_v0_6, s32 *, 4) = 0;
    if (*(s32 *)0x8017A638 != 1) {
        temp_v0_7 = *(s32 **)0x801497C8;
        *(s32 **)0x801497C8 = temp_v0_7 + 2;
        M2C_FIELD(temp_v0_7, s32 *, 4) = 0x8000;
        M2C_FIELD(temp_v0_7, s32 *, 0) = 0xE3001001;
        *(s32 *)0x8017A638 = 1;
    }
    if (M2C_FIELD(temp_s0, s32 *, 8) != 0) {
        temp_f0 = M2C_FIELD(temp_s0, f32 *, 0x24);
        temp_f2 = M2C_FIELD(temp_s0, f32 *, 0x28);
        temp_t7 = arg0 << 6;
        var_s1 = temp_t7;
        guOrtho(0x3F000000, (u8 *)*(void **)0x8015B250 + temp_t7 + 0x80, -temp_f0 * 0.5f, temp_f0 * 0.5f, -temp_f2 * 0.5f, temp_f2 * 0.5f, M2C_FIELD(temp_s0, f32 *, 0x38) * 16.0f, M2C_FIELD(temp_s0, f32 *, 0x3C) * 16.0f, 1.0f);
    } else {
        temp_t8_2 = arg0 << 6;
        var_s1 = temp_t8_2;
        guPerspective((u8 *)*(void **)0x8015B250 + temp_t8_2 + 0x80, &sp142, M2C_FIELD(temp_s0, f32 *, 0x10) * *(f32 *)0x80123B60, M2C_FIELD(temp_s0, s32 *, 0x34), M2C_FIELD(temp_s0, f32 *, 0x38) * 16.0f, M2C_FIELD(temp_s0, f32 *, 0x3C) * 16.0f, 1.0f);
        temp_v0_8 = *(s32 **)0x801497C8;
        *(s32 **)0x801497C8 = temp_v0_8 + 2;
        M2C_FIELD(temp_v0_8, s32 *, 0) = 0xDB0E0000;
        M2C_FIELD(temp_v0_8, s32 *, 4) = (s32) sp142;
    }
    temp_v0_9 = *(s32 **)0x801497C8;
    *(s32 **)0x801497C8 = temp_v0_9 + 2;
    M2C_FIELD(temp_v0_9, s32 *, 0) = 0xDA380007;
    M2C_FIELD(temp_v0_9, s32 *, 4) = (s32) ((u8 *)*(void **)0x8015B250 + var_s1 + 0x80);
    if (*(s8 *)0x80151AD8 != 0) {
        temp_t0_2 = (arg0 * 0x98) + 0x80150B70;
        sp48 = temp_t0_2;
        math_utility(temp_t0_2, &sp114);
        func_8009EA68(*(f32 *)0x80123B64, &sp114);
        if (*(s8 *)0x80140A04 != 0) {
            sp12C *= -1.0f;
            sp120 *= -1.0f;
        }
        guLookAtF(0x801613F0, 0, 0, 0, -sp12C, sp130, sp134, -sp120, sp124, sp128);
    } else if (*(s32 *)0x80140A04 != 0) {
        temp_t0_3 = (arg0 * 0x98) + 0x80150B70;
        sp48 = temp_t0_3;
        guLookAtF(0x801613F0, 0, 0, 0, --M2C_FIELD(temp_t0_3, f32 *, 0x18), M2C_FIELD(temp_t0_3, f32 *, 0x1C), M2C_FIELD(temp_t0_3, f32 *, 0x20), --M2C_FIELD(temp_t0_3, f32 *, 0xC), M2C_FIELD(temp_t0_3, f32 *, 0x10), M2C_FIELD(temp_t0_3, f32 *, 0x14));
    } else {
        temp_t0_4 = (arg0 * 0x98) + 0x80150B70;
        sp48 = temp_t0_4;
        guLookAtF(0x801613F0, 0, 0, 0, -M2C_FIELD(temp_t0_4, f32 *, 0x18), M2C_FIELD(temp_t0_4, f32 *, 0x1C), M2C_FIELD(temp_t0_4, f32 *, 0x20), -M2C_FIELD(temp_t0_4, f32 *, 0xC), M2C_FIELD(temp_t0_4, f32 *, 0x10), M2C_FIELD(temp_t0_4, f32 *, 0x14));
    }
    guLookAtF(0x80124EF0, M2C_FIELD(sp48, s32 *, 0x18), M2C_FIELD(sp48, s32 *, 0x1C), M2C_FIELD(sp48, s32 *, 0x20), 0.0f, 0.0f, 0.0f, M2C_FIELD(sp48, f32 *, 0xC), M2C_FIELD(sp48, f32 *, 0x10), M2C_FIELD(sp48, f32 *, 0x14));
    temp_f0_2 = *(f32 *)0x801613B8;
    var_a0 = (f32 *)0x80124EF0;
    if (temp_f0_2 != 1.0f) {
        do {
            var_v0 = var_a0;
            var_v1 = 1;
            var_f18 = *var_v0 * temp_f0_2;
            if (1 != 3) {
                do {
                    var_v1 += 1;
                    M2C_FIELD(var_v0, f32 *, 0) = var_f18;
                    var_f18 = M2C_FIELD(var_v0, f32 *, 4) * temp_f0_2;
                    var_v0 = var_v0 + 1;
                } while (var_v1 != 3);
            }
            *var_v0 = var_f18;
            var_a0 = var_a0 + 4;
        } while (var_a0 != (f32 *)0x80124F20);
    }
    func_8009E9D8(0x80124F30, 3, 0x80124F20);
    if (*(s16 *)0x8014A108 < 2) {
        M2C_FIELD(((u8 *)*(void **)0x8015B250 + temp_s3), s16 *, 0x40) = (s16) (s32) (M2C_FIELD((void *)0x80124EF0, f32 *, 8) * 480.0f);
        M2C_FIELD(((u8 *)*(void **)0x8015B250 + temp_s3), s16 *, 0x42) = (s16) (s32) (M2C_FIELD((void *)0x80124EF0, f32 *, 0x18) * 480.0f);
        M2C_FIELD(((u8 *)*(void **)0x8015B250 + temp_s3), s16 *, 0x44) = (s16) (s32) (M2C_FIELD((void *)0x80124EF0, f32 *, 0x28) * 480.0f);
    } else {
        M2C_FIELD(((u8 *)*(void **)0x8015B250 + temp_s3), s16 *, 0x40) = 0;
        M2C_FIELD(((u8 *)*(void **)0x8015B250 + temp_s3), s16 *, 0x42) = 0;
        M2C_FIELD(((u8 *)*(void **)0x8015B250 + temp_s3), s16 *, 0x44) = 0;
    }
    func_8009E8B4(0x801613F0, (u8 *)*(void **)0x8015B250 + var_s1 + 0x180);
    temp_v0_10 = *(s32 **)0x801497C8;
    *(s32 **)0x801497C8 = temp_v0_10 + 2;
    M2C_FIELD(temp_v0_10, s32 *, 0) = 0xDA380005;
    M2C_FIELD(temp_v0_10, s32 *, 4) = (s32) ((u8 *)*(void **)0x8015B250 + var_s1 + 0x180);
    guMtxIdent((u8 *)*(void **)0x8015B250 + 0x680);
    var_t0 = sp48;
    if (*(s32 *)0x80140A04 == 0) {
        M2C_FIELD(*(void **)0x8015B250, s32 *, 0x680) = 0xFFFF0000;
    }
    temp_v0_11 = *(s32 **)0x801497C8;
    *(s32 **)0x801497C8 = temp_v0_11 + 2;
    M2C_FIELD(temp_v0_11, s32 *, 0) = 0xDA380003;
    M2C_FIELD(temp_v0_11, s32 *, 4) = (s32) ((u8 *)*(void **)0x8015B250 + 0x680);
    *(s32 *)0x80161430 = 0;
    *(s16 *)0x80124FC4 = 0;
    *(s16 *)0x80124FCC = -1;
    temp_t1_2 = *(s16 *)0x8015B254;
    *(s16 *)0x80124FE0 = 0;
    *(s16 *)0x80124FE2 = -1;
    if (temp_t1_2 >= 0) {
        track_collision_wall(0x801497C8, 0, (temp_t1_2 * 0x44) + 0x8012E700, arg0, 0, 1, 0, 0);
        *(s32 *)0x8017A638 = -1;
        *(s32 *)0x8017A508 = 0;
    }
    var_a2 = *(s32 *)0x801613B4;
    sp174 = 1;
    var_s6 = (void *)0x8015B268;
    sp178 = -1;
    sp16C = 0;
    if (var_a2 > 0) {
        do {
            temp_a0 = M2C_FIELD(var_s6, u16 *, 2);
            if (!(temp_a0 & 0x8000) && (temp_a0 & (1 << arg0)) && (M2C_FIELD(var_s6, s16 *, 0) != 0)) {
                temp_v0_13 = *(s32 **)0x801497C8;
                *(s32 **)0x801497C8 = temp_v0_13 + 2;
                M2C_FIELD(temp_v0_13, s32 *, 4) = 0;
                M2C_FIELD(temp_v0_13, s32 *, 0) = 0xE7000000;
                if (M2C_FIELD(var_s6, u16 *, 2) & 0x2000) {
                    if (sp170 < 0) {
                        temp_v0_14 = *(s32 **)0x801497C8;
                        *(s32 **)0x801497C8 = temp_v0_14 + 2;
                        M2C_FIELD(temp_v0_14, s32 *, 4) = 4;
                        M2C_FIELD(temp_v0_14, s32 *, 0) = 0xE2001D00;
                        if (sp170 != M2C_FIELD(var_s6, s16 *, 6)) {
                            temp_v0_15 = *(s32 **)0x801497C8;
                            *(s32 **)0x801497C8 = temp_v0_15 + 2;
                            M2C_FIELD(temp_v0_15, s32 *, 0) = 0xEE000000;
                            M2C_FIELD(temp_v0_15, s32 *, 4) = (s32) (M2C_FIELD(var_s6, s16 *, 6) << 0x10);
                        }
                        sp170 = (s32) M2C_FIELD(var_s6, s16 *, 6);
                    }
                } else if (sp170 >= 0) {
                    temp_v0_16 = *(s32 **)0x801497C8;
                    sp170 = -1;
                    *(s32 **)0x801497C8 = temp_v0_16 + 2;
                    M2C_FIELD(temp_v0_16, s32 *, 0) = 0xE2001D00;
                    M2C_FIELD(temp_v0_16, s32 *, 4) = 0;
                }
                if (M2C_FIELD(var_s6, u16 *, 2) & 0x4000) {
                    if (sp178 != 0) {
                        temp_v0_17 = *(s32 **)0x801497C8;
                        sp178 = 0;
                        *(s32 **)0x801497C8 = temp_v0_17 + 2;
                        M2C_FIELD(temp_v0_17, s32 *, 0) = 0xE200001C;
                        M2C_FIELD(temp_v0_17, s32 *, 4) = 0x504240;
                        temp_v0_18 = *(s32 **)0x801497C8;
                        *(s32 **)0x801497C8 = temp_v0_18 + 2;
                        M2C_FIELD(temp_v0_18, s32 *, 4) = 0xFFFDF6FB;
                        M2C_FIELD(temp_v0_18, s32 *, 0) = 0xFCFFFFFF;
                    }
                    temp_v0_19 = *(s32 **)0x801497C8;
                    *(s32 **)0x801497C8 = temp_v0_19 + 2;
                    M2C_FIELD(temp_v0_19, s32 *, 0) = 0xFA000000;
                    M2C_FIELD(temp_v0_19, s32 *, 4) = (s32) (M2C_FIELD(var_s6, u8 *, 0x1B) | (M2C_FIELD(var_s6, u8 *, 0x18) << 0x18) | (M2C_FIELD(var_s6, u8 *, 0x19) << 0x10) | (M2C_FIELD(var_s6, u8 *, 0x1A) << 8));
                    temp_v0_20 = *(s32 **)0x801497C8;
                    *(s32 **)0x801497C8 = temp_v0_20 + 2;
                    M2C_FIELD(temp_v0_20, s32 *, 0) = (((s32) M2C_FIELD(var_s6, f32 *, 0x20) & 0x3FF) * 4) | 0xF6000000 | (((s32) M2C_FIELD(var_s6, f32 *, 0x1C) & 0x3FF) << 0xE);
                    M2C_FIELD(temp_v0_20, s32 *, 4) = (s32) ((((s32) M2C_FIELD(var_s6, f32 *, 0xC) & 0x3FF) * 4) | (((s32) M2C_FIELD(var_s6, f32 *, 8) & 0x3FF) << 0xE));
                    temp_v0_21 = *(s32 **)0x801497C8;
                    *(s32 **)0x801497C8 = temp_v0_21 + 2;
                    M2C_FIELD(temp_v0_21, s32 *, 4) = 0;
                    M2C_FIELD(temp_v0_21, s32 *, 0) = 0xE7000000;
                } else {
                    var_s3 = 0;
                    if (M2C_FIELD(var_s6, s16 *, 0) > 0) {
                        var_s1_2 = var_s6;
loop_50:
                        if (!(M2C_FIELD(var_s6, u16 *, 2) & 0x10)) {
                            spD4 = M2C_FIELD(var_s1_2, f32 *, 8) - (M2C_FIELD(var_t0, f32 *, 0x24) * 16.0f);
                            spD8 = M2C_FIELD(var_s1_2, f32 *, 0xC) - (M2C_FIELD(var_t0, f32 *, 0x28) * 16.0f);
                            spDC = M2C_FIELD(var_s1_2, f32 *, 0x10) - (M2C_FIELD(var_t0, f32 *, 0x2C) * 16.0f);
                            if (!(spD4 <= -32768.0f) && !(spD4 >= 32768.0f) && !(spD8 <= -32768.0f) && !(spD8 >= 32768.0f) && !(spDC <= -32768.0f) && !(spDC >= 32768.0f)) {
                                var_s0 = var_s3 * 0x10;
                                var_s2 = (u8 *) var_s1_2 + 0x14;
                                goto block_59;
                            }
                        } else {
                            var_s0 = var_s3 * 0x10;
                            var_s2 = (u8 *) var_s1_2 + 0x14;
                            sp48 = var_t0;
                            func_8009E820((u8 *) var_s1_2 + 8, &spD4, var_t0);
                            var_t0 = sp48;
block_59:
                            *(s16 *)(*(s32 *)0x80161434 + var_s0) = (s16) (s32) spD4;
                            M2C_FIELD((*(s32 *)0x80161434 + var_s0), s16 *, 2) = (s16) (s32) spD8;
                            M2C_FIELD((*(s32 *)0x80161434 + var_s0), s16 *, 4) = (s16) (s32) spDC;
                            sp48 = var_t0;
                            memcpy((var_s3 * 0x10) + *(s32 *)0x80161434 + 8, var_s2, 8);
                            var_s3 += 1;
                            var_s1_2 = (u8 *) var_s1_2 + 0x14;
                            if (var_s3 >= M2C_FIELD(var_s6, s16 *, 0)) {
                                goto block_61;
                            }
                            goto loop_50;
                        }
                    } else {
block_61:
                        temp_v0_22 = M2C_FIELD(var_s6, u16 *, 2) & 0x100;
                        if ((temp_v0_22 != 0) && (sp174 != 0)) {
                            temp_v0_23 = *(s32 **)0x801497C8;
                            sp174 = 0;
                            sp178 = -1;
                            *(s32 **)0x801497C8 = temp_v0_23 + 2;
                            M2C_FIELD(temp_v0_23, s32 *, 0) = 0xD9FEFFFF;
                            M2C_FIELD(temp_v0_23, s32 *, 4) = 0;
                            temp_v0_24 = *(s32 **)0x801497C8;
                            *(s32 **)0x801497C8 = temp_v0_24 + 2;
                            M2C_FIELD(temp_v0_24, s32 *, 4) = 0;
                            M2C_FIELD(temp_v0_24, s32 *, 0) = 0xE7000000;
                            temp_v0_25 = *(s32 **)0x801497C8;
                            *(s32 **)0x801497C8 = temp_v0_25 + 2;
                            M2C_FIELD(temp_v0_25, s32 *, 4) = 0;
                            M2C_FIELD(temp_v0_25, s32 *, 0) = 0xE3000A01;
                        } else if ((temp_v0_22 == 0) && (sp174 == 0)) {
                            temp_v0_26 = *(s32 **)0x801497C8;
                            sp174 = 1;
                            sp178 = -1;
                            *(s32 **)0x801497C8 = temp_v0_26 + 2;
                            M2C_FIELD(temp_v0_26, s32 *, 4) = 0x10000;
                            M2C_FIELD(temp_v0_26, s32 *, 0) = 0xD9FFFFFF;
                            temp_v0_27 = *(s32 **)0x801497C8;
                            *(s32 **)0x801497C8 = temp_v0_27 + 2;
                            M2C_FIELD(temp_v0_27, s32 *, 4) = 0;
                            M2C_FIELD(temp_v0_27, s32 *, 0) = 0xE7000000;
                            temp_v0_28 = *(s32 **)0x801497C8;
                            *(s32 **)0x801497C8 = temp_v0_28 + 2;
                            M2C_FIELD(temp_v0_28, s32 *, 4) = 0x100000;
                            M2C_FIELD(temp_v0_28, s32 *, 0) = 0xE3000A01;
                        }
                        if (sp174 != 0) {
                            temp_v0_29 = *(s32 **)0x801497C8;
                            *(s32 **)0x801497C8 = temp_v0_29 + 2;
                            M2C_FIELD(temp_v0_29, s32 *, 0) = 0xFA000000;
                            M2C_FIELD(temp_v0_29, s32 *, 4) = (s32) (M2C_FIELD(var_s6, u8 *, 0x1B) | (M2C_FIELD(var_s6, u8 *, 0x18) << 0x18) | (M2C_FIELD(var_s6, u8 *, 0x19) << 0x10) | (M2C_FIELD(var_s6, u8 *, 0x1A) << 8));
                        }
                        temp_a0_2 = M2C_FIELD(var_s6, u16 *, 2);
                        if (temp_a0_2 & 0x400) {
                            temp_v0_30 = M2C_FIELD(var_s6, u16 *, 4);
                            temp_t9 = ((s32) temp_v0_30 >> 0xA) * 8;
                            temp_s1_2 = M2C_FIELD((temp_t9 + 0x80150000), s32 *, 0x1AE8) + ((temp_v0_30 & 0x3FF) * 0x24);
                            if (temp_s1_2 != *(s32 *)0x8017A508) {
                                spB4 = temp_s1_2;
                                sp48 = var_t0;
                                render_display_list(temp_a0_2, temp_t9);
                                *(s32 *)0x8017A508 = temp_s1_2;
                            }
                            if (M2C_FIELD(var_s6, u16 *, 2) & 0x200) {
                                if (sp178 != 2) {
                                    sp178 = 2;
                                    if (sp174 == 0) {
                                        temp_v0_31 = *(s32 **)0x801497C8;
                                        *(s32 **)0x801497C8 = temp_v0_31 + 2;
                                        M2C_FIELD(temp_v0_31, s32 *, 0) = 0xE200001C;
                                        M2C_FIELD(temp_v0_31, s32 *, 4) = 0x504A70;
                                        temp_v0_32 = *(s32 **)0x801497C8;
                                        *(s32 **)0x801497C8 = temp_v0_32 + 2;
                                        var_t7 = 0xFC121824;
                                        M2C_FIELD(temp_v0_32, s32 *, 4) = 0xFF33FFFF;
                                        goto block_85;
                                    }
                                    temp_v0_33 = *(s32 **)0x801497C8;
                                    *(s32 **)0x801497C8 = temp_v0_33 + 2;
                                    M2C_FIELD(temp_v0_33, s32 *, 4) = 0xC8104A50;
                                    M2C_FIELD(temp_v0_33, s32 *, 0) = 0xE200001C;
                                    temp_v0_34 = *(s32 **)0x801497C8;
                                    *(s32 **)0x801497C8 = temp_v0_34 + 2;
                                    M2C_FIELD(temp_v0_34, s32 *, 4) = -0x1C8;
                                    M2C_FIELD(temp_v0_34, s32 *, 0) = 0xFC1217FF;
                                }
                            } else if (sp178 != 3) {
                                sp178 = 3;
                                if (sp174 == 0) {
                                    temp_v0_35 = *(s32 **)0x801497C8;
                                    *(s32 **)0x801497C8 = temp_v0_35 + 2;
                                    M2C_FIELD(temp_v0_35, s32 *, 0) = 0xE200001C;
                                    M2C_FIELD(temp_v0_35, s32 *, 4) = 0x504240;
                                    temp_v0_36 = *(s32 **)0x801497C8;
                                    *(s32 **)0x801497C8 = temp_v0_36 + 2;
                                    M2C_FIELD(temp_v0_36, s32 *, 4) = 0xFF33FFFF;
                                    M2C_FIELD(temp_v0_36, s32 *, 0) = 0xFC121824;
                                } else {
                                    temp_v0_37 = *(s32 **)0x801497C8;
                                    *(s32 **)0x801497C8 = temp_v0_37 + 2;
                                    M2C_FIELD(temp_v0_37, s32 *, 4) = 0xC8104240;
                                    M2C_FIELD(temp_v0_37, s32 *, 0) = 0xE200001C;
                                    temp_v0_38 = *(s32 **)0x801497C8;
                                    *(s32 **)0x801497C8 = temp_v0_38 + 2;
                                    M2C_FIELD(temp_v0_38, s32 *, 4) = -0x1C8;
                                    M2C_FIELD(temp_v0_38, s32 *, 0) = 0xFC1217FF;
                                }
                            }
                        } else if (sp178 != 1) {
                            sp178 = 1;
                            if (sp174 == 0) {
                                temp_v0_39 = *(s32 **)0x801497C8;
                                *(s32 **)0x801497C8 = temp_v0_39 + 2;
                                M2C_FIELD(temp_v0_39, s32 *, 0) = 0xE200001C;
                                M2C_FIELD(temp_v0_39, s32 *, 4) = 0x504A50;
                                temp_v0_40 = *(s32 **)0x801497C8;
                                *(s32 **)0x801497C8 = temp_v0_40 + 2;
                                M2C_FIELD(temp_v0_40, s32 *, 4) = 0xFFFE793C;
                                M2C_FIELD(temp_v0_40, s32 *, 0) = 0xFCFFFFFF;
                            } else {
                                temp_v0_41 = *(s32 **)0x801497C8;
                                *(s32 **)0x801497C8 = temp_v0_41 + 2;
                                M2C_FIELD(temp_v0_41, s32 *, 4) = 0xC8104A70;
                                M2C_FIELD(temp_v0_41, s32 *, 0) = 0xE200001C;
                                temp_v0_42 = *(s32 **)0x801497C8;
                                *(s32 **)0x801497C8 = temp_v0_42 + 2;
                                var_t7 = 0xFCFFFFFF;
                                M2C_FIELD(temp_v0_42, s32 *, 4) = 0xFFFE7638;
block_85:
                                M2C_FIELD(*(s32 **)0x801497C8, s32 *, 0) = var_t7;
                            }
                        }
                        temp_v0_43 = *(s32 **)0x801497C8;
                        *(s32 **)0x801497C8 = temp_v0_43 + 2;
                        temp_a0_3 = M2C_FIELD(var_s6, s16 *, 0);
                        M2C_FIELD(temp_v0_43, s32 *, 0) = ((temp_a0_3 & 0xFF) << 0xC) | 0x01000000 | ((temp_a0_3 & 0x7F) * 2);
                        M2C_FIELD(temp_v0_43, s32 *, 4) = (s32) *(s32 *)0x80161434;
                        *(s32 *)0x80161434 += M2C_FIELD(var_s6, s16 *, 0) * 0x10;
                        if (M2C_FIELD(var_s6, u16 *, 2) & 0x1000) {
                            temp_v0_44 = *(s32 **)0x801497C8;
                            *(s32 **)0x801497C8 = temp_v0_44 + 2;
                            M2C_FIELD(temp_v0_44, s32 *, 4) = 0x40002;
                            M2C_FIELD(temp_v0_44, s32 *, 0) = 0x06000402;
                            if (M2C_FIELD(var_s6, s16 *, 0) >= 4) {
                                temp_v0_45 = *(s32 **)0x801497C8;
                                *(s32 **)0x801497C8 = temp_v0_45 + 2;
                                M2C_FIELD(temp_v0_45, s32 *, 4) = 0x406;
                                M2C_FIELD(temp_v0_45, s32 *, 0) = 0x06040006;
                            }
                        } else if (*(s32 *)0x80140A04 != 0) {
                            if (M2C_FIELD(var_s6, s16 *, 0) == 3) {
                                temp_v0_46 = *(s32 **)0x801497C8;
                                var_t9 = 0x05020004;
                                *(s32 **)0x801497C8 = temp_v0_46 + 2;
                                M2C_FIELD(temp_v0_46, s32 *, 4) = 0;
                                goto block_96;
                            }
                            temp_v0_47 = *(s32 **)0x801497C8;
                            *(s32 **)0x801497C8 = temp_v0_47 + 2;
                            M2C_FIELD(temp_v0_47, s32 *, 4) = 0x604;
                            M2C_FIELD(temp_v0_47, s32 *, 0) = 0x06020004;
                        } else if (M2C_FIELD(var_s6, s16 *, 0) == 3) {
                            temp_v0_48 = *(s32 **)0x801497C8;
                            *(s32 **)0x801497C8 = temp_v0_48 + 2;
                            M2C_FIELD(temp_v0_48, s32 *, 4) = 0;
                            M2C_FIELD(temp_v0_48, s32 *, 0) = 0x05040002;
                        } else {
                            temp_v0_49 = *(s32 **)0x801497C8;
                            var_t9 = 0x06040002;
                            *(s32 **)0x801497C8 = temp_v0_49 + 2;
                            M2C_FIELD(temp_v0_49, s32 *, 4) = 0x406;
block_96:
                            M2C_FIELD(*(s32 **)0x801497C8, s32 *, 0) = var_t9;
                        }
                    }
                }
                var_a2 = *(s32 *)0x801613B4;
            }
            var_s6 = (u8 *) var_s6 + 0x58;
            temp_v0_12 = sp16C + 1;
            sp16C = temp_v0_12;
        } while (temp_v0_12 < var_a2);
    }
    if (*(s32 *)0x8017A638 != 1) {
        temp_v0_50 = *(s32 **)0x801497C8;
        *(s32 **)0x801497C8 = temp_v0_50 + 2;
        M2C_FIELD(temp_v0_50, s32 *, 4) = 0x8000;
        M2C_FIELD(temp_v0_50, s32 *, 0) = 0xE3001001;
        *(s32 *)0x8017A638 = 1;
    }
    if (sp170 >= 0) {
        temp_v0_51 = *(s32 **)0x801497C8;
        *(s32 **)0x801497C8 = temp_v0_51 + 2;
        M2C_FIELD(temp_v0_51, s32 *, 4) = 0;
        M2C_FIELD(temp_v0_51, s32 *, 0) = 0xE2001D00;
    }
    if (sp174 == 0) {
        temp_v0_52 = *(s32 **)0x801497C8;
        *(s32 **)0x801497C8 = temp_v0_52 + 2;
        M2C_FIELD(temp_v0_52, s32 *, 4) = 0x10000;
        M2C_FIELD(temp_v0_52, s32 *, 0) = 0xD9FFFFFF;
        temp_v0_53 = *(s32 **)0x801497C8;
        *(s32 **)0x801497C8 = temp_v0_53 + 2;
        M2C_FIELD(temp_v0_53, s32 *, 4) = 0;
        M2C_FIELD(temp_v0_53, s32 *, 0) = 0xE7000000;
        temp_v0_54 = *(s32 **)0x801497C8;
        *(s32 **)0x801497C8 = temp_v0_54 + 2;
        M2C_FIELD(temp_v0_54, s32 *, 4) = 0x100000;
        M2C_FIELD(temp_v0_54, s32 *, 0) = 0xE3000A01;
    }
    temp_v0_55 = *(s32 *)0x80124FCC;
    if (temp_v0_55 >= 0) {
        *(s32 *)0x80124FC4 = 1;
        *(s32 *)0x80124FE0 = 0;
        track_collision_wall(0x801497C8, 0, (temp_v0_55 * 0x44) + 0x8012E700, arg0, 0, 1, 0, 0);
        *(s32 *)0x8017A638 = -1;
        *(s32 *)0x8017A508 = 0;
    }
    temp_v0_56 = *(s32 *)0x80124FE2;
    if (temp_v0_56 >= 0) {
        *(s32 *)0x80124FC4 = 0;
        *(s32 *)0x80124FE0 = 1;
        track_collision_wall(0x801497C8, 0, (temp_v0_56 * 0x44) + 0x8012E700, arg0, 0, 1, 0, 0);
        *(s32 *)0x8017A638 = -1;
        *(s32 *)0x8017A508 = 0;
    }
}
