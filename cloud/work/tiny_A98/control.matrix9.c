/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef signed short s16;
typedef float f32;
typedef struct Object {
    u8 other0[748];
    f32 basis[9];
    u8 other784[472];
    f32 wheel1256;
    u8 other1260[68];
    f32 wheel1328;
    u8 other1332[16];
    f32 multiplier1348;
    u8 other1352[68];
    f32 multiplier1420;
    u8 other1424[60];
    f32 state_a[4];
    u8 other1500[16];
    f32 state_b[4];
    u8 other1532[312];
    f32 state_c[9];
    s16 speed;u8 other1882[2];
    f32 saved_a[4];
    f32 saved_b[4];
    f32 saved_c[9];
    f32 saved_basis[9];
} Object;
extern f32 D_801241A4;
extern void math_utility(f32 *,f32 *);
void func_800D4DFC(Object *object)
{
    math_utility(object->basis,object->saved_basis);
    object->speed=(s16)(int)(((object->multiplier1348*object->multiplier1420+object->wheel1328*object->wheel1256)*0.5f)*D_801241A4);
    {
        int i;
        for(i=0;i<4;i++) {
            object->saved_a[i]=object->state_a[i];
            object->saved_b[i]=object->state_b[i];
        }
    }
    {
        int i;
        for(i=0;i<9;i++)object->saved_c[i]=object->state_c[i];
    }
}
