/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef void *OSMesg;
typedef struct OSThread OSThread;
typedef struct OSMesgQueue {OSThread *mtqueue,*fullqueue;s32 validCount,first,msgCount;OSMesg *msg;} OSMesgQueue;
extern s32 osSendMesg(OSMesgQueue *,OSMesg,s32),osRecvMesg(OSMesgQueue *,OSMesg *,s32);
#define NULL 0

typedef struct {
    s8 connected;       /* 0x0 */
    u8 pad1[3];
    u32 buttons;        /* 0x4 */
    s8 stick[2];        /* 0x8 */
    u8 padA[6];
} ContRec16;
extern s8 D_80111954;
extern OSMesgQueue D_801497A8;
extern ContRec16 D_80156CF0[4];   /* controller records filled by func_800E73D8 */
extern volatile s8 D_80149B50[4][2];   /* gStickRaw */
extern volatile s8 D_80149AF8[4][2];   /* gStickCal */
extern s32 D_80149B30[4];         /* gHeld */
extern s32 D_80149B10[4];         /* gPrevPressed */
extern s32 D_8013FED0[4];         /* buttons */
extern s32 D_801403C0[4];         /* pressed this frame */
extern f32 D_80140620[4][2];      /* scaled stick */
extern s8 D_80149B74[4];          /* stick-as-dpad per controller */
void func_800E7038(void);

/* scale a raw stick value to [-range, range] (locked, controller_poll group) */
f32 func_800C9590(f32 range, f32 calib, s32 raw)
{
    f32 v;

    v = ((f32) raw * range) / calib;
    if (v < -range) {
        v = -range;
    } else if (range < v) {
        v = range;
    }
    return v;
}

void func_800E7134(void)
{
    s32 i;
     u32  buttons;
    s32 pressed;

    if (D_80111954 == 0) {
        func_800E7038();
    }
    osRecvMesg(&D_801497A8, NULL, 1);
    for (i = 0; i < 4; i++) {
        
        if (D_80156CF0[i].connected == 0) {
             buttons = 0; 
            D_80149B50[i][0] = 0;
            D_80149B50[i][1] = 0;
            D_80149AF8[i][0] = 70;
            D_80149AF8[i][1] = 70;
        } else {
            buttons = D_80156CF0[i].buttons;
            D_80149B50[i][0] = D_80156CF0[i].stick[0];
            D_80149B50[i][1] = D_80156CF0[i].stick[1];
            if (D_80149B74[i] != 0 ) {
                if (D_80149B50[i][0] < -56) {
                    buttons |= 0x1000;
                }
                if (D_80149B50[i][0] > 56) {
                    buttons |= 0x2000;
                }
                if (D_80149B50[i][1] < -56) {
                    buttons |= 0x800;
                }
                if (D_80149B50[i][1] > 56) {
                    buttons |= 0x400;
                }
            }
            if (buttons & 0x40000) {
                D_80149AF8[i][0] = 70;
                D_80149AF8[i][1] = 70;
            }
            if ((D_80149AF8[i][0] < D_80149B50[i][0] || D_80149B50[i][0] < -D_80149AF8[i][0]) && D_80149AF8[i][0] < 127) {
                D_80149AF8[i][0]++;
            }
            if ((D_80149AF8[i][1] < D_80149B50[i][1] || D_80149B50[i][1] < -D_80149AF8[i][1]) && D_80149AF8[i][1] < 127) {
                D_80149AF8[i][1]++;
            }
        }
        D_8013FED0[i] = buttons;
        pressed = (D_80149B30[i] ^ buttons) & buttons;
        D_80149B10[i] |= pressed;
        D_80149B30[i] = buttons;
        D_801403C0[i] = pressed;
        D_80140620[i][0] = func_800C9590(1.0f, D_80149AF8[i][0], D_80149B50[i][0]);
        D_80140620[i][1] = func_800C9590(1.0f, D_80149AF8[i][1], D_80149B50[i][1]);
    }
    osSendMesg(&D_801497A8, NULL, 0);
}
