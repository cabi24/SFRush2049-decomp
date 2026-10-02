/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8; typedef unsigned char u8; typedef int s32;
typedef unsigned int u32;
typedef struct ResSlot {s8 f0,loaded,f2,f3,f4,f5;u8 type,pad07;s32 pad08;u8 **p0C;u8 *w10;} ResSlot;
typedef ResSlot Voice;
extern Voice D_80156D38[64];
extern s32 audio_frame_sync(s32 a, s32 b, s32 c, s32 d, void *e);
extern void display_list_alloc(s32 a);

s32 func_80097694(s32 arg0, s8 arg1)
{
  s32 i;

  for (i = 0; i < 64; i++)
  {
    if ((D_80156D38[i].p0C != 0) && (arg0 == D_80156D38[i].type) && ((arg1 < 0) || (arg1 == D_80156D38[i].f5)))
    {
      return i;
    }
  }
  return -1;
}

void resource_slot_clear(s32 id)
{
  if (func_80097694(id, -1) < 0)
  {
    display_list_alloc(audio_frame_sync(id, 0, 0, 0, 0));
  }
}

void resource_slots_clear_multiple(void)
{
  resource_slot_clear(54);
  resource_slot_clear(58);
  resource_slot_clear(59);
}

extern unsigned int D_801174B4;
extern void wheel_setup_initial(s32);
extern void tire_compound_set(void);
extern void player_cleanup_slots(void);
void state_change_preprocess(void)
{
    unsigned int state=D_801174B4;
    if(state&0x4000) wheel_setup_initial(57);
    else if(state&0x100) {
        wheel_setup_initial(56);
        tire_compound_set();
    } else if(state&0x80) wheel_setup_initial(60);
    else if(state&0x20000000) {
        wheel_setup_initial(55);
        resource_slot_clear(54);
        resource_slot_clear(58);
        resource_slot_clear(59);
    } else if(state&0x10000000) player_cleanup_slots();
}


typedef struct OSMesgQueue OSMesgQueue;
#define NULL ((void *)0)
extern s32 D_8002E580[],D_801569AC;
extern u8 D_80035440[],D_80156BB0[];
extern void entity_render_mode(s32);
extern void *audio_task_complete(void *,s32);
extern void *audio_dma_sync(s32,s32);
extern s32 audio_buffer_sync(u32);
extern void func_800972C4(s32);
extern void func_80097164(s32);
extern s32 osRecvMesg(OSMesgQueue *,void *,s32);
extern s32 osJamMesg(OSMesgQueue *,void *,s32);
s32 audio_frame_sync(s32 kind, s32 skip, s32 async, s32 flag, void *buf) {
    ResSlot *r;
    s32 i;
    s32 slot;
    s32 res;
    void *msg;

    if (skip == 0 && (res = func_80097694(kind, flag)) >= 0) {
        slot = res;
        if (D_80156D38[slot].f3 != 0) {
            entity_render_mode(slot);
        }
        return slot;
    }
    r = D_80156D38;
    for (i = 0; i != 64; i++, r++) {
        if (r->p0C == 0) {
            r->f0 = 0;
            r->loaded = 0;
            r->f2 = 0;
            r->f4 = 0;
            r->type = kind;
            r->f5 = flag;
            if (flag != 0) {
                r->p0C = (u8 **) audio_task_complete(buf, (D_8002E580[kind] + 0x1F) & ~0x1F);
            } else {
                r->p0C = (u8 **) audio_dma_sync((s32) buf, (D_8002E580[kind] + 0x1F) & ~0x1F);
            }
            *(s32 *) ((u8 *) r + 8) = audio_buffer_sync((u32) r->p0C);
            func_800972C4(i);
            slot = i;
            goto done;
        }
    }
    slot = res;
done:
    if (async != 0) {
        osRecvMesg((OSMesgQueue *) &D_80035440, &msg, 1);
        D_801569AC = slot;
        osJamMesg((OSMesgQueue *) &D_80156BB0, NULL, 1);
    } else {
        func_80097164(slot);
    }
    return slot;
}
