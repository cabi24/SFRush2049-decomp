/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
#pragma pack(1)
typedef struct VoicePrefix {u8 unknown00[46];u8 channel2E;u8 unknown2F[49];u32 identifier60;} VoicePrefix;
#pragma pack(0)
typedef struct ChannelLink {u8 previous,next;u16 active;} ChannelLink;
typedef struct GroupLink {u16 next,previous;} GroupLink;
extern ChannelLink D_800504C8[32];
extern u8 D_80050548[256];
extern GroupLink D_80050648[256];
extern u16 D_80050A48;
extern void func_8001EE9C(VoicePrefix *);
void func_8001EF8C(VoicePrefix *state, u8 channel)
{
    u8 index;
    ChannelLink *link;
    u16 current;
    u16 previous;
    index = state->identifier60;
    link = &D_800504C8[index];
    if (link->active == 1) {
        if (channel == state->channel2E) return;
        func_8001EE9C(state);
    }
    link->active = 1;
    link->previous = 255;
    link->next = D_80050548[channel];
    if (D_80050548[channel] != 255) {
        D_800504C8[D_80050548[channel]].previous = index;
    } else {
        if (D_80050A48 != 65535) {
            if (channel >= D_80050A48) {
                current = D_80050A48;
                while (current != 65535 && channel >= current) {
                    previous = current;
                    current = D_80050648[current].next;
                }
                D_80050648[previous].next = channel;
                D_80050648[channel].next = current;
                D_80050648[channel].previous = previous;
                if (current != 65535) D_80050648[current].previous = channel;
            } else {
                D_80050648[channel].next = D_80050A48;
                D_80050648[channel].previous = 65535;
                D_80050648[D_80050A48].previous = channel;
                D_80050A48 = channel;
            }
        } else {
            D_80050648[channel].next = 65535;
            D_80050648[channel].previous = 65535;
            D_80050A48 = channel;
        }
    }
    D_80050548[channel] = index;
    state->channel2E = channel;
}
