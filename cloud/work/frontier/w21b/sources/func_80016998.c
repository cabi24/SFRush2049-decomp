/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80016998 - lib_16320 (IDO 7.1). Remove a macro reference (MusyX dataRemoveMacro; control flow from the
 * GameCube MusyX decomp): finds id in main entry D_8003DA28[id >> 6]'s run of the sorted sub table
 * D_8003E228; when its references drop to 0, moves the rest of the sub table down one, decrements every main
 * entry's first index above base (IDO unrolls the 512-entry loop by four), count-1, total D_8003DA20 - 1.
 * Always returns 0. func_80014594/func_800145DC = hwDisableIrq/hwEnableIrq.
 * w21b (first attempt; w20b triage: D10 structure-mismatch, 52 aligned rows): MusyX shape with `long`
 * locals main/base/i and the sub table through the TU's packed-fields/word-union pattern (as func_8001671C),
 * the search test written `entry->id != id` (true=40 -> 20, reloc_blind 0). No devices, no unused locals.
 * Relocations: retail names two compiler-made addresses by splat labels (D_8003E220 = &D_8003E228[-1] in the
 * move loop end, D_8003E228 = &D_8003DA28[512] in the scan loop end): needs the reloc_addrs entries in
 * reloc_addrs_w21b.txt. Against the current target true=20 reloc_blind=0; against the re-split target
 * true=0 objdump-dr identical. Types: the TU's ResourceRange (hoisted by tools/tu_edits.py). */
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
int func_80016998(u16 id)
{
    long main;
    long base;
    long i;
    func_80014594();
    main = id >> 6;
    if (D_8003DA28[main].count != 0) {
        base = D_8003DA28[main].first;
        for (i = 0; i < D_8003DA28[main].count && ((RecordFields_8003E228 *)&D_8003E228[base + i])->id != id; ++i) {
        }
        if (i < D_8003DA28[main].count) {
            if (--((RecordFields_8003E228 *)&D_8003E228[base + i])->references == 0) {
                for (i = base + i + 1; i < D_8003DA20; ++i) {
                    D_8003E228[i - 1] = D_8003E228[i];
                }
                for (i = 0; i < 512; ++i) {
                    if (D_8003DA28[i].first > base) {
                        --D_8003DA28[i].first;
                    }
                }
                --D_8003DA28[main].count;
                --D_8003DA20;
            }
        }
    }
    func_800145DC();
    return 0;
}
