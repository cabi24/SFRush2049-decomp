/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef int s32;
typedef unsigned char u8;
void struct_fields_init(void *head, void *pool, s32 size, s32 count, s32 flags);
extern u8 D_80395EE8[];
extern u8 D_80395F00[];
extern u8 D_80396B80[];
extern u8 D_80396B98[];

void func_8038D1A8(void)
{
    struct_fields_init(D_80395EE8, D_80395F00, 64, 50, 0);
    struct_fields_init(D_80396B80, D_80396B98, 28, 150, 0);
}
