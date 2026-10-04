/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;
typedef struct Track {
    u32 fraction;
    s32 whole;
    u32 unknown08;
    u32 active0C;
    u32 active10;
    u32 active14;
    u8 unknown18[16];
} Track;
typedef struct Context {
    u8 unknown000[0x118];
    u32 fraction118;
    s32 whole11C;
    u8 unknown120[0x448];
    Track tracks[64];
} Context;
extern Context *D_8004BE80;
void func_800180A0(void)
{
    int i;
    s32 sum;
    for (i = 0; i < 64; i++) {
        if (D_8004BE80->tracks[i].active0C || D_8004BE80->tracks[i].active10 ||
            D_8004BE80->tracks[i].active14) {
            sum = (s32)(D_8004BE80->tracks[i].fraction + D_8004BE80->fraction118);
            D_8004BE80->tracks[i].fraction = (u32)sum & 65535;
            sum = sum >> 16;
            D_8004BE80->tracks[i].whole = (s32)((u32)D_8004BE80->tracks[i].whole +
                (u32)sum + (u32)D_8004BE80->whole11C);
        }
    }
}
