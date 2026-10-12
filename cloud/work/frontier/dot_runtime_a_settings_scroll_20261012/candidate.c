/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Image A scrolling settings renderer: complete NONMATCH research, 58/192
 * words differ. The first source had 128/192 differing words. Unsigned label
 * offset arithmetic recovers the full 192-word geometry, but original types
 * are not established. Stock IDO still rematerializes the local buffer
 * address at the formatting call instead of using the native carried pointer;
 * the following temporary register ring is one position behind retail.
 * The frame is 128 bytes rather than 136; no padding locals were added.
 *
 * This callback's incoming context is unused. The 24-byte text buffer is an
 * inferred capacity, unchanged during diagnosis. Its only write is the
 * authenticated "%d" format of a signed byte, at most five bytes with NUL.
 * Language/table indices must be valid and the selected setting lies in
 * 0..20. Named fields are partial views; original names/arcade ancestry and
 * complete record layouts are unknown. No private ABI, helper implementation,
 * production installation, image or ROM match is claimed.
 */
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef int s32; typedef unsigned int u32;
typedef float f32;
typedef struct Language {
    s32 unknown0; char **text; s32 unknown8; u16 *indices; char **indexed;
    s32 unknown14, unknown18; u16 *setting_indices; u16 *setting_labels;
    char **setting_text;
} Language;
extern Language D_8017A4E0;
extern s32 D_803B3094;
extern s8 D_801613AB;
extern s8 D_8011648C[];
extern s16 D_801164A4[];
extern char D_803B8668[];
extern void render_helper(f32);
extern void *object_create(s32);
extern void dispatch_handler(s32);
extern s16 sound_pitch_diff_halved(void *,s16);
extern s16 sound_pitch_diff_calc(void *,s16);
extern s16 object_type_byte2_get(void);
extern s16 object_helper(void);
extern void state_utility(s16,s16,void *);
extern void fcvt_wrapper(char *,const char *,...);

s32 func_803A5358(void *context)
{
    char text[24];
    s32 i, option, y;
    char *label, *value;
    s16 x;
    render_helper(0.0f);
    object_create(10);
    dispatch_handler(12);
    x = sound_pitch_diff_halved(D_8017A4E0.text[54],151);
    state_utility(x,6+(18-object_type_byte2_get())/2,D_8017A4E0.text[54]);
    object_create(12);
    y=47;
    for (i=0;i<13;i++) {
        option=i+D_803B3094-6;
        if (option<0) option+=21;
        else if (option>=21) option-=21;
        value=text;
        if (option==18) {
            fcvt_wrapper(value,D_803B8668,D_801613AB);
        } else {
            value=D_8017A4E0.setting_text[(u32)D_8017A4E0.setting_labels[D_8017A4E0.setting_indices[0]+option]+D_801164A4[option]+1];
        }
        label=D_8017A4E0.setting_text[(u32)D_8017A4E0.setting_labels[D_8017A4E0.setting_indices[0]+option]];
        object_create(option==D_803B3094 ? 11:12);
        dispatch_handler(option==D_803B3094 ? 22:1);
        state_utility(sound_pitch_diff_calc(label,150),y-(option==D_803B3094 ? 1:0),label);
        object_create(D_8011648C[option] && option==D_803B3094 ? 11:12);
        dispatch_handler(D_8011648C[option] ? (option==D_803B3094 ? 11:1):4);
        state_utility(166,y-(D_8011648C[option] && option==D_803B3094 ? 1:0),value);
        y+=object_helper();
    }
    render_helper(-1.0f);
    return 1;
}
