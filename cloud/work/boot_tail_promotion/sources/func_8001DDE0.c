/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Shared native contract adaptation; compile with -Iinclude. */
#include "boot_tail_sample_contract.h"
/* Source lead: AxioDL/musyx snd3d.c s3dHandle, CC0-1.0, revision
 * 78d2e16e4905fc675952162d331c24d5198b2687. Native-specific interfaces below.
 * The five real output locals intentionally have no invented defaults.
 * Defined-path semantics require every consumed value to have been written.
 */
extern StateNode *D_8004FD50;
extern const float D_8002D90C;
extern void func_8001D928(void);
extern void func_8001D084(StateNode *);
extern void func_8001C860(StateNode *, float *, float *, float *, float *, float *);
extern int func_8001DA74(StateNode *, float, float, float, float, float);
extern unsigned int func_8001B1D0(unsigned short, unsigned char, unsigned char);
extern unsigned int func_800201D0(unsigned int);
extern void func_8001D944(StateNode *, float);
extern int func_8001B8C4(unsigned int);
extern void func_8001CCDC(StateNode *, float, float, float, float, float);
extern void func_8001DC08(void);

void func_8001DDE0(void)
{
    StateNode *emitter;
    StateNode *next;
    float volume;
    float x_pan;
    float y_pan;
    float z_pan;
    float pitch;

    func_8001D928();
    emitter = D_8004FD50;
    for (; emitter != 0; emitter = next) {
        next = emitter->next;
        if (emitter->flags08 & 0x40000) {
            func_8001D084(emitter);
            continue;
        }
        if (emitter->flags08 & 0x20001) {
            func_8001C860(emitter, &volume, &pitch, &x_pan, &y_pan, &z_pan);
        }
        if (!(emitter->flags08 & 0x80000)) {
            if (emitter->flags08 & 0x20000) {
                if (volume == 0.0f && (emitter->flags08 & 4)) {
                    emitter->flags08 |= 0x80000;
                    emitter->flags08 &= ~0x20000;
                    goto found_emitter;
                }
                if (emitter->flags08 & 1) {
                    if (func_8001DA74(emitter, volume, x_pan, y_pan, z_pan, pitch)) {
                        continue;
                    }
                } else {
                    if ((emitter->identifier34 = func_8001B1D0(emitter->sound_id, 127, 64)) == 0xFFFFFFFFU) {
                        if (!(emitter->flags08 & 2)) {
                            emitter->flags08 |= 0x40000;
                            emitter->flags08 &= ~0x20000;
                        } else {
                            continue;
                        }
                    }
                }
            } else if ((emitter->identifier34 = func_800201D0(emitter->identifier34)) == 0xFFFFFFFFU) {
                if (emitter->flags08 & 2) {
                    emitter->flags08 |= 0x20000;
                } else {
                    emitter->flags08 |= 0x40000;
                }
            }
found_emitter:
            if (emitter->identifier34 != 0xFFFFFFFFU) {
                if (emitter->flags08 & 1) {
                    func_8001D944(emitter, volume);
                }
                if (volume == 0.0f && (emitter->flags08 & 4)) {
                    func_8001B8C4(emitter->identifier34);
                    emitter->identifier34 = 0xFFFFFFFFU;
                    if (emitter->flags08 & 2) {
                        emitter->flags08 |= 0x80000;
                    } else {
                        emitter->flags08 |= 0x40000;
                    }
                } else {
                    func_8001CCDC(emitter, volume, x_pan, y_pan, z_pan, pitch);
                }
            }
            if (emitter->flags08 & 0x100000) {
                emitter->fade += D_8002D90C;
                if (emitter->fade >= 1.0f) {
                    emitter->flags08 &= ~0x100000;
                }
            }
        } else if (volume != 0.0f) {
            emitter->flags08 &= ~0x80000;
            emitter->flags08 |= 0x20000;
        }
    }
    func_8001DC08();
}
