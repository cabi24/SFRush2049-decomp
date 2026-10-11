#pragma pack(1)
typedef struct PitchState_80022CD4 {
    u8 unknown00[0x24];
    u32 flags24;
    u8 unknown28[0x28];
    u16 note50;
    u8 unknown52[0xA];
    u32 sample5C;
    u8 unknown60[0x60];
    s8 detuneC0;
} PitchState_80022CD4;
#pragma pack(0)
typedef struct PitchCommand_80022CD4 { u32 word[2]; } PitchCommand_80022CD4;
