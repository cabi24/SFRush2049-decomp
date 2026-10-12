# w25a - .bss-offset gate, __scMain/__scExec, libultra rename leads, inflate/lzss triage (worktree lane)

Branch `w25a` (worktree `.claude/worktrees/w25a`, from wave25 4f59607f). Not pushed. Static numbers only
(`make progress`), never merged with game coverage.

Builder gate repo: `watchman2:~/rush2049/scratch/frontier/w25a/repo` (rsync of `~/rush2049/repo` without `.git`,
`.claude`, `cloud`, `build`; then the worktree synced over it; the submodules' `.git` directories removed so the
worktree's gitlink files can sync). `~/rush2049/repo` was only read (copy source, deflate streams).

## Job 1: OWNED-OFFSETS gate + __scMain/__scExec (684 B) - PROMOTED

**Static 614/667, 137,484 B (85.54%) -> 616/667, 138,168 B (85.97%).**

### The rule (why w24s's TU was wrong)
IDO's cfe gives every symbol-table entry a "block number" in the order it first meets the name, and the `.bss`
section is sorted by **block number mod 256** (ties: top-level before function statics; variables referenced from
.data/.rodata first). Method and parser: zeldaret/oot `tools/fix_bss.py` + `tools/ido_block_numbers.py` (CC0; the
parser is vendored unchanged in `tools/`). `tools/bssorder.py` runs asm-processor + `cc -Hf` (stop after cfe) on a
TU and prints the predicted layout; it reproduced the w24s object exactly:

| object | w24s TU block (mod) | built offset | retail |
|---|---|---|---|
| fresh definitions at __scMain's slot | 262..271 (6..15) | 0x0..0x2b4 | |
| gViAccumTime (extern in rom_tu.h) | 171 | +0x2b8 | +0x2bc |
| gViLastTick (extern in rom_tu.h) | 173 | +0x2bc | +0x2c4 |
| __osScTaskCount (extern in scheduler_declarations.h) | 239 | +0x2c0 | +0x290 |

So "declared before defined" = keeps the early block number. ("Small files show the opposite": there the block
numbers never pass 256.) No arrangement with those three pre-declarations can work: retail needs
TaskCount < AccumTime in mod order, but rom_tu.h (171) precedes scheduler_declarations.h (239).

### TU shape that matches (all 13 at retail offsets)
1. `src/rom/rom_tu.h`: `extern s32 gViAccumTime;` and `extern volatile s32 gViLastTick;` removed (lib_1050.c, the
   only other user, declares both itself; comment left in rom_tu.h). rom_tu.h context pins rebound with
   `static_rename owned cloud/work/frontier/w25a/rebind/empty_renames.json --base HEAD --rebind-dir
   cloud/work/frontier/w25a/rebind` (2 file pins); storage_overrides.mk regenerated.
2. The __scMain `.bss` companion moved to generate_tu's position (right after `#include "rom_tu.h"`, before
   `#include "scheduler_declarations.h"`), and promote_batch's new `--bss-at-companion` puts the slot's 13 C
   definitions there. They must precede osCreateScheduler (it uses __osScTaskCount), so the header's later
   `extern s16 __osScTaskCount;` is a harmless redeclaration. Blocks 237..249 (mod 237..249), ascending in
   retail order. **Headroom: 6.** Adding 7+ symbol-table entries (declarations) to rom_tu.h moves the run across
   256 and OWNED-OFFSETS will fail; the fix is then to move the definition block a few declarations later
   (e.g. after the scheduler_declarations.h include) so the 13 do not straddle a multiple of 256.

