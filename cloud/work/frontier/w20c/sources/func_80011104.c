/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80011104 - lib_11640 (IDO 7.1): audio voice/record setup. w19a D10 source, w20c: declarations
 * aligned with the TU (boot_tail_audio_record.h's AudioState: bit_index = unknown60, changed =
 * release_pending; D_80038298 is the TU's `void *` (func_8001144C), read through an AudioState
 * cast; the two block buffers are the TU's two-cell D_800382D8 table, the second cell named
 * D_800382D8+4 by reloc_addrs as for func_8001144C). Callees by their symbol_addrs names
 * (bzero, osWritebackDCache). Type views only; no shaping device. */
#include "boot_tail_audio_record.h"
extern void *(*D_80038018)(unsigned int, unsigned int);
extern void *D_80038298;
extern unsigned short D_8003829C;
extern unsigned char D_8003829E;
extern unsigned char D_80038290;
extern float D_8002D8B8;
extern float D_8002D8BC;
extern unsigned short D_800382CE;
extern unsigned short *D_800382D8[2];
extern unsigned short D_800382E0[2];
extern void *D_800382E4;
extern void bzero(void *, int);
extern void osWritebackDCache(void *, int);
void func_80011104(unsigned short count, unsigned int frequency)
{
    unsigned short i;
    D_8003829C = count;
    D_80038298 = D_80038018(D_8003829C * sizeof(AudioState), 0);
    D_80038294 = D_80038298;
    D_8003829E = 0;
    for (i = 0; i < D_8003829C; i++) {
        ((AudioState *)D_80038298)[i].active = 0;
        ((AudioState *)D_80038298)[i].release_pending = 0;
        ((AudioState *)D_80038298)[i].unknown60 = 255;
    }
    if (D_80038290 == 0) {
        D_800382CE = (unsigned short) (((float)frequency / 50.0f / 192.0f) * D_8002D8B8) + 1;
    } else {
        D_800382CE = (unsigned short) (((float)frequency / 25.0f / 192.0f) * D_8002D8BC) + 1;
    }
    D_800382D8[0] = D_80038018(D_800382CE * 2576U, 0);
    D_800382D8[1] = D_80038018(D_800382CE * 2576U, 0);
    D_800382E0[1] = 0;
    D_800382E0[0] = 0;
    D_800382E4 = D_80038018(664, 0);
    bzero(D_800382E4, 664);
    osWritebackDCache(D_800382E4, 664);
}
