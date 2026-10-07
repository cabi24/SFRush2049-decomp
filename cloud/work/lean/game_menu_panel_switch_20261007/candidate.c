/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Already-submitted 76-byte helper, unclaimed context here. The actual consumed local voice snapshot
 * preserves the loaded pointer and the native out-of-line body in this real
 * two-caller context. No unused local or synthetic caller is present.
 */
typedef signed char s8;
typedef struct Voice Voice;
extern Voice *D_8011472C;
extern s8 D_80114728;
void func_800F857C(int);
void sound_handles_clear(int);
void sound_stop(Voice *);
void audio_update_d(void)
{
    Voice *voice;
    func_800F857C(0);
    sound_handles_clear(1);
    voice=D_8011472C;
    if (voice) {
        sound_stop(voice);
        D_8011472C=0;
    }
    D_80114728=0;
}