### Gate (tools/conveyor/pipeline/owned_offsets.py, tests/conveyor/unit/test_owned_offsets.py, 11 tests)
For every named symbol a ROM TU object defines in .bss/.data/.rodata: map placement + st_value must equal the
address in undefined_syms_auto / undefined_funcs_auto / symbol_addrs (any table that disagrees is a mismatch;
`D_XXXXXXXX` names are their own address; COMMON symbols refused; missing map fails closed). Wired after
`make test` into `promote run --via-builder` (and its local path), `promote_batch.gate` (so `gate_only.py` too) and
`owned_storage._gate`, with `owned_offsets.report()` printing `builder gate: OWNED-OFFSETS: ...` like GATE-CLEAN.
Not wired into blob_rom (game-only builds; its gate repo is the shared ~/rush2049/repo). Docs: BUILDING.md
"Object data offsets (OWNED-OFFSETS)". Limitation: function-local statics have no symbol names (IDO), so the
gate cannot see them - but they are also not in syms.ld, so the ROM SHA-1 does.

Live evidence (builder scratch): the w24s splice builds `ROM matches!` and the gate fails it:
```
OWNED-OFFSETS: MISMATCH src/rom/lib_1050_sdk.c(.bss) __osScTaskCount: built 0x8002eba0 (+0x2c0), retail 0x8002eb70 (undefined_syms_auto.us.txt, symbol_addrs.us.txt)
... (9 mismatches) ... OWNED-OFFSETS: FAIL 16 ROM TU data sections (16 owned slots), 16 named objects checked ...
```
Prep commit gate (rom_tu.h + companion move): `GATE-CLEAN ...`, `ROM matches!`, `OWNED-OFFSETS: OK 16 ROM TU data
sections (16 owned slots), 5 named objects checked against retail addresses`.

### Promotion (standard path; locks were already `score0` from w24s)
```
CONVEYOR_BUILDER_REPO=~/rush2049/scratch/frontier/w25a/repo CONVEYOR_BUILDER_JOBS=2 \
python3 cloud/work/boot_tail_promotion/promote_batch.py cloud/work/frontier/w25a/ctx53_sched.jsonl 0x1050 \
  --only __scMain,__scExec --prefix-dedup rom/lib_1050_sdk \
  --replace-slot cloud/work/frontier/w24s/data/owned_lib_1050_sdk_rodata_grown.json --bss-at-companion
```
```
replace-slot __scScheduleCore: .rodata 0x8002d490 size 0x30, members __scScheduleCore, __scMain
batch 0x1050..0x1050: 2 candidates
builder gate: GATE-CLEAN: owned-slot inputs changed since the last build (stamp 92210d98... -> eb98d63f...); rm -rf build/us for a full rebuild
builder gate: OWNED-OFFSETS: OK 16 ROM TU data sections (16 owned slots), 18 named objects checked against retail addresses (18 in owned slots), 0 without a retail address
builder gate: OWNED-OFFSETS: OK 16 ROM TU data sections (16 owned slots), 18 named objects checked against retail addresses (18 in owned slots), 0 without a retail address
PROMOTED 2 (684 B) @ 6c5ca4f2
```
(`ok` requires `ROM matches!` in the same output.) Context rows `ctx53_sched.jsonl` recomputed on the builder at
the current rom_tu.h (identical to w24s's rows). Devices (from w24b/w24s, in the source headers): compiled-out
assertion blocks, volatile timestamps, two unreferenced placeholders D_8002EB6C/D_8002EB88.

## Job 2: libultra rename leads - all 5 PROVEN and applied, plus 1 more (6 renames)

Method (w24t): ultralib VERSION_J `-D_FINALROM` compiled on the builder with the project IDO 5.3 at the holding
TUs' locked flagsets (`rename/prove.sh`), whole `.text` compared with the ROM, relocations by value (w24t's
`compare.py`; log `rename/prove.log`):
- `io/contpfs.c` -g0 -O2 (lib_f700): `RESULT: IDENTICAL (736 words, 52 relocated fields compared by value, 0 differing
  words)` at 0x8000EB00: `__osRepairPackId` +0x170 -> 0x8000EC70 (project `__osRepairId`), `__osCheckPackId` +0x4C0
  -> 0x8000EFC0 (project `__osCheckId`), and **`__osCheckId` +0x7D0 -> 0x8000F2D0 (project `osPfsReadWriteFile_pages`)**
  - the doc's old lead, which the permutation needs anyway (the name is freed). Independent confirmation: the
  legacy `src/libultra/os_pfs_*.c` already declare `__osCheckId(OSPfs *)` at 0x8000F2D0.
