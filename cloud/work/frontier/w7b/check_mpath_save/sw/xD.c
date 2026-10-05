/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
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
    u8 index;s8 enabled;u8 opaque2[4];s8 present;u8 opaque[5];
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

extern s8 D_8011EAE4;
extern OSMesgQueue D_80035458;
extern s32 osMotorStart(OSMesgQueue *, OSPfs *, s32);
extern s32 osMotorInit(OSPfs *, s32);

void check_mpath_save(void)
{
    s32 i;
    Pak772 *p;

    D_8011EAE4 = 0;
    pak_lock();
    for (i = 0; i < 4; i++) {
        p = &D_80144030[i];
        if (p->present != 0) {
            p->opaque116[9] = 0;
            osMotorStart(&D_80035458, &p->pfs, i);
            if (osMotorInit(&p->pfs, 0) == 0) {
                p->opaque116[8] = 0;
            }
        } else {
            continue;
        }
    }
    pak_unlock();
}
