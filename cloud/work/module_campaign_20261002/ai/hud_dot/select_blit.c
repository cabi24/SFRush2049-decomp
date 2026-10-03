/* IDO flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct { u8 other[16]; u16 count; } DataCount;
typedef struct {
    void *data; s32 word4; DataCount *buffer; u16 half12; s16 x,y; u16 half18;
    s16 width,height; u8 status,flag; s8 disabled; u8 other27;
    s16 left,bottom,right,top; u8 other36[4]; s32 active; u32 packed;
} PadConfig;
extern void Input_ApplyPadConfig(PadConfig *);
extern void func_800EF5B0(PadConfig *,void *,s32);
void stat_race_update(PadConfig *pad,s32 index,s32 step,s32 span)
{
    s32 rows,col,page;
    DataCount *buffer;
    if(pad==0) return;
    buffer=pad->buffer;
    if(buffer==0 || buffer->count==0) {
        func_800EF5B0(pad,pad->data,1);
        buffer=pad->buffer;
        if(buffer==0) return;
    }
    rows=buffer->count/step;
    if(rows==0) rows=1;
    col=index%rows;
    page=index/rows;
    rows-=col;
    pad->left=page*span;
    pad->bottom=pad->left+span-1;
    if(pad->flag) {
        pad->right=pad->buffer->count%step + (rows-1)*step;
        pad->top=pad->right+step-1;
    } else {
        pad->right=col*step;
        pad->top=pad->right+step-1;
    }
    Input_ApplyPadConfig(pad);
}
