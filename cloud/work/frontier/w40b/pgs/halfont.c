/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* object_create (0x800B42F0) = HAL_SetFont@HAL_text.c: MAT3 two-level form, the semaphore helpers (MAT3 statics at the
 * end of HAL_text.c; N64 bodies = the D_801461D0 font-queue recv/send) defined after it. */
typedef signed int s32;
typedef unsigned int u32;
typedef struct { u32 opaque[6]; } OSMesgQueue;
extern OSMesgQueue D_801461D0;
s32 osRecvMesg(OSMesgQueue *mq, void **msg, s32 flag);
s32 osSendMesg(OSMesgQueue *mq, void *msg, s32 flag);
s32 slot_state_setup(s32 fontID);       /* _HAL_SetFont (IPA s2 parameter) */
void _GetSemaphore_HT(void);
void _ReleaseSemaphore_HT(void);

s32 object_create(s32 fontID)           /* HAL_SetFont */
{
    s32 old;
    _GetSemaphore_HT();
    old = slot_state_setup(fontID);
    _ReleaseSemaphore_HT();
    return old;
}

void _GetSemaphore_HT(void)
{
    osRecvMesg(&D_801461D0, (void **)0, 1);
}

void _ReleaseSemaphore_HT(void)
{
    osSendMesg(&D_801461D0, (void *)0, 0);
}
