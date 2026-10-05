/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/* NONMATCH research: func_8010D85C, 368 native bytes, 12/92 words differ.
 * Names describe observed fields, not verified arcade ancestry.
 * D_80140BDC's volatile qualification follows the accepted directory lookup.
 * Reuse kind for the post-call slot read; callback mutation must remain visible.
 *
 * Caller-boundary caveat: retail passes a fifth stack value 1, while the
 * accepted sound_bank_load definition declares only four parameters. The
 * five-argument extern below records this caller's observed boundary. It is
 * NOT a compatible declaration for combining with that accepted definition.
 * No extra formal is added to the callee, and this file is not a group claim.
 * The native 88-byte frame remains unexplained; this candidate uses 64 bytes.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef int s32;
typedef struct ResourceState {
    u8 unknown00[4];
    u8 flags;
    u8 unknown05[9];
    s16 slot;
    u8 unknown10[74];
    s16 timer;
    u8 unknown5c[16];
    u8 *text;
} ResourceState;
typedef struct ResourceNode {
    s32 unknown00;
    s16 variant;
    u8 unknown06[6];
    ResourceState *state;
    float time;
    void (*callback)(void);
} ResourceNode;
typedef struct ResourceSlot { char *handle; u8 unknown04[64]; } ResourceSlot;
extern s32 D_801170FC;
extern volatile u8 D_80140BDC;
extern u8 D_80121DA0[], D_80121DA4[], D_80121DA8[];
extern char *D_80118E08[];
extern ResourceSlot D_8012E738[];
extern void entity_transform_apply(ResourceNode *, s32);
extern u8 *func_800A464C(u8 *, u8 *);
extern char *sound_bank_load(char *, u16 *, s8, s8, s32);
extern void audio_channel_setup(void);
void func_8010D85C(ResourceNode *node, s16 enabled)
{
    ResourceState *state;
    u8 *text;
    u8 *found;
    s32 kind;
    char *handle;
    s8 variant;
    u16 resource_id;
    if (!enabled) {
        entity_transform_apply(node, 1);
        return;
    }
    if (D_801170FC) return;
    state = node->state;
    if (!(state->flags & 1)) {
        text = state->text;
        state->timer = 10;
        if (func_800A464C(text, D_80121DA0)) kind = 2;
        else if (func_800A464C(text, D_80121DA4)) kind = 1;
        else kind = 0;
        handle = sound_bank_load(D_80118E08[kind], &resource_id, 0, (s8)(D_80140BDC - 1), 1);
        kind = state->slot;
        D_8012E738[kind].handle = handle;
        found = func_800A464C(state->text, D_80121DA8);
        if (found) variant = found[4] - '0';
        else variant = 0;
        node->variant = variant;
        state->flags |= 1;
    }
    node->callback = audio_channel_setup;
    node->time = 0.0f;
}
