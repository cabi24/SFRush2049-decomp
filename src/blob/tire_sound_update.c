/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * tire_sound_update (historical label; 0x800B338C, 504 bytes): resolves the shared model/texture name
 * tables into ids for the current language and then instantiates the level objects.
 *   D_801427C0[157..215]  <- D_8011B02C[59]   ("HULKL1_LOD1", "GEOMEXPSPHERE01", ...)
 *   D_801427C0[216..223]  <- D_8011AFDC[8]    weapon models ("WEP_CANNG1", ...)
 *   D_801427C0[224..235]  <- D_8011AFFC[12]   wing parts ("WINGSWING1L", ...)
 *   one more "MINEG1" entry when D_8013FECC or D_8013FECD is set (the following indices shift by one)
 *   the rest up to [385]  <- D_8011B118[i - 236] (track props, "CONE1G1", ...)
 * then calls engine_sound_update (level-object instantiation, locked in frontier_level_objects).
 * One running index i across all loops (retail's s2 = 4*i); string_copy_format(name, 0, lang - 1, 0)
 * returns the id. Own .rodata: the "MINEG1" literal at 0x8012325C (verified by the scorer).
 * Single kept function in the whole-program unit; no shaping quirks.
 */
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
