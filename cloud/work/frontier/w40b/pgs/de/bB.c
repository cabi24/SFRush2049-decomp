extern s32 D_80152770[];                /* heap semaphore queue */
void audio_reverb_update(u32 address, s32 tag); /* _DeallocateMemory (internal, IPA a1/a2) */
static s32 sLoaded = 0;                 /* 0x801174C0 */

void display_enable(s32 load)           /* LoadFrontEndOverlay */
{
    if (load) {
        if (!sLoaded) {
            if (D_8011ED04x[0]) {
                osRecvMesg((OSMesgQueue *)D_80152770, (void **)0, 1);
                audio_reverb_update(0x8038A400, 0);
                osSendMesg((OSMesgQueue *)D_80152770, (void *)0, 0);
                D_8011ED04x[0] = 0;
            }
            sync_maxpath_to_checkpoint();
            sLoaded = 1;
        }
    } else if (sLoaded) {
        if (D_8011ED00x[0]) {
            osRecvMesg((OSMesgQueue *)D_80152770, (void **)0, 1);
            audio_reverb_update(0x8038A400, 0);
            osSendMesg((OSMesgQueue *)D_80152770, (void *)0, 0);
            D_8011ED00x[0] = 0;
        }
        sLoaded = 0;
    }
}

