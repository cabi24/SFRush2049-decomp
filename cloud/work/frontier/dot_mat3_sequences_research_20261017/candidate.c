/* Native-adapted MAT3 sequence flow. The state local is a documented
 * native reconstruction choice, not a MAT3 DWARF local. Context supplies
 * scalar, Link/List/Effect types and genuine external service contracts. */
typedef struct Sequence { Link ll; u32 id; s8 free,active,stop; f32 startTime,timeout; u8 next; s32 ids[4]; s32 subId,replace; u8 priority; s32 sndTag; } Sequence;
typedef struct FxTag { Link ll; List *list; u32 id; s32 state,type; s8 paused,loop; u8 numCmds,priority; s32 wpriority; f32 startTime,vol,lrpan,fbpan,pitch; s32 fxid,subId; u32 sysId; void *data; } FxTag;
s32 func_80095198(f32,f32);
void func_800988D8(void) {
 Sequence *s,*next;
 FxTag *t;
 for(s=(Sequence *)D_80149860.head;s;s=next) {
  next=(Sequence *)s->ll.next;
  if(s->stop || (s->sndTag == -1 && func_80095198(s->startTime,s->timeout))) {
   scheduler_recv(s->sndTag);
   entity_flag_check((s32 *)s);
  } else {
   t=(FxTag *)func_80091BA8(s->sndTag);
   if(t) {
    s32 state=t->state;
    if(state==2 && !t->numCmds && !entity_state_check(t->sysId))
     audio_effect_setup((Effect *)t);
    else continue;
   }
   while(s->ids[s->next]==-1) {
    s->next++;
    if(s->next>=4) break;
   }
   if(s->next>=4) entity_flag_check((s32 *)s);
   else {
    s->sndTag=entity_flags_apply(s->ids[s->next++],s->subId,s->replace,s->priority);
    ((FxTag *)func_80091BA8(s->sndTag))->data=s;
   }
  }
 }
}

