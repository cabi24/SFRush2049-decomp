/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef unsigned int u32;
typedef unsigned char u8;
extern u32 D_80123418[];
extern float func_8008B2E4(float);
u8 func_800B23E0(u8 index) {
    u8 choice;
    do {
        choice = (u32)func_8008B2E4(32.0f);
    } while (!(D_80123418[index] & (1U << choice)));
    return choice;
}