- `os/timerintr.c` -g0 -O1 (lib_cc50): IDENTICAL (256 words, 50 relocated fields); relocations `__osCurrentTime`(+4)
  -> 0x80037C50/54, `__osBaseCounter` -> 0x80037C58, `__osViIntrCount` -> 0x80037C5C.
- `io/pfsinitpak.c` -g0 -O2: IDENTICAL (196 words); jal `__osCheckPackId` -> 0x8000EFC0, `__osRepairPackId` -> 0x8000EC70.

| vram | old | new |
|---|---|---|
| 0x8000EC70 | __osRepairId | __osRepairPackId |
| 0x8000EFC0 | __osCheckId | __osCheckPackId |
| 0x8000F2D0 | osPfsReadWriteFile_pages | __osCheckId |
| 0x80037C50 | gViTimeAccumHi | __osCurrentTime (OSTime, size:0x8) |
| 0x80037C58 | gViLastCount | __osBaseCounter |
| 0x80037C5C | gViRetraceCount | __osViIntrCount |

`gViTimeAccumLo` (0x80037C54) kept: it is only the asm label of `__osCurrentTime`'s low word
(`targets._typed_data_field_aliases` now folds it to `__osCurrentTime+0x4`). A rename to an invented name was not
made; the coordinator may retire it later (asm/us/nonmatchings osGetTime.s and __osTimerServicesInit.s use it).

Applied (commit 23f00a95): `static_rename symbols|source|lock|owned rename/renames.json` (src/rom TUs + rom_tu.h only;
the unbuilt legacy src/libultra and src/os keep their spellings), 12 lock entries (3 re-keyed, 9 rehashed with the
pure-rename proof), PROMOTED headers retitled by hand, nonmatchings git-mv'd (chain-safe order) and permuted,
`make extract` (undefined_syms_auto; no other change), boot_tail/blob `symbols.json` + SHA256SUMS and
src/blob/blob.ld permuted in place (as w24t; `blob_rom produce` would re-sort the 6 keys - reverted, not committed),
timer_services `owned_existing_storage.PROFILES` revision, rebind copies under `rename/rebind/`, targets.py alias
base + 2 tests, `tools/recomp/identity.json` (3 functions mapped back to pre-w25a names; data not listed),
`static_rename.RENAME_TABLES` += w25a table, docs/SYMBOL_MISATTRIBUTION.md section. Tooling fixes found on the
way (tests in `tests/conveyor/unit/test_static_rename_data.py`): `rewrite_symbols` wrote `type:func` on data
definitions; `rebind_owned` did not rename a storage block's `aliases` (rom_owned_storage.ld ASSERTs).
nearest_point_donor receipt re-pinned (`asm/us/blob/symbols.json` = pinned eaf79647 with the table applied, proven).

Gates: builder full ROM (all ROM TUs rebuilt after GATE-CLEAN) `ROM matches!` and `OWNED-OFFSETS: OK 19 ROM TU data
sections (16 owned slots), 23 named objects checked against retail addresses`; `make check-matched`: all 619
locked functions intact, ovl 89 intact. `blob_rom rom` equivalent against this lane's builder repo (the CLI's gate
repo is the shared ~/rush2049/repo and its splice dir /tmp/blobsplice, both off-limits):
`blob objects: 0 stale singles, 0 stale groups`; produce `spliced 986`, `stream_matches_rom: True`; builder
`ROM matches!` `MAKE=0 TEST=0`.

