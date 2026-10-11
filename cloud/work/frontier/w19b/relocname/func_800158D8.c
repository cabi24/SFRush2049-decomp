/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_800158D8 (IDO 7.1): code identical; relocation-name split only (w19b). Retail reaches a label that splat
 * placed at a compiler-made address (base-8/-12 loop end pointer or base+4 field). With the reloc_addrs.us.txt
 * entries in cloud/work/frontier/w19b/RESULTS.md (re-split target) this source scores
 * LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical. Prior: D10/BT03 (and w18c relocname/ for 1661C/158D8/15F28). */
/* Eight-byte table records have a word-aligned copy representation and a
 * packed field view. Payload bits are opaque here. Only active records may
 * be searched or shifted; the native tables supply that bounded domain. */
typedef unsigned short u16;
#pragma pack(1)
typedef struct {
    void *payload;
    u16 id;
    u16 references;
} RecordFields_8003C618;
#pragma pack()
extern int D_8003C610;
typedef union {
    RecordFields_8003C618 fields;
    unsigned int words[2];
} RecordStorage_8003C618;
extern RecordStorage_8003C618 D_8003C618[];
extern void func_80014594(void);
extern void func_800145DC(void);

int func_800158D8(u16 id)
{
    int i;
    int j;
    func_80014594();
    for (i = 0; i < D_8003C610 && ((RecordFields_8003C618 *)&D_8003C618[i])->id != id; i++) {
    }
    if (i != D_8003C610) {
        if (--((RecordFields_8003C618 *)&D_8003C618[i])->references == 0) {
            for (j = i + 1; j < D_8003C610; j++) {
                D_8003C618[j - 1] = D_8003C618[j];
            }
            D_8003C610--;
            func_800145DC();
            return 1;
        }
    }
    func_800145DC();
    return 0;
}
