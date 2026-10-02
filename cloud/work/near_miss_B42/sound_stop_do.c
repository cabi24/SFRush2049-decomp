/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned short u16;
typedef struct Voice {
    unsigned char prefix[0x34];
    u16 index;
    unsigned char gap[6];
    struct Voice *next;
} Voice;
typedef struct Config { unsigned char prefix[22]; signed char state; unsigned char tail[9]; } Config;
extern Voice *D_80149450[];
extern int D_80149788;
extern Config pad_config[];
void sound_stop(Voice *voice) {
    int count,index;
    if (!voice) return;
    do {
        for (index=0;index<D_80149788;index++) {
            if (D_80149450[index]==voice) break;
        }
        pad_config[voice->index].state=2;
        D_80149788--;
        count=D_80149788;
        D_80149450[index]=D_80149450[count];
        D_80149450[count]=voice;
        voice=voice->next;
    } while (voice);
}
