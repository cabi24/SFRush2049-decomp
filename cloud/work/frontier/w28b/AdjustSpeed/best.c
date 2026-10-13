/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * w28b AdjustSpeed (0x800A2680, 784 B) -- NOT a match: 166 of 196 words / 99 rows, frame 232/232, structure exact.
 * Label is a speculative arcade attribution; the function deletes a controller-pak file with retry:
 * lock (lazy queue init, inlined func_8008A704) -> osPfsDeleteFile(pfs, company, game_code, game_name, ext_name)
 * -> on error: error = func_800A1E94(r), unlock, D_80144008(port, error, 0, 0, &retry, &status, func_8008A6A4),
 * check the node is still queued in D_80144D60[port], relock, retry or unlock+return; on success osPfsFreeBlocks
 * (func_800A2670 helper, retail stub), done(node), file_size = 0, free the buffer under the D_80152770 heap lock
 * (pak_buffer_free = audio_effect_process with nested lock/unlock helpers: needed for the 232 frame), move the node to
 * D_801460E0 (func_800A2678 helper, retail stub), unlock.
 * Unit: blob_unit --tag w28b score AdjustSpeed --with THIS.c --internal func_800A2670 --internal func_800A2678
 *   --internal pak_buffer_free --internal pak_heap_lock --internal pak_heap_unlock --neighbours
 * Residual: callee-saved priority. 10 candidate webs for 9 s-registers; ours gives the &func_8008A6A4 callback
 * constant s0 (tot 20 > cost 10.25) and spills pfs; retail spills the callback (t0 -> 96(sp)) and keeps pfs in s8.
 * Base: w2d AdjustSpeed/best.c (osPfsRename -> osPfsDeleteFile, osJamMesg -> osSendMesg after the libultra renames).
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
