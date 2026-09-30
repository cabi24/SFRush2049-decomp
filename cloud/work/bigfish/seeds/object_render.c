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

s32 __ashldi3(M2C_UNK, u16, M2C_UNK, M2C_UNK);      /* extern */
M2C_UNK func_80086A50(s32, s32);                    /* extern */
u16 func_80087804(u16);                             /* extern */
M2C_UNK func_800878E0(M2C_UNK, s32);                /* extern */

void object_render(s32 arg0, u8 arg1, u8 arg2, u16 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 arg10) {
    s32 sp274;
    u16 sp272;
    void *sp258;
    void *sp220;
    void *sp1E4;
    void *sp168;
    void *spF4;
    void *spBC;
    u32 sp5C;
    s32 sp58;
    u32 sp54;
    s32 sp50;
    s32 sp40;
    s32 sp3C;
    s32 sp38;
    s32 sp34;
    s32 sp30;
    s32 sp2C;
    s32 sp28;
    s32 sp24;
    s32 temp_a3;
    s32 temp_a3_2;
    s32 temp_a3_3;
    s32 temp_a3_4;
    s32 temp_a3_5;
    s32 temp_a3_6;
    s32 temp_ret;
    s32 temp_ret_2;
    s32 temp_s0;
    s32 temp_s0_10;
    s32 temp_s0_11;
    s32 temp_s0_2;
    s32 temp_s0_3;
    s32 temp_s0_4;
    s32 temp_s0_5;
    s32 temp_s0_6;
    s32 temp_s0_7;
    s32 temp_s0_8;
    s32 temp_s0_9;
    s32 temp_s1_10;
    s32 temp_s1_11;
    s32 temp_s1_12;
    s32 temp_s1_13;
    s32 temp_s1_2;
    s32 temp_s1_3;
    s32 temp_s1_4;
    s32 temp_s1_5;
    s32 temp_s1_6;
    s32 temp_s1_7;
    s32 temp_s1_8;
    s32 temp_s1_9;
    s32 temp_t0;
    s32 temp_t0_2;
    s32 temp_t0_3;
    s32 temp_t0_4;
    s32 temp_t0_5;
    s32 temp_t0_6;
    s32 temp_t0_7;
    s32 temp_t0_8;
    s32 temp_t0_9;
    s32 temp_t1;
    s32 temp_t1_10;
    s32 temp_t1_11;
    s32 temp_t1_12;
    s32 temp_t1_13;
    s32 temp_t1_14;
    s32 temp_t1_15;
    s32 temp_t1_16;
    s32 temp_t1_2;
    s32 temp_t1_3;
    s32 temp_t1_4;
    s32 temp_t1_5;
    s32 temp_t1_6;
    s32 temp_t1_7;
    s32 temp_t1_8;
    s32 temp_t1_9;
    s32 temp_t2;
    s32 temp_t2_2;
    s32 temp_t2_3;
    s32 temp_t2_4;
    s32 temp_t2_5;
    s32 temp_t2_6;
    s32 temp_t2_7;
    s32 temp_t2_8;
    s32 temp_t2_9;
    s32 temp_t3;
    s32 temp_t3_2;
    s32 temp_t3_3;
    s32 temp_t6;
    s32 temp_t6_2;
    s32 temp_t6_3;
    s32 temp_t6_4;
    s32 temp_t6_5;
    s32 temp_t7;
    s32 temp_t7_2;
    s32 temp_t7_3;
    s32 temp_t7_4;
    s32 temp_t7_5;
    s32 temp_t7_6;
    s32 temp_t8;
    s32 temp_t8_2;
    s32 temp_t8_3;
    s32 temp_t8_4;
    s32 temp_t9;
    s32 temp_t9_2;
    s32 temp_t9_3;
    s32 temp_t9_4;
    s32 temp_t9_5;
    s32 temp_t9_6;
    s32 temp_t9_7;
    s32 temp_t9_8;
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_v1_2;
    s32 temp_v1_3;
    s32 temp_v1_4;
    s32 temp_v1_5;
    s32 temp_v1_6;
    s32 var_a1;
    s32 var_s0;
    s32 var_s1;
    s32 var_t0;
    s32 var_t0_2;
    s32 var_t0_3;
    s32 var_t0_4;
    s32 var_t0_5;
    s32 var_t0_6;
    s32 var_t0_7;
    s32 var_t0_8;
    s32 var_t0_9;
    s32 var_t2;
    s32 var_t2_2;
    s32 var_t2_3;
    s32 var_t2_4;
    s32 var_t2_5;
    s32 var_t2_6;
    s32 var_t2_7;
    s32 var_t2_8;
    s32 var_t2_9;
    s32 var_t3;
    s32 var_t4;
    s32 var_t8;
    s32 var_t8_2;
    s32 var_v0_10;
    s32 var_v0_2;
    s32 var_v0_3;
    s32 var_v0_4;
    s32 var_v0_5;
    s32 var_v0_6;
    s32 var_v0_7;
    s32 var_v0_8;
    s32 var_v0_9;
    u16 temp_s1;
    u16 var_v0;
    void *temp_a0;
    void *temp_a0_100;
    void *temp_a0_101;
    void *temp_a0_102;
    void *temp_a0_103;
    void *temp_a0_104;
    void *temp_a0_105;
    void *temp_a0_106;
    void *temp_a0_107;
    void *temp_a0_108;
    void *temp_a0_109;
    void *temp_a0_10;
    void *temp_a0_110;
    void *temp_a0_111;
    void *temp_a0_112;
    void *temp_a0_113;
    void *temp_a0_114;
    void *temp_a0_115;
    void *temp_a0_116;
    void *temp_a0_117;
    void *temp_a0_118;
    void *temp_a0_119;
    void *temp_a0_11;
    void *temp_a0_120;
    void *temp_a0_121;
    void *temp_a0_122;
    void *temp_a0_123;
    void *temp_a0_124;
    void *temp_a0_125;
    void *temp_a0_126;
    void *temp_a0_127;
    void *temp_a0_128;
    void *temp_a0_129;
    void *temp_a0_12;
    void *temp_a0_130;
    void *temp_a0_131;
    void *temp_a0_132;
    void *temp_a0_13;
    void *temp_a0_14;
    void *temp_a0_15;
    void *temp_a0_16;
    void *temp_a0_17;
    void *temp_a0_18;
    void *temp_a0_19;
    void *temp_a0_20;
    void *temp_a0_21;
    void *temp_a0_22;
    void *temp_a0_23;
    void *temp_a0_24;
    void *temp_a0_25;
    void *temp_a0_26;
    void *temp_a0_27;
    void *temp_a0_28;
    void *temp_a0_29;
    void *temp_a0_2;
    void *temp_a0_30;
    void *temp_a0_31;
    void *temp_a0_32;
    void *temp_a0_33;
    void *temp_a0_34;
    void *temp_a0_35;
    void *temp_a0_36;
    void *temp_a0_37;
    void *temp_a0_38;
    void *temp_a0_39;
    void *temp_a0_3;
    void *temp_a0_40;
    void *temp_a0_41;
    void *temp_a0_42;
    void *temp_a0_43;
    void *temp_a0_44;
    void *temp_a0_45;
    void *temp_a0_46;
    void *temp_a0_47;
    void *temp_a0_48;
    void *temp_a0_49;
    void *temp_a0_4;
    void *temp_a0_50;
    void *temp_a0_51;
    void *temp_a0_52;
    void *temp_a0_53;
    void *temp_a0_54;
    void *temp_a0_55;
    void *temp_a0_56;
    void *temp_a0_57;
    void *temp_a0_58;
    void *temp_a0_59;
    void *temp_a0_5;
    void *temp_a0_60;
    void *temp_a0_61;
    void *temp_a0_62;
    void *temp_a0_63;
    void *temp_a0_64;
    void *temp_a0_65;
    void *temp_a0_66;
    void *temp_a0_67;
    void *temp_a0_68;
    void *temp_a0_69;
    void *temp_a0_6;
    void *temp_a0_70;
    void *temp_a0_71;
    void *temp_a0_72;
    void *temp_a0_73;
    void *temp_a0_74;
    void *temp_a0_75;
    void *temp_a0_76;
    void *temp_a0_77;
    void *temp_a0_78;
    void *temp_a0_79;
    void *temp_a0_7;
    void *temp_a0_80;
    void *temp_a0_81;
    void *temp_a0_82;
    void *temp_a0_83;
    void *temp_a0_84;
    void *temp_a0_85;
    void *temp_a0_86;
    void *temp_a0_87;
    void *temp_a0_88;
    void *temp_a0_89;
    void *temp_a0_8;
    void *temp_a0_90;
    void *temp_a0_91;
    void *temp_a0_92;
    void *temp_a0_93;
    void *temp_a0_94;
    void *temp_a0_95;
    void *temp_a0_96;
    void *temp_a0_97;
    void *temp_a0_98;
    void *temp_a0_99;
    void *temp_a0_9;

    var_s0 = arg10;
    temp_s1 = arg3 & 0xFFFF;
    if (var_s0 == 0) {
        if ((u16) arg2 == 3) {
            arg0 += (u16) arg6 * temp_s1 * 4;
        } else if ((u16) arg2 == 2) {
            sp3C = (s32) (u16) arg6;
            arg0 += (u16) arg6 * temp_s1 * 2;
        } else {
            if ((u16) arg2 == 1) {
                sp3C = (s32) (u16) arg6;
                var_t8 = arg0 + ((u16) arg6 * temp_s1);
            } else {
                sp3C = (s32) (u16) arg6;
                var_t8 = arg0 + ((s32) ((u16) arg6 * temp_s1) / 2);
            }
            arg0 = var_t8;
        }
        (s16) arg4 = ((u16) arg8 - (u16) arg6) + 1;
        sp34 = (s32) (u16) arg8;
    } else {
        temp_ret = __ashldi3(0, (u16) arg6, 0, 0x20);
        sp50 = temp_ret;
        sp54 = (u32) (u64) temp_ret;
        temp_ret_2 = __ashldi3(0, (u16) arg5, 0, 0x30);
        sp58 = temp_ret_2;
        sp5C = (u32) (u64) temp_ret_2;
        temp_t7 = (u32) (u64) __ashldi3(0, (u16) arg7, 0, 0x10) + sp5C;
        sp34 = (s32) (u16) arg8;
        var_s0 = temp_t7 + sp54 + (u16) arg8;
    }
    sp30 = (s32) (u16) arg1;
    if (((u16) arg1 == 0) || ((u16) arg1 == 2)) {
        temp_v0 = *(s32 *)0x8012E608;
        if ((temp_v0 & 0x20) || (temp_v0 & 0x10) || (var_a1 = 0, ((temp_v0 & 0x8000) != 0))) {
            var_a1 = 1;
        }
        goto block_24;
    }
    var_a1 = 4;
    if (sp30 == 5) {
        goto block_24;
    }
    if ((sp30 == 4) || (sp30 == 3)) {
        var_a1 = 2;
        if (*(s32 *)0x8012E608 & 1) {
            var_a1 = 3;
        }
block_24:
        sp274 = var_a1;
    }
    if (sp30 == 3) {
        func_800878E0(0x20, sp274);
    }
    if (sp274 != *(s32 *)0x8014A248) {
        func_80086A50(sp274, sp274);
    }
    if ((arg0 != *(s32 *)0x8012E684) || (*(s32 *)0x8012E688 != (var_s0 >> 0x1F)) || (*(s32 *)0x8012E68C != var_s0)) {
        sp28 = var_s0 >> 0x1F;
        sp2C = var_s0;
        if (var_s0 != 0) {
            sp3C = (s32) (u16) arg6;
            sp272 = func_80087804((u16) arg5 - (u16) arg7);
            var_v0 = func_80087804(sp3C - sp34);
        } else {
            sp272 = func_80087804(temp_s1);
            var_v0 = func_80087804((u16) arg4);
        }
        if (sp30 == 0) {
            sp38 = (s32) temp_s1;
            sp3C = (s32) (u16) arg6;
            sp24 = (s32) (u16) arg5;
            if (*(s32 *)0x8012E680 != 0) {
                temp_a0 = *(void **)0x80149438;
                *(void **)0x80149438 = (u8 *) temp_a0 + 8;
                M2C_FIELD(temp_a0, s32 *, 4) = 0;
                M2C_FIELD(temp_a0, s32 *, 0) = 0xE3001001;
                *(s32 *)0x8012E680 = 0;
            }
            if ((u16) arg2 == 2) {
                if (var_s0 != 0) {
                    temp_a0_2 = *(s32 *)0x80149438;
                    *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_2 + 8);
                    M2C_FIELD(temp_a0_2, s32 *, 0) = (s32) (((sp38 - 1) & 0xFFF) | 0xFD100000);
                    M2C_FIELD(temp_a0_2, s32 *, 4) = arg0;
                    temp_a0_3 = *(s32 *)0x80149438;
                    *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_3 + 8);
                    temp_t6 = ((((s32) ((((u16) arg7 - sp24) * 2) + 9) >> 3) & 0x1FF) << 9) | 0xF5100000;
                    sp40 = temp_t6;
                    M2C_FIELD(temp_a0_3, s32 *, 0) = temp_t6;
                    temp_s1_2 = (var_v0 & 0xF) << 0xE;
                    temp_s0 = (sp272 & 0xF) * 0x10;
                    M2C_FIELD(temp_a0_3, s32 *, 4) = (s32) (temp_s1_2 | 0x07080000 | 0x200 | temp_s0);
                    temp_a0_4 = *(s32 *)0x80149438;
                    *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_4 + 8);
                    M2C_FIELD(temp_a0_4, s32 *, 4) = 0;
                    M2C_FIELD(temp_a0_4, s32 *, 0) = 0xE6000000;
                    temp_a0_5 = *(s32 *)0x80149438;
                    *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_5 + 8);
                    temp_a3 = ((sp24 * 4) & 0xFFF) << 0xC;
                    temp_t0 = (sp3C * 4) & 0xFFF;
                    M2C_FIELD(temp_a0_5, s32 *, 0) = (s32) (temp_a3 | 0xF4000000 | temp_t0);
                    temp_t1 = (((u16) arg7 * 4) & 0xFFF) << 0xC;
                    temp_t2 = (sp34 * 4) & 0xFFF;
                    M2C_FIELD(temp_a0_5, s32 *, 4) = (s32) (temp_t1 | 0x07000000 | temp_t2);
                    temp_a0_6 = *(s32 *)0x80149438;
                    *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_6 + 8);
                    sp258 = temp_a0_6;
                    M2C_FIELD(temp_a0_6, s32 *, 0) = 0xE7000000;
                    M2C_FIELD(sp258, s32 *, 4) = 0;
                    temp_a0_7 = *(s32 *)0x80149438;
                    *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_7 + 8);
                    M2C_FIELD(temp_a0_7, s32 *, 0) = sp40;
                    M2C_FIELD(temp_a0_7, s32 *, 4) = (s32) ((((u16) arg9 & 0xF) << 0x14) | 0x80000 | temp_s1_2 | 0x200 | temp_s0);
                    temp_a0_8 = *(s32 *)0x80149438;
                    *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_8 + 8);
                    M2C_FIELD(temp_a0_8, s32 *, 4) = (s32) (temp_t1 | temp_t2);
                    M2C_FIELD(temp_a0_8, s32 *, 0) = (s32) (temp_a3 | 0xF2000000 | temp_t0);
                } else {
                    temp_a0_9 = *(s32 *)0x80149438;
                    *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_9 + 8);
                    M2C_FIELD(temp_a0_9, s32 *, 0) = 0xFD100000;
                    M2C_FIELD(temp_a0_9, s32 *, 4) = arg0;
                    temp_a0_10 = *(s32 *)0x80149438;
                    temp_t9 = (var_v0 & 0xF) << 0xE;
                    *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_10 + 8);
                    M2C_FIELD(temp_a0_10, s32 *, 0) = 0xF5100000;
                    temp_s0_2 = (sp272 & 0xF) * 0x10;
                    M2C_FIELD(temp_a0_10, s32 *, 4) = (s32) (temp_t9 | 0x07080000 | 0x200 | temp_s0_2);
                    temp_a0_11 = *(s32 *)0x80149438;
                    *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_11 + 8);
                    M2C_FIELD(temp_a0_11, s32 *, 0) = 0xE6000000;
                    M2C_FIELD(temp_a0_11, s32 *, 4) = 0;
                    temp_a0_12 = *(s32 *)0x80149438;
                    var_t2 = 0x7FF;
                    *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_12 + 8);
                    M2C_FIELD(temp_a0_12, s32 *, 0) = 0xF3000000;
                    temp_t1_2 = (sp38 * (u16) arg4) - 1;
                    if (temp_t1_2 < 0x7FF) {
                        var_t2 = temp_t1_2;
                    }
                    temp_t1_3 = sp38 * 2;
                    temp_v1 = temp_t1_3 / 8;
                    if (temp_v1 <= 0) {
                        var_t0 = 1;
                    } else {
                        var_t0 = temp_v1;
                    }
                    if (temp_v1 <= 0) {
                        var_v0_2 = 1;
                    } else {
                        var_v0_2 = temp_v1;
                    }
                    M2C_FIELD(temp_a0_12, s32 *, 4) = (s32) ((((s32) (var_t0 + 0x7FF) / var_v0_2) & 0xFFF) | 0x07000000 | ((var_t2 & 0xFFF) << 0xC));
                    temp_a0_13 = *(s32 *)0x80149438;
                    *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_13 + 8);
                    M2C_FIELD(temp_a0_13, s32 *, 4) = 0;
                    M2C_FIELD(temp_a0_13, s32 *, 0) = 0xE7000000;
                    temp_a0_14 = *(s32 *)0x80149438;
                    *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_14 + 8);
                    M2C_FIELD(temp_a0_14, s32 *, 0) = (s32) (((((s32) (temp_t1_3 + 7) >> 3) & 0x1FF) << 9) | 0xF5100000);
                    M2C_FIELD(temp_a0_14, s32 *, 4) = (s32) ((((u16) arg9 & 0xF) << 0x14) | 0x80000 | temp_t9 | 0x200 | temp_s0_2);
                    temp_a0_15 = *(s32 *)0x80149438;
                    *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_15 + 8);
                    M2C_FIELD(temp_a0_15, s32 *, 0) = (s32) ((((sp24 * 4) & 0xFFF) << 0xC) | 0xF2000000 | ((sp3C * 4) & 0xFFF));
                    var_t8_2 = (((((sp24 + sp38) - 1) * 4) & 0xFFF) << 0xC) | ((((sp3C + (u16) arg4) - 1) * 4) & 0xFFF);
                    goto block_153;
                }
            } else if (var_s0 != 0) {
                temp_a0_16 = *(s32 *)0x80149438;
                *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_16 + 8);
                M2C_FIELD(temp_a0_16, s32 *, 0) = (s32) (((sp38 - 1) & 0xFFF) | 0xFD180000);
                M2C_FIELD(temp_a0_16, s32 *, 4) = arg0;
                temp_a0_17 = *(s32 *)0x80149438;
                *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_17 + 8);
                temp_t6_2 = ((((s32) ((((u16) arg7 - sp24) * 2) + 9) >> 3) & 0x1FF) << 9) | 0xF5180000;
                sp40 = temp_t6_2;
                M2C_FIELD(temp_a0_17, s32 *, 0) = temp_t6_2;
                temp_s1_3 = (var_v0 & 0xF) << 0xE;
                temp_s0_3 = (sp272 & 0xF) * 0x10;
                M2C_FIELD(temp_a0_17, s32 *, 4) = (s32) (temp_s1_3 | 0x07080000 | 0x200 | temp_s0_3);
                temp_a0_18 = *(s32 *)0x80149438;
                *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_18 + 8);
                M2C_FIELD(temp_a0_18, s32 *, 4) = 0;
                M2C_FIELD(temp_a0_18, s32 *, 0) = 0xE6000000;
                temp_a0_19 = *(s32 *)0x80149438;
                *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_19 + 8);
                temp_a3_2 = ((sp24 * 4) & 0xFFF) << 0xC;
                temp_t0_2 = (sp3C * 4) & 0xFFF;
                M2C_FIELD(temp_a0_19, s32 *, 0) = (s32) (temp_a3_2 | 0xF4000000 | temp_t0_2);
                temp_t1_4 = (((u16) arg7 * 4) & 0xFFF) << 0xC;
                temp_t2_2 = (sp34 * 4) & 0xFFF;
                M2C_FIELD(temp_a0_19, s32 *, 4) = (s32) (temp_t1_4 | 0x07000000 | temp_t2_2);
                temp_a0_20 = *(s32 *)0x80149438;
                *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_20 + 8);
                sp220 = temp_a0_20;
                M2C_FIELD(temp_a0_20, s32 *, 0) = 0xE7000000;
                M2C_FIELD(sp220, s32 *, 4) = 0;
                temp_a0_21 = *(s32 *)0x80149438;
                *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_21 + 8);
                M2C_FIELD(temp_a0_21, s32 *, 0) = sp40;
                M2C_FIELD(temp_a0_21, s32 *, 4) = (s32) ((((u16) arg9 & 0xF) << 0x14) | 0x80000 | temp_s1_3 | 0x200 | temp_s0_3);
                temp_a0_22 = *(s32 *)0x80149438;
                *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_22 + 8);
                M2C_FIELD(temp_a0_22, s32 *, 4) = (s32) (temp_t1_4 | temp_t2_2);
                M2C_FIELD(temp_a0_22, s32 *, 0) = (s32) (temp_a3_2 | 0xF2000000 | temp_t0_2);
            } else {
                temp_a0_23 = *(s32 *)0x80149438;
                *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_23 + 8);
                M2C_FIELD(temp_a0_23, s32 *, 0) = 0xFD180000;
                M2C_FIELD(temp_a0_23, s32 *, 4) = arg0;
                temp_a0_24 = *(s32 *)0x80149438;
                temp_t9_2 = (var_v0 & 0xF) << 0xE;
                *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_24 + 8);
                M2C_FIELD(temp_a0_24, s32 *, 0) = 0xF5180000;
                temp_s0_4 = (sp272 & 0xF) * 0x10;
                M2C_FIELD(temp_a0_24, s32 *, 4) = (s32) (temp_t9_2 | 0x07080000 | 0x200 | temp_s0_4);
                temp_a0_25 = *(s32 *)0x80149438;
                *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_25 + 8);
                M2C_FIELD(temp_a0_25, s32 *, 0) = 0xE6000000;
                M2C_FIELD(temp_a0_25, s32 *, 4) = 0;
                temp_a0_26 = *(s32 *)0x80149438;
                var_t2_2 = 0x7FF;
                *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_26 + 8);
                M2C_FIELD(temp_a0_26, s32 *, 0) = 0xF3000000;
                temp_t1_5 = (sp38 * (u16) arg4) - 1;
                if (temp_t1_5 < 0x7FF) {
                    var_t2_2 = temp_t1_5;
                }
                temp_t8 = (s32) (sp38 * 4) / 8;
                if (temp_t8 <= 0) {
                    var_t0_2 = 1;
                } else {
                    var_t0_2 = temp_t8;
                }
                if (temp_t8 <= 0) {
                    var_v0_3 = 1;
                } else {
                    var_v0_3 = temp_t8;
                }
                M2C_FIELD(temp_a0_26, s32 *, 4) = (s32) ((((s32) (var_t0_2 + 0x7FF) / var_v0_3) & 0xFFF) | 0x07000000 | ((var_t2_2 & 0xFFF) << 0xC));
                temp_a0_27 = *(s32 *)0x80149438;
                *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_27 + 8);
                M2C_FIELD(temp_a0_27, s32 *, 4) = 0;
                M2C_FIELD(temp_a0_27, s32 *, 0) = 0xE7000000;
                temp_a0_28 = *(s32 *)0x80149438;
                *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_28 + 8);
                M2C_FIELD(temp_a0_28, s32 *, 0) = (s32) (((((s32) ((sp38 * 2) + 7) >> 3) & 0x1FF) << 9) | 0xF5180000);
                M2C_FIELD(temp_a0_28, s32 *, 4) = (s32) ((((u16) arg9 & 0xF) << 0x14) | 0x80000 | temp_t9_2 | 0x200 | temp_s0_4);
                temp_a0_29 = *(s32 *)0x80149438;
                *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_29 + 8);
                M2C_FIELD(temp_a0_29, s32 *, 0) = (s32) ((((sp24 * 4) & 0xFFF) << 0xC) | 0xF2000000 | ((sp3C * 4) & 0xFFF));
                M2C_FIELD(temp_a0_29, s32 *, 4) = (s32) ((((((sp24 + sp38) - 1) * 4) & 0xFFF) << 0xC) | ((((sp3C + (u16) arg4) - 1) * 4) & 0xFFF));
            }
        } else if (sp30 != 2) {
            if (sp30 == 5) {
                goto block_65;
            }
            if (sp30 == 4) {
                sp38 = (s32) temp_s1;
                sp3C = (s32) (u16) arg6;
                sp24 = (s32) (u16) arg5;
                if (*(s32 *)0x8012E680 != 0) {
                    temp_a0_30 = *(s32 *)0x80149438;
                    *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_30 + 8);
                    M2C_FIELD(temp_a0_30, s32 *, 4) = 0;
                    M2C_FIELD(temp_a0_30, s32 *, 0) = 0xE3001001;
                    *(s32 *)0x8012E680 = 0;
                }
                if ((u16) arg2 == 1) {
                    if (var_s0 != 0) {
                        temp_a0_31 = *(s32 *)0x80149438;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_31 + 8);
                        M2C_FIELD(temp_a0_31, s32 *, 0) = (s32) (((sp38 - 1) & 0xFFF) | 0xFD880000);
                        M2C_FIELD(temp_a0_31, s32 *, 4) = arg0;
                        temp_a0_32 = *(s32 *)0x80149438;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_32 + 8);
                        temp_t8_2 = ((((s32) (((u16) arg7 - sp24) + 8) >> 3) & 0x1FF) << 9) | 0xF5880000;
                        sp40 = temp_t8_2;
                        M2C_FIELD(temp_a0_32, s32 *, 0) = temp_t8_2;
                        temp_s1_4 = (var_v0 & 0xF) << 0xE;
                        temp_s0_5 = (sp272 & 0xF) * 0x10;
                        M2C_FIELD(temp_a0_32, s32 *, 4) = (s32) (temp_s1_4 | 0x07080000 | 0x200 | temp_s0_5);
                        temp_a0_33 = *(s32 *)0x80149438;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_33 + 8);
                        M2C_FIELD(temp_a0_33, s32 *, 4) = 0;
                        M2C_FIELD(temp_a0_33, s32 *, 0) = 0xE6000000;
                        temp_a0_34 = *(s32 *)0x80149438;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_34 + 8);
                        temp_a3_3 = ((sp24 * 4) & 0xFFF) << 0xC;
                        temp_t0_3 = (sp3C * 4) & 0xFFF;
                        M2C_FIELD(temp_a0_34, s32 *, 0) = (s32) (temp_a3_3 | 0xF4000000 | temp_t0_3);
                        temp_t1_6 = (((u16) arg7 * 4) & 0xFFF) << 0xC;
                        temp_t2_3 = (sp34 * 4) & 0xFFF;
                        M2C_FIELD(temp_a0_34, s32 *, 4) = (s32) (temp_t1_6 | 0x07000000 | temp_t2_3);
                        temp_a0_35 = *(s32 *)0x80149438;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_35 + 8);
                        sp168 = temp_a0_35;
                        M2C_FIELD(temp_a0_35, s32 *, 0) = 0xE7000000;
                        M2C_FIELD(sp168, s32 *, 4) = 0;
                        temp_a0_36 = *(s32 *)0x80149438;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_36 + 8);
                        M2C_FIELD(temp_a0_36, s32 *, 0) = sp40;
                        M2C_FIELD(temp_a0_36, s32 *, 4) = (s32) ((((u16) arg9 & 0xF) << 0x14) | 0x80000 | temp_s1_4 | 0x200 | temp_s0_5);
                        temp_a0_37 = *(s32 *)0x80149438;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_37 + 8);
                        M2C_FIELD(temp_a0_37, s32 *, 4) = (s32) (temp_t1_6 | temp_t2_3);
                        M2C_FIELD(temp_a0_37, s32 *, 0) = (s32) (temp_a3_3 | 0xF2000000 | temp_t0_3);
                    } else {
                        temp_a0_38 = *(s32 *)0x80149438;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_38 + 8);
                        M2C_FIELD(temp_a0_38, s32 *, 0) = 0xFD900000;
                        M2C_FIELD(temp_a0_38, s32 *, 4) = arg0;
                        temp_a0_39 = *(s32 *)0x80149438;
                        temp_t7_2 = (var_v0 & 0xF) << 0xE;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_39 + 8);
                        M2C_FIELD(temp_a0_39, s32 *, 0) = 0xF5900000;
                        temp_t6_3 = (sp272 & 0xF) * 0x10;
                        M2C_FIELD(temp_a0_39, s32 *, 4) = (s32) (temp_t7_2 | 0x07080000 | 0x200 | temp_t6_3);
                        temp_a0_40 = *(s32 *)0x80149438;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_40 + 8);
                        M2C_FIELD(temp_a0_40, s32 *, 0) = 0xE6000000;
                        M2C_FIELD(temp_a0_40, s32 *, 4) = 0;
                        temp_a0_41 = *(s32 *)0x80149438;
                        var_t2_3 = 0x7FF;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_41 + 8);
                        M2C_FIELD(temp_a0_41, s32 *, 0) = 0xF3000000;
                        temp_t1_7 = ((s32) ((sp38 * (u16) arg4) + 1) >> 1) - 1;
                        if (temp_t1_7 < 0x7FF) {
                            var_t2_3 = temp_t1_7;
                        }
                        temp_v1_2 = sp38 / 8;
                        if (temp_v1_2 <= 0) {
                            var_t0_3 = 1;
                        } else {
                            var_t0_3 = temp_v1_2;
                        }
                        if (temp_v1_2 <= 0) {
                            var_v0_4 = 1;
                        } else {
                            var_v0_4 = temp_v1_2;
                        }
                        M2C_FIELD(temp_a0_41, s32 *, 4) = (s32) ((((s32) (var_t0_3 + 0x7FF) / var_v0_4) & 0xFFF) | 0x07000000 | ((var_t2_3 & 0xFFF) << 0xC));
                        temp_a0_42 = *(s32 *)0x80149438;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_42 + 8);
                        M2C_FIELD(temp_a0_42, s32 *, 4) = 0;
                        M2C_FIELD(temp_a0_42, s32 *, 0) = 0xE7000000;
                        temp_a0_43 = *(s32 *)0x80149438;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_43 + 8);
                        M2C_FIELD(temp_a0_43, s32 *, 0) = (s32) (((((s32) (sp38 + 7) >> 3) & 0x1FF) << 9) | 0xF5880000);
                        M2C_FIELD(temp_a0_43, s32 *, 4) = (s32) ((((u16) arg9 & 0xF) << 0x14) | 0x80000 | temp_t7_2 | 0x200 | temp_t6_3);
                        temp_a0_44 = *(s32 *)0x80149438;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_44 + 8);
                        M2C_FIELD(temp_a0_44, s32 *, 0) = (s32) ((((sp24 * 4) & 0xFFF) << 0xC) | 0xF2000000 | ((sp3C * 4) & 0xFFF));
                        M2C_FIELD(temp_a0_44, s32 *, 4) = (s32) ((((((sp24 + sp38) - 1) * 4) & 0xFFF) << 0xC) | ((((sp3C + (u16) arg4) - 1) * 4) & 0xFFF));
                    }
                } else if (var_s0 != 0) {
                    temp_a0_45 = *(s32 *)0x80149438;
                    *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_45 + 8);
                    M2C_FIELD(temp_a0_45, s32 *, 0) = (s32) ((((sp38 >> 1) - 1) & 0xFFF) | 0xFD880000);
                    M2C_FIELD(temp_a0_45, s32 *, 4) = arg0;
                    temp_a0_46 = *(s32 *)0x80149438;
                    *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_46 + 8);
                    temp_t3 = (((s32) (((s32) (((u16) arg7 - sp24) + 1) >> 1) + 7) >> 3) & 0x1FF) << 9;
                    M2C_FIELD(temp_a0_46, s32 *, 0) = (s32) (temp_t3 | 0xF5880000);
                    temp_s1_5 = (var_v0 & 0xF) << 0xE;
                    temp_s0_6 = (sp272 & 0xF) * 0x10;
                    M2C_FIELD(temp_a0_46, s32 *, 4) = (s32) (temp_s1_5 | 0x07080000 | 0x200 | temp_s0_6);
                    temp_a0_47 = *(s32 *)0x80149438;
                    *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_47 + 8);
                    M2C_FIELD(temp_a0_47, s32 *, 4) = 0;
                    M2C_FIELD(temp_a0_47, s32 *, 0) = 0xE6000000;
                    temp_a0_48 = *(s32 *)0x80149438;
                    *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_48 + 8);
                    temp_t0_4 = (sp3C * 4) & 0xFFF;
                    M2C_FIELD(temp_a0_48, s32 *, 0) = (s32) ((((sp24 * 2) & 0xFFF) << 0xC) | 0xF4000000 | temp_t0_4);
                    temp_t2_4 = (sp34 * 4) & 0xFFF;
                    M2C_FIELD(temp_a0_48, s32 *, 4) = (s32) (((((u16) arg7 * 2) & 0xFFF) << 0xC) | 0x07000000 | temp_t2_4);
                    temp_a0_49 = *(s32 *)0x80149438;
                    *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_49 + 8);
                    M2C_FIELD(temp_a0_49, s32 *, 4) = 0;
                    M2C_FIELD(temp_a0_49, s32 *, 0) = 0xE7000000;
                    temp_a0_50 = *(s32 *)0x80149438;
                    *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_50 + 8);
                    M2C_FIELD(temp_a0_50, s32 *, 0) = (s32) (temp_t3 | 0xF5800000);
                    M2C_FIELD(temp_a0_50, s32 *, 4) = (s32) ((((u16) arg9 & 0xF) << 0x14) | 0x80000 | temp_s1_5 | 0x200 | temp_s0_6);
                    temp_a0_51 = *(s32 *)0x80149438;
                    *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_51 + 8);
                    M2C_FIELD(temp_a0_51, s32 *, 0) = (s32) ((((sp24 * 4) & 0xFFF) << 0xC) | 0xF2000000 | temp_t0_4);
                    M2C_FIELD(temp_a0_51, s32 *, 4) = (s32) (((((u16) arg7 * 4) & 0xFFF) << 0xC) | temp_t2_4);
                } else {
                    temp_a0_52 = *(s32 *)0x80149438;
                    *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_52 + 8);
                    M2C_FIELD(temp_a0_52, s32 *, 0) = 0xFD900000;
                    M2C_FIELD(temp_a0_52, s32 *, 4) = arg0;
                    temp_a0_53 = *(s32 *)0x80149438;
                    temp_s1_6 = (var_v0 & 0xF) << 0xE;
                    *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_53 + 8);
                    M2C_FIELD(temp_a0_53, s32 *, 0) = 0xF5900000;
                    temp_t8_3 = (sp272 & 0xF) * 0x10;
                    M2C_FIELD(temp_a0_53, s32 *, 4) = (s32) (temp_s1_6 | 0x07080000 | 0x200 | temp_t8_3);
                    temp_a0_54 = *(s32 *)0x80149438;
                    *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_54 + 8);
                    M2C_FIELD(temp_a0_54, s32 *, 0) = 0xE6000000;
                    M2C_FIELD(temp_a0_54, s32 *, 4) = 0;
                    temp_a0_55 = *(s32 *)0x80149438;
                    var_t2_4 = 0x7FF;
                    *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_55 + 8);
                    M2C_FIELD(temp_a0_55, s32 *, 0) = 0xF3000000;
                    temp_t1_8 = ((s32) ((sp38 * (u16) arg4) + 3) >> 2) - 1;
                    if (temp_t1_8 < 0x7FF) {
                        var_t2_4 = temp_t1_8;
                    }
                    temp_t6_4 = sp38 / 16;
                    if (temp_t6_4 <= 0) {
                        var_t0_4 = 1;
                    } else {
                        var_t0_4 = temp_t6_4;
                    }
                    if (temp_t6_4 <= 0) {
                        var_v0_5 = 1;
                    } else {
                        var_v0_5 = temp_t6_4;
                    }
                    M2C_FIELD(temp_a0_55, s32 *, 4) = (s32) ((((s32) (var_t0_4 + 0x7FF) / var_v0_5) & 0xFFF) | 0x07000000 | ((var_t2_4 & 0xFFF) << 0xC));
                    temp_a0_56 = *(s32 *)0x80149438;
                    *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_56 + 8);
                    M2C_FIELD(temp_a0_56, s32 *, 4) = 0;
                    M2C_FIELD(temp_a0_56, s32 *, 0) = 0xE7000000;
                    temp_a0_57 = *(s32 *)0x80149438;
                    *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_57 + 8);
                    M2C_FIELD(temp_a0_57, s32 *, 0) = (s32) (((((s32) ((sp38 >> 1) + 7) >> 3) & 0x1FF) << 9) | 0xF5800000);
                    M2C_FIELD(temp_a0_57, s32 *, 4) = (s32) ((((u16) arg9 & 0xF) << 0x14) | 0x80000 | temp_s1_6 | 0x200 | temp_t8_3);
                    temp_a0_58 = *(s32 *)0x80149438;
                    *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_58 + 8);
                    M2C_FIELD(temp_a0_58, s32 *, 0) = (s32) ((((sp24 * 4) & 0xFFF) << 0xC) | 0xF2000000 | ((sp3C * 4) & 0xFFF));
                    M2C_FIELD(temp_a0_58, s32 *, 4) = (s32) ((((((sp24 + sp38) - 1) * 4) & 0xFFF) << 0xC) | ((((sp3C + (u16) arg4) - 1) * 4) & 0xFFF));
                }
            } else if (sp30 == 3) {
                sp38 = (s32) temp_s1;
                sp3C = (s32) (u16) arg6;
                sp24 = (s32) (u16) arg5;
                if (*(s32 *)0x8012E680 != 0) {
                    temp_a0_59 = *(s32 *)0x80149438;
                    *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_59 + 8);
                    M2C_FIELD(temp_a0_59, s32 *, 4) = 0;
                    M2C_FIELD(temp_a0_59, s32 *, 0) = 0xE3001001;
                    *(s32 *)0x8012E680 = 0;
                }
                if ((u16) arg2 == 2) {
                    if (var_s0 != 0) {
                        temp_a0_60 = *(s32 *)0x80149438;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_60 + 8);
                        M2C_FIELD(temp_a0_60, s32 *, 0) = (s32) (((sp38 - 1) & 0xFFF) | 0xFD700000);
                        M2C_FIELD(temp_a0_60, s32 *, 4) = arg0;
                        temp_a0_61 = *(s32 *)0x80149438;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_61 + 8);
                        temp_t9_3 = ((((s32) ((((u16) arg7 - sp24) * 2) + 9) >> 3) & 0x1FF) << 9) | 0xF5700000;
                        sp40 = temp_t9_3;
                        M2C_FIELD(temp_a0_61, s32 *, 0) = temp_t9_3;
                        temp_s1_7 = (var_v0 & 0xF) << 0xE;
                        temp_s0_7 = (sp272 & 0xF) * 0x10;
                        M2C_FIELD(temp_a0_61, s32 *, 4) = (s32) (temp_s1_7 | 0x07080000 | 0x200 | temp_s0_7);
                        temp_a0_62 = *(s32 *)0x80149438;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_62 + 8);
                        M2C_FIELD(temp_a0_62, s32 *, 4) = 0;
                        M2C_FIELD(temp_a0_62, s32 *, 0) = 0xE6000000;
                        temp_a0_63 = *(s32 *)0x80149438;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_63 + 8);
                        temp_a3_4 = ((sp24 * 4) & 0xFFF) << 0xC;
                        temp_t0_5 = (sp3C * 4) & 0xFFF;
                        M2C_FIELD(temp_a0_63, s32 *, 0) = (s32) (temp_a3_4 | 0xF4000000 | temp_t0_5);
                        temp_t1_9 = (((u16) arg7 * 4) & 0xFFF) << 0xC;
                        temp_t2_5 = (sp34 * 4) & 0xFFF;
                        M2C_FIELD(temp_a0_63, s32 *, 4) = (s32) (temp_t1_9 | 0x07000000 | temp_t2_5);
                        temp_a0_64 = *(s32 *)0x80149438;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_64 + 8);
                        spF4 = temp_a0_64;
                        M2C_FIELD(temp_a0_64, s32 *, 0) = 0xE7000000;
                        M2C_FIELD(spF4, s32 *, 4) = 0;
                        temp_a0_65 = *(s32 *)0x80149438;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_65 + 8);
                        M2C_FIELD(temp_a0_65, s32 *, 0) = sp40;
                        M2C_FIELD(temp_a0_65, s32 *, 4) = (s32) ((((u16) arg9 & 0xF) << 0x14) | 0x80000 | temp_s1_7 | 0x200 | temp_s0_7);
                        temp_a0_66 = *(s32 *)0x80149438;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_66 + 8);
                        M2C_FIELD(temp_a0_66, s32 *, 4) = (s32) (temp_t1_9 | temp_t2_5);
                        M2C_FIELD(temp_a0_66, s32 *, 0) = (s32) (temp_a3_4 | 0xF2000000 | temp_t0_5);
                    } else {
                        temp_a0_67 = *(s32 *)0x80149438;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_67 + 8);
                        M2C_FIELD(temp_a0_67, s32 *, 0) = 0xFD700000;
                        M2C_FIELD(temp_a0_67, s32 *, 4) = arg0;
                        temp_a0_68 = *(s32 *)0x80149438;
                        temp_s1_8 = (var_v0 & 0xF) << 0xE;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_68 + 8);
                        M2C_FIELD(temp_a0_68, s32 *, 0) = 0xF5700000;
                        temp_t9_4 = (sp272 & 0xF) * 0x10;
                        M2C_FIELD(temp_a0_68, s32 *, 4) = (s32) (temp_s1_8 | 0x07080000 | 0x200 | temp_t9_4);
                        temp_a0_69 = *(s32 *)0x80149438;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_69 + 8);
                        M2C_FIELD(temp_a0_69, s32 *, 0) = 0xE6000000;
                        M2C_FIELD(temp_a0_69, s32 *, 4) = 0;
                        temp_a0_70 = *(s32 *)0x80149438;
                        var_t2_5 = 0x7FF;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_70 + 8);
                        M2C_FIELD(temp_a0_70, s32 *, 0) = 0xF3000000;
                        temp_t1_10 = (sp38 * (u16) arg4) - 1;
                        if (temp_t1_10 < 0x7FF) {
                            var_t2_5 = temp_t1_10;
                        }
                        temp_t7_3 = sp38 * 2;
                        temp_v1_3 = temp_t7_3 / 8;
                        var_t0_5 = temp_v1_3;
                        if (temp_v1_3 <= 0) {
                            var_t0_5 = 1;
                        }
                        if (temp_v1_3 <= 0) {
                            var_v0_6 = 1;
                        } else {
                            var_v0_6 = temp_v1_3;
                        }
                        M2C_FIELD(temp_a0_70, s32 *, 4) = (s32) ((((s32) (var_t0_5 + 0x7FF) / var_v0_6) & 0xFFF) | 0x07000000 | ((var_t2_5 & 0xFFF) << 0xC));
                        temp_a0_71 = *(s32 *)0x80149438;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_71 + 8);
                        M2C_FIELD(temp_a0_71, s32 *, 4) = 0;
                        M2C_FIELD(temp_a0_71, s32 *, 0) = 0xE7000000;
                        temp_a0_72 = *(s32 *)0x80149438;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_72 + 8);
                        M2C_FIELD(temp_a0_72, s32 *, 0) = (s32) (((((s32) (temp_t7_3 + 7) >> 3) & 0x1FF) << 9) | 0xF5700000);
                        M2C_FIELD(temp_a0_72, s32 *, 4) = (s32) ((((u16) arg9 & 0xF) << 0x14) | 0x80000 | temp_s1_8 | 0x200 | temp_t9_4);
                        temp_a0_73 = *(s32 *)0x80149438;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_73 + 8);
                        M2C_FIELD(temp_a0_73, s32 *, 0) = (s32) ((((sp24 * 4) & 0xFFF) << 0xC) | 0xF2000000 | ((sp3C * 4) & 0xFFF));
                        M2C_FIELD(temp_a0_73, s32 *, 4) = (s32) ((((((sp24 + sp38) - 1) * 4) & 0xFFF) << 0xC) | ((((sp3C + (u16) arg4) - 1) * 4) & 0xFFF));
                    }
                } else if ((u16) arg2 == 1) {
                    if (var_s0 != 0) {
                        temp_a0_74 = *(s32 *)0x80149438;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_74 + 8);
                        M2C_FIELD(temp_a0_74, s32 *, 0) = (s32) (((sp38 - 1) & 0xFFF) | 0xFD680000);
                        M2C_FIELD(temp_a0_74, s32 *, 4) = arg0;
                        temp_a0_75 = *(s32 *)0x80149438;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_75 + 8);
                        temp_t9_5 = ((((s32) (((u16) arg7 - sp24) + 8) >> 3) & 0x1FF) << 9) | 0xF5680000;
                        sp40 = temp_t9_5;
                        M2C_FIELD(temp_a0_75, s32 *, 0) = temp_t9_5;
                        temp_s1_9 = (var_v0 & 0xF) << 0xE;
                        temp_s0_8 = (sp272 & 0xF) * 0x10;
                        M2C_FIELD(temp_a0_75, s32 *, 4) = (s32) (temp_s1_9 | 0x07080000 | 0x200 | temp_s0_8);
                        temp_a0_76 = *(s32 *)0x80149438;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_76 + 8);
                        M2C_FIELD(temp_a0_76, s32 *, 4) = 0;
                        M2C_FIELD(temp_a0_76, s32 *, 0) = 0xE6000000;
                        temp_a0_77 = *(s32 *)0x80149438;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_77 + 8);
                        temp_a3_5 = ((sp24 * 4) & 0xFFF) << 0xC;
                        temp_t0_6 = (sp3C * 4) & 0xFFF;
                        M2C_FIELD(temp_a0_77, s32 *, 0) = (s32) (temp_a3_5 | 0xF4000000 | temp_t0_6);
                        temp_t1_11 = (((u16) arg7 * 4) & 0xFFF) << 0xC;
                        temp_t2_6 = (sp34 * 4) & 0xFFF;
                        M2C_FIELD(temp_a0_77, s32 *, 4) = (s32) (temp_t1_11 | 0x07000000 | temp_t2_6);
                        temp_a0_78 = *(s32 *)0x80149438;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_78 + 8);
                        spBC = temp_a0_78;
                        M2C_FIELD(temp_a0_78, s32 *, 0) = 0xE7000000;
                        M2C_FIELD(spBC, s32 *, 4) = 0;
                        temp_a0_79 = *(s32 *)0x80149438;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_79 + 8);
                        M2C_FIELD(temp_a0_79, s32 *, 0) = sp40;
                        M2C_FIELD(temp_a0_79, s32 *, 4) = (s32) ((((u16) arg9 & 0xF) << 0x14) | 0x80000 | temp_s1_9 | 0x200 | temp_s0_8);
                        temp_a0_80 = *(s32 *)0x80149438;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_80 + 8);
                        M2C_FIELD(temp_a0_80, s32 *, 4) = (s32) (temp_t1_11 | temp_t2_6);
                        M2C_FIELD(temp_a0_80, s32 *, 0) = (s32) (temp_a3_5 | 0xF2000000 | temp_t0_6);
                    } else {
                        temp_a0_81 = *(s32 *)0x80149438;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_81 + 8);
                        M2C_FIELD(temp_a0_81, s32 *, 0) = 0xFD700000;
                        M2C_FIELD(temp_a0_81, s32 *, 4) = arg0;
                        temp_a0_82 = *(s32 *)0x80149438;
                        temp_s1_10 = (var_v0 & 0xF) << 0xE;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_82 + 8);
                        M2C_FIELD(temp_a0_82, s32 *, 0) = 0xF5700000;
                        temp_t9_6 = (sp272 & 0xF) * 0x10;
                        M2C_FIELD(temp_a0_82, s32 *, 4) = (s32) (temp_s1_10 | 0x07080000 | 0x200 | temp_t9_6);
                        temp_a0_83 = *(s32 *)0x80149438;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_83 + 8);
                        M2C_FIELD(temp_a0_83, s32 *, 0) = 0xE6000000;
                        M2C_FIELD(temp_a0_83, s32 *, 4) = 0;
                        temp_a0_84 = *(s32 *)0x80149438;
                        var_t2_6 = 0x7FF;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_84 + 8);
                        M2C_FIELD(temp_a0_84, s32 *, 0) = 0xF3000000;
                        temp_t1_12 = ((s32) ((sp38 * (u16) arg4) + 1) >> 1) - 1;
                        if (temp_t1_12 < 0x7FF) {
                            var_t2_6 = temp_t1_12;
                        }
                        temp_v1_4 = sp38 / 8;
                        var_t0_6 = temp_v1_4;
                        if (temp_v1_4 <= 0) {
                            var_t0_6 = 1;
                        }
                        if (temp_v1_4 <= 0) {
                            var_v0_7 = 1;
                        } else {
                            var_v0_7 = temp_v1_4;
                        }
                        M2C_FIELD(temp_a0_84, s32 *, 4) = (s32) ((((s32) (var_t0_6 + 0x7FF) / var_v0_7) & 0xFFF) | 0x07000000 | ((var_t2_6 & 0xFFF) << 0xC));
                        temp_a0_85 = *(s32 *)0x80149438;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_85 + 8);
                        M2C_FIELD(temp_a0_85, s32 *, 4) = 0;
                        M2C_FIELD(temp_a0_85, s32 *, 0) = 0xE7000000;
                        temp_a0_86 = *(s32 *)0x80149438;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_86 + 8);
                        M2C_FIELD(temp_a0_86, s32 *, 0) = (s32) (((((s32) (sp38 + 7) >> 3) & 0x1FF) << 9) | 0xF5680000);
                        M2C_FIELD(temp_a0_86, s32 *, 4) = (s32) ((((u16) arg9 & 0xF) << 0x14) | 0x80000 | temp_s1_10 | 0x200 | temp_t9_6);
                        temp_a0_87 = *(s32 *)0x80149438;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_87 + 8);
                        M2C_FIELD(temp_a0_87, s32 *, 0) = (s32) ((((sp24 * 4) & 0xFFF) << 0xC) | 0xF2000000 | ((sp3C * 4) & 0xFFF));
                        M2C_FIELD(temp_a0_87, s32 *, 4) = (s32) ((((((sp24 + sp38) - 1) * 4) & 0xFFF) << 0xC) | ((((sp3C + (u16) arg4) - 1) * 4) & 0xFFF));
                    }
                } else {
                    if (var_s0 != 0) {
                        temp_a0_88 = *(s32 *)0x80149438;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_88 + 8);
                        M2C_FIELD(temp_a0_88, s32 *, 0) = (s32) ((((sp38 >> 1) - 1) & 0xFFF) | 0xFD680000);
                        M2C_FIELD(temp_a0_88, s32 *, 4) = arg0;
                        temp_a0_89 = *(s32 *)0x80149438;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_89 + 8);
                        temp_t3_2 = (((s32) (((s32) (((u16) arg7 - sp24) + 1) >> 1) + 7) >> 3) & 0x1FF) << 9;
                        M2C_FIELD(temp_a0_89, s32 *, 0) = (s32) (temp_t3_2 | 0xF5680000);
                        temp_s1_11 = (var_v0 & 0xF) << 0xE;
                        temp_s0_9 = (sp272 & 0xF) * 0x10;
                        M2C_FIELD(temp_a0_89, s32 *, 4) = (s32) (temp_s1_11 | 0x07080000 | 0x200 | temp_s0_9);
                        temp_a0_90 = *(s32 *)0x80149438;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_90 + 8);
                        M2C_FIELD(temp_a0_90, s32 *, 4) = 0;
                        M2C_FIELD(temp_a0_90, s32 *, 0) = 0xE6000000;
                        temp_a0_91 = *(s32 *)0x80149438;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_91 + 8);
                        temp_t0_7 = (sp3C * 4) & 0xFFF;
                        M2C_FIELD(temp_a0_91, s32 *, 0) = (s32) ((((sp24 * 2) & 0xFFF) << 0xC) | 0xF4000000 | temp_t0_7);
                        temp_t2_7 = (sp34 * 4) & 0xFFF;
                        M2C_FIELD(temp_a0_91, s32 *, 4) = (s32) (((((u16) arg7 * 2) & 0xFFF) << 0xC) | 0x07000000 | temp_t2_7);
                        temp_a0_92 = *(s32 *)0x80149438;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_92 + 8);
                        M2C_FIELD(temp_a0_92, s32 *, 4) = 0;
                        M2C_FIELD(temp_a0_92, s32 *, 0) = 0xE7000000;
                        temp_a0_93 = *(s32 *)0x80149438;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_93 + 8);
                        M2C_FIELD(temp_a0_93, s32 *, 0) = (s32) (temp_t3_2 | 0xF5600000);
                        M2C_FIELD(temp_a0_93, s32 *, 4) = (s32) ((((u16) arg9 & 0xF) << 0x14) | 0x80000 | temp_s1_11 | 0x200 | temp_s0_9);
                        temp_a0_94 = *(s32 *)0x80149438;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_94 + 8);
                        M2C_FIELD(temp_a0_94, s32 *, 0) = (s32) ((((sp24 * 4) & 0xFFF) << 0xC) | 0xF2000000 | temp_t0_7);
                        var_t8_2 = ((((u16) arg7 * 4) & 0xFFF) << 0xC) | temp_t2_7;
                    } else {
                        temp_a0_95 = *(s32 *)0x80149438;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_95 + 8);
                        M2C_FIELD(temp_a0_95, s32 *, 0) = 0xFD700000;
                        M2C_FIELD(temp_a0_95, s32 *, 4) = arg0;
                        temp_a0_96 = *(s32 *)0x80149438;
                        temp_s1_12 = (var_v0 & 0xF) << 0xE;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_96 + 8);
                        M2C_FIELD(temp_a0_96, s32 *, 0) = 0xF5700000;
                        temp_t8_4 = (sp272 & 0xF) * 0x10;
                        M2C_FIELD(temp_a0_96, s32 *, 4) = (s32) (temp_s1_12 | 0x07080000 | 0x200 | temp_t8_4);
                        temp_a0_97 = *(s32 *)0x80149438;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_97 + 8);
                        M2C_FIELD(temp_a0_97, s32 *, 0) = 0xE6000000;
                        M2C_FIELD(temp_a0_97, s32 *, 4) = 0;
                        temp_a0_98 = *(s32 *)0x80149438;
                        var_t2_7 = 0x7FF;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_98 + 8);
                        M2C_FIELD(temp_a0_98, s32 *, 0) = 0xF3000000;
                        temp_t1_13 = ((s32) ((sp38 * (u16) arg4) + 3) >> 2) - 1;
                        if (temp_t1_13 < 0x7FF) {
                            var_t2_7 = temp_t1_13;
                        }
                        temp_t7_4 = sp38 / 16;
                        if (temp_t7_4 <= 0) {
                            var_t0_7 = 1;
                        } else {
                            var_t0_7 = temp_t7_4;
                        }
                        if (temp_t7_4 <= 0) {
                            var_v0_8 = 1;
                        } else {
                            var_v0_8 = temp_t7_4;
                        }
                        M2C_FIELD(temp_a0_98, s32 *, 4) = (s32) ((((s32) (var_t0_7 + 0x7FF) / var_v0_8) & 0xFFF) | 0x07000000 | ((var_t2_7 & 0xFFF) << 0xC));
                        temp_a0_99 = *(s32 *)0x80149438;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_99 + 8);
                        M2C_FIELD(temp_a0_99, s32 *, 4) = 0;
                        M2C_FIELD(temp_a0_99, s32 *, 0) = 0xE7000000;
                        temp_a0_100 = *(s32 *)0x80149438;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_100 + 8);
                        M2C_FIELD(temp_a0_100, s32 *, 0) = (s32) (((((s32) ((sp38 >> 1) + 7) >> 3) & 0x1FF) << 9) | 0xF5600000);
                        M2C_FIELD(temp_a0_100, s32 *, 4) = (s32) ((((u16) arg9 & 0xF) << 0x14) | 0x80000 | temp_s1_12 | 0x200 | temp_t8_4);
                        temp_a0_101 = *(s32 *)0x80149438;
                        *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_101 + 8);
                        M2C_FIELD(temp_a0_101, s32 *, 0) = (s32) ((((sp24 * 4) & 0xFFF) << 0xC) | 0xF2000000 | ((sp3C * 4) & 0xFFF));
                        var_t8_2 = (((((sp24 + sp38) - 1) * 4) & 0xFFF) << 0xC) | ((((sp3C + (u16) arg4) - 1) * 4) & 0xFFF);
                    }
