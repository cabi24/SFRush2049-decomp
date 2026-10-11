/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80015720 (IDO 7.1): code identical; relocation-name split only (w19b). Retail reaches a label that splat
 * placed at a compiler-made address (base-8/-12 loop end pointer or base+4 field). With the reloc_addrs.us.txt
 * entries in cloud/work/frontier/w19b/RESULTS.md (re-split target) this source scores
 * LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical. Prior: D10/BT03 (and w18c relocname/ for 1661C/158D8/15F28). */
/* The native record is eight bytes: payload pointer, ID, reference count.
 * Fields permit byte alignment; whole-record shifts use aligned storage. */
typedef unsigned short u16;
#pragma pack(1)
typedef struct {
    void *payload;
    u16 id;
    u16 references;
} RecordFields_8003C618;
#pragma pack()
typedef union {
    RecordFields_8003C618 fields;
    unsigned int words[2];
} RecordStorage_8003C618;
extern int D_8003C610;
extern RecordStorage_8003C618 D_8003C618[];
extern void func_80014594(void);
extern void func_800145DC(void);

int func_80015720(u16 id, void *payload)
{
    int i;
    int j;
    func_80014594();
    for (i = 0; i < D_8003C610 && ((RecordFields_8003C618 *)&D_8003C618[i])->id < id; i++) {
    }
    if (i < D_8003C610) {
        if (((RecordFields_8003C618 *)&D_8003C618[i])->id != id) {
            if (D_8003C610 < 2048) {
                for (j = D_8003C610 - 1; j >= i; j--) {
                    D_8003C618[j + 1] = D_8003C618[j];
                }
                D_8003C610++;
            } else {
                func_800145DC();
                return 0;
            }
        } else {
            ((RecordFields_8003C618 *)&D_8003C618[i])->references++;
            func_800145DC();
            return 0;
        }
    } else {
        if (D_8003C610 < 2048) {
            D_8003C610++;
        } else {
            func_800145DC();
            return 0;
        }
    }
    ((RecordFields_8003C618 *)&D_8003C618[i])->payload = payload;
    ((RecordFields_8003C618 *)&D_8003C618[i])->id = id;
    ((RecordFields_8003C618 *)&D_8003C618[i])->references = 1;
    func_800145DC();
    return 1;
}
