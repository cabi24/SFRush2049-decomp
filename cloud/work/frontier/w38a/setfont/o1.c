/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed int s32;
typedef unsigned int u32;
typedef struct { u32 opaque[6]; } OSMesgQueue;
extern OSMesgQueue D_801461D0;
s32 osRecvMesg(OSMesgQueue *, void **, s32);
s32 osSendMesg(OSMesgQueue *, void *, s32);
s32 slot_state_setup(s32 selection);

s32 object_create(s32 fontID)
{
    s32 old;
    osRecvMesg(&D_801461D0, (void *)0, 1);
    old = slot_state_setup(fontID);
    osSendMesg(&D_801461D0, (void *)0, 0);
    return old;
}
