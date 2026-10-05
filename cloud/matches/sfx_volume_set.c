/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * sfx_volume_set: look up the three per-car model parts for one car slot.  For each part suffix
 * D_8011B42C[j] ("FRAME1", "HOOD", "SHEEN") it formats "<car prefix><suffix>" with the prefix
 * D_80110D3C[car] ("CAR1", ...) and stores the object index from string_copy_format
 * (the N64 MBOX_FindObject_Sub(name, lo, hi, err), here with lo = hi = model and MBOX_WARN)
 * into D_801427C0[slot * 3 + j].  The historical name is not the semantics.
 * Arcade ancestor: the car-part loop of InitDynamicObjs (game/visuals.c):
 *   sprintf(partName, "TEMP%s", ObjNames[k]);
 *   gObjList[k++] = MBOX_FindObject_Sub(partName, gCarModelNums[i], gCarModelNums[i], MBOX_FATAL);
 * Shaping (all compile-affecting): the arcade declaration list (`i` unused, `char partName[20]`,
 * which also sets the 104-byte frame); `k` declared after the buffer; `++j, ++k` in the for-header
 * (k++ in the subscript lets as1 fill the delay slot with the pointer increment instead of the store);
 * `(s32)model` arguments (an int web for the model, narrowed at each call as retail does).
 * The format string "%s%s" lives in this module's .data at 0x801233B8 and is referenced by name.
 * Also matches at -O2.
 */
typedef signed short s16;
typedef unsigned short u16;
typedef signed char s8;
typedef signed int s32;

#define MBOX_WARN 1
#define CAR_PARTS 3

extern char *D_80110D3C[];
extern char *D_8011B42C[CAR_PARTS];
extern char D_801233B8[]; /* "%s%s" */
extern u16 D_801427C0[];
extern int sprintf(char *, const char *, ...);
extern u16 string_copy_format(char *name, s8 lo, s8 hi, s32 err);

void sfx_volume_set(s16 slot, s16 car, s8 model) {
    s32 i, j;
    char partName[20];
    s32 k;

    k = slot * CAR_PARTS;
    for (j = 0; j < CAR_PARTS; ++j, ++k) {
        sprintf(partName, D_801233B8, D_80110D3C[car], D_8011B42C[j]);
        D_801427C0[k] = string_copy_format(partName, (s32)model, (s32)model, MBOX_WARN);
    }
}
