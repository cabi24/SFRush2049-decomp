# w33l — game-side integration of w33d control_settings (wave 33, worktree, branch `w33l`)

Worktree `.claude/worktrees/w33l`, branch `w33l` from `wave33` = `24f19f24d`. w33d's uncommitted lane dir was copied from the
main checkout (git-ignored `v/` excluded) and committed first: `w33d: lane results` (5ec2c1e53). build/ inputs copied from
the main checkout (build/{blob,ovl,us,m2c_asm}, build/*.json, game_code.bin/.sym, blob_unit/cache; baserom symlinked).
`blob_splice rebuild-stale --builder-dir ~/rush2049/scratch/frontier/w33l/bs --group-builder-dir .../bg` rebuilt 58 singles +
50 groups (`rebuild_stale.log`; dry run afterwards: `0 stale singles, 0 stale groups`). The copied `build/blob_unit.json` was
stale (Oct 12); `blob_unit manifest` regenerated it after the install. Builder scratch watchman2:~/rush2049/scratch/frontier/w33l
(copy of base + rsync of src/blob, src/rom, include, tools/cloud, tools/conveyor, asm/us/blob, asm/us/boot_tail,
blob_matched.lock.json; toolkit via `trace/install.sh --reuse wtk`; `IDO_DIR` = toolkit 796ae99a IDO 5.3; unit remote dir
`rush2049/scratch/frontier/w33l/unit`) — deleted at the end.

| Step | Change | New bytes | Game after (`blob_rom coverage`) |
|---|---|---:|---|
| (start) | — | — | 1076/1216, 406,060 B (62.75%) |
| 1 | `codex_control_settings_a4` superseded in place (w33d): control_settings (0x800DD4AC, keep) context -> member | 2,552 | **1077/1216, 408,612 B (63.15%)** |

Static (632/667, 151,176 B, 94.06%), overlay A (81/193, 51,060 B) and overlay B (21/49, 9,020 B) unchanged.
`coverage_before.log`, `coverage_after.log`.

## Packet checks
- group.json vs installed: members + control_settings; context - control_settings (slot_state_setup, object_create,
  func_800B669C unchanged); keep [control_settings], files [group.c, ctx_gr3_b.c], flags unchanged; `claims` (all seven) added
  in the packet and stripped by install_group.py as usual; provenance "w29c/w30d/w31d/w32d drafts -> w33d; ...".
- ctx_gr3_b.c `cmp`-identical to the installed copy and to src/blob/groups/credits_scroll_grp/gr3_b.c.
- func_800DCCE0 / func_800DCD58 bodies textually identical to the installed group.c (brace-range extraction + diff).
- control_cell_text2/text3: body text identical, parameter lists permuted (old text2 `(sel, row, col, a, b, text)` -> new
  `(row, col, text, a, b, sel)`; old text3 `(sel1, row, text, s1, s2, s3, col, sel2)` -> new `(row, col, text, s1, s2, s3,
  sel1, sel2)`); the compiled bodies stay byte-identical (group MATCH, unit EQUAL, lock entries changed only by the file hash).
- No "provisional on the caller form" text in the packet's header or provenance (grep), and none in provisional.json for any
  of the seven.

## Scores before install
Builder (`builder.log`), `score.py group cand/codex_control_settings_a4` with and without `--claims` (identical, rc=0 both):
```
func_800DCD58: MATCH / func_800DCCE0: MATCH / control_cell_text: MATCH / control_cell_text2: MATCH /
control_cell_text3: MATCH / func_800DD0C0: MATCH /
control_settings: MATCH
    own .rodata verified at 0x801242CC..0x80124308
Context (informational): slot_state_setup 2/58, object_create 28/28, func_800B669C 5/5
rc=0
```
(The 15-entry jump table is a single own-.rodata window and verifies directly; no NOT VERIFIED line.)
Unit (`unit_pre.log`; packet with helpers renamed `_DC` -> `_DQ` as in w33d's run.sh, `--internal` for the four cells and
every helper):
```
EQUAL control_settings: 638 words (kept, c_w33l_cand.c)
EQUAL control_cell_text: 66 words / control_cell_text3: 205 / control_cell_text2: 173 / func_800DD0C0: 227 /
EQUAL func_800DCD58: 39 / func_800DCCE0: 28   (all internal, c_w33l_cand.c)
locked bodies that differ in this unit: 0
blob_unit score: 7/7 equal
```

## Install and gates
```
PYTHONPATH=. python3 cloud/work/frontier/tools/install_group.py cloud/work/frontier/w33d/groups/codex_control_settings_a4 codex_control_settings_a4
python3 -m tools.conveyor.pipeline.blob_group check ; blob_splice check ; lock check
python3 -m tools.conveyor.pipeline.blob_unit manifest
python3 -m tools.conveyor.pipeline.blob_unit --tag w33l --remote-dir rush2049/scratch/frontier/w33l/unit check
python3 -m tools.conveyor.pipeline.blob_rom produce ; blob_rom rom ; blob_rom coverage
python3 -m tools.conveyor.pipeline.blob_layout derive
python3 -m tools.conveyor.pipeline.blob_tu symbols
python3 -m tools.conveyor.pipeline.blob_splice check
python3 cloud/work/frontier/dot_slot_contract_audit_20261006/audit.py --write
```
- Install (`install.log`): `spliced codex_control_settings_a4: 7 members, new: ['control_settings']`, `image coverage:
  1077/1216 functions, 408612/647072 bytes (63.15%)`.
- Gates (`gates.log`, `produce.log`, `rom.log`): `group lock: 0 problems`; `blob lock: 1077 entries, 0 problems`; `all 635
  locked functions intact`; manifest `940 files, 1077 locked bodies, 1318 functions (957 kept, 361 internal)`; `blob_unit:
  1077 locked bodies, 1077 equal to the image in one 941-file unit, 0 differ [1038 plain; stub tail 35, own bss 5; 467 own
  .rodata/.data references in 144 bodies checked against image bytes] (958 kept, 361 internal)`; `blob_rom: image 647072 B
  (1077 spliced) -> blob 326180 B sha256 9e632d21b7d0d293…`, `byte-identical to the ROM's stream: True`;
  **`ROM build with the source-built blob: SHA-1 EXACT`**, `build/us/rush2049.us.z64: OK`, `ROM matches!`, `MAKE=0 TEST=0`.
  Final `blob_splice check`: `blob lock: 1077 entries, 0 problems`.
- unit_overrides: **none added.** The regenerated manifest derives all 13 new helpers (fsS_DC, fsL_DC, TB_CenterText_DC,
  cs_draw1_DC, cs_draw3_DC, cs_drawT0..T3_DC, cs_wait11..13_DC, cs_ctl10_DC) internal and control_settings keep;
  `unused_overrides []`. (w33d's suggested force_internal additions are unnecessary once the group is locked, as w33i–w33k found.)
  The existing wave-32 entries (control_cell_text/2/3, func_800DD0C0, gfx_lock_DC, gfx_unlock_DC, font_set_DC, font_set1_DC,
  pair_set_DC) are still used (not in unused_overrides) and were left as they are.
- Lock diff vs the start (`blob_matched.lock.json`): exactly 1 added entry (control_settings, group codex_control_settings_a4,
  `verified: image_gate`), 0 removed; 6 changed = the group's old members (func_800DCD58, func_800DCCE0, control_cell_text,
  control_cell_text2, control_cell_text3, func_800DD0C0), only `source_sha256` (530ac97c… -> 5e817976…, the new group.json/
  group.c) and `verified_at`. No other locked body changed.
- Layout/symbols (`layout_symbols.log`): `functions: 1216 561860 bytes (86.8%)`; symbols.json and blob.ld UNCHANGED; the asm
  diff is one `/* compiled from ... */` provenance comment (blob_800d6690.s: control_settings) + SHA256SUMS. No data-symbol changes.
- Stale prototypes: the eight other TUs that declare uncalled m2c prototypes of control_cell_text2/text3
  (Effects_UpdateEmitters.c, menu_confirm_dialog.c, func_80092D80.c, menu_render_list.c, object_byte71_set_sync.c,
  func_800FDF88.c, groups codex_input_aux / func_800E56F8 / audio_frame_sync group.c) did NOT trouble the build, the unit
  (0 differ), `blob_tu symbols` or any gate — they already disagreed with the wave-32 prototypes. Nothing regenerated or edited.
- Slot-contract audit `--write`: control_settings becomes an accepted caller, `unaccepted_caller_bytes` 39372 -> 36820
  (evidence.json, 3 lines). `tests/cloud/test_slot_contract_audit.py` frozen list (+ control_settings after func_800DD0C0, in the
  audit's order) and count updated as w32i/w33k did (`slot_audit_write.diff` holds both diffs).

## Harness register map vs the new prototype order (for w33r; tools/recomp NOT edited)
- `tools/recomp/ipa_callees.json` has NO entry for control_cell_text2/text3 (only func_800DD0C0: `params ["sp+0","sp+4"]`,
  prototype `(s32 row, s32 col)`, unchanged by this packet). The `sel=s5,row=s6,col=s7,text=s8` map lives in
  `cloud/work/frontier/w28d/attract_video_handler/dc.args` (and text3's `sel1=s5,row=s6,text=sp+8,s1=sp+12,s2=sp+16,s3=sp+20,
  col=s7,sel2=s8` in `w28d/attract_demo_handler/dc.args`; both quoted in tools/recomp/README.md ~l.1093).
- draft_check's `--params` is keyed by parameter NAME (draft_check.py:126–137), and the new bodies use every name with the same
  meaning as before (body text identical, callee colouring unchanged), so the name -> register maps stay valid under the new
  prototype order: no contradiction. Only the prototype POSITIONS moved; any future positional `ipa_callees.json` entry for
  these two must list registers in the new order (text2: s6,s7,s8,?,?,s5 — a/b are not read by the callee, per the w31r note).

## CLAIMS.md
- New "wave 33 landed (w33d via w33l)" row for control_settings (devices, levers, no overrides, audit change).
- The wave-32 row's "**PROVISIONAL on control_settings' caller form** (...)" sentence replaced by a note that the caveat was
  removed in wave 33; the race/world pool row's "(wave-32 row, provisional on the caller)" likewise, and that row now opens
  with "control_settings LANDED wave 33". The highscore_anim caveat (different caller, highscore_animText) is kept.

## Tests
Pi, worktree, `make test-fast JOBS=4` (`test_fast.log`; tools/mips_to_c and tools/decomp-permuter symlinked to the main
checkout's for the run, empty dirs restored afterwards): conveyor `1 failed, 1591 passed, 800 skipped`; cloud `1 failed, 853
passed, 207 skipped`; rc=2.
1. Expected: `tests/conveyor/test_entity_vector_boundaries.py::test_native_metadata_and_call_contract` (entity_iterate locked
   since w33j; owner decision pending; receipt untouched).
2. **NEW, caused by this landing:** `tests/cloud/test_recomp_phase12.py::test_overlay_census_scripts_and_matched_lock` asserts
   `"control_settings" not in dc.matched_functions()` — it used control_settings as its example of an unmatched game function.
   Not edited (outside this lane's edit scope). Proposed one-line fix in `proposed_phase12_test.patch` (use the stop-ruled,
   unmatched `LoadTrack` instead); applied temporarily, the test passes; restored.

## Bookkeeping
- No provisional.json changes (none of the seven had an entry). No unit_overrides changes. No data-symbol changes.
- Builder scratch watchman2:~/rush2049/scratch/frontier/w33l (incl. unit/) deleted. `install_group.py` and `blob_rom rom` use
  their fixed builder dirs (/tmp/blobgroup, ~/rush2049/repo) as in every integration lane.
- Permission denials: none.
