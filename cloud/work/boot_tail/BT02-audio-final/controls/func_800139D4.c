/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef struct AudioState {
    unsigned char active;
    unsigned char unknown01[0x5F];
    unsigned char bit_index;
    unsigned char changed;
    unsigned char unknown62[6];
} AudioState;
typedef struct AudioLoop {
    unsigned int unknown00;
    int length;
} AudioLoop;
typedef struct AudioCommand {
    unsigned char unknown00[44];
    unsigned short status;
    unsigned char unknown2E[22];
    void *output;
    unsigned char unknown48[8];
} AudioCommand;
typedef struct AudioBlock {
    unsigned short count;
    unsigned short sample;
    unsigned int changed;
    void *work;
    AudioLoop *loop;
    AudioCommand commands[32];
} AudioBlock;
extern AudioState *D_80038294;
extern unsigned short D_8003829C;
extern AudioBlock *D_800382D0;
extern unsigned short *D_800382D4;
extern unsigned short D_800382CE;
extern void *D_800382E4;
extern AudioLoop *D_800382E8;
extern int D_800382EC;
extern void *D_80038030;
extern void *D_80038034;
extern void func_80012D18(AudioState *, unsigned short, unsigned char);
void func_800139D4(void *output, unsigned short samples)
{
    unsigned short i;
    unsigned char active;
    unsigned int changed;
    active = 0;
    changed = 0;
    for (i = 0; i < D_8003829C; i++) {
        if (D_80038294[i].changed != 0) {
            D_80038294[i].changed = 0;
            changed |= 1U << D_80038294[i].bit_index;
        }
        if (D_80038294[i].active != 0) {
            func_80012D18(&D_80038294[i], samples, active);
            active++;
        }
    }
    if (D_800382D0[*D_800382D4].count == 0) {
        D_800382D0[*D_800382D4].commands[0].status = 0;
        D_800382D0[*D_800382D4].commands[0].output = output;
    } else {
        D_800382D0[*D_800382D4].commands[D_800382D0[*D_800382D4].count - 1].output = output;
    }
    D_800382D0[*D_800382D4].work = D_800382E4;
    D_800382D0[*D_800382D4].changed = changed;
    D_800382D0[*D_800382D4].loop = D_800382E8;
    if (D_800382D0[*D_800382D4].loop != 0) {
        D_800382D0[*D_800382D4].sample = D_800382EC / 192;
        D_800382EC += 192;
        if (D_800382EC == D_800382E8->length) {
            D_800382EC = 0;
        }
    }
    (*D_800382D4)++;
    if (*D_800382D4 < D_800382CE) {
        D_80038034 = (void *) (((unsigned int)&D_800382D0[*D_800382D4].commands[0]) & ~15U);
        D_80038030 = &D_800382D0[*D_800382D4].commands[0];
        D_800382D0[*D_800382D4].count = 0;
    }
}
