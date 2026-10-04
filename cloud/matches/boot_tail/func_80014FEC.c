/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned short u16;
typedef struct ResourceOffsets { unsigned int offsets[4]; } ResourceOffsets;
extern void *func_80014EBC(u16, ResourceOffsets *);
extern int func_80015720(u16, void *);

void func_80014FEC(u16 *ids, ResourceOffsets *resources)
{
    u16 id;
    unsigned char *entry;
    while ((id = *ids) != 0xFFFF) {
        entry = func_80014EBC(id, resources);
        if (entry != 0) {
            func_80015720(*ids, entry + 8);
        }
        ++ids;
    }
}
