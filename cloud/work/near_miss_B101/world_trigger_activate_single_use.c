/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;typedef signed char s8;typedef short s16;typedef unsigned int u32;
typedef void *OSMesg;
typedef struct OSMesgQueue {void *mtqueue,*fullqueue;int validCount,first,msgCount;OSMesg *msg;} OSMesgQueue;
typedef struct EventMessage {s16 type;} EventMessage;
typedef struct FrameOwnerView {u8 prefix[636];void *framebuffer;} FrameOwnerView;
extern OSMesgQueue D_8002ECF8,D_8002ECC0;
extern EventMessage D_80142D90;
extern s8 D_80035471,D_80035470,D_80035472,D_8015F72D,D_8011EAE0;
extern s16 D_801525F0;
extern FrameOwnerView D_8002E8E8;
extern void *D_8002EB98;
extern u32 D_80114730;
extern int D_80151AE0,D_80115BCC;
extern int osRecvMesg(OSMesgQueue *,OSMesg *,int);
extern int osJamMesg(OSMesgQueue *,OSMesg,int);
extern void *osViGetCurrentFramebuffer(void),*osViGetFramebuffer(void);
extern u32 osSetIntMask(u32);
extern void func_800205E4(void),apply_display_mode(void),check_mpath_save(void);
extern void attract_or_transition(void),world_collision_response(void);
void world_trigger_activate(void)
{
 OSMesg message=0;
 void *framebuffer;
 s8 *current=(s8 *)(u32)&D_80035470,*render=(s8 *)(u32)&D_80035472;
 osRecvMesg(&D_8002ECF8,&message,1);
 switch(((EventMessage *)message)->type) {
 case 4:
  func_800205E4();apply_display_mode();osSetIntMask(1);*(s16 *)(u32)&D_801525F0=0;
  check_mpath_save();D_8011EAE0=0;for(;;){}
 case 1:
  framebuffer=osViGetCurrentFramebuffer();
  if(osViGetFramebuffer()==framebuffer && !(*(s8 *)(u32)&D_80035471) && !(*current) && !(*render) &&
     ((FrameOwnerView *)(u32)&D_8002E8E8)->framebuffer!=*(void **)(u32)&D_8002EB98 && *(u32 *)(u32)&D_80114730<2) {
   D_80151AE0=0;attract_or_transition();(*current)=1;osJamMesg(&D_8002ECC0,&D_80142D90,1);
  }
  D_80151AE0++;break;
 case 2750:
  (*render)=0;world_collision_response();D_8015F72D^=1;D_80114730++;break;
 case 2:
  D_80115BCC^=1;D_80114730--;break;
 }
}
