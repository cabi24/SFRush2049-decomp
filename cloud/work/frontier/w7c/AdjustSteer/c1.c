typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32;
#define NULL ((void *)0)
typedef struct OSMesgQueue OSMesgQueue;
s32 osRecvMesg(OSMesgQueue *, void **, s32);
s32 osJamMesg(OSMesgQueue *, void *, s32);
void audio_reverb_update(u32 address, s32 tag);
extern OSMesgQueue D_80152770;

typedef struct {
    u8 pad0[0x22];
    u8 busy;            /* 0x22 */
    u8 pad23[5];
} SlotEntry;            /* 0x28 */

typedef struct {
    u8 pad0[0x64];
    SlotEntry entry[16];  /* 0x64 */
    u8 pad[0x304 - 0x64 - 16 * 0x28];
} PlayerSlot;           /* 0x304 */
extern PlayerSlot D_80144030[];

typedef struct {
    u8 pad0[0x10];
    u8 player;          /* 0x10 */
    u8 index;           /* 0x11 */
    u8 pad12[0x48 - 0x12];
    u32 allocation;     /* 0x48 */
} SteerObj;

void AdjustSteer(SteerObj **h)
{
    SteerObj *o = *h;

    if (o->allocation) {
        osRecvMesg(&D_80152770, NULL, 1);
        audio_reverb_update(o->allocation, 0);
        osJamMesg(&D_80152770, NULL, 0);
        o->allocation = 0;
        D_80144030[o->player].entry[o->index].busy = 0;
    }
}
