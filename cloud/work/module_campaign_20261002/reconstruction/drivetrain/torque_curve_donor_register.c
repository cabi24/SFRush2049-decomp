/* flags: -g0 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/* Native adaptation of drivetra.c:enginetorque's bilinear curve lookup.
 * Native spacing is 1150 rpm. The real input formals are signed shorts.
 * Every register local below is consumed by the lookup; none adds storage. */
typedef float f32;
typedef signed short s16;
typedef signed char s8;
typedef unsigned char u8;
typedef struct Tuning {u8 prefix[8]; f32 multiplier;} Tuning;
typedef struct Model {u8 prefix[4]; Tuning *tuning; u8 gap8[3]; s8 column,row;} Model;
extern f32 D_801110C4[][3];
s16 func_800E2D18(Model *m,s16 rpm,s16 throttle,s16 *torquecurve)
{
    register s16 rindex,tindex,rrem,trem,left,right;
    register s16 *low_ptr,*hi_ptr;
    register int rpmperent=1150;
    rindex=rpm/rpmperent;
    rrem=rpm%rpmperent;
    if (rindex<0) {rindex=0;rrem=0;}
    if (rindex>=11) {rindex=10;rrem=1149;}
    tindex=throttle/(s16)14;
    trem=throttle%(s16)14;
    if (tindex>=9) {tindex=8;trem=13;}
    low_ptr=torquecurve+tindex*12+rindex;
    hi_ptr=torquecurve+tindex*12+rindex+1;
    left=*low_ptr+((*hi_ptr-*low_ptr)*rrem)/1149;
    low_ptr=torquecurve+(tindex+1)*12+rindex;
    hi_ptr=torquecurve+(tindex+1)*12+rindex+1;
    right=*low_ptr+((*hi_ptr-*low_ptr)*rrem)/1149;
    right=left+((right-left)*trem)/14;
    return (s16)((f32)right*(m->tuning->multiplier*D_801110C4[m->row][m->column]));
}
