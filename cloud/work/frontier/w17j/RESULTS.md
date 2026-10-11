# Wave 17, lane w17j: merged packaging of codex_heap_release_a25 (w17h func_800C8918 + stubs, w17i func_80096130)

Status: done. One group dir, `groups/codex_heap_release_a25/`, supersedes the locked group of the same name and both
lane deliveries (w17h, w17i). Every member MATCH in `score.py group` without `--claims`; whole-program unit 0 locked
bodies differing. Nothing committed, spliced or pushed; src/blob untouched.

## Contents
- Old files, byte-identical to src/blob/groups/codex_heap_release_a25 (`cmp` clean): group.c, alloc_at.c,
  car_damage_visual.c, func_800CB9D0.c, deflate_mem.c, overlay_loader.c, wheel_params_set.c.
- func_80091AF0.c, func_80091AF8.c: byte-identical to w17h's.
- func_80096130.c: byte-identical to w17i's (one disclosed `if (slot) {}`).
- func_800C8918.c: w17h's file with ONE packaging change: `post_type1`/`post_type7` are non-static (declarations
  and definitions), plus a header paragraph disclosing it. No other change.
- group.json: members = 16 old + func_800C8918, func_80091AF0, func_80091AF8, func_80096130 (20);
  context unchanged (6); keep = old 17 + func_800C8918, func_80096130; files = old 7 + func_80091AF0.c,
  func_80091AF8.c, func_800C8918.c, func_80096130.c (in that order); claims = [func_800C8918, func_80096130].

## Why w17h's dir was refused, and the fix
blob_group.relocate measures each member from its symbol to the NEXT FUNCTION SYMBOL in the group object and refuses
nonzero words past the member's extent. umerge emits procedures in its own (call-graph) order, independent of
group.json file order (tested 4 orders: the AF8, AF0 stubs always come out adjacent, followed by the two deleted
copies of func_800C8918.c). File-static deleted procedures leave a `jr ra; nop` stub WITHOUT a symbol, so the two
stubs of post_type1/post_type7 were counted as func_80091AF0's tail (`2 extra words`). (func_80096130.c's static
dma_wait stub has the same property: in w17h's file order + func_80096130.c appended it landed after func_800C8918 and
gave `func_800C8918: 1 extra words`; with func_800C8918.c before func_80096130.c it lands after func_800960CC, which is
not a member, so it is harmless and w17i's file stays byte-identical.)
Fix: make post_type1/post_type7 global. They are not on the keep list, so uld -kp internalises them and umerge
inlines/deletes them exactly as before (func_800C8918's body is unchanged: MATCH), but their stubs now carry symbols
(object layout: `0x300 func_80091AF8 8; 0x308 func_80091AF0 8; 0x310 post_type7 8; 0x318 post_type1 8;
0x320 audio_effect_process`), so every member slice stops at the next symbol. Non-member global stubs in the group
object already have precedent (heap_free_locked, mem_free, zcfree from deflate_mem.c). Names checked unique
(`grep -rw post_type1|post_type7` over src include asm: none).

## Verification (current tree 534d2cbe + coordinator's working tree)
Builder (`~/rush2049/scratch/frontier/w17j`, synced src/blob, include, tools, asm/us/blob, lock):
```
python3 tools/cloud/score.py group cand/codex_heap_release_a25      -> exit 0
  all 20 members `MATCH` (incl. func_800C8918: MATCH, func_80091AF0: MATCH, func_80091AF8: MATCH,
  func_80096130: MATCH; InitMaxPath / sync_maxpath_to_checkpoint own .rodata verified), context 6/6 MATCH;
  no MISMATCH, no "extra words".
python3 tools/cloud/score.py group --claims cand/codex_heap_release_a25 -> exit 0
```
Whole-program unit (Pi):
```
python3 -m tools.conveyor.pipeline.blob_unit --tag w17j --remote-dir rush2049/scratch/frontier/w17j --jobs 2 score \
  func_800C8918 func_80096130 func_80091AF0 func_80091AF8 audio_reverb_update audio_effect_process wheel_params_set \
  func_800C885C --internal func_80091AF0 --internal func_80091AF8 --internal post_type1 --internal post_type7 \
  --with <dir>/func_80091AF0.c --with <dir>/func_80091AF8.c --with <dir>/func_800C8918.c --with <dir>/func_80096130.c \
  --neighbours
  EQUAL func_800C8918: 157 words (kept, c_func_800C8918.c)
  EQUAL func_80096130: 66 words (kept, c_func_80096130.c)
  EQUAL func_80091AF0: 2 words (internal, c_func_80091AF0.c)
  EQUAL func_80091AF8: 2 words (internal, c_func_80091AF8.c)
  EQUAL audio_reverb_update: 61 words (internal, g_codex_heap_release_a25__group.c)
  EQUAL audio_effect_process: 23 words (kept, g_codex_heap_release_a25__group.c)
  EQUAL wheel_params_set: 58 words (kept, g_codex_heap_release_a25__wheel_params_set.c)
  EQUAL func_800C885C: 47 words (kept, g_codex_heap_release_a25__group.c)
  locked bodies that differ in this unit: 0
blob_unit score: 8/8 equal; object build/blob_unit/w17j/unit.o (7.3s)
```
(w17h's "stub tail: 4 words of deleted-procedure stubs follow" note on func_80091AF0 no longer prints.)

## Integration (coordinator)
```
PYTHONPATH=. python3 cloud/work/frontier/tools/install_group.py cloud/work/frontier/w17j/groups/codex_heap_release_a25 \
    codex_heap_release_a25 --revert-single func_80091AF0 --revert-single func_80091AF8
```
- `--revert-single` exactly func_80091AF0 and func_80091AF8 (both locked -O2 empty singles src/blob/func_80091AF*.c;
  they become group members). No `--revert-group` (same group name; install_group replaces the dir in place).
  func_800C8918 and func_80096130 are not locked: nothing to revert.
- **No unit_overrides entries needed.** w17h's suggestion (force_internal + prefer_definition like func_80095CF4/CFC)
  applied to the unit run with the singles still locked. blob_unit.derive: after the install the lock names
  func_80091AF0/AF8 as members of codex_heap_release_a25 and blob_splice.revert deletes their single sources, so
  unit_files emits no s_func_80091AF*.c records; their canonical definition is the group file ("locked group member")
  and, being defined by the group and absent from `keep`, they fall in `nonkeep` -> internal. The same derivation
  makes post_type1/post_type7 internal. func_80095CF4/CFC/960CC need overrides only because they stay locked
  singles that are NOT group members (their bodies live in wheel_params_set.c) — leave those entries as they are.
- No data-symbol changes. New game bytes: func_800C8918 628 + func_80096130 264 = +892 B (the two stubs were already
  locked; they change owner only).
- func_80096130 packaging: kept as a group member (not w17i's single alternative). A single is compiled standalone by
  blob_splice, where func_800960CC is an external `jal`, not inlined (w17i: `score.py fn` 66/66), so the single
  could only match in the unit, not install.
- After install run blob_group check, blob_splice check, blob_unit check, blob_rom rom (install_group's own note).

## What generalises
- A file-static helper that umerge inlines and deletes leaves a symbol-less `jr ra; nop`; if umerge places it right
  after a member, the splice gate counts it in that member's extent. Making the helper a non-kept global gives
  identical code and a symbol that ends the member slice. group.json file order does not control umerge's emission
  order in this group.
