/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Image B: release the HUD resource and each player's active scene handles.
 * Reconstructed from the protected native body; no known arcade donor.
 * Field names are descriptive; unknown fields preserve real record storage.
 * NONMATCH research: four of 95 complete native words differ (branch operand
 * order twice, loop endpoint register twice). No matching/ROM credit.
 * The first record pointer is initialized only inside the signed-positive
 * player-count guard. This is native-consistent lifetime reconstruction and
 * removes one live block from its allocation priority (six blocks to five).
 * Next-handle carriers are consumed after the preceding state store; each
 * is loaded after the preceding release call, preserving helper mutations.
 * The live player count is a separate consumed signed carrier. Inner-counter
 * initialization precedes the effect pointer. Three independent initializers
 * share a source line to recover stock assembler scheduling. All locals are
 * used. Original declaration names/order and source spelling are unknown.
 * No padding, changed array capacity, added volatility, fake caller/formal,
 * compiler force or inline assembly. Valid bounds/helper limits are in RESULTS.
 * sound_call_minimal is the real accepted s16 O32 wrapper; sound_stop receives
 * the resource pointer. Historical names do not imply audio semantics.
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
    s32 i, count;
    s16 handle;

    if (D_80395ED4 != 0) {
        sound_stop(D_80395ED4);
        D_80395ED4 = 0;
    }
    if (D_80151AD0 > 0) {
        first = D_80395C00;
        second = D_80395CD0; third = D_80395DA0; group = D_803950C0;
        do {
            if (first->handle != -1) {
                sound_call_minimal(first->handle);
                first->handle = -1;
            }
            handle = second->handle;
            first->state = 8;
            if (handle != -1) {
                sound_call_minimal(handle);
                second->handle = -1;
            }
            handle = third->handle;
            second->state = 0;
            if (handle != -1) {
                sound_call_minimal(handle);
                third->handle = -1;
            }
            i = 0;
            effect = group->effect;
            for (; i < 10; i++, effect++) {
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
            count = D_80151AD0;
        } while (group < &D_803950C0[count]);
    }
}
