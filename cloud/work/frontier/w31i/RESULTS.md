# w31i — game-side integration + blob_group jump-table rule (wave 31, worktree, branch `w31i`)

Merged wave31 `882c9f77` into w31i (fast-forward). Worktree build/ copied from the main checkout (build/{blob,ovl,us,m2c_asm},
*.json, game_code.bin/.sym, blob_unit/cache, baserom); `blob_splice rebuild-stale --builder-dir ~/rush2049/scratch/frontier/w31i/bs
--group-builder-dir .../bg` rebuilt 101 objects bound to other source (copied objects were stale) before any install (`rebuild_stale.log`).

| Step | Change | New bytes | Game after (`blob_rom coverage`) |
|---|---|---:|---|
| (start) | — | — | 1050/1216, 378,232 B (58.45%) |
| 1a | blob_group `_independent_regions` jump-table branch + tests (tools change) | — | — |
| 1b | splicing-docs paragraph (tools/conveyor/README.md, section 008) | — | — |
| 1c | NEW group `func_8010AEAC` (w30b packet, unchanged) | 3,452 | **1053/1216, 381,684 B (58.99%)** |

## Job 1a: the rule (tools/conveyor/pipeline/blob_group.py)
`_independent_regions(..., address=None)` (verify_own_data passes the member's image address). Old: any relocation in the
section refused (`relocated words (a jump table) need the one-base or jump-table proof`) — still the message when no address
is given. New branch:
- `_own_jump_table_entries`: every relocation of the section must be an aligned R_MIPS_32 against the function's text
  section (section symbol or a function symbol in it); else `relocation type N is not a jump-table entry ...` /
  `relocated word against X is not a jump-table entry into the function's text`.
- `_relocate_own_jump_table`: each entry inside one of THIS function's windows must target [lo, hi) of the function
  (`jump-table entry targets .text+0x.., outside f (...)`) and is rewritten to `address + (target - lo)`; the window
  is then compared whole against retail as before; a mismatch in a relocated window refuses with `(jump-table window,
  N entries relocated) differs from retail 0x... at +0x.. after relocation (retail .., got ..)` (word-aligned).
- Entries outside this function's windows (another member's table, e.g. func_8010A8D0's in the same .rodata) are not
  compared through this function's windows; that member's own proof covers them (A8D0 passes owndata's one-base rule).
- Trailing-zero trimming of the last window happens after relocation, so an entry with a zero addend is never trimmed away.
- One-base path (owndata) untouched; every other independent-regions rule unchanged.
Tests (tests/conveyor/test_owndata.py; the old `test_rodata_regions_refuse_jump_tables` became the passing case):
`test_rodata_regions_accept_a_jump_table_into_the_function` (also: function-symbol entries; no address -> old refusal),
`..._refuse_a_jump_table_entry_outside_the_function`, `..._refuse_a_jump_table_that_differs_after_relocation`,
`..._refuse_other_relocations` (type 4; against .data). `tests/conveyor/test_owndata.py`: all pass.

## Job 1c: func_8010AEAC group (w30b `groups/func_8010AEAC/`, installed unchanged)
- Unit (`unit_job1.log`): `blob_unit --tag w31i score func_8010AEAC func_8010A8D0 func_8010A7A4 --with faeac.c --internal`
  A7A4, A8D0 and the six *_AEAC helpers `--neighbours` -> `EQUAL func_8010AEAC: 413 words (kept, c_faeac.c)`,
  `EQUAL func_8010A8D0: 375 words (internal, c_faeac.c)`, `EQUAL func_8010A7A4: 75 words (internal, c_faeac.c)`,
  `locked bodies that differ in this unit: 0`, `blob_unit score: 3/3 equal`.
- Builder (watchman2 scratch w31i, IDO 5.3 796ae99a, `builder_job1.log`) `score.py group cand/w31i/func_8010AEAC --claims`
  and without: rc=1 only from AEAC's expected line `NOT VERIFIED (4 section-relative relocations unverified: .rodata+0x0
  at +0xd0, .rodata+0x0 at +0xe8, .rodata+0x5c at +0x208, .rodata+0x5c at +0x210; own .rodata: this function's references
  disagree on the section's image address (0x80121018 for +0x0, 0x801248C0 for +0x5c) ...)`; `func_8010A8D0: MATCH`
  (own .rodata 0x801248D8..0x8012491C), `func_8010A7A4: MATCH` (0x801248D4..0x801248D8).
- unit_overrides force_internal += func_8010A7A4, func_8010A8D0, gfx_lock_AEAC, gfx_unlock_AEAC, font_set_AEAC,
  TB_FlashTextColors_AEAC, TB_SetJust_AEAC, TB_CenterText_AEAC (before install).
- Install (`install_job1.log`): `install_group.py cloud/work/frontier/w30b/groups/func_8010AEAC func_8010AEAC` ->
  `spliced func_8010AEAC: 3 members, new: ['func_8010A7A4', 'func_8010A8D0', 'func_8010AEAC']`,
  `image coverage: 1053/1216 functions, 381684/647072 bytes (58.99%)`.
- Gates (`gates_job1.log`, `rom_job1.log`): `group lock: 0 problems`; `blob lock: 1053 entries, 0 problems`;
  `blob_unit: 1053 locked bodies, 1053 equal to the image in one 927-file unit, 0 differ`; `unused_overrides []`;
  `ROM build with the source-built blob: SHA-1 EXACT`, `build/us/rush2049.us.z64: OK`.
- symbols.json / blob.ld unchanged (asm diff = 3 provenance comments + SHA256SUMS): no receipt re-pin.
  Slot-contract audit `audit.py --write`: func_8010A8D0 and func_8010AEAC accepted, unaccepted_caller_bytes 53504 -> 50352;
  frozen list/count in tests/cloud/test_slot_contract_audit.py updated.
- provisional.json: func_8010A7A4 retired (its real caller A8D0 is now locked). All nine ctx_gr3_b.c copies
  byte-identical to credits_scroll_grp/gr3_b.c. Not a scout target. No data-symbol changes.
- CLAIMS: wave-31 landed row added; menus open row now carries w31b's func_800DA2C0 202/579 + DB1E0 pair, func_8010E8B4 3/85.

## Receipt rebind (commit fb90f65f)
The blob_group.py change moves its sha256 (b8faa781 -> a2d1bce8), pinned as `tools_sha256` by four receipts:
dot_material_selector_20261006, dot_billboard_context_20261005, ipa-groups/dot_random_range_b2e4_20261005,
ipa-groups/dot_masked_random_b23e0_20261006 (only that hash changed; precedent eae0c646). Their four test files: 35 passed on rocky.

## Tests
- Pi `make test-fast JOBS=4` at c4c94e49 (main checkout's tools/mips_to_c and tools/decomp-permuter symlinked in temporarily,
  removed afterwards): conveyor `1593 passed, 799 skipped`; cloud `820 passed, 204 skipped`; rc=0.
- rocky clone `~/rush2049/scratch/w31i` (copy of the w30i clone, branch from a bundle, `origin/master` = 880e729e, submodules
  populated, CI venv) `REQUIRE_TOOLCHAIN=1 make test-fast JOBS=16`: at c4c94e49 2 failures (the two receipt tests above,
  tools_sha256 of blob_group.py) -> rebind; at fb90f65f conveyor `2340 passed, 52 skipped`; cloud `1019 passed, 5 skipped,
  8 warnings, 27 subtests passed`; rc=0.
- Builder scratch watchman2:~/rush2049/scratch/frontier/w31i and unit/w31i deleted.
