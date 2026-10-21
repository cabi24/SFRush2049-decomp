/* _UninitScreen (MAT3 trackscr.c, GC 0x80088ca8); w36e body split into MAT3's inlined statics uninitButtons
 * (stub 0x8038B938) and clearTrackMaps (stub 0x8038B930). */
void func_8038B940(void)
{
    func_8038B938();
    if (D_803B7CD0) { sound_stop(D_803B7CD0); D_803B7CD0 = 0; }
    ambient_sounds_clear();
    func_8038B388();
    func_8038B930();
    func_800A5B3C();
    D_803B7CD4 = 0;
}
