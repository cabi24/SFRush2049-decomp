typedef struct { u8 pad0; u8 car; u8 pad2[74]; } Ent;
typedef struct { u8 pad[6]; s8 ok; u8 pad7[765]; } Car;
typedef struct { u8 pad[26]; s8 flag; } Ctx;
extern s16 D_8014A108;
extern Ent D_8014A118[];
extern Car D_80144030[];
void Input_ApplyPadConfig(Ctx *);
s32 audio_channel_reset(Ctx *c) {
    s32 flag = 1;
    s32 i;
    for (i = 0; i < D_8014A108; i++) {
        if (D_80144030[D_8014A118[i].car].ok == 0) {
            flag = 0;
        }
    }
    if (flag != c->flag) {
        c->flag = flag;
        Input_ApplyPadConfig(c);
    }
    return 1;
}
