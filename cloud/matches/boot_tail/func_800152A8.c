/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned short u16;
extern int func_800163A8(u16);

void func_800152A8(u16 *ids)
{
    u16 *end;
    end = ids;
    while (*end != 0xFFFF) {
        ++end;
    }
    --end;
    while (end >= ids) {
        func_800163A8(*end);
        --end;
    }
}
