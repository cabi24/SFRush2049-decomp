/* loadPathData (inlined) */
void func_8038CCEC(void)
{
    if (D_8014978C >= 0 && D_8014978C <= 5) {
        D_803B7CDC = audio_frame_sync(D_8014978C + 158, 0, 0, 0, 0);
        D_80153F20 = slot_value_get(D_803B7CDC);
        audio_dsp_process();
        audio_effect_apply();
    } else D_803B7CDC = -1;
}
