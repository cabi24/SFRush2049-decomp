/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct { s32 opaque[6]; } OSMesgQueue;
extern OSMesgQueue D_801461D0;
extern s32 osRecvMesg(OSMesgQueue *, void **, s32);
extern s32 osSendMesg(OSMesgQueue *, void *, s32);
extern s32 slot_state_setup(s32 selection);
extern void func_800B6748(u8 *rgba, f32 *amount, s16 *x, s16 *y, s8 *bank, s8 *mode, s8 *variant);
extern s32 mode_byte2_set();
extern s32 mode_byte_set();
extern void func_800ED66C(f32);
extern s32 func_800B7360();
extern void audio_doppler_calc(u8 *, u16);
extern s32 D_80149B48;
extern f32 D_80114748;
extern s16 D_80149D92, D_80149D9E;
extern s8 D_80149DA0, D_80149B60, D_80149B70;
extern s32 D_80149B88;
extern s8 D_8011474C, D_8011473C;
extern s32 D_80149DD0;
extern u8 D_80149DD8[800];
#define NULL 0
void world_velocity_integrate(s8 recording) {
 s32 pos=0; s16 x,y; u32 bits; union {u32 w; f32 f;} cvt;
 u8 *p; u16 len;
 if(D_8011474C) {
  osRecvMesg((OSMesgQueue *)&D_801461D0,NULL,1);
  D_8011473C=recording;
  func_800B6748((u8 *)&D_80149B48,&D_80114748,&D_80149D92,&D_80149D9E,&D_80149DA0,&D_80149B60,&D_80149B70);
  slot_state_setup(D_80149DA0);
  D_80149B88 &= 0x8000;
  while(pos<D_80149DD0) {
   p=D_80149DD8+pos;
   switch(*p++) {
   case 0: pos++;slot_state_setup(*p);pos++;break;
   case 1: pos++;mode_byte2_set(*p);pos++;break;
   case 2: pos++;mode_byte_set(*p);pos++;break;
   case 3: pos++;x=(s16)((p[0]<<8)|p[1]);y=(s16)((p[2]<<8)|p[3]);if(x!=-32768)D_80149D92=x;if(y!=-32768)D_80149D9E=y;pos+=4;break;
   case 4: pos++;bits=((u32)p[0]<<24)|p[3]|((u32)p[1]<<16)|((u32)p[2]<<8);cvt.w=bits;func_800ED66C(cvt.f);pos+=4;break;
   case 5: pos++;func_800B7360(p[0],p[1],p[2],p[3]);pos+=4;break;
   case 6: pos++;len=*p++;pos++;audio_doppler_calc(p,len);pos+=len;break;
   default: pos++;break;
   }
  }
  osSendMesg((OSMesgQueue *)&D_801461D0,NULL,0);
 }
}
