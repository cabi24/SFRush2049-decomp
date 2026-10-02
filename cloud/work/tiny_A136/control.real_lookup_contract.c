/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed short s16;
typedef unsigned char u8;
typedef int s32;
typedef struct Object68 {
    u8 prefix[22];
    s16 child;
    s16 sibling;
    u8 tail[42];
} Object68;
extern Object68 D_8012E700[];
extern s16 D_8015B254;
extern s16 func_8009002C(s16);
extern s16 func_8008FFD0(s16);
extern void render_mode_select(s16,s16);
void car_mass_set(s16 object,s16 parent)
{
    s16 previous=func_8009002C(object);
    if(previous==-1 || previous!=parent) {
        if(previous>=0) {
            D_8012E700[previous].child=D_8012E700[object].sibling;
        } else if(object==D_8015B254) {
            D_8015B254=D_8012E700[object].sibling;
        } else {
            previous=func_8008FFD0(object);
            if(previous>=0) {
                D_8012E700[previous].sibling=D_8012E700[object].sibling;
            }
        }
        D_8012E700[object].sibling=-1;
        render_mode_select(object,parent);
    }
}
