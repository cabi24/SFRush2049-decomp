/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Release/free the selected voice and clear its blocked byte.
 * MusyX family: synthvoice.c:voiceUnblock, AxioDL/musyx 78d2e16 (CC0).
 * N64 runtime reconstruction; no arcade equivalent. Native ABI/layout and
 * helper behavior remain authoritative over the newer public donor.
 */
typedef unsigned char u8;
typedef unsigned int u32;
#pragma pack(1)
typedef struct VoiceState {
    u8 unknown00[36];
    u32 flags;
    u32 value28;
    u8 unknown2C[52];
    u32 identifier;
    u8 unknown64[89];
    u8 valueBD;
    u8 unknownBE[226];
} VoiceState;
#pragma pack(0)
extern VoiceState D_8004BEB8[];
extern u8 func_8001467C(int);
extern void func_80014AF0(int);
extern void func_8001F6EC(VoiceState *);

void func_8001F954(u32 index)
{
    if (index == 0xFFFFFFFFU) {
        return;
    }
    if (func_8001467C(index)) {
        func_80014AF0(index);
    }
    D_8004BEB8[index].identifier = index;
    func_8001F6EC(&D_8004BEB8[index]);
    D_8004BEB8[index].valueBD = 0;
}
