/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned int u32;
#pragma pack(1)
typedef struct VoicePrefix {
    u8 unknown00[96];
    u32 identifier60;
} VoicePrefix;
#pragma pack()
extern u8 D_80055000[][134];
void func_80020DA8(u8 controller, VoicePrefix *destination, VoicePrefix *source)
{
    u32 dst;
    u32 src;
    dst = destination->identifier60 & 255;
    src = source->identifier60 & 255;
    if (controller < 64) {
        controller &= 31;
        D_80055000[dst][controller] = D_80055000[src][controller];
        D_80055000[dst][controller + 32] = D_80055000[src][controller + 32];
    } else if (controller == 128 || controller == 129) {
        controller &= 254;
        D_80055000[dst][controller] = D_80055000[src][controller];
        D_80055000[dst][controller + 1] = D_80055000[src][controller + 1];
    } else if (controller == 132 || controller == 133) {
        controller &= 254;
        D_80055000[dst][controller] = D_80055000[src][controller];
        D_80055000[dst][controller + 1] = D_80055000[src][controller + 1];
    } else {
        D_80055000[dst][controller] = D_80055000[src][controller];
    }
}
