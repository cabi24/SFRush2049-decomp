/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80015D68 - lib_16320 (IDO 7.1). MusyX synthdata.c dataInsertCurve/dataInsertKeymap shape (DATA_TAB insert;
 * doldecomp/ttyd libs/musyx, shape only): with interrupts off (func_80014594/func_800145DC), find ID's sorted
 * position in table D_80038610 (D_80038608 entries, max 2048); an existing entry is re-enabled and its reference
 * count bumped AFTER the interrupt enable (retail order: the call precedes the increment), returning 0; otherwise
 * the tail moves up one entry and {payload, id, references 1} is written (returns 1).
 * w23c (D10 BT03-low-insert-pair true=1310): MusyX's statement shape with `long` locals, the TU's packed-fields /
 * word-union pattern (as its sibling func_80015F28 = the remove), `entry->id != id`, and func_800145DC() before
 * the increment in the existing-entry path (490 -> 20). Code identical (reloc_blind=0); the remaining true=20 is
 * relocation naming: `&D_80038610[0].id` (splat label D_80038614) and the move loop's `&D_80038610[1]`
 * (D_80038618): reloc_addrs entries (relocname/reloc_addrs_w23c.txt).
 * TU edit needed: the RecordFields_80038610/RecordStorage_80038610/D_80038608/D_80038610 declarations move from the
 * func_80015F28 block to before this slot (tools/tu_edits.py). No devices, no unused locals. */
typedef unsigned short u16;
#pragma pack(1)
typedef struct { unsigned int payload; u16 id; u16 references; } RecordFields_80038610;
#pragma pack()
extern int D_80038608;
typedef union { RecordFields_80038610 fields; unsigned int words[2]; } RecordStorage_80038610;
extern RecordStorage_80038610 D_80038610[];
extern void func_80014594(void);
extern void func_800145DC(void);
int func_80015D68(u16 id, void *payload)
{
    long i;
    long j;
    func_80014594();
    for (i = 0; i < D_80038608 && ((RecordFields_80038610 *)&D_80038610[i])->id < id; ++i) {
    }
    if (i < D_80038608) {
        if (((RecordFields_80038610 *)&D_80038610[i])->id != id) {
            if (D_80038608 < 2048) {
                for (j = D_80038608 - 1; j >= i; --j) {
                    D_80038610[j + 1] = D_80038610[j];
                }
                ++D_80038608;
            } else {
                func_800145DC();
                return 0;
            }
        } else {
            func_800145DC();
            ((RecordFields_80038610 *)&D_80038610[i])->references++;
            return 0;
        }
    } else if (D_80038608 < 2048) {
        ++D_80038608;
    } else {
        func_800145DC();
        return 0;
    }
    ((RecordFields_80038610 *)&D_80038610[i])->id = id;
    ((RecordFields_80038610 *)&D_80038610[i])->payload = (unsigned int)payload;
    ((RecordFields_80038610 *)&D_80038610[i])->references = 1;
    func_800145DC();
    return 1;
}
