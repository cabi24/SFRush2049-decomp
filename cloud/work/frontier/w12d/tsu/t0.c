typedef signed char s8;
typedef unsigned char u8;
typedef unsigned short u16;
typedef int s32;

extern u8 D_80140BDC;
extern s8 D_8013FECC;
extern s8 D_8013FECD;
extern u16 D_801427C0[];
extern char *D_8011B02C[];
extern char *D_8011AFDC[];
extern char *D_8011AFFC[];
extern char *D_8011B118[];
extern s32 string_copy_format(char *, s8, s8, s8);
extern void engine_sound_update(void);

void tire_sound_update(void)
{
    s32 i;

    for (i = 157; i < 216; i++) {
        D_801427C0[i] = string_copy_format(D_8011B02C[i - 157], 0, (s8)(D_80140BDC - 1), 0);
    }
    for (; i < 224; i++) {
        D_801427C0[i] = string_copy_format(D_8011AFDC[i - 216], 0, (s8)(D_80140BDC - 1), 0);
    }
    for (; i < 236; i++) {
        D_801427C0[i] = string_copy_format(D_8011AFFC[i - 224], 0, (s8)(D_80140BDC - 1), 0);
    }
    if (D_8013FECC || D_8013FECD) {
        D_801427C0[i] = string_copy_format("MINEG1", 0, (s8)(D_80140BDC - 1), 0);
        i++;
    }
    for (; i < 386; i++) {
        D_801427C0[i] = string_copy_format(D_8011B118[i - 236], 0, (s8)(D_80140BDC - 1), 0);
    }
    engine_sound_update();
}
