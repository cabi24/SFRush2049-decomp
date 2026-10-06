typedef signed char s8;
typedef unsigned char u8;
typedef unsigned int u32;

typedef struct List16 { u8 indirect, doubly, pad2[2]; u32 count; void *head, *tail; } List16;

extern volatile s8 D_8011028C;
extern s8 D_80110284;
extern volatile List16 D_80146160;
extern u32 D_80144C48, D_80146100;
void func_800958B8(void);
typedef struct OSMesgQueue OSMesgQueue;
int osRecvMesg(OSMesgQueue *, void **, int);
int osJamMesg(OSMesgQueue *, void *, int);
extern char D_80152770[];
void audio_reverb_update(u32 address, int tag);
static void hfree2(u32 address, int tag)
{
    osRecvMesg((OSMesgQueue *)D_80152770, 0, 1);
    audio_reverb_update(address, tag);
    osJamMesg((OSMesgQueue *)D_80152770, 0, 0);
}
static void hfree(u32 address)
{
    hfree2(address, 0);
}


void wheel_params_set(void)
{
    func_800958B8();
    while (D_8011028C != 0) {}
    while (D_80146160.head != 0) {}
    D_80110284 = 0;
    hfree(D_80144C48);
    hfree(D_80146100);
}
