/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Source lead: AxioDL/musyx snd3d.c s3dHandle, CC0-1.0, revision
 * 78d2e16e4905fc675952162d331c24d5198b2687. Native-specific interfaces below.
 * The five real output locals intentionally have no invented defaults.
 * Defined-path semantics require every consumed value to have been written.
 */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef struct Emitter {
    struct Emitter *next;
    struct Emitter *previous;
    u32 flags;
    u8 unknown0C[40];
    u32 identifier;
    u32 group;
    u16 sound_id;
    u16 counter;
    float fade;
} Emitter;
extern Emitter *D_8004FD50;
extern const float D_8002D90C;
extern void func_8001D928(void);
extern void func_8001D084(Emitter *);
extern void func_8001C860(Emitter *, float *, float *, float *, float *, float *);
extern int func_8001DA74(Emitter *, float, float, float, float, float);
extern u32 func_8001B1D0(u16, u8, u8);
extern u32 func_800201D0(u32);
extern void func_8001D944(Emitter *, float);
extern int func_8001B8C4(u32);
extern void func_8001CCDC(Emitter *, float, float, float, float, float);
extern void func_8001DC08(void);

void func_8001DDE0(void)
{
    Emitter *emitter;
    Emitter *next;
    float volume;
    float x_pan;
    float y_pan;
    float z_pan;
    float pitch;

    func_8001D928();
    emitter = D_8004FD50;
    for (; emitter != 0; emitter = next) {
        next = emitter->next;
        if (emitter->flags & 0x40000) {
            func_8001D084(emitter);
            continue;
        }
        if (emitter->flags & 0x20001) {
            func_8001C860(emitter, &volume, &pitch, &x_pan, &y_pan, &z_pan);
        }
        if (!(emitter->flags & 0x80000)) {
            if (emitter->flags & 0x20000) {
                if (volume == 0.0f && (emitter->flags & 4)) {
                    emitter->flags |= 0x80000;
                    emitter->flags &= ~0x20000;
                    goto found_emitter;
                }
                if (emitter->flags & 1) {
                    if (func_8001DA74(emitter, volume, x_pan, y_pan, z_pan, pitch)) {
                        continue;
                    }
                } else {
                    if ((emitter->identifier = func_8001B1D0(emitter->sound_id, 127, 64)) == 0xFFFFFFFFU) {
                        if (!(emitter->flags & 2)) {
                            emitter->flags |= 0x40000;
                            emitter->flags &= ~0x20000;
                        } else {
                            continue;
                        }
                    }
                }
            } else if ((emitter->identifier = func_800201D0(emitter->identifier)) == 0xFFFFFFFFU) {
                if (emitter->flags & 2) {
                    emitter->flags |= 0x20000;
                } else {
                    emitter->flags |= 0x40000;
                }
            }
found_emitter:
            if (emitter->identifier != 0xFFFFFFFFU) {
                if (emitter->flags & 1) {
                    func_8001D944(emitter, volume);
                }
                if (volume == 0.0f && (emitter->flags & 4)) {
                    func_8001B8C4(emitter->identifier);
                    emitter->identifier = 0xFFFFFFFFU;
                    if (emitter->flags & 2) {
                        emitter->flags |= 0x80000;
                    } else {
                        emitter->flags |= 0x40000;
                    }
                } else {
                    func_8001CCDC(emitter, volume, x_pan, y_pan, z_pan, pitch);
                }
            }
            if (emitter->flags & 0x100000) {
                emitter->fade += D_8002D90C;
                if (emitter->fade >= 1.0f) {
                    emitter->flags &= ~0x100000;
                }
            }
        } else if (volume != 0.0f) {
            emitter->flags &= ~0x80000;
            emitter->flags |= 0x20000;
        }
    }
    func_8001DC08();
}
