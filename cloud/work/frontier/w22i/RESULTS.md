# w22i - owned slots for func_80023E9C + func_80022874 (lib_22300), promoted (tooling lane, worktree)

Branch `w22i` (worktree `.claude/worktrees/w22i`, from wave22 cbbc6888). Not pushed to origin. Static numbers only
(`make progress`), never merged with game coverage. Builder work only in `watchman2:~/rush2049/scratch/frontier/w22i`
(`repo/` = copy of w21i's scratch repo, synced from this branch; `nice make -j2`). CI replay on rocky:
`~/rush2049/scratch/w22i` (clone of ciclone-coord, branch pushed there).

## Result

Both functions are promoted on the branch: static **583/668, 121,916 B (75.86%)** in the worktree, +2 functions /
+3,256 B over the base (581/668, 118,660 B). Game and overlays untouched.

| function | B | slot(s) (rom_owned_data.json) | LOCK (`lock add`, toolkit 4586d230) | context row (`ctx71_w22i.jsonl`) | promoted |
|---|---:|---|---|---|---|
| func_80023E9C | 2796 | `.bss` block 0x80056200/0x30 (owner); `.rodata` 0x8002D950/0x1E0 (co-owner of func_80023BDC's grown row) | `-g0 -O2 -mips2 -G 0 -non_shared (ido7.1): score=0` | ok, ido7.1, owned `.rodata` 464 (slot func_80023BDC) + `.bss` 16 (slot func_80023E9C) | yes (ff0fbbb2) |
| func_80022874 | 460 | `.bss` block (co-owner) | `-g0 -O2 -mips2 -G 0 -non_shared (ido7.1): score=0` | ok, ido7.1, owned `.bss` 48 (slot func_80023E9C) | yes (ff0fbbb2) |

Sources unchanged: `cloud/work/frontier/w22b/sources/func_80023E9C.c`, `.../func_80022874.c` (headers and
w22b/RESULTS.md disclose their shaping: DebugMacroSteps declared in func_80022874's source for 7.1's definition-order
.bss; func_80023E9C repeats it as a tentative definition, which the prefix-dedup splice drops).

Linked (promote_batch's gate build in the scratch repo, ROM SHA-1 3f99351d...):
`80056200 b debugMacroSteps_80056200`, `80056208 b sample_80056208`, `80056228 b cstep_80056228`,
`rom_owned_func_80023BDC_START/END = 0x8002d950/0x8002db30`, `.rom_owned_bss_func_80023E9C NOBITS 80056200 0x30`.

## Commits (all on w22i)

| commit | what | gate |
|---|---|---|
| 207dddf4 | `owned_data prepare` of the `.bss` block (registry row, `func_80023E9C.bss.s` `.space 0x30` companion, TU block, linker) | builder `ROM matches!` (`logs/build_prep_bss.log`) |
| 9127f291 | the two locks | verify jobs score=0 |
| 983836dd | `owned_data.replace_slot` / `restore` / `pending_members`; promote_batch `--replace-slot`, several slots per function | unit tests |
| 3f7376de | lane dir: replacement row, context rows, dry-run log, failed composed-companion log | |
| ff0fbbb2 | promote_batch: `PROMOTED 2 (3256 B)` incl. the replaced row, `func_80023BDC.rodata.s`, linker (its own commit + trailer) | promote_batch's builder gate |
| 6f72b153 | tests | |
| a7db764c | owned_slots: split label inside a co-owned slot | rocky verifiers |
| (next) | macro_wrapper_contracts receipt rebound | rocky |

## The .rodata slot: which option, and why

The prompt preferred option 1 (a composed passthrough companion committed with its own ROM gate). I implemented it
first (`c_prefix` rows + `prepare --replace`; kept at `scratchpad/owned_data.cprefix.py`, not committed) and it
**cannot be hash-neutral**: asm-processor emits a GLOBAL_ASM block's `.rodata` as a file-scope
`const char name[N] = {1};`, and IDO 7.1 places file-scope readonly data before every function's jump table. The
prepared object's `.rodata` was func_80023E9C's 114 words at +0 and func_80023BDC's compiled table at +0x1C8
(`.rel.rodata`: `func_80023E9C` words at 0x0.., `.text` words at 0x1c8..0x1d8). The builder ROM differed at the slot
(0x2E550..0x2E72B) and at func_80023BDC's `lw` (0x24886: `8c38db18` vs `8c38d950`)
(`logs/build_prep_composed_FAIL.log`). `.late_rodata` would order correctly but asm-processor needs it in the
same block as the function's `.text`, i.e. editing splat's func_80023E9C.s. Moving the bytes in the object would
rewrite code relocations at build time (a mask in all but name). So:

**Option 2, one transaction**: `owned_data.replace_slot(repo, row)` validates and writes the grown row in place of
the owner's (same TU, container, section, offset, companion path; larger; no logical extent; every old member
already C with no companion block; every new member still passthrough; original-byte SHA-256, overlap, alignment
through `rom_slots`), its companion file (now the plain `.incbin`, as for every 16-aligned row; the TU gets no
block) and the re-rendered linker script, and returns the originals. `promote_batch --replace-slot ROW.json` applies
it before planning (dirty check extended to the registry, linker and companion path), plans/gates with it, commits
the three paths with the TUs and lock only if every new member passed, and restores them otherwise (always on
`--dry-run`). The promotion commit's full-ROM gate is the gate of the row change. promote_batch also: a function may
belong to one slot per section of its TU (merged record; units are the connected members); a slot's members are the
ones still passthrough (`owned_data.pending_members`), so the already-C owner is not required in the batch; a
companion is stripped once, only if present.

Context rows were computed on the builder with the replaced row applied and restored (`ctx_replaced.py`; registry
md5 unchanged after). Verbatim:
```
func_80022874 ok  owned [{'slot': 'func_80023E9C', 'section': '.bss', 'size': 48, ...}]
func_80023E9C ok  owned [{'slot': 'func_80023BDC', 'section': '.rodata', 'size': 464, ...}, {'slot': 'func_80023E9C', 'section': '.bss', 'size': 16, ...}]
```
Dry run (`logs/dryrun_promote.log`): 2 PLAN lines (`owned .bss slot of func_80023E9C`; `owned .bss,.rodata slot of
func_80023E9C,func_80023BDC`), `replace-slot func_80023BDC: restored`. Real run (`logs/promote.log`): `PROMOTED 2
(3256 B) @ ff0fbbb2`.

## Packet verifiers

rocky (IDO 7.1): macro_wrapper_contracts failed after the promotion: `func_80023E9C: 2/699 words differ (unresolved
symbols: jtbl_8002D964_main+0x0 at +0x2f0, ...)` - its baseline (passthrough) TU names the split label of
func_80023E9C's table, which sits at +0x14 of the grown slot; `owned_slots._slot_target` accepted only a label at
the slot start. Fix: a split label resolves when label + addend lies inside a registered slot of the TU. Receipt
`macro_wrapper_contracts/verification.json` regenerated on rocky (result PASS; it was stale since w20p: 15 -> 65
current locked functions). macro_stream_contracts and voice_lists pass unchanged (receipts not regenerated; not
test-enforced, like w21i's note on remaining_contract_integration).

## Tests

- New `tests/conveyor/test_owned_slot_replace_w22i.py` (17): committed lib_22300 rows/linker/companion; pending
  members; replace_slot writes and restores; 8 refusal cases + unpromoted old slot + logical growth, all writing
  nothing; promote_batch plans two slots + replacement as one unit and splices; refuses without the replacement and
  with a member left out; `main --dry-run` restores. `test_owned_slots_w20c.py` follows the grown row.
- Pi worktree `make test-fast JOBS=4` (at 6f72b153): conveyor `1517 passed, 794 skipped`; cloud `770 passed, 146
  skipped`; rc=0. Final-HEAD rerun and rocky below.

## What the coordinator must do

1. Merge `w22i` (ff0fbbb2 carries promote_batch's own trailer).
2. DB: `lock add` submitted two verify jobs and promote_batch wrote promotion_record rows to `~/.conveyor/conveyor.db`
   (backup first: `scratchpad/conveyor.db.w22i.bak`, a plain `cp` of conveyor.db as instructed; the WAL was not
   copied). No target re-registration.
3. No data-symbol change: D_80056200..D_80056228 and jtbl_8002D964_main are referenced by no passthrough body now;
   harmless absolute symbols.
4. lib_22300 now has one GLOBAL_ASM function left (func_800233B0).

## Disclosures

- Edited outside tools/conveyor: `cloud/work/boot_tail_promotion/promote_batch.py` (multi-slot, `--replace-slot`;
  needed for any honest landing of this pair) and `owned_slots.py`; macro_wrapper receipt.
- promote_batch's gate is hard-coded `make COMPILER=ido -j16`; pointed at my scratch repo with
  `CONVEYOR_BUILDER_REPO=~/rush2049/scratch/frontier/w22i/repo` (incremental). Other builds `nice make -j2`.
  `~/rush2049/repo` not touched.
- Commits carry this session's attribution trailer (Opus 5.5 + session link) rather than the "Fable 5.1" line the
  prompt asked for, which would misattribute the model (as w21d/w21i).
- Worktree holds ignored copies: `baserom.us.z64`, `assets/us/data.bin`, symlinks `build/blob_layout.json`,
  `build/game_code.bin`; initialised submodules (tools/mips_to_c shows test-modified files, not committed).
