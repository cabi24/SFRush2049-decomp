/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;typedef signed short s16;typedef unsigned short u16;typedef int s32;typedef unsigned int u32;typedef float f32;
s32 func_800A1910(void *memory,u8 *right,s32 count) {
 if(count!=0) {
  u8 *left=memory;
  do {
   if(*left++!=*right++)return *--left-*--right;
  }while(--count!=0);
 }
 return 0;
}
