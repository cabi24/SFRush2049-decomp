/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Image B: release the HUD resource and each player's active scene handles.
 * Reconstructed from the protected native body; no known arcade donor.
 * Field names are descriptive and padding is native record storage.
 */
typedef signed short s16;
typedef signed int s32;
typedef unsigned char u8;

typedef struct HudEffect {
    s16 state;
    s16 handle;
    u8 unknown04[0x44];
} HudEffect;
typedef struct HudEffectGroup { HudEffect effect[10]; } HudEffectGroup;
typedef struct HudStatus {
    s16 state;
    s16 handle;
    u8 unknown04[0x30];
} HudStatus;
typedef struct HudHandle {
    s16 handle;
    u8 unknown02[0x32];
} HudHandle;

extern void *D_80395ED4;
extern s16 D_80151AD0;
extern HudEffectGroup D_803950C0[];
extern HudStatus D_80395C00[];
extern HudStatus D_80395CD0[];
extern HudHandle D_80395DA0[];
extern void sound_stop(void *);
extern void sound_call_minimal(s16);

void func_8039244C(void)
{
    HudStatus *first;
    HudStatus *second;
    HudHandle *third;
    HudEffectGroup *group;
    HudEffect *effect;
    s32 i;

    if (D_80395ED4 != 0) {
        sound_stop(D_80395ED4);
        D_80395ED4 = 0;
    }
    first = D_80395C00;
    if (D_80151AD0 > 0) {
        second = D_80395CD0;
        third = D_80395DA0;
        group = D_803950C0;
        do {
            if (first->handle != -1) {
                sound_call_minimal(first->handle);
                first->handle = -1;
            }
            first->state = 8;
            if (second->handle != -1) {
                sound_call_minimal(second->handle);
                second->handle = -1;
            }
            second->state = 0;
            if (third->handle != -1) {
                sound_call_minimal(third->handle);
                third->handle = -1;
            }
            effect = group->effect;
            for (i = 0; i < 10; i++, effect++) {
                if (effect->handle != -1) {
                    sound_call_minimal(effect->handle);
                    effect->handle = -1;
                }
                effect->state = 11;
            }
            first++;
            second++;
            third++;
            group++;
        } while (group < &D_803950C0[D_80151AD0]);
    }
}
