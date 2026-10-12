/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80015A0C - lib_16320 (IDO 7.1). MusyX synthdata.c dataInsertLayer (doldecomp/ttyd libs/musyx; shape only):
 * with interrupts off (func_80014594/func_800145DC), find ID's sorted position in the layer table D_8003CE20
 * (D_8003CE18 entries, max 256); an existing entry gets its reference count bumped (returns 0), otherwise the
 * tail moves up one entry and the new entry {payload, id, type, references 1} is written (returns 1).
 * w23c (D10 BT03-low-final-resource true=2600): MusyX's statement shape with `long` locals, the TU's packed-fields /
 * word-union pattern (as its sibling func_80015C0C = dataRemoveLayer), the found-entry test written
 * `entry->id != id` (operand order). Code identical (reloc_blind=0); the remaining true=20 is relocation naming:
 * the first `.id` read is `&D_8003CE20[0].id` (splat label D_8003CE24) and the move loop's end pointer
 * `&D_8003CE20[1]` (D_8003CE2C): reloc_addrs entries (relocname/reloc_addrs_w23c.txt), as w21b's func_8001671C.
 * TU edit needed: the RecordFields_8003CE20/RecordStorage_8003CE20/D_8003CE18/D_8003CE20 declarations move from the
 * func_80015C0C block to before this slot (their first user). No devices, no unused locals. */
typedef unsigned short u16;
#pragma pack(1)
typedef struct { unsigned int payload; u16 id; u16 type; u16 references; u16 metadata; } RecordFields_8003CE20;
#pragma pack()
extern int D_8003CE18;
typedef union { RecordFields_8003CE20 fields; unsigned int words[3]; } RecordStorage_8003CE20;
extern RecordStorage_8003CE20 D_8003CE20[];
extern void func_80014594(void);
extern void func_800145DC(void);
int func_80015A0C(u16 id, void *payload, u16 type)
{
    long i;
    long j;
    func_80014594();
    for (i = 0; i < D_8003CE18 && ((RecordFields_8003CE20 *)&D_8003CE20[i])->id < id; ++i) {
    }
    if (i < D_8003CE18) {
        if (((RecordFields_8003CE20 *)&D_8003CE20[i])->id != id) {
            if (D_8003CE18 < 256) {
                for (j = D_8003CE18 - 1; j >= i; --j) {
                    D_8003CE20[j + 1] = D_8003CE20[j];
                }
                ++D_8003CE18;
            } else {
                func_800145DC();
                return 0;
            }
        } else {
            ((RecordFields_8003CE20 *)&D_8003CE20[i])->references++;
            func_800145DC();
            return 0;
        }
    } else if (D_8003CE18 < 256) {
        ++D_8003CE18;
    } else {
        func_800145DC();
        return 0;
    }
    ((RecordFields_8003CE20 *)&D_8003CE20[i])->id = id;
    ((RecordFields_8003CE20 *)&D_8003CE20[i])->payload = (unsigned int)payload;
    ((RecordFields_8003CE20 *)&D_8003CE20[i])->type = type;
    ((RecordFields_8003CE20 *)&D_8003CE20[i])->references = 1;
    func_800145DC();
    return 1;
}
