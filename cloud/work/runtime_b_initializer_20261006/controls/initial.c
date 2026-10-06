/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Runtime image B: initialize four pools and resolve model/texture names.
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
extern volatile u8 D_80140BDC;
extern s16 D_80394F88;
extern u8 D_8039A520[], D_80399B60[], D_8039A5F8[], D_8039A538[];
extern u8 D_80394F70[], D_8039AE98[], D_8039AE80[], D_8039A610[];
extern char *D_803942EC[10], *D_80394314[7];
extern char *D_80394330[5], *D_80394344[5];
extern s32 D_80399B18[10], D_80399B40[7];
extern s16 D_80399AF8[5], D_80399B08[5];
extern s32 D_80394F90, D_80394FC4, D_80394FF8, D_8039502C;
void func_803908D0(void)
{
    char **name;
    s32 *model;
    s16 *texture;
    D_80394F88 = 0;
    struct_fields_init(D_8039A520, D_80399B60, 104, 24, 0);
    struct_fields_init(D_8039A5F8, D_8039A538, 8, 24, 0);
    struct_fields_init(D_80394F70, D_8039AE98, 60, 24, 0);
    struct_fields_init(D_8039AE80, D_8039A610, 72, 30, 0);
    name = D_803942EC;
    for (model = D_80399B18; model < D_80399B18 + 10; model++) {
        *model = string_copy_format(*name++, 0, D_80140BDC - 1, 0);
    }
    name = D_80394314;
    for (model = D_80399B40; model < D_80399B40 + 7; model++) {
        *model = string_copy_format(*name++, 0, D_80140BDC - 1, 0);
    }
    name = D_80394330;
    for (texture = D_80399AF8; texture < D_80399AF8 + 5; texture++) {
        func_800B24EC(*name++, texture, 0, D_80140BDC - 1, 1);
    }
    name = D_80394344;
    for (texture = D_80399B08; texture < D_80399B08 + 5; texture++) {
        func_800B24EC(*name++, texture, 0, D_80140BDC - 1, 1);
    }
    D_80394F90 = -1;
    D_80394FC4 = -1;
    D_80394FF8 = -1;
    D_8039502C = -1;
    func_8038D1A8();
}
