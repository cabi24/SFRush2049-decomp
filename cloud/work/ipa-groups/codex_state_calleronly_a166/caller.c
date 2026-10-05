/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef short s16;typedef unsigned int u32;
extern s8 D_8011414C,D_80114154,D_80156994,D_80157244;
extern s16 D_80114158;
extern void *D_80114150;
extern int D_80138660,state_word_b,D_801174BC;
extern u32 D_8015694C;
extern char D_80114128[];
void *sound_control(s16,s16,void *,s16);
void sound_stop(void *);
void viScheduleTick(float);
int viDeadlinePassed(void);
void entity_audio_update(void);
void debug_stats(int);
void debug_collision(void)
{
    int selected;
    if (!D_8011414C) {
        D_80114150=sound_control(0,0,D_80114128,1);
        viScheduleTick(30.0f);
        D_80114158++;
        D_80138660=0;
        if (D_80114158>=6) {
            D_80138660=1;
            D_80114158=0;
        }
        if (!D_80156994 && D_80114158>=5) {
            D_80138660=1;
            D_80114158=0;
        }
        D_8011414C=1;
        entity_audio_update();
    }
    debug_stats(1);
    if (D_80157244 && D_80114154) {
        if (D_80114150) {
            sound_stop(D_80114150);
            D_80114150=0;
        }
        debug_stats(0);
        D_8011414C=0;
        state_word_b=4;
    } else if (((D_8015694C & 7) && D_80114154) || viDeadlinePassed()) {
        selected=(D_8015694C & 1)!=0;
        if (D_80114150) {
            sound_stop(D_80114150);
            D_80114150=0;
        }
        debug_stats(0);
        D_8011414C=0;
        if (D_80114158==5 && viDeadlinePassed()) {
            state_word_b=2;
        } else {
            state_word_b=4;
            if (selected) D_801174BC=16;
        }
    }
}
