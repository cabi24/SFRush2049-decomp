/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef unsigned char u8;typedef signed short s16;typedef unsigned short u16;typedef int s32;
extern u8 D_80151968[20][13];extern u16 D_80151A78[20];extern s32 D_80151690[12][3][5];
extern s32 func_8008AD04(u8 *,u8 *);extern u8 *func_800A473C(u8 *,u8 *);
s16 func_800F1D04(u8 *name) {
 s16 i,j,k,best,oldest;s16 references[20];s32 slot;
 for(i=0;i<20;i++)D_80151A78[i]++;
 for(i=0;i<20;i++)if(func_8008AD04(D_80151968[i],name)==0){D_80151A78[i]=0;return i;}
 for(i=0;i<20;i++)if(D_80151968[i][0]==0){func_800A473C(D_80151968[i],name);D_80151A78[i]=0;return i;}
 for(i=0;i<20;i++)references[i]=0;
 for(j=0;j<12;j++)for(k=0;k<3;k++)for(i=0;i<5;i++) {
  slot=D_80151690[j][k][i];if(slot>=0&&slot<20)references[slot]++;
 }
 for(i=0;i<20;i++)if(references[i]==0){func_800A473C(D_80151968[i],name);D_80151A78[i]=0;return i;}
 oldest=0;
 for(i=0;i<20;i++)if(oldest<D_80151A78[i]){oldest=D_80151A78[i];best=i;}
 for(j=0;j<12;j++)for(k=0;k<3;k++)for(i=0;i<5;i++)if(D_80151690[j][k][i]==best)D_80151690[j][k][i]=-1;
 func_800A473C(D_80151968[best],name);D_80151A78[best]=0;return best;
}

s32 func_8008AD04(u8 *arg0, u8 *arg1)
{
  int v;
  v = *arg0;
  if (v != 0 && v == *arg1)
  {
    do
    {
      v = arg0[1];
      arg0++;
      arg1++;
    } while (v != 0 && v == *arg1);
  }
  return v - *arg1;
}

u8 *func_800A473C(u8 *arg0, u8 *arg1) {
    u8 *r = arg0;
    while ((*arg0++ = *arg1++) != 0) {
    }
    return r;
}
