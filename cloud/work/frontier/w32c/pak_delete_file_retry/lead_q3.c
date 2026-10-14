/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* w32c lead: w31c best.c + compiled-out `if (pfs) {}` before `if (retry == 0)` (DEVICE). Unit: 123 of 196 words, body
 * length exact, rows 105, ops 21, frame 232/232; pfs coloured (s7) but the callback keeps s0. Not a deliverable. */
/*
 * w31c pak_delete_file_retry (0x800A2680, 784 B; ex-label AdjustSpeed) -- NOT a match. Body = w28b best.c unchanged.
 * Unit (w31c, 880e729e): FAIL 165 of 196 words; rows 98, ops 12; frame 232/232 (UARGS: --internal func_800A2670
 *   --internal func_800A2678 --internal pak_buffer_free --internal pak_heap_lock --internal pak_heap_unlock).
 * draft_check: EQUIVALENT over 2 calls / 2 scripts (ghost_resave 1, ghost_delete_fail 1) with dc.args
 *   `--callee 'void audio_reverb_update(u32 address, s32 tag)=a1,a2/s0,s1'`. The "a1 bug" was a harness
 *   convention, not the C: audio_reverb_update is IPA-internal (address a1, tag a2, clobbers s0/s1 unsaved), so the
 *   (address, 0) call is right in the unit; standalone o32 put address in a0 and lost node (s0/s1) -> fault.
 * Frame finding (w31c): the "8 B too high" lock message slots are the named `pfs` local, not an inline area:
 *   without it (v/p5.c) the lock slots sit 36 B below the frame top as in retail and every named home keeps retail's
 *   relative offset; the frame is then 224 = retail - 8, the 8 B being retail's spill slot for the &func_8008A6A4
 *   callback constant (t0 -> 96(sp)), which ours colours s0 (ctrace p1 w41 tot 20 > callee cost 10.5) while
 *   retail's pfs expression (controller+12, s8) is split in ours (w99 tot 10 < 10.5). Residual = callee-saved colour.
 */
typedef signed char s8;typedef unsigned char u8;typedef signed int s32;typedef unsigned int u32;
typedef unsigned short u16; typedef signed short s16;
typedef struct { s32 w[6]; } OSMesgQueue;
typedef struct OSPfsState {
    u32 file_size;
    u32 game_code;
    u16 company_code;
    char ext_name[4];
    char game_name[16];
} OSPfsState;
typedef struct PakFile {
    u8 unk0;
    s8 unk1;
    s8 unk2;
    u16 unk4;
    OSPfsState state;
} PakFile; /* 0x28 */
typedef struct Controller {
    char pad0[0xC];
    char pfs[0x68];
    s32 error;      /* 0x74 */
    s32 freeBytes;  /* 0x78 */
    char pad7C[8];
    PakFile files[16];
    char pad[0x304 - 0x84 - 16 * 0x28];
} Controller; /* 0x304 */
struct PakNode;
typedef struct PakRequest {
    struct PakNode *next;
    s32 unk4;
    void (*done)(struct PakNode *);
    s32 unkC;
    u8 port;
    u8 index;
    char pad12[0x36];
    u32 buffer;   /* 0x48 */
} PakRequest;
typedef struct PakNode { PakRequest *req; } PakNode;
typedef struct PakList { s32 unk0; s32 unk4; PakNode *head; s32 unkC; } PakList;
extern s8 D_8011194C,D_8011EAE8;
extern OSMesgQueue D_801497D0;
extern Controller D_80144030[];
extern PakList D_80144D60[];
extern PakList D_801460E0;
s32 osSendMesg(OSMesgQueue *,void *,s32);
s32 osPfsDeleteFile(void *,u16,u32,char *,char *);
s32 osPfsFreeBlocks(void *,s32 *);
void osCreateMesgQueue(OSMesgQueue *, void **, s32);
s32 func_8008A6A4(s32);
typedef void (*PakErrorCallback)(s32,s32,s32,s32,s8 *,s8 *,s32 (*)(s32));
extern PakErrorCallback D_80144008;
void func_8009211C(PakList *,PakNode *);
void func_80091FBC(PakList *,PakNode *,PakNode *);
s32 func_800A1E94(s32);
extern OSMesgQueue D_80152770;
extern void *D_801527E4;
s32 osRecvMesg(OSMesgQueue *,void **,s32);
void audio_reverb_update(u32,s32);
void func_8008A704(void); void sync_release_video(void); void pak_heap_lock(void) {
    osRecvMesg(&D_80152770, 0, 1);
}
void pak_heap_unlock(void) {
    osSendMesg(&D_80152770, 0, 0);
}
void pak_buffer_free(u32 address) {
    pak_heap_lock();
    audio_reverb_update(address, 0);
    pak_heap_unlock();
}
#define LOCK() func_8008A704()
#define UNLOCK() sync_release_video()
void func_800A2670(Controller *controller) {
    osPfsFreeBlocks(controller->pfs,&controller->freeBytes);
}
void func_800A2678(PakNode *node) {
    func_8009211C(&D_80144D60[node->req->port], node);
    func_80091FBC(&D_801460E0, node, D_801460E0.head);
}

void pak_delete_file_retry(PakNode *node)
{
    Controller *controller;
    OSPfsState *file;
    s32 port;
    PakNode *cur;
    s8 retry,status;
    PakRequest *request;
    s32 result;
    char *pfs;

    LOCK();
    request=node->req;
    port=request->port;
    file=&D_80144030[request->port].files[request->index].state;
    controller=&D_80144030[port];
    pfs=controller->pfs;
    for(;;) {
        result=osPfsDeleteFile(pfs,file->company_code,file->game_code,file->game_name,file->ext_name);
        if(result==0) break;
        controller->error=func_800A1E94(result);
        UNLOCK();
        D_8011EAE8=port;
        D_80144008(port,controller->error,0,0,&retry,&status,func_8008A6A4);
        D_8011EAE8=-1;
        for (cur = D_80144D60[port].head; cur != 0; cur = cur->req->next) {
            if (node == cur) {
                break;
            }
        }
        if (cur == 0) return;
        LOCK();
        if (pfs) {}
        if(retry==0) {
            UNLOCK();
            return;
        }
    }
    func_800A2670(controller);
    if(request->done) {
        request->done(node);
    }
    file->file_size=0;
    if(request->buffer) {
        pak_buffer_free(request->buffer);
        request->buffer=0;
    }
    func_800A2678(node);
    UNLOCK();
}
