#include <string.h>
#ifndef CANDIDATE
#define CANDIDATE "../../../matches/ovl_a/func_803AE940.c"
#endif
#include CANDIDATE
s32 D_803B7714;
s16 D_803BA878, D_803BA898, D_803BA85A, D_803BA8B0[32];
s8 D_803BA908;
char *D_803B8294[16][2];
Point D_803B9224[16][4];
ResourceTables D_8017A4E0;
Texture D_80140BF0[16];
static ResourceIndices indices;
static char strings[128][8];
static char *pointers[128];
static Blit b;
static int *out, n, hook, updates;
static float fade;
static void emit(int event, int arg)
{
    int i;
    out[n++]=event; out[n++]=arg;
    out[n++]=b.X; out[n++]=b.Y; out[n++]=b.Width; out[n++]=b.Height;
    out[n++]=b.Alpha; out[n++]=b.Flip; out[n++]=b.Hide;
    out[n++]=(int)b.AnimID; out[n++]=b.Texture;
    out[n++]=D_803BA878; out[n++]=D_803BA898; out[n++]=D_803BA85A; out[n++]=D_803BA908;
    for(i=0;i<16;i++)out[n++]=D_80140BF0[i].flags;
}
static void effect(int stage)
{
    if(hook!=stage)return;
    b.X=-11; b.Y=17; b.Width=-31; b.Height=19; b.AnimID=0x12345678; b.Texture=7;
    D_803BA878=2; D_803BA898=12; D_803BA85A=3; D_803BA908=-1; indices.first=9;
}
s8 input_new_data_wrapper(Blit *p,s32 hide)
{
    emit(1,hide);
    if(p->Hide!=hide){p->Hide=hide; Input_ApplyPadConfig(p);}
    return p->Hide;
}
void Input_ApplyPadConfig(Blit *p)
{
    (void)p; emit(2,0);updates++;
    if(updates==1)effect(1);
}
static int string_id(char *p)
{
    int i;for(i=0;i<128;i++)if(p==strings[i])return i;return -999;
}
void func_800EF5B0(Blit *p,char *name,s32 mode)
{
    (void)p;if(mode!=1)out[n++]=-999;
    emit(3,string_id(name));effect(2);
}
float func_800BEA30(void){emit(4,0);effect(3);return fade;}
void func_800B42F0(s32 f){emit(5,f);effect(4);}
s32 func_800B3FA4(char *str,s32 arg)
{
    int id=string_id(str);
    if(arg!=-1)out[n++]=-999;
    emit(6,id);effect(5);return id*3-7;
}
int host_run(const int *c,int *result)
{
    int i,j,rv;u32 bits;
    out=result;n=0;hook=c[12];updates=0;
    memset(&b,0,sizeof(b));
    b.X=-321;b.Y=654;b.Width=c[9];b.Height=c[10];b.Alpha=77;b.Flip=2;b.Hide=c[13];
    b.AnimID=(u32)c[0]|(u32)c[1]<<4|(u32)c[2]<<8|(u32)c[3]<<12|0xABCD0000U;b.Texture=5;
    D_803B7714=c[4];D_803BA878=c[5];D_803BA898=c[6];D_803BA908=c[7];D_803BA85A=c[8];
    bits=(u32)c[11];memcpy(&fade,&bits,4);
    indices.unused=55;indices.first=2;D_8017A4E0.indices=&indices;D_8017A4E0.strings=pointers;
    for(i=0;i<128;i++)pointers[i]=strings[i];
    for(i=0;i<32;i++)D_803BA8B0[i]=i*2+1;
    for(i=0;i<16;i++){
        D_80140BF0[i].flags=i*11;
        for(j=0;j<2;j++)D_803B8294[i][j]=strings[i*2+j];
        for(j=0;j<4;j++){D_803B9224[i][j].x=i*100+j*23-150;D_803B9224[i][j].y=i*70+j*17-90;}
    }
    rv=func_803AE940(&b);emit(7,rv);return n;
}
