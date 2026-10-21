/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef void *OSMesg;
typedef struct { s32 pad[6]; } OSMesgQueue;

extern s8 D_80111954;
extern u8 D_80111950;
s8 D_80149AF8[4][2];
extern OSMesgQueue D_801497A8;
extern OSMesg D_80152730[];
void player_state_set(s32 player, s32 value);
void player_mode_set(s32 player, s32 value);
void controller_poll(void);
void osCreateMesgQueue(OSMesgQueue *mq, OSMesg *msg, s32 count);
s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flag);

void func_800E7038(void)
{
    static s8 D_80111968 = 0;
    s32 i;

    if (D_80111954 == 0) {
        player_state_set(-1, 1);
        player_mode_set(-1, 1);
    }
    D_80111954 = 1;
    for (i = 0; i < 4; i++) {
         D_80149AF8[i][1] = 70; D_80149AF8[i][0] = 70; 
    }
    
    if (D_80111968 == 0) {
        D_80111968 = 1;
        osCreateMesgQueue(&D_801497A8, D_80152730, 1);
        
        osSendMesg(&D_801497A8, 0, 0);
        
    }
    while (*(volatile u8 *)&D_80111950 == 0x80) {
    }
    controller_poll();
}