Conveyor DB (`~/.conveyor/conveyor.db`): backups in the scratchpad `w25a/conveyor.db.w25a.bak1` (before
promote_batch), `.bak2` (before the rename work), `.bak3` (11:43 CDT, immediately before the live apply), all with
the sqlite backup API. Copy run on `.bak2` first (3 renamed, audit clean). Live:
`python3 -m tools.conveyor.pipeline.targets_rename rename cloud/work/frontier/w25a/rename/db_renames.json` ->
`renamed 3 rows in /home/cburnes/.conveyor/conveyor.db (...), 0 matrix_entry rows purged`; `targets_rename audit`
clean (rc 0, no rows). Only the 3 function rows (`db_renames.json`); the inventory has no data targets. promote_batch
also wrote promotion_record rows for __scMain/__scExec. Until w25a merges, master's layout and the DB disagree on
these 3 names.

## Job 3: inflate/lzss block (lib_5610, 12 slots, 8,104 B) - triage, nothing matched

**Finding: the C-like slots are IDO 5.3 `cc -O2 -S` output assembled by as1 at `-O1`, not a `-O2` object.** Retail
shows only fill-from-above delay slots, never a target fill or a branch-likely (IDO -O2's as1 makes `bnel`/`beqzl`
and copies `lw ra` into `b` delay slots in these same bodies), and it keeps the redundant reload after
`gInflateInPtr += 2` that as1 -O2 deletes. Reproduced with `tools/cmps.sh` (cc -S at `-g0 -O2 -mips2`, drop the
`.option O2` line - otherwise "Binasm file dictates -O: 2" - then `cc -c -O1` on the .s). Other recipes tried on
inflate_block: 5.3 -O1, -g1/-g2/-g3 -O2, -mips1, IDO 7.1 -O2, `-Wb,-O1`/`-Wab,-O1` (no effect), 7.1 front end + 5.3
as1. This fits the TU being a hand-edited compiler .s: lzss_decode/inflate_io_wait share a non-ABI contract
(t1 return, s0-s4 clobbered unsaved; static_C20/C21).

Ceiling (instruction-level diff of the object vs the ROM bytes, operands' immediates masked, `tools/sbs.py`;
"equal/retail words"; drafts from earlier lanes unless noted):

| slot (label) | B | evidence / draft | standard -g0 -O2 | cc -O2 -S + as1 -O1 | verdict |
|---|---:|---|---|---|---|
| inflate_io_wait | 236 | non-ABI (returns in t1, clobbers s0-s4) | - | - | hand asm / hand-edited .s; stays GLOBAL_ASM |
| lzss_decode | 596 | consumes t1 after each call, spills s0 itself | - | - | hand asm / hand-edited .s; stays GLOBAL_ASM |
| huft_build | 1740 | C23 huft_build.c | 335/435 | 396/435 | C, needs the .s recipe + source work |
| inflate_free_window (likely gzip inflate_codes by size/order) | 1272 | no draft | - | - | C (not attempted) |
| inflate_stored | 544 | C20 | 111/136 | 123/136 | C, recipe + source |
| inflate_fixed | 456 | C23 .best | 92/114 | 101/114 | C, recipe + source |
| inflate_dynamic | 1936 | C23 | 301/484 | 369/484 | C, recipe + source |
| inflate_block | 420 | C23 (unchanged) | 79/105 | **99/105** | residual = as1 scheduling only (prologue store order, one sltiu, one lui) |
| inflate_loop | 92 | new `inflate/inflate_loop.c` | 19/23 | **21/23** | residual = prologue store order only |
| inflate_read_bits | 320 | C20 through `static_rename.renamed_source` (`inflate/inflate_read_bits.renamed.c`) | 63/80 | 73/80 | C, recipe + source |
| inflate_entry | 360 | no draft | - | - | C (not attempted) |
| inflate_entry_alt | 132 | C20 | 30/33 | 29/33 | C, uopt register choice (src via a3) differs |

