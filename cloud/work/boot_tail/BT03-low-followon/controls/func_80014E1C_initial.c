/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef struct RelativeEntry {
    unsigned int next_offset;
    unsigned short id;
} RelativeEntry;

void *func_80014E1C(unsigned short id, void *start)
{
    RelativeEntry *entry;
    entry = start;
    while (entry->next_offset != 0xFFFFFFFFU) {
        if (entry->id == id) {
            return entry;
        }
        entry = (RelativeEntry *)((unsigned char *)entry + entry->next_offset);
    }
    return 0;
}
