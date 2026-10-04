/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned int u32;
typedef struct Entry {
    struct Entry *next;
    struct Entry *previous;
    u32 identifier;
} Entry;
typedef struct ContextPrefix {
    u8 unknown000[0xF68];
    u32 activeF68;
    u32 savedF6C;
    u32 lowF70;
    u32 highF74;
    Entry *headF78;
    Entry *headF7C;
} ContextPrefix;
extern int func_8001FA18(u32);

void func_8001734C(ContextPrefix *context)
{
    Entry *entry;
    entry = context->headF78;
    while (entry != 0) {
        func_8001FA18(entry->identifier);
        entry = entry->next;
    }
    entry = context->headF7C;
    while (entry != 0) {
        func_8001FA18(entry->identifier);
        entry = entry->next;
    }
}
