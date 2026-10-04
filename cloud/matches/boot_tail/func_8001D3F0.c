/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Native helper reconstruction; real pointer, scalar and O32 argument slots audited. */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
extern u8 D_8002C630;
extern int func_8001D1F4(void *, const void *, const void *, float, float, u32, u16, u32, u8, u8);
int func_8001D3F0(void *state, const void *first, const void *second, float fourth,
                 float fifth, u32 flags, u16 identifier, u8 eighth, u8 ninth)
{
    if (D_8002C630) {
        return func_8001D1F4(state, first, second, fourth, fifth, flags,
                            identifier, identifier | 0x80000000, eighth, ninth);
    }
    return -1;
}
