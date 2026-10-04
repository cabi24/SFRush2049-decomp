/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
#pragma pack(1)
typedef struct VoicePrefix {
    u8 unknown00[46];
    u8 channel2E;
    u8 unknown2F[49];
    u32 identifier60;
} VoicePrefix;
#pragma pack(0)
typedef struct ChannelLink { u8 previous, next; u16 active; } ChannelLink;
typedef struct GroupLink { u16 next, previous; } GroupLink;
extern ChannelLink D_800504C8[32];
extern u8 D_80050548[256];
extern GroupLink D_80050648[256];
extern u16 D_80050A48;
void func_8001EE9C(VoicePrefix *state)
{
    ChannelLink *link;
    GroupLink *group;
    link = &D_800504C8[state->identifier60 & 255];
    if (link->active == 1) {
        if (link->previous != 255) D_800504C8[link->previous].next = link->next;
        else D_80050548[state->channel2E] = link->next;
        if (link->next != 255) D_800504C8[link->next].previous = link->previous;
        else if (link->previous == 255) {
            group = &D_80050648[state->channel2E];
            if (group->previous != 65535) D_80050648[group->previous].next = group->next;
            else D_80050A48 = group->next;
            if (group->next != 65535) D_80050648[group->next].previous = group->previous;
        }
        link->active = 0;
    }
}
