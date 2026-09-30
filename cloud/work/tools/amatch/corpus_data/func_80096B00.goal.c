/* flags: -g0 -O2 -mips2 -G 0 -non_shared */

/*@@HDR 4 24@@*/
typedef union 
{
  struct 
  {
    u32 w0;
    u32 w1;
  } words;
  u64 force_structure_alignment;
/*@@HDR 30 47@@*/
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
  struct __OSThreadprofile_s *thprof;
  __OSThreadContext context;
} OSThread;
void osCreateThread(OSThread *thread, OSId id, void (*entry)(void *), void *arg, void *sp, OSPri priority);
/*@@HDR 98 108@@*/
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
/*@@HDR 206 215@@*/
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
/*@@HDR 232 241@@*/
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
/*@@HDR 253 268@@*/
typedef struct 
{
  u32 ctrl;
  u32 width;
  u32 burst;
  u32 vSync;
  u32 hSync;
  u32 leap;
  u32 hStart;
  u32 xScale;
  u32 vCurrent;
} OSViCommonRegs;
typedef struct 
{
  u32 origin;
  u32 yScale;
  u32 vStart;
  u32 vBurst;
  u32 vIntr;
} OSViFieldRegs;
typedef struct 
{
  u8 type;
  OSViCommonRegs comRegs;
  OSViFieldRegs fldRegs[2];
} OSViMode;
typedef struct 
{
  f32 factor;
  u16 offset;
  u32 scale;
} __OSViScale;
typedef struct 
{
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
typedef struct 
{
  u32 ramarray[15];
  u32 pifstatus;
} OSPifRam;
extern OSPifRam __osSiDmaBuffer;
extern u8 __osPfsBuffer[64];
extern s32 __osContRamWrite(OSMesgQueue *mq, s32 channel, u16 address, u8 *buffer, s32 force);
extern s16 gViewportOffsetX[32];
extern s16 gViewportOffsetY[32];
extern s32 dma_wait(s32 blocking);
extern void dma_signal(void);
extern s32 __osSiDmaRetry;
extern void __osEnqueueAndYield(OSThread **queue);
extern s32 __osContRamRead(OSMesgQueue *mq, s32 channel, u16 address, u8 *buffer);
typedef struct OSScTask_s
{
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
typedef struct OSScClient_s
{
  struct OSScClient_s *next;
  OSMesgQueue *msgQueue;
} OSScClient;
typedef struct 
{
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
extern s32 __osPiRawStartDma(void *mb, s32 priority, s32 direction, u32 devAddr, void *dramAddr, u32 size, OSMesgQueue *mq);
typedef struct 
{
  s32 flag;
  u8 pad04[0x8 - 0x4];
  s32 unk8;
  u8 pad0C[0x1C - 0xC];
/*@@HDR 366 380@@*/
typedef struct __OSTimerNode_s
{
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
typedef enum GState
{
  ATTRACT,
  TRKSEL,
  CARSEL,
  PLAYGAME,
  ENDGAME,
  GAMEOVER,
  HISCORE,
  PREPLAY,
  PREPLAY2,
  COUNTDOWN,
  NUM_GAME_STATES
} GState;
extern u8 gstate;
extern s32 frame_counter;
extern s32 game_state_flags;
extern s32 state_word_a;
extern s32 state_word_b;
extern OSMesgQueue *msgq_ptr;
typedef struct 
{
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
typedef struct 
{
  f32 unk0;
  f32 unk4;
} D_80156958_Entry;
extern D_80156958_Entry D_80156958[4];
typedef struct 
{
  u8 pad0000[0x9CC0];
  OSMesgQueue unk9CC0;
} SegmentHeader;
typedef struct 
{
  u8 pad00[0x58];
  SegmentHeader *unk58;
  u8 pad5C[0x7C - 0x5C];
  void *unk7C;
} SegmentTableEntry;
extern SegmentTableEntry D_80156BE0[];
typedef struct 
{
  u8 pad0[8];
  s32 unk8;
} D_8014A160_Target;
extern D_8014A160_Target **D_8014A160;
typedef struct D_8012E6E0_Node
{
  struct D_8012E6E0_Node *next;
} D_8012E6E0_Node;
extern D_8012E6E0_Node *D_8012E6E0;
typedef struct 
{
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
typedef struct 
{
  u8 pad00[0x39];
  u8 unk39;
  u8 unk3A;
  u8 unk3B;
} PlaygameSettings;
extern PlaygameSettings playgame_settings;
typedef struct 
{
  u8 pad000[0x1F0];
  s32 unk1F0;
  s32 unk1F4;
  s32 unk1F8;
  s32 unk1FC;
  s32 unk200;
} CountdownObject;
typedef struct 
{
  u8 pad000[0x19C];
  void *unk19C;
  u8 pad1A0[0x200 - 0x1A0];
  void *unk200;
  void *unk204;
  void *unk208;
} CountdownDetail;
typedef struct 
{
  void *unk0;
  CountdownDetail *unk4;
  void *unk8;
  void *unkC;
  void **unk10;
} CountdownState;
extern CountdownState countdown_state;
extern CountdownObject *countdown_object;
typedef struct PadConfig_s
{
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
typedef struct 
{
  PadConfig *unk0;
  s32 unk4;
} D_80138670_Entry;
extern D_80138670_Entry D_80138670[];
extern s32 game_loop_tick;
extern s16 active_player_count;
extern s32 gameplay_mode;
extern f32 D_8002AFB4;
extern f32 D_8002AFB8;
extern s32 D_8002AFC0;
extern s32 D_8002AFC4;
extern s32 D_8002EBB0;
extern u16 D_8002EB70;
extern u8 D_80035470;
extern u8 D_80035471;
extern u8 D_80035472;
extern s32 D_80111958;
extern u8 D_80114650;
extern u8 D_80114654;
extern u8 D_801146F0;
extern s32 D_801146F8;
extern s32 D_801170FC;
extern u8 D_80117350;
extern u8 D_80117354;
extern s32 D_801174BC;
extern u8 D_8011ED0B;
extern u16 D_8011ED0C[];
extern f32 D_80123FB4;
extern f32 D_80123FB8;
extern f32 D_80123FBC;
extern f32 D_801242A8;
extern u8 D_80124F84;
extern s32 D_80124FC8;
extern u8 D_8012E67C;
extern u8 D_8013FECB;
extern s32 D_80140008;
extern u16 D_80140618;
extern s32 D_801406B8;
extern s32 D_801407BC;
extern s32 D_80140804;
extern s32 D_80140A00;
extern s32 D_80140AD8;
extern s32 D_80140B08;
extern s32 D_80140BD8;
extern u8 D_80140C26;
extern s32 D_80140D70;
extern s32 D_80141428;
extern s32 D_80142510;
extern u8 D_80142690;
extern u8 D_80142699;
extern u8 D_80142760;
extern s32 D_80143F10;
extern f32 D_8014401C;
extern u8 D_801461F8;
extern u8 D_80146204;
extern u8 D_80146205;
extern u8 D_80149414;
extern s32 D_80149438;
extern u8 D_80149774;
extern u8 D_80149794;
extern u8 D_801497C4;
extern s32 D_801497F4;
extern s32 D_80149D98;
extern u8 D_8014B240;
extern u8 D_80150EFC;
extern u8 D_80150F14;
extern s32 D_80150000;
extern u16 D_80151AD0;
extern u8 D_80151AD8;
extern u8 D_8015256C;
extern u8 D_80152744;
extern u8 D_80152F29;
extern s32 D_8015204C;
extern s32 D_801520C4;
extern s32 D_80153308;
extern f32 D_801525F4;
extern f32 D_801543CC;
extern u16 D_80152734;
extern s32 D_8015698C;
extern u8 D_80156994;
extern u8 D_80156CF0;
extern u8 D_80157244;
extern u8 D_8015F72D;
extern s32 D_8015B250;
extern s32 D_8015B260;
extern s32 D_8015F738;
extern s32 D_80161380;
extern s32 D_80161398;
extern s32 D_801613A4;
extern s32 D_801613AC;
extern s32 D_801613B0;
extern s32 D_80161434;
extern s32 D_8017A4B0;
extern s32 D_8017A508;
extern s32 D_8017A638;
typedef struct 
{
  u16 unk00;
  u16 unk02;
  u8 pad04[0x60 - 0x04];
  s32 unk60;
  s32 unk64;
} D_80143FD8_Record;
extern D_80143FD8_Record *D_80143FD8;
typedef struct 
{
  u8 pad000[0x7C6];
  u16 unk7C6;
  u8 pad7C8[0x7E8 - 0x7C8];
  u8 unk7E8;
} D_8014A250_Record;
extern D_8014A250_Record D_8014A250;
typedef struct 
{
  u8 pad00[0x0C];
  u8 unk0C;
  u8 unk0D;
  u8 unk0E;
} D_80146108_Record;
extern D_80146108_Record D_80146108;
typedef struct 
{
  F32 start_time[8];
  F32 end_time[8];
  S16 loop_chkpnt;
  S16 finish_line;
  S16 before_finish;
  S16 number_of_laps;
} Track_Data;
typedef struct SoundClearRecord
{
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
typedef struct SoundState
{
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
/*@@HDR 624 637@@*/
extern s32 PhysicsObjectList_Update(void);
/*@@HDR 638 3050@@*/
/*@@HDR 3051 3086@@*/
u8 *func_800A4770(u8 *arg0, u8 *arg1);
/*@@HDR 3087 3639@@*/
typedef s32 M2C_UNK;
typedef s8 M2C_UNK8;
typedef s16 M2C_UNK16;
typedef s32 M2C_UNK32;
typedef s64 M2C_UNK64;
typedef struct { s32 key; s32 a; s32 b; } E12;
typedef struct { s32 pad0; s32 count; E12 *tbl; } H12;
void *func_80096B00(H12 *h, s32 arg1)
{
  s32 i;
  if (h == 0) {
    return 0;
  }
  for (i = 0; i < h->count; i++) {
    if (h->tbl[i].key == arg1) {
      return &h->tbl[i];
    }
  }
  return 0;
}
