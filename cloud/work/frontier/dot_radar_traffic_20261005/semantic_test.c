/* Synthetic call contracts isolate this callback; they do not prove gameplay. */
#include <assert.h>
#include <string.h>
#ifndef CANDIDATE
#define CANDIDATE "candidate.c"
#endif
#include CANDIDATE
Car player_array[16];
Model D_8014A250[16];
s16 D_80151AD0,D_801543CA;
s32 state_word_a,D_8011617C,D_80116180;
s8 D_80156CE8,D_80140A04;
Pos D_80116028[4][4];
static Blit *current;
static const int *input;
static int calls[16],call_count,updates;
static s32 active(Blit *b) {(void)b;return 1;}
static void record(int event) {assert(call_count<16);calls[call_count++]=event;}
void Input_ApplyPadConfig(Blit *b)
{
    assert(b==current);
    record(1);++updates;
    if(updates==1 && input[15]!=99) b->Hide=input[15];
}
s32 func_800CF604(s16 slot)
{
    assert(slot==input[1]);record(2);return input[10];
}
void func_800A61B0(f32 *v,f32 *out,f32 *matrix)
{
    int i;
    record(3);
    assert(matrix==player_array[input[0]].dr_uvs[0]);
    for(i=0;i<3;i++) out[i]=(v[0]*matrix[i*3]+v[1]*matrix[i*3+1])+v[2]*matrix[i*3+2];
}
void stat_race_update(Blit *b,s32 index,s32 width,s32 height)
{
    assert(b==current);
    assert(index==(input[11]==2?input[1]+2:5));
    assert(width==(int)(input[7]*0.125f));assert(height==input[8]);
    record(100+index);
}
void run_case(const int *c,int *out)
{
    Blit blt,before;
    int i,j,result;
    input=c;
    memset(&blt,0,sizeof(blt));memset(player_array,0,sizeof(player_array));
    memset(D_8014A250,0,sizeof(D_8014A250));
    D_80151AD0=c[2];D_801543CA=c[3];state_word_a=c[4];D_80156CE8=c[5];
    D_80140A04=c[12];D_8011617C=c[13];D_80116180=c[14];
    for(i=0;i<16;i++) {
        D_8014A250[i].index=i+2;D_8014A250[i].in_game=c[9];
        D_8014A250[i].drone_type=c[11];player_array[i].mode=c[6];
        player_array[i].dr_uvs[0][0]=1.0f;player_array[i].dr_uvs[1][1]=1.0f;
        player_array[i].dr_uvs[2][2]=1.0f;
    }
    player_array[c[1]].dr_pos[0]=c[16];player_array[c[1]].dr_pos[1]=c[17];
    player_array[c[1]].dr_pos[2]=c[18];
    for(i=0;i<4;i++)for(j=0;j<4;j++) {
        D_80116028[i][j].x=(i+1)*31+j*17-40;
        D_80116028[i][j].y=(i+1)*29-j*11-20;
    }
    blt.AnimID=((u32)c[0]<<8)|c[1];blt.AnimFunc=active;
    blt.X=-321;blt.Y=654;blt.Width=c[7];blt.Height=c[8];blt.Hide=c[19];
    before=blt;current=&blt;call_count=updates=0;
    result=func_80108F40(&blt);
    out[0]=result;out[1]=blt.X;out[2]=blt.Y;out[3]=blt.Hide;
    out[4]=blt.AnimFunc!=0;out[5]=call_count;
    for(i=0;i<16;i++)out[i+6]=i<call_count?calls[i]:0;
    before.X=blt.X;before.Y=blt.Y;before.Hide=blt.Hide;before.AnimFunc=blt.AnimFunc;
    assert(memcmp(&before,&blt,sizeof(blt))==0);
}
