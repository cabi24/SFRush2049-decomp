/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
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
s32 osJamMesg(OSMesgQueue *,void *,s32);
s32 osPfsRename(void *,u16,u32,char *,char *);
s32 osPfsFreeBlocks(void *,s32 *);
void func_8008A6A4(void);
typedef void (*PakErrorCallback)(s32,s32,s32,s32,s8 *,s8 *,void (*)(void));
extern PakErrorCallback D_80144008;
void func_8009211C(PakList *,PakNode *);
void func_80091FBC(PakList *,PakNode *,PakNode *);
void func_8008A704(void); void sync_release_video(void);
s32 func_800A1E94(s32);
extern OSMesgQueue D_80152770;
s32 osRecvMesg(OSMesgQueue *,void **,s32);
void audio_reverb_update(u32,s32);
void func_80095CF4(void) {
 osRecvMesg(&D_80152770,0,1);
}
void func_80095CFC(void) {
 osJamMesg(&D_80152770,0,0);
}
void audio_effect_process(u32 address) {
 func_80095CF4();
 audio_reverb_update(address,0);
 func_80095CFC();
}

void func_800A2670(Controller *controller) {
    osPfsFreeBlocks(controller->pfs,&controller->freeBytes);
}
void func_800A2674(PakNode *node) {
    func_80091FBC(&D_801460E0, node, D_801460E0.head);
}
void func_800A2678(PakNode *node) {
    func_8009211C(&D_80144D60[node->req->port], node);
    func_800A2674(node);
}

void AdjustSpeed(PakNode *node)
{
    Controller *controller;
    OSPfsState *file;
    s32 port;
    PakNode *cur;
    s8 retry,status;
    PakRequest *request;
    s32 result;

    func_8008A704();
    request=node->req;
    port=request->port;
    file=&D_80144030[request->port].files[request->index].state;
    controller=&D_80144030[port];
    for(;;) {
        result=osPfsRename(controller->pfs,file->company_code,file->game_code,file->game_name,file->ext_name);
        if(result==0) break;
        D_80144008((controller->error=func_800A1E94(result), sync_release_video(), D_8011EAE8=port, port),controller->error,0,0,&retry,&status,func_8008A6A4);
        D_8011EAE8=-1;
        for (cur = D_80144D60[port].head; cur != 0; cur = cur->req->next) {
            if (node == cur) {
                break;
            }
        }
        if (cur == 0) return;
        func_8008A704();
        if(retry==0) {
            sync_release_video();
            return;
        }
    }
    osPfsFreeBlocks(controller->pfs,&controller->freeBytes);
    if(request->done) {
        request->done(node);
    }
    file->file_size=0;
    if(request->buffer) {
        audio_effect_process(request->buffer);
        request->buffer=0;
    }
    func_800A2678(node);
    sync_release_video();
}
