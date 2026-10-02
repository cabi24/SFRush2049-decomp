/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef unsigned short u16;typedef int s32;typedef unsigned int u32;
typedef struct Range {u8 kind;u8 pad1[9];u16 count;s16 (*points)[3];} Range;
typedef struct Mesh {u16 primary;u8 pad2[6];u8 count;u8 pad9[3];Range *ranges;} Mesh;
extern s16 D_801407D4[3],D_801407B4[3];extern Mesh D_801407F0;extern s16 (*D_801409E8)[3];extern u16 D_801527A4;
void func_800B9740(void) {
 s32 i,j,k,eligible;Range *range;s16 *point,*minimum,*maximum;
 D_801407D4[0]=D_801407D4[1]=D_801407D4[2]=32767;
 D_801407B4[0]=D_801407B4[1]=D_801407B4[2]=-32767;
 for(i=0;i<D_801527A4;i++) {
  eligible=0;
  if(i<D_801407F0.primary)eligible=1;
  else {
   point=D_801409E8[i];range=D_801407F0.ranges;
   for(j=0;j<D_801407F0.count;j++,range++) {
    if((u32)point>=(u32)range->points && (u32)point<(u32)(range->points+range->count) && range->kind==1)eligible=1;
   }
  }
  if(eligible==1) {
   point=D_801409E8[i];minimum=D_801407D4;maximum=D_801407B4;
   for(k=0;k<3;k++,point++,minimum++,maximum++) {
    *minimum=(*minimum<*point)?*minimum:*point;
    *maximum=(*point<*maximum)?*maximum:*point;
   }
  }
 }
}
