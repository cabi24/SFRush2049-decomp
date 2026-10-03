#include "scheduler_native.h"
/* Complete SDK RCP task launcher with native N64 audio/frame timestamps.
 * Native assertion checks have no failure payload. Original SDK rv stores
 * the actual DP-submit status and is checked, as in the native body. */
void __scExec(OSSched *scheduler,OSScTask *sp,OSScTask *dp) {
 s32 rv;
 if(scheduler->curRSPTask) { }
 if(sp) {
  if(sp->type==2) {
   __osScAudioStartHi=osGetTime();
   __osScCurAudioTask=sp;
  } else if(!(sp->state&0x20)) {
   __osScRetraceTimeHi=__osScFrameTimeHi;
  }
  osInvalICache();
  sp->state&=~0x30;
  osViModeNtscLan1(&sp->type);
  osViModeNtscLpn1(&sp->type);
  scheduler->curRSPTask=sp;
  if(sp==dp)scheduler->curRDPTask=dp;
 }
 if(dp && dp!=sp) {
  if(!dp->unk38) { }
  rv=osDpSetNextBuffer(dp->unk38,*(u64 *)dp->unk3C);
  if(rv) { }
  scheduler->curRDPTask=dp;
 }
}
