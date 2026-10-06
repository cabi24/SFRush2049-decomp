typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef void *OSMesg;
typedef struct OSMesgQueue OSMesgQueue;

s32 osJamMesg(OSMesgQueue *mq, OSMesg msg, s32 flag);
s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flag);
void viUpdateTime(void);
void process_inputs(void);
s32 input_init_flag_get(void);

extern volatile u8 D_80035470;
extern volatile u8 D_80035471;
extern volatile u8 D_80035472;
extern OSMesgQueue D_8002ECC0;
extern OSMesgQueue D_8002ECF8;
extern s32 D_801497C8;
extern s32 D_801497F4;

typedef struct { s16 type; s16 pad; u8 data[28]; } SchedMsg;

void game_mode_handler(void)
{
    SchedMsg sm;
    OSMesg msg;

    D_80035472 = 1;
    sm.type = 2750;
    osJamMesg(&D_8002ECF8, &sm, 1);
    D_80035471 = 0;
    if (osRecvMesg(&D_8002ECC0, &msg, 0) == -1) {}
    osRecvMesg(&D_8002ECC0, &msg, 1);
    while (osRecvMesg(&D_8002ECC0, &msg, 0) != -1) {
    }
    D_80035471 = 1;
    D_80035470 = 0;
    D_801497F4 = D_801497C8;
    viUpdateTime();
    process_inputs();
    input_init_flag_get();
}
