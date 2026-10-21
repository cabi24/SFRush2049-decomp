/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* func_800E73D8 (0x800E73D8, 596 B): N64 per-frame controller read (model thread; no MAT3/arcade ancestor).
 * Under the SI/pak lock (pak_queue_init/pak_lock/pak_unlock statics, as in the matched func_800E7710 and
 * func_800A1A60): drain the SI event queue D_80035458; every 30th call (function-local static counter, retail
 * .data byte 0x1D = 29 so the first call queries) re-query the pads (osContStartQuery, wait for SI message 5,
 * osContGetQuery(D_80149440), func_800A43FC()); start a read (osContStartReadData; on failure unlock and return 0),
 * wait, osContGetReadData(pads), unlock; per controller: connected flag from the status/pad errno bit 8
 * (CONT_NO_RESPONSE_ERROR), disconnected pads read as zero, 19-entry button map D_8011FAD4 -> game button mask,
 * stick copied, record bytes 10..13 cleared; then func_800E7134() and return 1.
 * Base: w36w3 best (4 words: the counter web in v1, retail v0). w37d: the compiled-out block end
 * `do {} while (0);` (disclosed; emits no code) after the drain splits uopt's basic block between the
 * osRecvMesg call and the counter, so v0 (the call's precoloured return) no longer forbids v0 for the counter web.
 */
typedef unsigned char u8;typedef signed char s8;typedef unsigned short u16;typedef short s16;
typedef unsigned int u32;typedef int s32;
typedef void *OSMesg;
typedef struct OSThread OSThread;
typedef struct OSMesgQueue {OSThread *mtqueue,*fullqueue;s32 validCount,first,msgCount;OSMesg *msg;} OSMesgQueue;
typedef struct OSContStatus {u16 type;u8 status,errno;} OSContStatus;
typedef struct OSContPad {u16 button;s8 stick_x,stick_y;u8 errno;} OSContPad;
extern void osCreateMesgQueue(OSMesgQueue *,OSMesg *,s32);
extern s32 osSendMesg(OSMesgQueue *,OSMesg,s32),osRecvMesg(OSMesgQueue *,OSMesg *,s32);
extern s32 osContStartQuery(OSMesgQueue *);
extern void osContGetQuery(OSContStatus *);
extern s32 osContStartReadData(OSMesgQueue *);
extern void osContGetReadData(OSContPad *);
typedef struct {
    s8 connected;       /* 0x0 */
    u8 pad1[3];
    u32 buttons;        /* 0x4 */
    s8 stick[2];        /* 0x8 */
    s8 unkA[4];         /* 0xA */
    u8 padE[2];
} ContRec16;
extern s8 D_8011194C;
extern OSMesgQueue D_801497D0;
extern OSMesg D_801527E4;
extern OSMesgQueue D_80035458;
extern OSContStatus D_80149440[];
extern volatile ContRec16 D_80156CF0[4];
extern u32 D_8011FAD4[19];
void func_800A43FC(void);
void func_800E7134(void);

static void pak_queue_init(void)
{
    if (!D_8011194C) {
        D_8011194C = 1; osCreateMesgQueue(&D_801497D0,&D_801527E4,1);
        osSendMesg(&D_801497D0,0,0);
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
    osSendMesg(&D_801497D0,0,0);
}

s32 func_800E73D8(void)
{
    static u8 queryTimer = 29;
    OSMesg msg = 0;
    s32 i;
    s32 j;
    OSContPad pads[4];

    pak_lock();
    osRecvMesg(&D_80035458, &msg, 0);
    do {} while (0);
    if (++queryTimer == 30) {
        queryTimer = 0;
        osContStartQuery(&D_80035458);
        do {
            osRecvMesg(&D_80035458, &msg, 1);
        } while (*(s16 *)msg != 5);
        osContGetQuery(D_80149440);
        func_800A43FC();
    }
    if (osContStartReadData(&D_80035458) != 0) {
        pak_unlock();
        return 0;
    }
    osRecvMesg(&D_80035458, &msg, 1);
    osContGetReadData(pads);
    pak_unlock();
    for (i = 0; i < 4; i++) {
        if ((D_80149440[i].errno & 8) || (pads[i].errno & 8)) {
            if (D_80156CF0[i].connected) {
                D_80156CF0[i].connected = 0;
            }
        } else {
            if (!D_80156CF0[i].connected) {
                D_80156CF0[i].connected = 1;
            }
        }
        if (!D_80156CF0[i].connected) {
            pads[i].stick_x = pads[i].stick_y = 0;
            pads[i].button = 0;
        }
        D_80156CF0[i].buttons = 0;
        for (j = 0; j < 19; j++) {
            if (pads[i].button & D_8011FAD4[j]) {
                D_80156CF0[i].buttons |= 1 << j;
            }
        }
        D_80156CF0[i].stick[0] = pads[i].stick_x;
        D_80156CF0[i].stick[1] = pads[i].stick_y;
        D_80156CF0[i].unkA[0] = 0;
        D_80156CF0[i].unkA[1] = 0;
        D_80156CF0[i].unkA[2] = 0;
        D_80156CF0[i].unkA[3] = 0;
    }
    func_800E7134();
    return 1;
}
