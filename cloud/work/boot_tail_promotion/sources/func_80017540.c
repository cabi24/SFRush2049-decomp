/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted from cloud/matches/boot_tail/func_80017540.c: file-local type names ContextPrefix, Entry suffixed _80017540 so several bodies share one ROM TU; no other change. */
/* Native helper reconstruction; real pointer, scalar and O32 argument slots audited. */
typedef unsigned char u8;
typedef unsigned int u32;
typedef struct Entry_80017540 {
    struct Entry_80017540 *next;
    struct Entry_80017540 *previous;
    u32 identifier;
} Entry_80017540;
typedef struct ContextPrefix_80017540 {
    u8 unknown000[0xF68];
    u32 activeF68;
    u32 savedF6C;
    u32 lowF70;
    u32 highF74;
    u32 unknownF78;
    Entry_80017540 *headF7C;
} ContextPrefix_80017540;
extern ContextPrefix_80017540 *D_8004BE80;
extern int func_800201D0(u32);
extern void func_80017470(Entry_80017540 *);
void func_80017540(void)
{
    Entry_80017540 *entry;
    Entry_80017540 *next;
    entry = D_8004BE80->headF7C;
    while (entry != 0) {
        next = entry->next;
        if (func_800201D0(entry->identifier) == -1) func_80017470(entry);
        entry = next;
    }
}
