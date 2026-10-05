/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_800A1A60 (historical label): does the Controller Pak file of a save handle still hold what the game
 * wrote? handle->object gives the port (+16) and file (+17); the pak record D_80144030[port] (772 bytes:
 * enabled byte +1, OSPfs +12, 16 file slots of 40 bytes at +140 holding the last OSPfsState) must be
 * enabled; under the SI/pak lock (queue D_801497D0, created once with D_8011194C as the init flag and
 * primed with one message) the SDK file state is read (0x8000A520, historical label osPfsDeleteFile; it
 * is osPfsFileState(pfs, file, &state)); returns 1 when the call succeeded and func_800A1910 (memcmp-like)
 * finds the 32 state bytes equal to the remembered ones, else 0. N64-only, no arcade ancestor.
 *
 * Shape (all inferred from frame and register evidence, each verified necessary):
 *  - pak_enabled / pak_lock (which calls pak_queue_init) / pak_unlock are static helpers inlined by umerge:
 *    the enabled byte lands in v0 (inlined return value, so no branch-likely), message is pak_lock's local
 *    (sp+44), and the inlined calls account for the 112-byte frame (no padding locals);
 *  - `if (error) {}` after the call is a compiled-out check (INFERRED: e.g. an empty debug print): it keeps
 *    error register-allocated (v1) and spilled across osJamMesg to its home sp+96, as retail does;
 *  - `D_8011194C = 1; osCreateMesgQueue(...)` on one line: as1 then puts `li t3,1` in the bnez delay slot.
 * The same lock helpers appear in check_mpath_save, track_collision_setup, MaxPathZeroControls,
 * func_800E73D8 and func_800E7710 (all read D_8011194C).
 */
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

static void pak_queue_init(void)
{
    if (!D_8011194C) {
        D_8011194C = 1; osCreateMesgQueue(&D_801497D0,&D_801527E4,1);
        osJamMesg(&D_801497D0,0,0);
    }
}

static void pak_lock(void)
{
    OSMesg message;

    pak_queue_init();
    osRecvMesg(&D_801497D0,&message,1);
}

static void pak_unlock(void)
{
    osJamMesg(&D_801497D0,0,0);
}

int func_800A1A60(Handle *handle)
{
    Pak772 *pak;
    u32 port,file;
    int error;
    OSPfsState state;
    port = handle->object->port;
    file = handle->object->file;
    pak = &D_80144030[port];
    if (!pak_enabled(pak)) return 0;
    pak_lock();
    error = osPfsDeleteFile(&pak->pfs,file,&state);
    if (error) {}
    pak_unlock();
    if (error) return 0;
    if (func_800A1910(&state,&D_80144030[port].files[file].state,32)) return 0;
    return 1;
}
