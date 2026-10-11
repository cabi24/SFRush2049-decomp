/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80014E1C - lib_15a10 (IDO 7.1). Find the record with identifier ID in a chain of
 * variable-length records: each record starts with the byte offset of the next record
 * (0xFFFFFFFF terminates the chain) followed by a u16 identifier. Returns the record or 0.
 * w18c: natural form; IDO 7.1 narrows the u16 id in place. No shaping devices.
 * Prior: D10 BT03-low-followon (5.3, 7 words; 7.1 true=20 via a temp 'offset' local). */
typedef struct RelativeEntry {
    unsigned int next_offset;
    unsigned short id;
} RelativeEntry;

void *func_80014E1C(unsigned short id, RelativeEntry *entry)
{
    while (entry->next_offset != 0xFFFFFFFFU) {
        if (entry->id == id) {
            return entry;
        }
        entry = (RelativeEntry *)((unsigned char *)entry + entry->next_offset);
    }
    return 0;
}
