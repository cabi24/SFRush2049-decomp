/* Synthetic callee effects test the caller, not gameplay or callee execution. */
#include <assert.h>
#include <string.h>
#ifndef CANDIDATE
#define CANDIDATE "../../../matches/func_800EF62C.c"
#endif
#include CANDIDATE
Car2056 D_8014A250[4];
Object952 D_80152818[4];
s16 D_80151AD0, D_8014A108;
s8 D_8015B25C;
char D_80120D9C[] = "two";
char D_80120DAC[] = "many";
s32 D_80115B68[4][4][2];
static Blit *current;
static int updates, renamed;
static int active(Blit *blt) { (void)blt; return 1; }
void Input_ApplyPadConfig(Blit *blt) { assert(blt == current); ++updates; }
void func_800EF5B0(Blit *blt, const char *name, s32 preserve)
{
    assert(blt == current && preserve == 0);
    assert(name == D_80120D9C || name == D_80120DAC);
    renamed = name == D_80120D9C ? 2 : 3;
    blt->Name = name;
    blt->Width = blt->Height = -1;
    blt->Top = blt->Bot = blt->Left = blt->Right = -1;
    blt->Alpha = 0;
}
void run_case(const int *c, int *out)
{
    Blit blt;
    int i, j, result;
    memset(&blt, 0, sizeof(blt));
    memset(D_8014A250, 0, sizeof(D_8014A250));
    memset(D_80152818, 0, sizeof(D_80152818));
    D_80151AD0=c[2]; D_8014A108=c[3]; D_8015B25C=c[4];
    for (i=0; i<4; ++i) {
        D_8014A250[i].flag=c[5]; D_8014A250[i].index=i;
        D_8014A250[i].value=c[10]; D_80152818[i].mode=c[6];
        for (j=0; j<4; ++j) {
            D_80115B68[i][j][0]=(i+1)*101+j*17-220;
            D_80115B68[i][j][1]=(i+1)*43-j*31-90;
        }
    }
    blt.AnimID=((unsigned)c[0]<<4)|c[1]; blt.AnimFunc=active;
    blt.Hide=c[7]; blt.Width=c[8]; blt.Height=c[9];
    blt.X=-321; blt.Y=654; blt.Top=-30000; blt.Bot=1234;
    blt.Left=-5678; blt.Right=30000; blt.Alpha=17;
    current=&blt; updates=renamed=0;
    result=func_800EF62C(&blt);
    out[0]=result; out[1]=blt.X; out[2]=blt.Y;
    out[3]=blt.Width; out[4]=blt.Height;
    out[5]=blt.Top; out[6]=blt.Bot; out[7]=blt.Left; out[8]=blt.Right;
    out[9]=blt.Alpha; out[10]=blt.Hide; out[11]=blt.AnimFunc!=0;
    out[12]=renamed; out[13]=updates;
}
