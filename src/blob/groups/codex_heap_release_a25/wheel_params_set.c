/*
 * wheel_params_set (historical label) -- shut down the sound/heap users and
 * free two allocations. Calls func_800958B8 (flag every active D_80144C48
 * entry, then set D_8011028C), spins until the audio side clears
 * D_8011028C and empties the list D_80146160, clears D_80110284 and frees
 * the allocations held in D_80144C48 and D_80146100.
 *
 * Structure recovered from retail: the free is the deleted internal helper
 * func_800960CC (stub between audio_reverb_update and audio_effect_process),
 * which takes the heap lock (deleted internal func_80095CF4), releases the
 * block through the internal audio_reverb_update(address, 0) (IPA: a1/a2,
 * s0/s1 left to the caller, hence the unused s0/s1 saves here) and unlocks
 * (deleted internal func_80095CFC). umerge inlines all three into every
 * caller; each inlined func_800960CC instance accounts for 24 bytes of
 * frame (homes 56 and 32, frame 64). Defining the three stubs for real
 * reproduces their retail jr-ra stubs; landing needs prefer_definition
 * entries for them. No arcade ancestor (N64 heap code).
 */
typedef signed char s8;
typedef unsigned char u8;
typedef unsigned int u32;

typedef struct List16 { u8 indirect, doubly, pad2[2]; u32 count; void *head, *tail; } List16;
typedef struct OSMesgQueue OSMesgQueue;

extern volatile s8 D_8011028C;
extern s8 D_80110284;
extern volatile List16 D_80146160;
extern u32 D_80144C48, D_80146100;
extern char D_80152770[];
int osRecvMesg(OSMesgQueue *, void **, int);
int osJamMesg(OSMesgQueue *, void *, int);
void audio_reverb_update(u32 address, int tag);
void func_800958B8(void);

void func_80095CF4(void)
{
    osRecvMesg((OSMesgQueue *)D_80152770, 0, 1);
}

void func_80095CFC(void)
{
    osJamMesg((OSMesgQueue *)D_80152770, 0, 0);
}

void func_800960CC(u32 address)
{
    func_80095CF4();
    audio_reverb_update(address, 0);
    func_80095CFC();
}

void wheel_params_set(void)
{
    func_800958B8();
    while (D_8011028C != 0) {}
    while (D_80146160.head != 0) {}
    D_80110284 = 0;
    func_800960CC(D_80144C48);
    func_800960CC(D_80146100);
}
