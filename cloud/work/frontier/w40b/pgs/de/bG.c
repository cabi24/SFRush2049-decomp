extern u8 D_8038A400[];
extern s32 D_80152770[];                /* heap semaphore queue */
void audio_reverb_update(u32 address, s32 tag); /* _DeallocateMemory (internal, IPA a1/a2) */

void display_enable(s32 load)           /* LoadFrontEndOverlay */
{
    static s32 sLoaded = 0;
    static s8 sStart = 0;
    static s8 sExtra = 0;
    if (load) {
        if (!sLoaded) {
            if (sExtra) {
                osRecvMesg((OSMesgQueue *)D_80152770, (void **)0, 1);
                audio_reverb_update((u32)D_8038A400, 0);
                osSendMesg((OSMesgQueue *)D_80152770, (void *)0, 0);
                sExtra = 0;
            }
            sync_maxpath_to_checkpoint();
            sLoaded = 1;
        }
    } else if (sLoaded) {
        if (sStart) {
            osRecvMesg((OSMesgQueue *)D_80152770, (void **)0, 1);
            audio_reverb_update((u32)D_8038A400, 0);
            osSendMesg((OSMesgQueue *)D_80152770, (void *)0, 0);
            sStart = 0;
        }
        sLoaded = 0;
    }
}

