/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Native helper reconstruction; real pointer, scalar and O32 argument slots audited. */
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
    u32 unknownF78;
    Entry *headF7C;
} ContextPrefix;
extern ContextPrefix *D_8004BE80;
extern int func_800201D0(u32);
extern void func_80017470(Entry *);
void func_80017540(void)
{
    Entry *entry;
    Entry *next;
    entry = D_8004BE80->headF7C;
    while (entry != 0) {
        next = entry->next;
        if (func_800201D0(entry->identifier) == -1) func_80017470(entry);
        entry = next;
    }
}
