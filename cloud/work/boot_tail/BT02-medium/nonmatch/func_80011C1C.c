/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef struct AudioState {
    unsigned char active;
    unsigned char enabled;
    unsigned short flags;
    unsigned short gain;
    unsigned char unknown06[0x12];
    unsigned short fields18[4];
    unsigned char unknown20[0x20];
    unsigned short fields40[4];
    unsigned char unknown48[0x20];
} AudioState;
extern AudioState *D_80038294;
void func_80011C1C(unsigned short index, unsigned short flags)
{
    AudioState *audio;
    audio = &D_80038294[index];
    audio->active = 0;
    audio->flags = flags;
    audio->gain = 4096;
    audio->enabled = 1;
    audio->fields18[0] = 0;
    audio->fields18[1] = 0;
    audio->fields18[2] = 0;
    audio->fields18[3] = 0;
    audio->fields40[0] = 0;
    audio->fields40[1] = 0;
    audio->fields40[2] = 0;
    audio->fields40[3] = 0;
}
