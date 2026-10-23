/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* HAL_text.c (N64) part, w40b.
 * object_create (0x800B42F0) = HAL_SetFont@HAL_text.c in the MAT3 two-level form: _GetSemaphore(); old = _HAL_SetFont(fontID);
 * _ReleaseSemaphore(); return old;  _HAL_SetFont = slot_state_setup (IPA s2 parameter).
 * func_800B42E8 = _GetSemaphore, func_800B41B8 = _ReleaseSemaphore: MAT3 statics defined at the end of HAL_text.c (after
 * their callers; as1 line ties), N64 bodies = the D_801461D0 font-queue recv / send. umerge inlines both into every caller
 * and deletes them; their retail stubs sit where the post-order layout puts HAL_SetFont's callees: _ReleaseSemaphore (last
 * call site) first at 0x800B41B8 before HAL_SetFontFixedWidth/_HAL_SetFont, _GetSemaphore at 0x800B42E8 right before
 * HAL_SetFont. Written non-static (not kept) so the stubs keep their symbols.
 * Knapsack fact: this form's u-code size (14, size increase 9, benefit 5) is what lets umerge inline exactly the last three
 * of initGameState's 13 HAL_SetFont calls and PrintScores' call (see state.c); the direct osRecvMesg/osSendMesg form
 * (size increase 21) is inlined nowhere. */
typedef signed int s32;
typedef unsigned int u32;
typedef struct { u32 opaque[6]; } OSMesgQueue;
extern OSMesgQueue D_801461D0;
s32 osRecvMesg(OSMesgQueue *mq, void **msg, s32 flag);
s32 osSendMesg(OSMesgQueue *mq, void *msg, s32 flag);
s32 slot_state_setup(s32 fontID);       /* _HAL_SetFont (IPA s2 parameter) */
void func_800B42E8(void);
void func_800B41B8(void);

s32 object_create(s32 fontID)           /* HAL_SetFont */
{
    s32 old;
    func_800B42E8();
    old = slot_state_setup(fontID);
    func_800B41B8();
    return old;
}

void func_800B42E8(void)
{
    osRecvMesg(&D_801461D0, (void **)0, 1);
}

void func_800B41B8(void)
{
    osSendMesg(&D_801461D0, (void *)0, 0);
}
