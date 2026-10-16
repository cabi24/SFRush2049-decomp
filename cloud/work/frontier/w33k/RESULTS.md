# w33k — game-side integration of w33b + w33a (wave 33, worktree, branch `w33k`)

Worktree `.claude/worktrees/w33k`, branch `w33k` from `wave33` = `412d5efdf` (w33j landing included). The two lanes'
uncommitted dirs were copied from the main checkout (git-ignored `v/` excluded) and committed first: `w33b: lane results`
(39a93d855), `w33a: lane results` (ecfc03575). build/ inputs were copied from the main checkout (build/{blob,ovl,us,m2c_asm},
build/*.json, game_code.bin/.sym, blob_unit/cache; baserom symlinked). `blob_splice rebuild-stale --builder-dir
~/rush2049/scratch/frontier/w33k/bs --group-builder-dir .../bg` rebuilt 58 singles + 49 groups (`rebuild_stale.log`; dry run
afterwards: `0 stale singles, 0 stale groups`). Builder scratch watchman2:~/rush2049/scratch/frontier/w33k (copy of base +
rsync of src/blob, src/rom, include, tools/cloud, tools/conveyor, asm/us/blob, asm/us/boot_tail, blob_matched.lock.json;
toolkit via `trace/install.sh --reuse wtk`; `IDO_DIR` = toolkit 796ae99a IDO 5.3; unit remote dir
`rush2049/scratch/frontier/w33k/unit`) — deleted at the end.

| Step | Change | New bytes | Game after (`blob_rom coverage`) |
|---|---|---:|---|
| (start) | — | — | 1072/1216, 399,916 B (61.80%) |
| 1 | `frontier_optionscr_buttons` superseded in place (w33b): + func_800DA0BC | 184 | 1073/1216, 400,100 B (61.83%) |
| 2 | NEW group `highscore_anim` (w33a): DrawName + DrawGhost + DrawUnlocked, provisional on the caller form | 5,960 | **1076/1216, 406,060 B (62.75%)** |

Total +6,144 B, as the lanes predicted. Static (632/667, 151,176 B, 94.06%), overlay A (81/193, 51,060 B) and overlay B
(21/49, 9,020 B) unchanged. `coverage_before.log`, `coverage_job1.log`, `coverage_after.log`.

## Scores before install
Builder (`builder.log`), `score.py group cand/<group>` with and without `--claims` (the same output both ways):
```
frontier_optionscr_buttons:
  func_800DB1E0: MATCH   (own .rodata verified at 0x80124248..0x80124280)
  func_800DA2C0: NOT VERIFIED (32 section-relative relocations unverified ...)
                 own .bss placed at 0x8015273C / 0x80152740 (4 bytes; zero-initialised: verified by address only)
  func_800DA0BC: MATCH
  rc=1
highscore_anim:
  DrawName: MATCH
  DrawGhost: NOT VERIFIED (6 section-relative relocations unverified ...; own .rodata ... (0x80120650 for +0x4/+0x14, 0x8012485C for +0x24) ...)
  DrawUnlocked: MATCH
  Context (informational): highscore_animText 27/445, slot_state_setup 2/58, object_create 28/28, func_800B669C 5/5
  rc=1
```
rc=1 comes only from the two known two-region NOT VERIFIED lines (func_800DA2C0: w32b; DrawGhost: w33a); the install's
splice proof (`blob_group.group_bodies`) and the image gate are the gate for them, as in w32i. (w33a quoted rc=0 for its
group: that was a piped exit status; the real status is 1, for the expected reason.) highscore_anim was scored twice: as
delivered, and after the w33k header paragraph (below) — identical.

Unit (`unit_pre.log`):
```
job1: blob_unit --tag w33k score func_800DA0BC func_800DB1E0 func_800DA2C0 --with .../buttons.c --internal func_800DA0BC
      --internal func_800DA2C0 --internal ZOID_SetColor_da --internal ZOID_SetColorP_da --neighbours
  EQUAL func_800DA0BC: 46 words (internal, c_buttons.c)
  EQUAL func_800DB1E0: 350 words (kept, c_buttons.c)
  EQUAL func_800DA2C0: 579 words (internal, c_buttons.c); own zero-initialised data (addresses consistent, ...)
  locked bodies that differ in this unit: 0          blob_unit score: 3/3 equal
job2: blob_unit --tag w33k score highscore_animText DrawName DrawGhost DrawUnlocked --with .../highscore.c
      $(cat cloud/work/frontier/w33a/hs/uargs_hs.txt) --neighbours
  FAIL highscore_animText: 27 of 445 words differ      (context: the EQUIVALENT caller draft, expected)
  EQUAL DrawName: 260 words (internal, c_highscore.c)
  EQUAL DrawGhost: 601 words (internal, c_highscore.c)
  EQUAL DrawUnlocked: 629 words (internal, c_highscore.c)
  locked bodies that differ in this unit: 0
```
Packet checks: w33b buttons.c = installed buttons.c + 4 header lines + the func_800DA0BC block (declarations and body
inserted before ZOID_SetColorP_da; `diff` shows nothing else); w33a `ctx_gr3_b.c` `cmp`-identical to locked
`frontier_results_table/ctx_gr3_b.c`; DrawName's `p in s5` checked in `tools/recomp/ipa_callees.json` (`"params": ["s5"]`,
w31r regmap_verify).

## Install and gates
```
python3 - (remove the "func_800DA0BC" row from cloud/work/frontier/provisional.json; indent 1, diff = 4 removed lines)
PYTHONPATH=. python3 cloud/work/frontier/tools/install_group.py cloud/work/frontier/w33b/groups/frontier_optionscr_buttons frontier_optionscr_buttons
python3 -m tools.conveyor.pipeline.blob_group check ; blob_splice check ; lock check
python3 -m tools.conveyor.pipeline.blob_unit manifest
python3 -m tools.conveyor.pipeline.blob_unit --tag w33k --remote-dir rush2049/scratch/frontier/w33k/unit check
python3 -m tools.conveyor.pipeline.blob_rom produce ; blob_rom rom ; blob_rom coverage
PYTHONPATH=. python3 cloud/work/frontier/tools/install_group.py cloud/work/frontier/w33a/groups/highscore_anim highscore_anim
(same gate chain)
python3 -m tools.conveyor.pipeline.blob_layout derive
python3 -m tools.conveyor.pipeline.blob_tu symbols
python3 -m tools.conveyor.pipeline.blob_splice check
python3 cloud/work/frontier/dot_slot_contract_audit_20261006/audit.py --write
```
- Job 1 (`install_job1.log`): `spliced frontier_optionscr_buttons: 3 members, new: ['func_800DA0BC']`, `image coverage:
  1073/1216 functions, 400100/647072 bytes (61.83%)`. Gates (`gates_job1.log`, `rom_job1.log`): `group lock: 0 problems`;
  `blob lock: 1073 entries, 0 problems`; `all 635 locked functions intact`; `blob_unit: 1073 locked bodies, 1073 equal to the
  image in one 939-file unit, 0 differ [1034 plain; stub tail 35, own bss 5; ...]`; `byte-identical to the ROM's stream: True`;
  `ROM build with the source-built blob: SHA-1 EXACT`, `build/us/rush2049.us.z64: OK`, `ROM matches!`, `MAKE=0 TEST=0`.
- Job 2 (`install_job2.log`): `spliced highscore_anim: 3 members, new: ['DrawGhost', 'DrawName', 'DrawUnlocked']`, `image
  coverage: 1076/1216 functions, 406060/647072 bytes (62.75%)`. Gates (`gates_job2.log`, `produce.log`, `rom_job2.log`):
  `group lock: 0 problems`; `blob lock: 1076 entries, 0 problems`; `all 635 locked functions intact`; `blob_unit: 1076 locked
  bodies, 1076 equal to the image in one 941-file unit, 0 differ [1037 plain; stub tail 35, own bss 5; 466 own .rodata/.data
  references in 143 bodies checked against image bytes] (958 kept, 348 internal)`; `blob_rom: image 647072 B (1076 spliced) ->
  blob 326180 B sha256 9e632d21b7d0d293…`, `byte-identical to the ROM's stream: True`; **`ROM build with the source-built
  blob: SHA-1 EXACT`**, `build/us/rush2049.us.z64: OK`, `ROM matches!`, `MAKE=0 TEST=0`. Final `blob_splice check`:
  `blob lock: 1076 entries, 0 problems`.
- unit_overrides: **none added.** The regenerated manifest derives func_800DA0BC, DrawName, DrawGhost, DrawUnlocked and the
  helpers (du_font_set, hs_SetFont, gh_font_set, DrawScores, DrawCircuitCode) internal, highscore_animText keep;
  `unused_overrides []` (as w33i/w33j found once the group is locked).
- Lock diff vs the start (`blob_matched.lock.json`): exactly 4 added entries (func_800DA0BC, DrawName, DrawGhost, DrawUnlocked),
  0 removed; 2 changed = the superseded group's old members func_800DB1E0 and func_800DA2C0, only `source_sha256`
  (348adfd0… -> 905ce54d…, the new buttons.c) and `verified_at`. No other locked body changed. No receipt pins the old hash.
- Layout/symbols (`layout_symbols.log`): `functions: 1216 561860 bytes (86.8%)`; symbols.json and blob.ld UNCHANGED (all four
  names were already in the symbol table); the asm diff is four `/* compiled from src/blob/... */` provenance comments
  (blob_800d6690.s: func_800DA0BC; blob_80100d24.s: DrawUnlocked, DrawName, DrawGhost) + SHA256SUMS. No data-symbol changes.
- Slot-contract audit `--write`: DrawUnlocked, DrawName, DrawGhost become accepted callers, `unaccepted_caller_bytes` 45332 ->
  39372 (evidence.json, 7 lines). `tests/cloud/test_slot_contract_audit.py` freezes that list and count, so it failed after the
  write (and also without it: `test_fresh_source_bound_census` raises `source-bound audit differs: summary` against the old
  receipt). I updated the frozen list (+ the three names, in the audit's order) and the count, as w32i did for its landings —
  NOTE: tests/ is outside this lane's stated edit scope; this is the one deviation, disclosed here (2 lines; `slot_audit_write.diff`
  holds the evidence change).

## highscore_anim caveat (coordinator decision)
The three members land **provisional on the caller form** (wave-32 control_settings-cells precedent): byte-exact members,
caller highscore_animText an EQUIVALENT draft (27/445, frame 704 vs 584) kept as context, regmap-verified conventions
(DrawName p in s5). Recorded in group.json `provenance` (w33a), in a new w33k paragraph at the top of the highscore.c header
(added in the packet before install; re-scored on the builder and in the unit afterwards — identical) and in CLAIMS.md. No
provisional.json entries (real locked matches). When highscore_animText closes, it joins this group as a member.

## Tests
Pi, worktree, `make test-fast JOBS=4` (`test_fast.log`; tools/mips_to_c and tools/decomp-permuter symlinked to the main
checkout's for the run, empty dirs restored afterwards): conveyor `1 failed, 1591 passed, 800 skipped`; cloud `854 passed,
207 skipped`; rc=2. The one failure is the expected one from w33j:
`tests/conveyor/test_entity_vector_boundaries.py::test_native_metadata_and_call_contract`
(`dot_entity_vector_boundaries_20261005/verify.py:214  assert FN not in locks ...`: entity_iterate is locked since w33j) —
owner/coordinator re-pin pending; the receipt was not touched.

## Bookkeeping
- `cloud/work/frontier/provisional.json`: func_800DA0BC (w14ob) row removed.
- `cloud/work/CLAIMS.md`: new "wave 33 landed (w33b/w33a via w33k)" row (the four functions, devices, the provisional-on-caller
  caveat, GC name lead func_800DA0BC = uninitScreen@optionscr.c, the still-open entity_iterate receipt); the menus row's
  "Lead: func_800DA0BC" replaced by its landing + w33b's controls/videoscr progress; the results/high-score row's
  highscore_animText / DrawUnlocked / animResults text updated from w33a RESULTS.
- Builder scratch watchman2:~/rush2049/scratch/frontier/w33k (incl. unit/) deleted. `install_group.py` and `blob_rom rom` use
  their fixed builder dirs (/tmp/blobgroup, ~/rush2049/repo) as in every integration lane.
- Permission denials: none.
