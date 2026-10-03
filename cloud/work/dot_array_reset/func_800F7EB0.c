/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/* NONMATCH: four of35 complete words differ; research only. */
extern short D_8014A108;
extern float D_80144DA8[];
extern unsigned char D_80144018[];
extern signed char D_80151AC0[3][5];
extern float D_80124618;
void func_800F7EB0(void)
{
 int i,j;
 for (i=0;i<D_8014A108;i++) {
  D_80144018[i]=0;
  D_80144DA8[i]=D_80124618;
 }
 for (i=0;i<3;i++) {
  for (j=0;j<5;j++) {
   D_80151AC0[i][j]=-1;
  }
 }
}
