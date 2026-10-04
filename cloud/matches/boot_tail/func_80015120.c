/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned short u16;
extern int func_80015F28(u16);

void func_80015120(u16 *ids)
{
    u16 id;
    id = *ids;
    while (id != 0xFFFF) {
        func_80015F28(id);
        id = *++ids;
    }
}
