extern u8 D_8038A400[];
void func_800960CC(u32 address);
extern s32 D_80152770[];                /* heap semaphore queue */
void audio_reverb_update(u32 address, s32 tag); /* _DeallocateMemory (internal, IPA a1/a2) */
static s32 sLoaded = 0;                 /* 0x801174C0 */

void display_enable(s32 load)           /* LoadFrontEndOverlay */
{
    if (load) {
        if (!sLoaded) {
            if (D_8011ED04) {
                func_800960CC((u32)D_8038A400);
                D_8011ED04 = 0;
            }
            sync_maxpath_to_checkpoint();
            sLoaded = 1;
        }
    } else if (sLoaded) {
        if (D_8011ED00) {
            func_800960CC((u32)D_8038A400);
            D_8011ED00 = 0;
        }
        sLoaded = 0;
    }
}

