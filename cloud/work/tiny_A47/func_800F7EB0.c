/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef signed short s16;typedef float f32;
extern s16 D_8014A108;extern f32 D_80144DA8[];extern s8 D_80144018[];extern f32 D_80124618;extern s8 D_80151AC0[15];
void func_800F7EB0(void) {
 f32 *value,*end; s8 *flag,*record;
 s16 count=D_8014A108;
 if(count>0) {
  value=D_80144DA8;flag=D_80144018;end=value+count;
  do {
   *flag++=0;*value++=D_80124618;
  }while(value<end);
 }
 for(record=D_80151AC0;record!=D_80151AC0+15;record+=5) {
  int j;record[0]=-1;
  for(j=1;j<5;j++)record[j]=-1;
 }
}
