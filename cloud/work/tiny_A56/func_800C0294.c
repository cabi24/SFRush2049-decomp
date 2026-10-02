/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;typedef signed short s16;typedef unsigned short u16;typedef int s32;typedef unsigned int u32;typedef float f32;
typedef struct Record32 {u16 id;u8 pad2[20];u16 point;u8 pad24[8];} Record32;
typedef struct Packed8 {s16 x,y,z;u16 fraction;} Packed8;
extern Record32 *D_801525EC;extern u16 D_8015267C;extern Packed8 *D_8015201C;
u32 func_800C0294(u32 id,f32 *delta) {
 s32 quantized[3],j;f32 position[3],scaled;u32 i,result=0;Record32 *record=D_801525EC;Packed8 *point;
 for(i=0;i<D_8015267C;i++,record++) {
  if(record->id==id) {
   result++;point=&D_8015201C[record->point];
   position[0]=(f32)((s32)point->x*32+((point->fraction&0x7C00)>>10))*0.03125f;
   position[1]=(f32)((s32)point->y*32+((point->fraction&0x3E0)>>5))*0.03125f;
   position[2]=(f32)((s32)point->z*32+(point->fraction&0x1F))*0.03125f;
   position[0]=delta[0]+position[0];position[1]=delta[1]+position[1];position[2]=delta[2]+position[2];
   for(j=0;j<3;j++) {
    scaled=position[j]*32.0f;
    if(scaled<0.0f)quantized[j]=(s32)(scaled-0.5f);
    else quantized[j]=(s32)(scaled+0.5f);
   }
   point->x=quantized[0]>>5;point->y=quantized[1]>>5;point->z=quantized[2]>>5;
   point->fraction=((u32)quantized[2]&0x1F)|(((u32)quantized[0]<<10)&0x7C00)|(((u32)quantized[1]<<5)&0x3E0);
  }
 }
 return result;
}
