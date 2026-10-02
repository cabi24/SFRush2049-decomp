# func_800F56E0 assembly audit

Independent audit of native target, 2026-10-02. Authoritative read: `python3 cloud/work/tools/tdis.py func_800F56E0`. Extent 0x800F56E0..0x800F5EF8, 518 words / 2072 bytes. Subsequent independent candidate compilation confirmed the final matching source; shared integration belongs to the parent coordinator.

## Entry and globals

No argument or nonstandard register is read at entry. Function returns no meaningful value. Stack frame 160 bytes, saves ra and s0..s8, no saved FPU registers. All floating constants are reloaded after sole call.

- `D_8014978C` (0x8014978C) signed byte: base selector. Initially both record index and persistence selector equal it.
- `D_80152570` signed byte: if nonzero, record index += 6 and persistence selector += 19 (0x800F5728..5730).
- `active_player_count` (0x8014A108), signed halfword, bounds outer player loop.
- `input_rec0` (0x8014A118), observed stride 76, offset0 unsigned player/model index, offset1 unsigned resource selector, offset64 u16 aggregate count, offset72 handle pointer.
- `D_80146150`, array of pointers to resource objects. Missing handle is initialized to address of entry `[record->selector]` (0x800F578C..57A0).
- Resource chain: handle is pointer to object pointer; object +44 is resource pointer; resource +0 is byte data pointer. Object +8 is optional persistence/reference pointer.
- `D_80144018[player]`: unsigned lap count; `D_80149A78[player][lap]`: float times, observed row stride32 / eight floats.
- `D_80150F88`: global 96-byte statistics rows, indexed by record index.
- `D_80151690`: handle reference rows, observed row stride60 / 15 pointers, indexed by record index. Only positions0..9 appear here.
- `D_80151AC0`: scratch player IDs for rankings; no selector scaling. Individual ranking positions0..4 and average positions5..9 are used.
- `player_array` (0x80152818): 952-byte rows, offset238 signed rank, offset239 signed finished flag, offset264 float distance-like accumulator.
- `D_80152744` signed byte total cars; `D_80152734` signed halfword target lap count; `D_80142760` signed byte extra mode flag.

## Regions and original loops

0x800F56E0..5774: initialize selectors, outer `for (player=0; player<active_player_count; player++)`. Hoisted constants: float 528.0f in f16, float zero independently in f14 and f2, integer4 s8, integer5 s2, integer1 t3. The record index is in ra, a general register, and survives call via stack156. This alone does not indicate IPA.

0x800F5778..57CC: ensure handle, return immediately if object->resource is null, compute recordIndex*96 once, materialize persisted selector as low unsigned byte, and lap-count pointer. This guard exits entire function, rather than skipping that player.

0x800F57D0..5834: two passes. Pass0 stats = resource->data +140+96*recordIndex; pass1 stats = D_80150F88+96*recordIndex. Recompute model pointer each pass from record->index. All following updates happen twice, once to each stats row.

0x800F5838..5A44: for every completed lap, search positions0..4 for first `old==0.0f || new<old`; ties are excluded. If found, shift positions4 down through position+1, write new time, and break position search. Zero means empty even for negative/NaN new time. Backward shift is a single natural loop with compiler peel plus unroll4; do not hand model peel branches. The float array starts at stats+4. The metadata shifts/insertion happen only in pass1: player ID at scratch[pos], handle at refs[recordIndex][pos] when `(*handle)->optional8` is nonnull. In every shift iteration the float is copied before the guarded metadata copy. Metadata store order differs between generated unrolled lanes; trust compiler scheduling. Lap count is reread after insertion and governs continued outer lap iteration.

0x800F5A48..5AA8: if lap count>0, compute a fresh sum initialized0.0f and average = sum/(f32)(u32)lapcount. The unsigned cast is real: bgez +2^32. The code tests count again before sum loop. The per-lap sum loop adds sequentially, so avoid reassociation.

0x800F5AAC..5C78: same search and backward shift for average, positions0..4 at stats+24. Guard `old==0.0f || avg<old`; metadata positions shifted/inserted at scratch[pos+5] and refs[recordIndex][pos+5]. One insertion then exit search. Original average remains live f12 across shifts.

0x800F5C7C..5D58: finish/rank counters. `which++` occurs here before tail but condition remains equivalent to two-pass for loop.

```
if (model->finished) {
    stats->finished70++;
    if (model->rank == 0 && numCars >= 2) stats->first72++;
    else if (model->rank == 1 && numCars >= 3) stats->second74++;
    else if (model->rank == 2 && numCars >= 4) stats->third76++;
    if (D_80142760) {
        stats->enabled80++;
        if (D_80152734 == lapCount) stats->enabledFinished82++;
    }
}
```

