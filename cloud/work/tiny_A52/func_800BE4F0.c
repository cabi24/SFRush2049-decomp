/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
u8 *func_800BE4F0(u8 *destination,u8 *source) {
 u8 bytes[256];u8 *out=destination,*in=source,*temporary;
 if(destination[0]==255 || source[0]==255) {
  if(destination[0]==255) {
   out=destination+1;
   while(out[0]!=0 || out[1]!=0)out+=2;
  }else {
   temporary=bytes;
   do {*temporary++=*out++;}while(temporary[-1]!=0);
   destination[0]=255;
   out=destination+1;temporary=bytes;
   while(*temporary!=0) {
    out[0]=0;out[1]=*temporary++;out+=2;
   }
  }
  if(source[0]==255) {
   in=source+1;
   while(in[0]!=0 || in[1]!=0) {
    out[0]=in[0];out[1]=in[1];out+=2;in+=2;
   }
  }else {
   while(*in!=0) {
    out[0]=0;out[1]=*in++;out+=2;
   }
  }
  out[0]=0;out[1]=0;
 }else {
  while(*out!=0)out++;
  do {*out++=*in++;}while(out[-1]!=0);
 }
 return destination;
}
