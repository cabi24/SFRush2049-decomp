/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef int s32; typedef float f32;
typedef struct { s32 a; s8 b; s8 c; u8 pad[6]; } Rec12;
extern Rec12 D_803BA230[];
s32 func_800AC898(s32 handle);
void func_80096130(s32 handle);
void func_800FE4AC(s32 id);

void func_80390D2C(s16 id)
{
    s32 i;
    Rec12 *p;

    for (i = 0; i < 16; i++) {
        p = &D_803BA230[i];
        if (p->c == id) {
            while (func_800AC898(p->a) == 0) {
            }
            func_80096130(p->a);
            func_800FE4AC(id);
            p->a = -1;
            p->c = -1;
            p->b = 0;
            return;
        }
    }
}
