typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;

typedef struct OSMesgQueue { s32 w[6]; } OSMesgQueue;
typedef void *OSMesg;

typedef struct PakFile {
    /* 0x00 */ s32 state;
    /* 0x04 */ u32 game_code;
    /* 0x08 */ u16 company_code;
    /* 0x0A */ u8 ext_name[4];
    /* 0x0E */ u8 game_name[26];
} PakFile; /* 0x28 */

typedef struct PakPort {
    /* 0x000 */ u8 pad0[12];
    /* 0x00C */ u8 pfs[0x68];
    /* 0x074 */ s32 status;
    /* 0x078 */ s32 free_blocks;
    /* 0x07C */ u8 pad7C[0x10];
    /* 0x08C */ PakFile files[16];
    /* 0x30C */
} PakPort;

typedef struct PakRequest {
    /* 0x00 */ struct PakNode *next;
    /* 0x04 */ u8 pad4[4];
    /* 0x08 */ void (*done)(void *node);
    /* 0x0C */ u8 padC[4];
    /* 0x10 */ u8 port;
    /* 0x11 */ u8 index;
    /* 0x12 */ u8 pad12[0x36];
    /* 0x48 */ u32 buffer;
} PakRequest;

typedef struct PakNode {
    PakRequest *request;
} PakNode;

typedef struct PakList {
    u8 pad0[8];
    PakNode *head;
    u8 padC[4];
} PakList;

typedef void (*PakErrorCallback)(s32, s32, s32, s32, s8 *, s8 *, s32 (*)(s32));

extern s8 D_8011194C;
extern s8 D_8011EAE8;
extern OSMesgQueue D_801497D0;
extern OSMesg D_801527E4;
extern PakPort D_80144030[];
extern PakList D_80144D60[];
extern u8 D_801460E0[];
extern PakErrorCallback D_80144008;

void osCreateMesgQueue(OSMesgQueue *, OSMesg *, s32);
s32 osRecvMesg(OSMesgQueue *, OSMesg *, s32);
s32 osJamMesg(OSMesgQueue *, OSMesg, s32);
s32 osPfsRename(void *, u16, u32, u8 *, u8 *);
s32 osPfsFreeBlocks(void *, s32 *);
s32 func_8008A6A4(s32);
s32 func_800A1E94(s32);
void func_8008A704(void);
void audio_effect_process(u32);
void func_8009211C(void *, void *);
void func_80091FBC(void *, void *, void *);

void AdjustSpeed(PakNode *node)
{
    PakRequest *request;
    PakPort *port;
    PakFile *file;
    PakNode *current;
    s32 channel;
    s32 result;
    s8 retry;
    s8 status;

    func_8008A704();
    request = node->request;
    channel = request->port;
    port = &D_80144030[channel];
    file = &port->files[request->index];
    for (;;) {
        result = osPfsRename(port->pfs, file->company_code, file->game_code, file->game_name, file->ext_name);
        if (result == 0) {
            break;
        }
        port->status = func_800A1E94(result);
        osJamMesg(&D_801497D0, 0, 0);
        D_8011EAE8 = channel;
        D_80144008(channel, port->status, 0, 0, &retry, &status, func_8008A6A4);
        current = D_80144D60[channel].head;
        D_8011EAE8 = -1;
        while (current != 0 && current != node) {
            current = current->request->next;
        }
        if (current == 0) {
            return;
        }
        func_8008A704();
        if (retry == 0) {
            osJamMesg(&D_801497D0, 0, 0);
            return;
        }
    }
    osPfsFreeBlocks(port->pfs, &port->free_blocks);
    if (request->done != 0) {
        request->done(node);
    }
    file->state = 0;
    if (request->buffer != 0) {
        audio_effect_process(request->buffer);
        request->buffer = 0;
    }
    func_8009211C(&D_80144D60[node->request->port], node);
    func_80091FBC(D_801460E0, node, *(void **)(D_801460E0 + 8));
    osJamMesg(&D_801497D0, 0, 0);
}
