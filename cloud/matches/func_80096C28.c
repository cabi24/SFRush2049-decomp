/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Binds a table header and rebases its 12-byte entries; no quirks beyond storing through table->header. */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;

typedef struct Entry {
    s32 unk0;
    s32 offset;
    s32 unk8;
} Entry;

typedef struct Header {
    u32 entries;
    s32 count;
} Header;

typedef struct Table {
    Header *header;
    s32 count;
    Entry *entries;
} Table;

void func_80096C28(Table *table, Header *header) {
    s32 i;
    Entry *e;

    if (table != 0) {
        table->header = header;
        table->entries = (Entry *) header->entries;
        table->count = header->count;
        if (!(header->entries & 0x80000000)) {
            table->entries = (Entry *) ((u32) table->entries + (u32) header);
            table->header->entries = (u32) table->entries;
            for (i = 0; i < table->count; i++) {
                e = &table->entries[i];
                e->offset += (u32) header;
            }
        }
    }
}
