/* Genuine HAL_FinishText donor, native queue/color/position behavior retained.
 * Original automatic declarations: int i, int length, unsigned int aLong.
 * Native rendering and its reserved texture-rectangle dependency stay external.
 */
typedef signed char s8; typedef unsigned char u8; typedef signed short s16;
typedef unsigned short u16; typedef signed int s32; typedef unsigned int u32;
typedef float f32; typedef struct OSMesgQueue OSMesgQueue;
extern OSMesgQueue D_801461D0;
extern s32 osRecvMesg(OSMesgQueue *,void **,s32),osSendMesg(OSMesgQueue *,void *,s32);
extern s32 slot_state_setup(s32);
extern void func_800B6748(u8 *,f32 *,s16 *,s16 *,s8 *,s8 *,s8 *);
extern void mode_byte2_set(s16),mode_byte_set(s16),func_800B70EC(s16,s16);
extern void func_800ED66C(f32),func_800B7360(u8,u8,u8,u8),audio_doppler_calc(const u8 *,u16);
typedef struct NativeTextColor { union { unsigned char rgba[8]; unsigned int rgbaU32; } fore; } NativeTextColor;
extern NativeTextColor D_80149B48; extern f32 D_80114748;
extern s16 D_80149D92,D_80149D9E; extern s8 D_80149DA0,D_80149B60,D_80149B70;
extern s32 D_80149B88,D_80149DD0; extern s8 D_8011474C,D_8011473C; extern u8 D_80149DD8[800];
void du_gfx_lock(void);void du_gfx_unlock(void);
void world_velocity_integrate(s8 end)
{
 s32 i;
 s32 length;
 u32 aLong;
 if(D_8011474C) {
  osRecvMesg(&D_801461D0,0,1);
  D_8011473C=end;
  func_800B6748(D_80149B48.fore.rgba,&D_80114748,&D_80149D92,&D_80149D9E,&D_80149DA0,&D_80149B60,&D_80149B70);
  slot_state_setup(D_80149DA0);
  D_80149B88 &= 0x8000;
  for(i=0;i<D_80149DD0;) {
   switch(D_80149DD8[i++]) {
   case 0:slot_state_setup(D_80149DD8[i++]);break;
   case 1:mode_byte2_set(D_80149DD8[i++]);break;
   case 2:mode_byte_set(D_80149DD8[i++]);break;
   case 3:
    func_800B70EC(((s16)D_80149DD8[i]<<8)|D_80149DD8[i+1],((s16)D_80149DD8[i+2]<<8)|D_80149DD8[i+3]);
    i+=4;break;
   case 4:
    aLong=((u32)D_80149DD8[i]<<24)|(D_80149DD8[i+1]<<16)|(D_80149DD8[i+2]<<8)|D_80149DD8[i+3];
    i+=4;func_800ED66C(*(f32 *)&aLong);break;
   case 5:
    func_800B7360(D_80149DD8[i],D_80149DD8[i+1],D_80149DD8[i+2],D_80149DD8[i+3]);
    i+=4;break;
   case 6:
    length=D_80149DD8[i++];audio_doppler_calc(D_80149DD8+i,length);i+=length;break;
   }
  }
  osSendMesg(&D_801461D0,0,0);
 }
}
