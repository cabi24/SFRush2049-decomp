typedef signed char s8;
typedef unsigned char u8;
typedef unsigned int u32;

typedef struct List16 { u8 indirect, doubly, pad2[2]; u32 count; void *head, *tail; } List16;
typedef struct OSMesgQueue OSMesgQueue;

extern u32 D_8011025C, D_80110260;
extern char D_80152770[];
int osRecvMesg(OSMesgQueue *, void **, int);
int osJamMesg(OSMesgQueue *, void *, int);
void audio_reverb_update(u32 address, int tag);

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

void func_800C885C(void)
{
    if (D_8011025C) {
        func_800960CC(D_8011025C);
        D_8011025C = 0;
    }
    if (D_80110260) {
        func_800960CC(D_80110260);
        D_80110260 = 0;
    }
}
