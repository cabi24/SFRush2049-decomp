/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef unsigned short u16;typedef int s32;typedef unsigned int u32;
extern u16 D_8011EAEC[256];
void func_800A150C(u8 *dest,u8 *src,u8 length) {
 s32 produced=0,index;u16 value;u16 *palette;
 if(*src==255) {
  src++;
  while(src[0] || src[1]) {
   value=(src[0]<<8)|src[1];src+=2;
   palette=D_8011EAEC;
   for(index=0;index<256;index++,palette++) {
    if(*palette==value) {*dest++=index;produced++;break;}
   }
   if(produced==length)break;
  }
 } else {
  while(*src) {
   value=*src;
   palette=D_8011EAEC;
   for(index=0;index<256;index++,palette++) {
    if(*palette==value) {*dest++=index;produced++;break;}
   }
   if(produced==length)break;
   src++;
  }
 }
 while(produced<length) {produced++;*dest++=0;}
}
