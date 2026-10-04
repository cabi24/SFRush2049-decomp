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
    u32 index;
    u8 *dstdata;
    u8 *srcdata;
    dst = destination->identifier60 & 255;
    src = source->identifier60 & 255;
    if (controller < 64) {
        index = controller & 31;
        dstdata = &D_80055000[dst][index];
        srcdata = &D_80055000[src][index];
        dstdata[0] = srcdata[0];
        dstdata[32] = srcdata[32];
    } else if (controller == 128 || controller == 129) {
        index = controller & 254;
        dstdata = &D_80055000[dst][index];
        srcdata = &D_80055000[src][index];
        dstdata[0] = srcdata[0];
        dstdata[1] = srcdata[1];
    } else if (controller == 132 || controller == 133) {
        index = controller & 254;
        dstdata = &D_80055000[dst][index];
        srcdata = &D_80055000[src][index];
        dstdata[0] = srcdata[0];
        dstdata[1] = srcdata[1];
    } else {
        D_80055000[dst][controller] = D_80055000[src][controller];
    }
}
