/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Complete native behavior reconstruction, not an original-source claim.
 * D_80113E8C is a word view of a 96-byte-stride record field. Its complete
 * enclosing record declaration and number of records are not established.
 */
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
extern s16 D_80151AD0;
extern s32 D_80113E8C[];
extern s16 D_8014A10A;
extern u32 D_801174B4;
extern unsigned char D_801461D0[];
extern s32 osRecvMesg(void *, void *, s32);
extern s32 osJamMesg(void *, void *, s32);
extern s32 slot_state_setup(s32);

void func_800D9058(void)
{
    s32 height;
    s32 count;
    s32 index;
    s32 enabled;

    height = D_80113E8C[D_80151AD0 * 24];
    osRecvMesg(D_801461D0, 0, 1);
    slot_state_setup(11);
    osJamMesg(D_801461D0, 0, 0);
    D_8014A10A = (s32)((u32)height - 24U) / 16;
    count = 2;
    for (index = 0; index < 12; index++) {
        enabled = 1;
        if (index == 8) {
            enabled = 0;
        }
        if (enabled) {
            count++;
        }
    }
    if (count < D_8014A10A || (D_801174B4 & 0x7C03FFFEU)) {
        D_8014A10A = count;
    }
}
