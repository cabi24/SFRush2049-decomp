# Source and contract evidence

## MAT3 donor

Read-only source/DWARF lookup dated 2026-10-17 identified `carload.c::LoadCars` at `0x800091B8`, 1,048 bytes. Original DWARF records `int select` and exactly three integer locals: `i`, `j`, `numLoadedCars`. The current reconstructed source and historical independently reviewed wave26 export agree on the control-flow body. No new MAT3 compilation was performed by that lookup.

Donor file hashes:
- carload.c: `4c5a697b49f9988469b077d29eeea8aaa6aa1bd9c7ccb8726b4e19a31b003655`
- fmath.c: `dc2b38176f2cde208697603c632cdffcc62d60ccc1d5da0f8b990fc9af7edbc5`
- waves.c: `b24af063890c9e75948d3f31d5f3910912dcaf8a154c6f129ef40accae9b636d`

The relevant real helper contracts are:

    float Random(float max);
    int FindWaveInstance(int waveNum) {
        return _FindWaveInstance(waveNum, -1);
    }
    static int _FindWaveInstance(int waveNum, signed char handle);
    int LoadWave(int waveNum, int loadAgain, int loadBackground,
                 int handle, void *heap);
    unsigned int gWaveSizes[];

The donor Random computes `max * (rand() & 0x7fff) / 32768.0f`. The native source uses the matching multiplication order with a real `rannum` float local; it and the native rand function remain exact. No invented integer return is used.

## Native adaptations and existing context

The native protected root and callers establish 8-byte selection records, unsigned car byte at +1, signed metadata bytes +2/+3/+4; vehicle stride 2,056 with signed mode at +1,996; signed-short handle array and player count; wave base 88; unsigned heap-free comparison with a 512-byte reserve; and the native buffer setup call. MAT3 has different 10-byte selection records, wave base 64, texture-RAM accounting, and palette setup, so those platform-specific facts are not imported.

The genuine pointer-bearing vehicle layout is supported by current `src/blob/func_800E1C30.c`. The selection layout and buffer effects are documented by the preserved `src/blob/func_800BB02C.c`. The selection overlay call takes no argument; native mode-2 exhaustion performs buffer setup after sharing slot 0. The three payload-table assignments are native additions to the donor flow. Metadata is assigned before any low-memory car rotation and is not silently recomputed afterward.

The unchanged complete heap source is `src/blob/groups/frontier_heap_free_total/group.c`; setup and rand are their canonical single files. Random comes from `cloud/work/frontier/w8c/func_800B23E0/rand_arc_unit.c`, a genuine helper definition whose native body is independently strict. Resource context is the complete `src/blob/groups/codex_state_audio_a146/group.c`, with only the table declaration changed from signed to unsigned. All existing preserved target extents and code comparisons are enforced separately.

## Negative evidence and promotion limit

Without the full resource context, the same natural LoadCars source is 119/343 words different. A donor-derived NewWaveInstance decomposition was also tried privately: it kept the root exact but did not solve LoadWave, and its deleted static helper introduced an unmapped residue. That experiment is excluded from the packet recipe. No guessed native identity was assigned to that helper.

The root match therefore establishes a source/context research result, not complete closure. Promotion requires resolving and independently reviewing the genuine loader context (or another source-proven complete group) without breaking any locked body. This packet intentionally makes no matching claim.
