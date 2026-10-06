/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Runtime image B: initialize four pools and resolve model/texture names.
 * NONMATCH: six scheduling words differ under ordinary IDO O2 and O3.
 * N64-specific initialization. No whole-function arcade source is established.
 * Model and texture lookup interfaces are witnessed by accepted helper bodies.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef int s32;
typedef struct NameEntry NameEntry;
extern void struct_fields_init(void *, void *, s32, s32, u8);
extern s32 string_copy_format(char *, s8, s8, s8);
extern NameEntry *func_800B24EC(char *, s16 *, s8, s8, s32);
extern void func_8038D1A8(void);
/* Same qualified view as both accepted lookup implementations; noncausal here. */
extern volatile u8 D_80140BDC;
extern s16 D_80394F88;
extern u8 D_8039A520[], D_80399B60[], D_8039A5F8[], D_8039A538[];
extern u8 D_80394F70[], D_8039AE98[], D_8039AE80[], D_8039A610[];
extern char *D_803942EC[10], *D_80394314[7];
extern char *D_80394330[5], *D_80394344[5];
extern s32 D_80399B18[10], D_80399B40[7];
extern s16 D_80399AF8[5], D_80399B08[5];
typedef struct PlayerEffect { s32 handle; float transform[12]; } PlayerEffect;
extern PlayerEffect D_80394F90[4];
void func_803908D0(void)
{
    s32 *model;
    char **name;
    char **texture_name;
    s16 *texture;
    s32 i;
    D_80394F88 = 0;
    struct_fields_init(D_8039A520, D_80399B60, 104, 24, 0);
    struct_fields_init(D_8039A5F8, D_8039A538, 8, 24, 0);
    struct_fields_init(D_80394F70, D_8039AE98, 60, 24, 0);
    struct_fields_init(D_8039AE80, D_8039A610, 72, 30, 0);
    for (name = D_803942EC, model = D_80399B18; model < D_80399B18 + 10; model++, name++) {
        *model = string_copy_format(*name, 0, D_80140BDC - 1, 0);
    }
    for (name = D_80394314, model = D_80399B40; model < D_80399B40 + 7; model++, name++) {
        *model = string_copy_format(*name, 0, D_80140BDC - 1, 0);
    }
    for (texture_name = D_80394330, texture = D_80399AF8; texture < D_80399AF8 + 5; texture++, texture_name++) {
        func_800B24EC(*texture_name, texture, 0, D_80140BDC - 1, 1);
    }
    for (texture_name = D_80394344, texture = D_80399B08; texture < D_80399B08 + 5; texture++, texture_name++) {
        func_800B24EC(*texture_name, texture, 0, D_80140BDC - 1, 1);
    }
    for (i = 0; i < 4; i++) {
        D_80394F90[i].handle = -1;
    }
    func_8038D1A8();
}
