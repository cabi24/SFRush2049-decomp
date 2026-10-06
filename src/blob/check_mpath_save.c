/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * check_mpath_save (historical label): Rumble Pak (re)initialisation for all four controller ports.
 * Clears the rumble-request byte D_8011EAE4, takes the SI/pak lock (queue D_801497D0, created once with
 * D_8011194C as the init flag -- same helpers as the matched func_800A1A60), then for every pak record
 * D_80144030[i] (772 bytes) whose present byte (+6) is set: clear its motor-on byte (+125), call the SDK
 * osMotorInit(&D_80035458, &pak->pfs, i) (0x8000A194, historical label osMotorStart) and the motor
 * stop access __osMotorAccess(&pak->pfs, MOTOR_STOP) (0x80009F20, historical label osMotorInit; the SDK's
 * osMotorStop macro); when the stop succeeds (returns 0) clear +124. Releases the lock with osJamMesg.
 * N64-only, no arcade ancestor.
 *
 * Shaping (each verified necessary, w8d):
 *  - the per-pak body is an inlined static helper with a VALUE early return (`if (!p->present) return -1;`)
 *    and no return value on the normal path (falls off the end). The value return gives retail's
 *    `bnez t8,body; ...; b next` jump-around (ugen emits `li $2,-1; b` and as1 deletes the dead li); a
 *    `void` helper, `continue`, `goto` or if-block give a single `beqz`. A `return 0;` at the end leaves a
 *    `move $2,$0` at the join and stops as1's `bnezl` fill of the motor-stop test;
 *  - pak_lock (which calls pak_queue_init) is a helper but the unlock is a direct osJamMesg, and main keeps
 *    `Pak772 *p`: every inlined helper call and the locals move the frame (88) and the OSMesg slot (sp+68).
 */
typedef unsigned char u8;typedef signed char s8;typedef unsigned short u16;
typedef unsigned int u32;typedef int s32;
typedef void *OSMesg;
typedef struct OSThread OSThread;
typedef struct OSMesgQueue {OSThread *mtqueue,*fullqueue;s32 validCount,first,msgCount;OSMesg *msg;} OSMesgQueue;
typedef struct OSPfs {s32 status;OSMesgQueue *queue;s32 channel;u8 id[32],label[32];s32 version,dir_size,inode_table,minode_table,dir_table,inode_start_page;u8 banks,activebank;} OSPfs;
typedef struct OSPfsState {u32 file_size,game_code;u16 company_code;char ext_name[4],game_name[16];} OSPfsState;
extern void osCreateMesgQueue(OSMesgQueue *,OSMesg *,s32);
extern s32 osJamMesg(OSMesgQueue *,OSMesg,s32),osRecvMesg(OSMesgQueue *,OSMesg *,s32);
typedef struct PakFile40 { u32 metadata;u16 count,flags;OSPfsState state; } PakFile40;
typedef struct Pak772 {
    u8 index;s8 enabled;u8 opaque2[4];s8 present;u8 opaque[5];
    OSPfs pfs;
    u8 opaque116[8];
    u8 motorStopped;
    u8 motorOn;
    u8 opaque126[6];
    PakFile40 files[16];
} Pak772;
extern Pak772 D_80144030[];
extern s8 D_8011194C;
extern OSMesgQueue D_801497D0;
extern OSMesg D_801527E4;
extern s8 D_8011EAE4;
extern OSMesgQueue D_80035458;
extern s32 osMotorStart(OSMesgQueue *, OSPfs *, s32);  /* SDK osMotorInit */
extern s32 osMotorInit(OSPfs *, s32);                 /* SDK __osMotorAccess (osMotorStop) */

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

static s32 motor_init(Pak772 *p, s32 i)
{
    if (!p->present) return -1;
    p->motorOn = 0;
    osMotorStart(&D_80035458, &p->pfs, i);
    if (osMotorInit(&p->pfs, 0) == 0) {
        p->motorStopped = 0;
    }
}

void check_mpath_save(void)
{
    s32 i;
    Pak772 *p;

    D_8011EAE4 = 0;
    pak_lock();
    for (i = 0; i < 4; i++) {
        p = &D_80144030[i];
        motor_init(p, i);
    }
    osJamMesg(&D_801497D0,0,0);
}