block_153:
                    M2C_FIELD(*(s32 *)0x80149438, s32 *, 4) = var_t8_2;
                }
            }
        } else {
block_65:
            sp38 = (s32) temp_s1;
            sp3C = (s32) (u16) arg6;
            sp24 = (s32) (u16) arg5;
            if (*(s32 *)0x8012E680 != 1) {
                temp_a0_102 = *(s32 *)0x80149438;
                *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_102 + 8);
                M2C_FIELD(temp_a0_102, s32 *, 4) = 0x8000;
                M2C_FIELD(temp_a0_102, s32 *, 0) = 0xE3001001;
                *(s32 *)0x8012E680 = 1;
            }
            if ((u16) arg2 == 1) {
                if (var_s0 != 0) {
                    temp_a0_103 = *(s32 *)0x80149438;
                    *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_103 + 8);
                    M2C_FIELD(temp_a0_103, s32 *, 0) = (s32) (((sp38 - 1) & 0xFFF) | 0xFD480000);
                    M2C_FIELD(temp_a0_103, s32 *, 4) = arg0;
                    temp_a0_104 = *(s32 *)0x80149438;
                    *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_104 + 8);
                    temp_t9_7 = ((((s32) (((u16) arg7 - sp24) + 8) >> 3) & 0x1FF) << 9) | 0xF5480000;
                    sp40 = temp_t9_7;
                    M2C_FIELD(temp_a0_104, s32 *, 0) = temp_t9_7;
                    var_s1 = (var_v0 & 0xF) << 0xE;
                    temp_s0_10 = (sp272 & 0xF) * 0x10;
                    M2C_FIELD(temp_a0_104, s32 *, 4) = (s32) (var_s1 | 0x07080000 | 0x200 | temp_s0_10);
                    temp_a0_105 = *(s32 *)0x80149438;
                    *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_105 + 8);
                    M2C_FIELD(temp_a0_105, s32 *, 4) = 0;
                    M2C_FIELD(temp_a0_105, s32 *, 0) = 0xE6000000;
                    temp_a0_106 = *(s32 *)0x80149438;
                    *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_106 + 8);
                    temp_a3_6 = ((sp24 * 4) & 0xFFF) << 0xC;
                    temp_t0_8 = (sp3C * 4) & 0xFFF;
                    M2C_FIELD(temp_a0_106, s32 *, 0) = (s32) (temp_a3_6 | 0xF4000000 | temp_t0_8);
                    temp_t1_14 = (((u16) arg7 * 4) & 0xFFF) << 0xC;
                    temp_t2_8 = (sp34 * 4) & 0xFFF;
                    M2C_FIELD(temp_a0_106, s32 *, 4) = (s32) (temp_t1_14 | 0x07000000 | temp_t2_8);
                    temp_a0_107 = *(s32 *)0x80149438;
                    *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_107 + 8);
                    sp1E4 = temp_a0_107;
                    M2C_FIELD(temp_a0_107, s32 *, 0) = 0xE7000000;
                    M2C_FIELD(sp1E4, s32 *, 4) = 0;
                    temp_a0_108 = *(s32 *)0x80149438;
                    *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_108 + 8);
                    M2C_FIELD(temp_a0_108, s32 *, 0) = sp40;
                    var_t3 = ((u16) arg9 & 0xF) << 0x14;
                    M2C_FIELD(temp_a0_108, s32 *, 4) = (s32) (var_t3 | 0x80000 | var_s1 | 0x200 | temp_s0_10);
                    temp_a0_109 = *(s32 *)0x80149438;
                    *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_109 + 8);
                    var_t4 = temp_a3_6 | 0xF2000000 | temp_t0_8;
                    M2C_FIELD(temp_a0_109, s32 *, 4) = (s32) (temp_t1_14 | temp_t2_8);
                    M2C_FIELD(temp_a0_109, s32 *, 0) = var_t4;
                } else {
                    temp_a0_110 = *(s32 *)0x80149438;
                    *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_110 + 8);
                    M2C_FIELD(temp_a0_110, s32 *, 0) = 0xFD500000;
                    M2C_FIELD(temp_a0_110, s32 *, 4) = arg0;
                    temp_a0_111 = *(s32 *)0x80149438;
                    var_s1 = (var_v0 & 0xF) << 0xE;
                    *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_111 + 8);
                    M2C_FIELD(temp_a0_111, s32 *, 0) = 0xF5500000;
                    temp_t7_5 = (sp272 & 0xF) * 0x10;
                    M2C_FIELD(temp_a0_111, s32 *, 4) = (s32) (var_s1 | 0x07080000 | 0x200 | temp_t7_5);
                    temp_a0_112 = *(s32 *)0x80149438;
                    *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_112 + 8);
                    M2C_FIELD(temp_a0_112, s32 *, 0) = 0xE6000000;
                    M2C_FIELD(temp_a0_112, s32 *, 4) = 0;
                    temp_a0_113 = *(s32 *)0x80149438;
                    var_t2_8 = 0x7FF;
                    *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_113 + 8);
                    M2C_FIELD(temp_a0_113, s32 *, 0) = 0xF3000000;
                    temp_t1_15 = ((s32) ((sp38 * (u16) arg4) + 1) >> 1) - 1;
                    if (temp_t1_15 < 0x7FF) {
                        var_t2_8 = temp_t1_15;
                    }
                    temp_v1_5 = sp38 / 8;
                    var_t0_8 = temp_v1_5;
                    if (temp_v1_5 <= 0) {
                        var_t0_8 = 1;
                    }
                    if (temp_v1_5 <= 0) {
                        var_v0_9 = 1;
                    } else {
                        var_v0_9 = temp_v1_5;
                    }
                    M2C_FIELD(temp_a0_113, s32 *, 4) = (s32) ((((s32) (var_t0_8 + 0x7FF) / var_v0_9) & 0xFFF) | 0x07000000 | ((var_t2_8 & 0xFFF) << 0xC));
                    temp_a0_114 = *(s32 *)0x80149438;
                    *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_114 + 8);
                    M2C_FIELD(temp_a0_114, s32 *, 4) = 0;
                    M2C_FIELD(temp_a0_114, s32 *, 0) = 0xE7000000;
                    temp_a0_115 = *(s32 *)0x80149438;
                    *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_115 + 8);
                    M2C_FIELD(temp_a0_115, s32 *, 0) = (s32) (((((s32) (sp38 + 7) >> 3) & 0x1FF) << 9) | 0xF5480000);
                    temp_t9_8 = ((u16) arg9 & 0xF) << 0x14;
                    var_t3 = temp_t9_8;
                    M2C_FIELD(temp_a0_115, s32 *, 4) = (s32) (temp_t9_8 | 0x80000 | var_s1 | 0x200 | temp_t7_5);
                    temp_a0_116 = *(s32 *)0x80149438;
                    *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_116 + 8);
                    var_t4 = (((sp24 * 4) & 0xFFF) << 0xC) | 0xF2000000 | ((sp3C * 4) & 0xFFF);
                    M2C_FIELD(temp_a0_116, s32 *, 0) = var_t4;
                    M2C_FIELD(temp_a0_116, s32 *, 4) = (s32) ((((((sp24 + sp38) - 1) * 4) & 0xFFF) << 0xC) | ((((sp3C + (u16) arg4) - 1) * 4) & 0xFFF));
                }
                if (sp30 == 5) {
                    temp_a0_117 = *(s32 *)0x80149438;
                    *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_117 + 8);
                    M2C_FIELD(temp_a0_117, s32 *, 0) = (s32) (((((s32) (sp38 + 7) >> 3) & 0x1FF) << 9) | 0xF5400000);
                    M2C_FIELD(temp_a0_117, s32 *, 4) = (s32) (var_t3 | 0x01000000 | 0x80000 | var_s1 | 0x200 | (((sp272 + 1) & 0xF) * 0x10) | 0xF);
                    temp_a0_118 = *(s32 *)0x80149438;
                    *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_118 + 8);
                    M2C_FIELD(temp_a0_118, s32 *, 0) = var_t4;
                    M2C_FIELD(temp_a0_118, s32 *, 4) = (s32) ((((((sp24 + sp38) - 1) * 4) & 0xFFF) << 0xC) | 0x01000000 | ((((sp3C + (u16) arg4) - 1) * 4) & 0xFFF));
                }
            } else if (var_s0 != 0) {
                temp_a0_119 = *(s32 *)0x80149438;
                *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_119 + 8);
                M2C_FIELD(temp_a0_119, s32 *, 0) = (s32) ((((sp38 >> 1) - 1) & 0xFFF) | 0xFD480000);
                M2C_FIELD(temp_a0_119, s32 *, 4) = arg0;
                temp_a0_120 = *(s32 *)0x80149438;
                *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_120 + 8);
                temp_t3_3 = (((s32) (((s32) (((u16) arg7 - sp24) + 1) >> 1) + 7) >> 3) & 0x1FF) << 9;
                M2C_FIELD(temp_a0_120, s32 *, 0) = (s32) (temp_t3_3 | 0xF5480000);
                temp_s1_13 = (var_v0 & 0xF) << 0xE;
                temp_s0_11 = (sp272 & 0xF) * 0x10;
                M2C_FIELD(temp_a0_120, s32 *, 4) = (s32) (temp_s1_13 | 0x07080000 | 0x200 | temp_s0_11);
                temp_a0_121 = *(s32 *)0x80149438;
                *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_121 + 8);
                M2C_FIELD(temp_a0_121, s32 *, 4) = 0;
                M2C_FIELD(temp_a0_121, s32 *, 0) = 0xE6000000;
                temp_a0_122 = *(s32 *)0x80149438;
                *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_122 + 8);
                temp_t0_9 = (sp3C * 4) & 0xFFF;
                M2C_FIELD(temp_a0_122, s32 *, 0) = (s32) ((((sp24 * 2) & 0xFFF) << 0xC) | 0xF4000000 | temp_t0_9);
                temp_t2_9 = (sp34 * 4) & 0xFFF;
                M2C_FIELD(temp_a0_122, s32 *, 4) = (s32) (((((u16) arg7 * 2) & 0xFFF) << 0xC) | 0x07000000 | temp_t2_9);
                temp_a0_123 = *(s32 *)0x80149438;
                *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_123 + 8);
                M2C_FIELD(temp_a0_123, s32 *, 4) = 0;
                M2C_FIELD(temp_a0_123, s32 *, 0) = 0xE7000000;
                temp_a0_124 = *(s32 *)0x80149438;
                *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_124 + 8);
                M2C_FIELD(temp_a0_124, s32 *, 0) = (s32) (temp_t3_3 | 0xF5400000);
                M2C_FIELD(temp_a0_124, s32 *, 4) = (s32) ((((u16) arg9 & 0xF) << 0x14) | 0x80000 | temp_s1_13 | 0x200 | temp_s0_11);
                temp_a0_125 = *(s32 *)0x80149438;
                *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_125 + 8);
                M2C_FIELD(temp_a0_125, s32 *, 0) = (s32) ((((sp24 * 4) & 0xFFF) << 0xC) | 0xF2000000 | temp_t0_9);
                M2C_FIELD(temp_a0_125, s32 *, 4) = (s32) (((((u16) arg7 * 4) & 0xFFF) << 0xC) | temp_t2_9);
            } else {
                temp_a0_126 = *(s32 *)0x80149438;
                *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_126 + 8);
                M2C_FIELD(temp_a0_126, s32 *, 0) = 0xFD500000;
                M2C_FIELD(temp_a0_126, s32 *, 4) = arg0;
                temp_a0_127 = *(s32 *)0x80149438;
                temp_t6_5 = (var_v0 & 0xF) << 0xE;
                *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_127 + 8);
                M2C_FIELD(temp_a0_127, s32 *, 0) = 0xF5500000;
                temp_t7_6 = (sp272 & 0xF) * 0x10;
                M2C_FIELD(temp_a0_127, s32 *, 4) = (s32) (temp_t6_5 | 0x07080000 | 0x200 | temp_t7_6);
                temp_a0_128 = *(s32 *)0x80149438;
                *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_128 + 8);
                M2C_FIELD(temp_a0_128, s32 *, 0) = 0xE6000000;
                M2C_FIELD(temp_a0_128, s32 *, 4) = 0;
                temp_a0_129 = *(s32 *)0x80149438;
                var_t2_9 = 0x7FF;
                *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_129 + 8);
                M2C_FIELD(temp_a0_129, s32 *, 0) = 0xF3000000;
                temp_t1_16 = ((s32) ((sp38 * (u16) arg4) + 3) >> 2) - 1;
                if (temp_t1_16 < 0x7FF) {
                    var_t2_9 = temp_t1_16;
                }
                temp_v1_6 = sp38 / 16;
                if (temp_v1_6 <= 0) {
                    var_t0_9 = 1;
                } else {
                    var_t0_9 = temp_v1_6;
                }
                if (temp_v1_6 <= 0) {
                    var_v0_10 = 1;
                } else {
                    var_v0_10 = temp_v1_6;
                }
                M2C_FIELD(temp_a0_129, s32 *, 4) = (s32) ((((s32) (var_t0_9 + 0x7FF) / var_v0_10) & 0xFFF) | 0x07000000 | ((var_t2_9 & 0xFFF) << 0xC));
                temp_a0_130 = *(s32 *)0x80149438;
                *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_130 + 8);
                M2C_FIELD(temp_a0_130, s32 *, 4) = 0;
                M2C_FIELD(temp_a0_130, s32 *, 0) = 0xE7000000;
                temp_a0_131 = *(s32 *)0x80149438;
                *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_131 + 8);
                M2C_FIELD(temp_a0_131, s32 *, 0) = (s32) (((((s32) ((sp38 >> 1) + 7) >> 3) & 0x1FF) << 9) | 0xF5400000);
                M2C_FIELD(temp_a0_131, s32 *, 4) = (s32) ((((u16) arg9 & 0xF) << 0x14) | 0x80000 | temp_t6_5 | 0x200 | temp_t7_6);
                temp_a0_132 = *(s32 *)0x80149438;
                *(s32 *)0x80149438 = (void *) ((u8 *) temp_a0_132 + 8);
                M2C_FIELD(temp_a0_132, s32 *, 0) = (s32) ((((sp24 * 4) & 0xFFF) << 0xC) | 0xF2000000 | ((sp3C * 4) & 0xFFF));
                M2C_FIELD(temp_a0_132, s32 *, 4) = (s32) ((((((sp24 + sp38) - 1) * 4) & 0xFFF) << 0xC) | ((((sp3C + (u16) arg4) - 1) * 4) & 0xFFF));
            }
        }
        *(s32 *)0x8012E684 = arg0;
        *(s32 *)0x8012E688 = sp28;
        *(s32 *)0x8012E68C = sp2C;
    }
}
