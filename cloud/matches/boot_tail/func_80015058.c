/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned short u16;
typedef struct ResourceOffsets { unsigned int offsets[4]; } ResourceOffsets;
typedef struct ResourceEntry {
    unsigned int next_offset;
    u16 id;
    u16 unknown06;
    u16 unknown08;
    u16 parameter;
} ResourceEntry;
extern void *func_80014EE8(u16, ResourceOffsets *);
extern int func_80015A0C(u16, void *, u16);

void func_80015058(u16 *ids, ResourceOffsets *resources)
{
    u16 id;
    ResourceEntry *entry;
    while ((id = *ids) != 0xFFFF) {
        entry = func_80014EE8(id, resources);
        if (entry != 0) {
            func_80015A0C(*ids, (unsigned char *)entry + 12, entry->parameter);
        }
        ++ids;
    }
}
