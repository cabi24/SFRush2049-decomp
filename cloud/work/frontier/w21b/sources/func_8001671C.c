/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_8001671C - lib_16320 (IDO 7.1). Insert a macro reference (MusyX dataInsertMacro; control flow from the
 * GameCube MusyX decomp): main table D_8003DA28[id >> 6] (count, first index into the sorted sub table), sub
 * table D_8003E228[2048] (payload, id, references), total D_8003DA20. Finds id in the main entry's run; an
 * existing entry gets references+1 and returns 0; otherwise (total < 2048) shifts every main entry's first
 * index above base, moves the sub table up one from pos, writes the new entry (references 1), count+1,
 * total+1, returns 1. func_80014594/func_800145DC = hwDisableIrq/hwEnableIrq.
 * w21b (from w20b best, true=6420): MusyX's statement shape and `long` locals main/pos/base/i (the
 * `pos = base = first = total` chain, the index loop over the 512 main entries that IDO turns into the
 * pointer loop ending at &D_8003DA28[512], `i = total - 1; for (; i >= pos; --i)` for the move); the sub table
 * through the TU's packed-fields/word-union pattern (RecordStorage_8003E228, as func_80015720's table) so the
 * entry move is two aligned word copies; the found test written `entry->id == id` (true=30 -> 20 -> 0
 * reloc_blind). No devices, no unused locals.
 * Relocations: retail names two compiler-made addresses by splat labels (D_8003E228 = &D_8003DA28[512] in the
 * scan loop end, D_8003E230 = &D_8003E228[1] in the move loop end): needs the reloc_addrs entries in
 * reloc_addrs_w21b.txt (reloc_split derive). Against the current target: true=20 reloc_blind=0 (only those
 * two addends); against the re-split target: true=0 objdump-dr identical.
 * Types: the TU's ResourceRange (hoisted by tools/tu_edits.py before this slot). */
typedef unsigned short u16;
#pragma pack(1)
typedef struct ResourceRange { u16 count; u16 first; } ResourceRange;
#pragma pack(0)
#pragma pack(1)
typedef struct { void *payload; u16 id; u16 references; } RecordFields_8003E228;
#pragma pack()
typedef union { RecordFields_8003E228 fields; unsigned int words[2]; } RecordStorage_8003E228;
extern int D_8003DA20;
extern ResourceRange D_8003DA28[512];
extern RecordStorage_8003E228 D_8003E228[];
extern void func_80014594(void);
extern void func_800145DC(void);
int func_8001671C(u16 id, void *payload)
{
    long main;
    long pos;
    long base;
    long i;
    func_80014594();
    main = id >> 6;
    if (D_8003DA28[main].count == 0) {
        pos = base = D_8003DA28[main].first = D_8003DA20;
    } else {
        base = D_8003DA28[main].first;
        for (i = 0; i < D_8003DA28[main].count && ((RecordFields_8003E228 *)&D_8003E228[base + i])->id < id; ++i) {
        }
        if (i < D_8003DA28[main].count) {
            pos = base + i;
            if (((RecordFields_8003E228 *)&D_8003E228[pos])->id == id) {
                ((RecordFields_8003E228 *)&D_8003E228[pos])->references++;
                func_800145DC();
                return 0;
            }
        } else {
            pos = base + i;
        }
    }
    if (D_8003DA20 < 2048) {
        for (i = 0; i < 512; ++i) {
            if (D_8003DA28[i].first > base) {
                D_8003DA28[i].first++;
            }
        }
        i = D_8003DA20 - 1;
        for (; i >= pos; --i) {
            D_8003E228[i + 1] = D_8003E228[i];
        }
        ((RecordFields_8003E228 *)&D_8003E228[pos])->id = id;
        ((RecordFields_8003E228 *)&D_8003E228[pos])->payload = payload;
        ((RecordFields_8003E228 *)&D_8003E228[pos])->references = 1;
        D_8003DA28[main].count++;
        D_8003DA20++;
        func_800145DC();
        return 1;
    }
    func_800145DC();
    return 0;
}
