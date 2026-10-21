
typedef struct TextTables {
    u16 numSingle; u8 **single;
    u16 numTab, numTabA; u16 *tab; u8 **taba;
    u16 numTab2, numTab2a, numTab2b; u16 *tab2; u16 *tab2a; u8 **tab2b;
} TextTables;
extern TextTables D_8017A4E0;   /* gText */
extern char D_803B8668[];       /* "%d" */
extern void render_helper(f32);
extern void *object_create(s32);
extern void dispatch_handler(s32);
extern s16 sound_pitch_diff_halved(void *, s16);
extern s16 sound_pitch_diff_calc(void *, s16);
extern u8 object_type_byte2_get(void);
extern s16 object_bytes_sum_global(void);
extern void state_utility(s16, s16, void *);
extern void fcvt_wrapper(u8 *, const char *, ...);
void func_803A5350(s32 x, s32 y, s32 item, u8 *name, u8 *value);

s32 func_803A5358(Blit *blit)
{
    s32 x;
    s32 i;
    s32 y;
    s32 cheat;
    u8 *str;
    u8 str1[4];
    u8 *name;

    render_helper(0.0f);
    object_create(10);
    dispatch_handler(12);
    state_utility(sound_pitch_diff_halved(D_8017A4E0.single[54], 151), (18 - object_type_byte2_get()) / 2 + 6, D_8017A4E0.single[54]);
    object_create(12);
    x = 154;
    y = 47;
    for (i = 0; i < 13; i++) {
        cheat = i + D_803B3094 - 6;
        if (cheat < 0) {
            cheat += 21;
        } else if (cheat >= 21) {
            cheat -= 21;
        }
        if (cheat == 18) {
            fcvt_wrapper(str1, D_803B8668, D_801613AB);
            str = str1;
        } else {
            str = D_8017A4E0.tab2b[(u32)D_8017A4E0.tab2a[*D_8017A4E0.tab2 + cheat] + D_801164A4[cheat] + 1];
        }
        name = D_8017A4E0.tab2b[D_8017A4E0.tab2a[*D_8017A4E0.tab2 + cheat]];
        func_803A5350(x, y, cheat, name, str);
        y += object_bytes_sum_global();
    }
    render_helper(-1.0f);
    return 1;
}

void func_803A5350(s32 x, s32 y, s32 item, u8 *name, u8 *value)
{
    object_create(D_803B3094 == item ? 11 : 12);
    dispatch_handler(D_803B3094 == item ? 22 : 1);
    state_utility(sound_pitch_diff_calc(name, x - 4), y - (D_803B3094 == item ? 1 : 0), name);
    object_create((D_8011648C[item] && D_803B3094 == item) ? 11 : 12);
    dispatch_handler(D_8011648C[item] ? (D_803B3094 == item ? 11 : 1) : 4);
    state_utility(x + 12, y - ((D_8011648C[item] && D_803B3094 == item) ? 1 : 0), value);
}
