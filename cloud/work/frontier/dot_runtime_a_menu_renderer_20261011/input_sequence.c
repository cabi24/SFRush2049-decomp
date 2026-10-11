/* NONMATCH research: complete input-sequence gate, image A 8038F648.
 * The 164-byte storage has a byte-count view and an aligned 41-word view;
 * held/pressed masks occupy words 1..20/21..40. The union avoids indexing
 * beyond one 20-word subarray. Alternate-member representation follows the
 * target IDO implementation; no portable serialization claim is made.
 * The existing elapsed-clock volatile contract is retained from accepted
 * frontier_mode_select and frontier_race_countdown_display sources.
 * No original arcade donor or exact source spelling is established.
 */
typedef unsigned char u8;
typedef unsigned int u32;
typedef int s32;
typedef union InputSequence {
    struct {
    u8 count;
    u8 unknown1[3];
    u32 held[20];
    u32 pressed[20];
    } fields;
    u32 words[41];
} InputSequence;
extern InputSequence *D_803B4278;
extern float D_803B427C;
extern s32 D_803B4280;
extern volatile float D_8002EB90;
extern u32 D_8015694C,D_80156944;
s32 func_8038F648(void)
{
    u32 held, pressed, other;
    u32 *step;
    if (!D_803B4278) return 0;
    if (D_803B4280 && D_803B427C <= D_8002EB90)
        D_803B4280 = 0;
    step = &D_803B4278->words[D_803B4280];
    pressed = step[21];
    held = step[1];
    other = ~(pressed | held);
    if (other & D_8015694C) D_803B4280 = 0;
    else if ((D_80156944 & held) == held &&
             (D_8015694C & pressed) == pressed &&
             !(other & D_80156944)) {
        D_803B427C = D_8002EB90 + 1.0f;
        D_803B4280++;
        if (D_803B4280 == D_803B4278->fields.count) return 1;
    }
    return 0;
}
