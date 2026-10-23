typedef signed short s16; typedef unsigned char u8; typedef signed int s32; typedef unsigned int u32;
typedef struct OSMesgQueue OSMesgQueue; extern OSMesgQueue D_801461D0;
extern s32 osRecvMesg(OSMesgQueue *,void **,s32),osSendMesg(OSMesgQueue *,void *,s32);
extern s32 slot_state_setup(s32);
void du_gfx_lock(void);void du_gfx_unlock(void);
s32 object_create(s32 font)
{
    s32 old;
    du_gfx_lock();
    old = slot_state_setup(font);
    du_gfx_unlock();
    return old;
}

void du_gfx_lock(void) { osRecvMesg(&D_801461D0, 0, 1); }

void du_gfx_unlock(void) { osSendMesg(&D_801461D0, 0, 0); }
