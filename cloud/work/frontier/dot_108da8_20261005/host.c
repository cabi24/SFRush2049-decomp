/* Host bridge: candidate behavior, genuine callback boundaries, no target bytes. */
#include <string.h>
#include "candidate.c"
Object952 D_80152818[6];
s16 D_80151AD0;
volatile s16 D_801543CA;
s8 D_80156CE8;
s32 state_word_a, D_8011617C, D_80116180;
s32 D_80116028[4][4][2];
char D_80120E34[1];
static int *result, count_events, count_updates, mutation;
static s32 callback(Blit *b) { return b != 0; }
static void event(Blit *b, int type) {
    int *p = result + 12 + count_events++ * 8;
    p[0]=type; p[1]=b->Hide; p[2]=b->X; p[3]=b->Y;
    p[4]=b->Width; p[5]=b->Height; p[6]=b->Alpha; p[7]=b->AnimFunc!=0;
}
void Input_ApplyPadConfig(Blit *b) {
    event(b,1);
    if (++count_updates == 1) {
        if (mutation==1 || mutation==4) b->Hide=0;
        if (mutation==2) b->Hide=-1;
        if (mutation==3 || mutation==4) {
            D_80151AD0=mutation==3?4:2; b->Width=-3000; b->Height=5000;
        }
    }
}
void func_800EF5B0(Blit *b, const char *name, s32 index) {
    if (name!=D_80120E34 || index!=0) __builtin_trap();
    event(b,2); b->Width=1234; b->Height=-2345;
}
void run_case(const int *in, int *out) {
    Blit b;
    int row,slot,axis;
    memset(&b,0,sizeof(b)); memset(out,0,36*sizeof(int));
    memset(D_80152818,0,sizeof(D_80152818));
    result=out; count_events=count_updates=0; mutation=in[7];
    b.AnimID=in[0]; b.AnimFunc=callback; b.Hide=in[6];
    b.X=-123; b.Y=234; b.Width=-300; b.Height=32767; b.Alpha=17;
    D_80151AD0=in[1]; state_word_a=in[2]; D_801543CA=in[3];
    D_80156CE8=in[4]; D_8011617C=777; D_80116180=-888;
    if (in[0]<6) D_80152818[in[0]].mode=in[5];
    for(row=0;row<4;row++) for(slot=0;slot<4;slot++) for(axis=0;axis<2;axis++)
        D_80116028[row][slot][axis]=10000*(row+1)+100*slot+7*axis-20000;
    out[0]=func_80108DA8(&b); out[1]=b.Hide; out[2]=b.AnimFunc!=0;
    out[3]=b.X; out[4]=b.Y; out[5]=b.Width; out[6]=b.Height; out[7]=b.Alpha;
    out[8]=D_8011617C; out[9]=D_80116180; out[10]=D_80151AD0; out[11]=count_events;
}