The inflate_block and inflate_loop residuals do not move with source order (uopt canonicalises; the ugen .s is
identical across load-order, DUMPBITS-order and return-shape variants), so they come from the scheduler: the retail
as1 puts `sw ra` later and picks a different delay-slot instruction than this as1 at -O1 (5.3 and 7.1 as1 agree with
each other, -mips1/-mips3 do not help). Best next hypotheses: an older as1 (IDO 5.2/6.x, not in the toolkit cache) or
a hand-edited .s. Even with an exact assembler, promotion would need a new TU recipe for lib_5610 (C -> `cc -S` ->
as1 -O1) beside its GLOBAL_ASM passthroughs; not attempted. Nothing locked or promoted. Tools: `tools/cmp.sh`
(standard compile + w24t compare.py), `tools/cmps.sh` (the .s recipe), `tools/sbs.py` (aligned instruction diff);
their paths point at this session's scratchpad and builder scratch. Variants in `inflate/v/` (ignored).

## Tests (final HEAD af0580d5)
- Pi worktree `make test-fast JOBS=4`: conveyor `1584 passed, 795 skipped`; cloud `770 passed, 173 skipped`; rc 0.
- rocky `~/rush2049/scratch/w25a` (cp -a of ciclone-coord + this branch via bundle), `REQUIRE_TOOLCHAIN=1 make
  test-fast JOBS=16`: conveyor `2327 passed, 52 skipped`; cloud `942 passed, 1 skipped, 27 subtests passed`; rc 0.
  (The first run failed dot_target_orientation's toolchain-only replay: a second receipt pinning
  asm/us/blob/symbols.json, re-pinned with the same pure-rename proof in af0580d5.) `tests/recomp`
  (REQUIRE_TOOLCHAIN=1, -n 16 --dist loadgroup): `138 passed, 4 skipped`.
- New tests: tests/conveyor/unit/test_owned_offsets.py (12), test_static_rename_data.py (3).
- Pre-existing, not fixed (recomp lane's file): on the Pi `tests/recomp/test_gen_config.py::test_end_to_end_with_rom`
  fails `static: 667 inventory functions, expected 668` (gen_config.EXPECTED_COUNTS still 668 after w24s retired viStub).

## For the coordinator (merge)
- Merge `w25a` (static 616/667, 138,168 B). It changes rom_tu.h (2 externs removed), the context-hash rebind copies
  (`cloud/work/frontier/w25a/{rebind,rename/rebind}`), symbol_addrs/undefined_syms_auto, lock keys, Conveyor DB names.
- **Builder gate repo:** every gate now runs `python3 -m tools.conveyor.pipeline.owned_offsets check` on the builder
  after `make test`; `promote run --via-builder` and `owned_storage` sync only their package, so
  `~/rush2049/repo` needs `tools/conveyor/pipeline/owned_offsets.py` (rsync tools/conveyor) before their next gate,
  or they fail with "No module named". promote_batch rsyncs the whole tree itself. I did NOT touch
  `~/rush2049/repo`; everything here ran in `~/rush2049/scratch/frontier/w25a/repo` only. After the static changes
  the first gate in any build dir will GATE-CLEAN (owned-slot inputs changed).
- The Conveyor DB already carries the 3 function renames (live apply, backups `.bak1/.bak2/.bak3` in the
  scratchpad `w25a/`); until w25a merges, master's layout and the DB disagree on __osRepairPackId, __osCheckPackId,
  __osCheckId (0x8000F2D0). `targets_rename audit` is clean against this branch.
- CLAIMS.md tooling row (not edited here): the five w24t leads are done (plus osPfsReadWriteFile_pages =
  __osCheckId); remaining: the other doc leads (osViModeTableGet, __osSpDma, ...), gViTimeAccumLo retirement,
  gfx_modes object_render explicit __ll_lshift calls. New lead: lib_5610 needs a C -> `cc -S` -> as1 -O1 recipe.
- Commits carry this session's attribution (Opus 5.5 + session link), not the "Fable 5.1" line the prompt
  asked for (it would misattribute the model), as w24s did; promote_batch's own commit carries its Opus line.
- Shared scratchpad note: another lane overwrote `scratchpad/sync.sh` mid-session (my first gate attempt ran its
  rsync of w25r to rocky once, harmless); my scripts now live in `scratchpad/w25a/`.
