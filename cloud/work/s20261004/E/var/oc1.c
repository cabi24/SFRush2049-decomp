s32 object_create(volatile s32 sel)
{
    volatile s32 old;
    osRecvMesg(&D_801461D0, (void*)0, 1);
    old = slot_state_setup(sel);
    osJamMesg(&D_801461D0, (void*)0, 0);
    return old;
}
