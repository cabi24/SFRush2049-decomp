/* Contract hooks for the unchanged caller; renderer internals are not run. */
#ifndef CANDIDATE
#define CANDIDATE "../../../matches/ovl_a/func_803A4134.c"
#endif
#include CANDIDATE
#include <string.h>
s16 D_8014A108;
s32 D_8014A110;
char D_803B85D8[1];
s8 D_803BA028[4];
float D_803BA190[4][4];
static int hook, used, *out, n;
static s32 callback(Blit *b) { (void)b; return 1; }
static void emit(int event, int arg, Blit *b) {
    int a[14]; int i;
    a[0]=event;a[1]=arg;a[2]=b->X;a[3]=b->Y;a[4]=b->Width;a[5]=b->Height;
    a[6]=b->Top;a[7]=b->Bot;a[8]=b->Left;a[9]=b->Right;a[10]=b->Hide;
    a[11]=b->AnimFunc!=0;a[12]=(s32)b->AnimID;a[13]=D_8014A108;
    for(i=0;i<14;i++) out[n++]=a[i];
}
void Input_ApplyPadConfig(Blit *b) { emit(3,0,b); }
void func_800EF5B0(Blit *b,char *name,s32 preserve) {
    emit(1,preserve,b);
    if(name!=D_803B85D8 || preserve!=0) __builtin_trap();
    b->Width=123;b->Height=-55;b->X=-21;b->Y=17;
    b->Top=18;b->Bot=19;b->Left=20;b->Right=21;
    b->AnimID=0x12345678U;
    if(hook==1) {D_8014A108=1;D_8014A110=0;}
}
s8 input_new_data_wrapper(Blit *b,s32 hide) {
    emit(2,hide,b);
    if(hide!=b->Hide) {
        b->Hide=hide;Input_ApplyPadConfig(b);
        if(!used) {
            used=1;
            if(hook==2) {b->Width=-21;b->Height=-17;b->AnimID=0x43210000U;}
            if(hook==3) b->Hide=0;
            if(hook==4) b->Hide=-1;
        }
    }
    return b->Hide;
}
int host_run(const int *c,int *result) {
    Blit b; int i,j,ret; unsigned int bits=c[10]; float ratio;
    memcpy(&ratio,&bits,4);memset(&b,0x5a,sizeof(b));
    D_8014A108=c[4];D_8014A110=c[5];
    for(i=0;i<4;i++) {D_803BA028[i]=c[6]+13*i;for(j=0;j<4;j++) D_803BA190[i][j]=ratio*(1.0f/(1U<<(4*i+j)));}
    b.X=-321;b.Y=654;b.Width=c[8];b.Height=c[9];b.Top=-30000;b.Bot=1234;b.Left=-5678;b.Right=30000;
    b.Hide=c[7];b.AnimFunc=callback;b.AnimID=((u32)c[0]<<31)|(c[1]<<8)|(c[2]<<4)|c[3]|(u32)c[12];
    out=result;n=0;hook=c[11];used=0;
    ret=func_803A4134(&b);emit(4,ret,&b);return n;
}
