/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* w40b test: overlay helper hypothesis (function-local static flags, __inline helper = stub func_800A0F64). */
typedef signed char s8;
typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;
typedef struct { u32 opaque[6]; } OSMesgQueue;
s32 osRecvMesg(OSMesgQueue *mq, void **msg, s32 flag);
s32 osSendMesg(OSMesgQueue *mq, void *msg, s32 flag);
extern OSMesgQueue D_80152770;
extern u8 *D_8002B020;
extern u8 *D_8002B024;
extern u8 D_80394F70[];
extern u8 D_8039B440[];
extern u8 D_803B9A50[];
extern u8 D_803BAE20[];
extern u8 D_00BE4C70[];
extern u8 D_00BDA100[];
extern u8 D_8038A400[];
void PrevMaxPath(u8 *dst, char *name, u8 *end, u8 *bssEnd, u8 *romStart, u8 *bssStart, s8 *loaded, u8 *rom);
void audio_reverb_update(u32 address, s32 tag);

__inline void func_800A0F64(s32 extra, s32 load)
{
    static s8 sStartLoaded = 0;
    static s8 sExtraLoaded = 0;
    if (extra) {
        if (load) {
            PrevMaxPath((u8 *) 0x8038A400, "Extra", (u8 *) 0x803BB380, D_8039B440, D_00BE4C70, D_80394F70, &sExtraLoaded, D_8002B024);
        } else if (sExtraLoaded) {
            osRecvMesg(&D_80152770, (void **)0, 1);
            audio_reverb_update((u32)D_8038A400, 0);
            osSendMesg(&D_80152770, (void *)0, 0);
            sExtraLoaded = 0;
        }
    } else {
        if (load) {
            PrevMaxPath((u8 *) 0x8038A400, "Start", (u8 *) 0x803BB380, D_803BAE20, D_00BDA100, D_803B9A50, &sStartLoaded, D_8002B020);
        } else if (sStartLoaded) {
            osRecvMesg(&D_80152770, (void **)0, 1);
            audio_reverb_update((u32)D_8038A400, 0);
            osSendMesg(&D_80152770, (void *)0, 0);
            sStartLoaded = 0;
        }
    }
}

void MP_TargetSpeed(void) { func_800A0F64(1, 0); }
void InitMaxPath(void) { func_800A0F64(1, 1); }
void assign_default_paths(void) { func_800A0F64(0, 0); }
void sync_maxpath_to_checkpoint(void) { func_800A0F64(0, 1); }
