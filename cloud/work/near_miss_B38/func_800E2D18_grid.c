/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed char s8;
typedef signed short s16;
typedef int s32;
typedef float f32;
typedef struct Scale {char pad[8];f32 multiplier;} Scale;
typedef struct Context {s32 word0;Scale *scale;char pad[3];s8 column;s8 row;} Context;
extern f32 D_801110C4[];
s16 func_800E2D18(Context *context,s16 x,s16 y,s16 (*grid)[12]) {
    s16 column=x/1150;
    s16 xr=x%1150;
    s16 row=y/14;
    s16 yr=y%14;
    s16 low,high,value;
    if(column<0) {column=0;xr=0;}
    if(column>=11) {column=10;xr=1149;}
    if(row>=9) {row=8;yr=13;}
    low=grid[row][column]+(grid[row][column+1]-grid[row][column])*xr/1149;
    high=grid[row+1][column]+(grid[row+1][column+1]-grid[row+1][column])*xr/1149;
    value=low+(high-low)*yr/14;
    return (s16)((f32)value*(context->scale->multiplier*D_801110C4[context->row*3+context->column]));
}
