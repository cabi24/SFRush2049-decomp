/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned int u32;
extern u32 D_80118E28, D_80118E2C;
void func_800BEA3C(u32 first, u32 second) {
    D_80118E28 = first;
    D_80118E2C = second;
}