0x800F5D5C..5DBC: unconditionally `stats->sum78 += record->count64; stats->count68 += lapCount;` then another fresh `for (j=0;j<lapCount;j++) stats->total64 += times[player][j];`. This updates total with a store after every lap. It must not reuse the average sum since each float addition rounds against an existing accumulator. Lap-count byte reloads in loop condition allow aliasing after stores.

0x800F5DBC..5E74: `stats->distance88 = (u32)((f32)stats->distance88 + model->distance264 / 528.0f);` (or native compound +=). Existing field is u32: conversion includes bgez +2^32. Final conversion is u32, not s32: real cfc1/ctc1 retry around2^31, restoring FCSR. 528.0f is exactly 0x44040000. The conversion failure cases and rounding behavior belong to compiler cast expansion, not bespoke C code.

0x800F5E70: repeat from57D0 if pass counter !=2; distance store occurs in branch delay slot.

0x800F5E78..5EC4: persist caller's record handle and low u8 persistence selector, advance player index and record pointer76, reload active count, restore hoisted float/base/integer constants, next outer iteration.

0x800F5EC8..5EF4: common epilogue, including early resource guard return.

## Original stack evidence

Offsets24..60 are saved s0..s8 and ra. Persistent source locals/temps:68 word containing low byte selector (read big-endian at71);100 recordIndex*96;140 model pointer;152 persistence selector (read low byte at155);156 recordIndex spill across call. Areas72..96,104..136,144..148 are untouched by target. Their origin remains unexplained. Do not add pressure padding or invented unused arrays to force this frame.

## func_800CD8EC entry contract

Native extent93 words, 372 bytes. Caller at0x800F5E80 loads only a0=record->handle and a1=(u8)selector. Callee stores a1 home slot, masks0xff, and derives everything from these two arguments. No hidden input. Public candidate declaration `extern void func_800CD8EC(Handle *, u8);` is justified.

Internally this callee holds handle in t0 and record pointer in a3 across calls to format_string_parse/slot_state_lookup; these internals belong to its own IPA/context problem and do not impose nonstandard parameters on F56E0. It selects96-byte rows for mode0..5 or19..24, computes checksum over next92 bytes, writes checksum at row0, and persists96 bytes. For19..24 its row offset is96*mode-1108 =140+96*(mode-13), agreeing with caller's base+6 row index. Other modes use distinct12/64/24-byte records. Existing `cloud/work/ipa-groups/codex_records_a90/group.c` already has full natural source, but this callee is not asserted matched here.

## Pitfalls and provenance

`build/m2c_asm/func_800F56E0.s` is wrong at the record handle/index reads: it replaces `lw v0,72(s6)` with a global alias expansion and inserts unlabelled extra instructions after5778 and57A0; it also corrupts578C,57D8,59E8,5C34,5E7C. This follows old shared D_8014A160 context whose address coincides with input_rec0+72. All C must follow raw native disassembly rather than those expanded aliases.

Arcade search of stats.c/hiscore.c found NVRAM game accounting, not an equivalent per-player two-record leaderboard updater. This target appears N64-specific from its explicit player records and resource persistence. Semantic naming above is inferred from repeated float min insertion, unsigned counters, and the528.0 distance scale; generic observed offsets remain appropriate where no source evidence exists.

## Final candidate review and independent compile

Reviewed `reconstruction/both_course_first.c`, then normalized `cloud/matches/func_800F56E0.c`: complete semantics agree with the native target. Mean division uses explicit u32 count conversion; live global sample counts remain available after insertion stores; sum accumulation stores after every sample; resource and aggregate records receive every update; all observed early returns and thresholds remain intact. The final average insertion uses the exhausted backward-shift counter for its player-tag address; that counter always equals the insertion rank, including the rank4 loop-skipping case.

Independent compile with `-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul` returned canonical **MATCH**, zero of518 native words differ, no unresolved or unverified relocations, no errors, no extra nonzero words. Frame160. Emitted520 words include two trailing zero alignment words. Unnormalized source SHA256 `0223e18754472236da63d688b757ec80a94da72664dab12af05f825aa1ae2a4f`; independent object SHA256 `0729e802ac6e2bd2f1af56cbc6b7ef81d8f0db23451b1c040e23e9615331a67e`. Sanitized evidence is `audit/entry_results/final_verified.json`. Raw word arrays are retained only in ignored `build/large_func_800F56E0/audit-artifacts/`.

Useful compiler controls were ordinary source choices: independent global reads for the two selector locals, source order of their alternate-bank increments, direct indexed player-record accesses, integer spelling of the real zero mean initializer, reuse of the actual shift counter for the two sums, and player-tag assignment before owner-reference assignment. No dummy inputs, artificial frame padding, unreachable operations, or target instructions were introduced into C.
