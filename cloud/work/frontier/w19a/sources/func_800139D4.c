/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_800139D4 - lib_11640 (IDO 7.1). Audio frame: for each of the D_8003829C voice records
 * (0x68 bytes, D_80038294) collect the "changed" flag (+0x61) into a bit mask (bit = +0x60) and
 * clear it, and render every active voice (func_80012D18, SAMPLES, running active index); then
 * finish the current command block D_800382D0[*D_800382D4] (0x810-byte blocks: count, sample,
 * changed mask, work buffer D_800382E4, loop D_800382E8, 32 0x58-byte commands): an empty block
 * gets one command with status 0, else the last command's output is OUTPUT; with a loop, record
 * the loop position (D_800382EC / 192) and advance it by 192 frames, wrapping at the loop length;
 * then step *D_800382D4 and, while below D_800382CE, open the next block (D_80038034 = its
 * commands rounded down to 16, D_80038030 = its commands, count 0).
 * Caller: func_80013D70 (lib_11640, locked).
 * Types: the TU's declarations as they are: boot_tail_audio_record.h AudioState (+0x60
 * unknown60, +0x61 release_pending), `unsigned short *` D_800382D0 / D_800382D4, `void *`
 * D_800382E4 / D_800382E8, `unsigned int` D_80038030 / D_80038034 (func_800119E0, func_80011074,
 * func_80013964 blocks). The block layout is a private view (AudioBlock/AudioLoop/AudioCommand,
 * D10's) reached by casting D_800382D0 / D_800382E8 at each use (type views, no code; the
 * casts do not change the code: LOCK identical with D10's typed externs too).
 * w19a: from D10 BT02-audio-final (5.3; 7.1 true=60): `commands` instead of `&commands[0]`
 * (addu operand order) and the loop length `unsigned int` (compare operand order; the
 * alternative `(D_800382EC += 192) == length` with an int length also matches).
 * No shaping devices, no unused locals. */
#include "boot_tail_audio_record.h"
typedef struct AudioLoop {
    unsigned int unknown00;
    unsigned int length;
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
extern unsigned short D_8003829C;
extern unsigned short *D_800382D0;
extern unsigned short *D_800382D4;
extern unsigned short D_800382CE;
extern void *D_800382E4;
extern void *D_800382E8;
extern int D_800382EC;
extern unsigned int D_80038030;
extern unsigned int D_80038034;
extern void func_80012D18(AudioState *, unsigned short, unsigned char);
void func_800139D4(void *output, unsigned short samples)
{
    unsigned short i;
    unsigned char active;
    unsigned int changed;
    active = 0;
    changed = 0;
    for (i = 0; i < D_8003829C; i++) {
        if (D_80038294[i].release_pending != 0) {
            D_80038294[i].release_pending = 0;
            changed |= 1U << D_80038294[i].unknown60;
        }
        if (D_80038294[i].active != 0) {
            func_80012D18(&D_80038294[i], samples, active);
            active++;
        }
    }
    if (((AudioBlock *)D_800382D0)[*D_800382D4].count == 0) {
        ((AudioBlock *)D_800382D0)[*D_800382D4].commands[0].status = 0;
        ((AudioBlock *)D_800382D0)[*D_800382D4].commands[0].output = output;
    } else {
        ((AudioBlock *)D_800382D0)[*D_800382D4].commands[((AudioBlock *)D_800382D0)[*D_800382D4].count - 1].output = output;
    }
    ((AudioBlock *)D_800382D0)[*D_800382D4].work = D_800382E4;
    ((AudioBlock *)D_800382D0)[*D_800382D4].changed = changed;
    ((AudioBlock *)D_800382D0)[*D_800382D4].loop = D_800382E8;
    if (((AudioBlock *)D_800382D0)[*D_800382D4].loop != 0) {
        ((AudioBlock *)D_800382D0)[*D_800382D4].sample = D_800382EC / 192;
        D_800382EC += 192;
        if (D_800382EC == ((AudioLoop *)D_800382E8)->length) {
            D_800382EC = 0;
        }
    }
    (*D_800382D4)++;
    if (*D_800382D4 < D_800382CE) {
        D_80038034 = (unsigned int)((AudioBlock *)D_800382D0)[*D_800382D4].commands & ~15U;
        D_80038030 = (unsigned int)((AudioBlock *)D_800382D0)[*D_800382D4].commands;
        ((AudioBlock *)D_800382D0)[*D_800382D4].count = 0;
    }
}
