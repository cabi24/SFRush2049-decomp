/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* SDK views copied from include/PR/os_pfs.h and os_message.h. */
typedef unsigned char u8;typedef signed char s8;typedef unsigned short u16;
typedef unsigned int u32;typedef int s32;
typedef void *OSMesg;
typedef struct OSThread OSThread;
typedef struct OSMesgQueue {OSThread *mtqueue,*fullqueue;s32 validCount,first,msgCount;OSMesg *msg;} OSMesgQueue;
typedef struct OSPfs {s32 status;OSMesgQueue *queue;s32 channel;u8 id[32],label[32];s32 version,dir_size,inode_table,minode_table,dir_table,inode_start_page;u8 banks,activebank;} OSPfs;
typedef struct OSPfsState {u32 file_size,game_code;u16 company_code;char ext_name[4],game_name[16];} OSPfsState;
extern void osCreateMesgQueue(OSMesgQueue *,OSMesg *,s32);
extern s32 osJamMesg(OSMesgQueue *,OSMesg,s32),osRecvMesg(OSMesgQueue *,OSMesg *,s32),osPfsDeleteFile(OSPfs *,s32,OSPfsState *);
typedef struct PakFile40 { u32 metadata;u16 count,flags;OSPfsState state; } PakFile40;
typedef struct Pak772 {
    u8 index;s8 enabled;u8 opaque[10];
    OSPfs pfs;
    u8 opaque116[16];
    PakFile40 files[16];
} Pak772;
typedef struct Object {u8 opaque[16];u8 port,file;} Object;
typedef struct Handle {Object *object;} Handle;
extern Pak772 D_80144030[];
extern s8 D_8011194C;
extern OSMesgQueue D_801497D0;
extern OSMesg D_801527E4;
extern int func_800A1910(void *,void *,int);
static s32 pak_enabled(Pak772 *pak)
{
    return pak->enabled;
}

static s32 pak_file_state(OSPfs *pfs, s32 file, OSPfsState *state)
{
    s32 error;
    OSMesg message;

    if (!D_8011194C) {
        D_8011194C = 1;
        osCreateMesgQueue(&D_801497D0,&D_801527E4,1);
        osJamMesg(&D_801497D0,0,0);
    }
    osRecvMesg(&D_801497D0,&message,1);
    error = osPfsDeleteFile(pfs,file,state);
    osJamMesg(&D_801497D0,0,0);
    return error;
}

int func_800A1A60(Handle *handle)
{
    u32 port,file;
    int error;
    OSPfsState state;
    Pak772 *pak;
    port = handle->object->port;
    file = handle->object->file;
    pak = &D_80144030[port];
    if (!pak_enabled(pak)) return 0;
    error = pak_file_state(&pak->pfs,file,&state);
    if (error) return 0;
    if (func_800A1910(&state,&D_80144030[port].files[file].state,32)) return 0;
    return 1;
}
