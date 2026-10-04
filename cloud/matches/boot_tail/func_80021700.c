/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Native controller helper; see cloud/work/boot_tail/C13-tail/README.md. */
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef unsigned int u32;

#pragma pack(1)
typedef struct VoiceState {
    u8 unknown00[36];
    u32 flags24;
    u8 unknown28[34];
    u8 channel4A;
    u8 set4B;
    u8 unknown4C[20];
    u32 id60;
    u8 unknown64[268];
    s16 lfo170;
    u8 unknown172[10];
    s16 lfo17C;
    u8 unknown17E[34];
} VoiceState;
#pragma pack(0)

extern VoiceState D_8004BEB8[];

int func_80021700(u32 id)
{
    VoiceState *voice;
    if (id != 0xFFFFFFFFU) {
        voice = &D_8004BEB8[id & 255];
        if (voice->id60 == id) {
            voice->flags24 |= 8;
            return 0;
        }
    }
    return -1;
}
