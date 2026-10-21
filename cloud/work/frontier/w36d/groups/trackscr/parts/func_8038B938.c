/* uninitButtons (MAT3 trackscr.c, GC 0x8008cac4), inlined into _UninitScreen */
void func_8038B938(void)
{
    s32 buttonIdx;
    for (buttonIdx = 0; buttonIdx < 48; buttonIdx++) {
        if (D_803B6B14[buttonIdx].handle != -1) {
            if (D_801174B4 & 0x7C03FFFE) func_80090254((s16)D_803B6B14[buttonIdx].handle);
            D_803B6B14[buttonIdx].handle = -1;
        }
    }
    D_803B7714 = 0;
}
