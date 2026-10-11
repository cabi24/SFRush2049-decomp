# w19c - static owned-data items (tooling lane, worktree)

Branch `worktree-agent-a2e5ae5d47c7190b1` (from master e46f0645), not pushed. Static numbers only; never merged
with game coverage. Live conveyor DB, `matched.lock.json`, `~/rush2049/repo`, `/tmp/blobsplice`, `/tmp/blobgroup`
untouched. Builder work only in `~/rush2049/scratch/frontier/w19c` (`repo/` = this branch, `nice make -j2`).

| function | bytes | segment / flags | state | evidence (verbatim) |
|---|---:|---|---|---|
| osAiSetNextBuffer | 144 | 0xca70, `-g0 -O2 -mips2 -G 0 -non_shared`, IDO 5.3 | slot prepared + committed; promotion proven in scratch | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr DIFFERS(6 rows) (43 rows)` |
| __fixunsdfdi | 160 | 0xa5f0, `-g0 -O1 -mips3 -32 -G 0 -non_shared`, IDO 5.3 | same; needs the reloc-aware target (DB apply) | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr DIFFERS(2 rows) (42 rows)` |
| __fixunssfdi | 156 | same | same | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr DIFFERS(2 rows) (41 rows)` |

The DIFFERS rows are relocation symbol names only (`R_MIPS_HI16/LO16 .data` vs `__osAiNeedsAlign`, `.rodata` vs
`gInfinity`): the C object addresses its own section, which the owned slot places at exactly that address. Words,
relocation types and offsets are identical (`prove.log`; `objdump -dr` target vs standalone object differs only in
`24: R_MIPS_HI16 __osAiNeedsAlign` -> `.data` (x6 sites) and `60: R_MIPS_HI16 gInfinity` -> `.rodata` (x2)). Against the
CURRENT live raw-word fixuns rows the same sources score `true=10 strict=10 reloc_blind=0`, so their locks must
follow the DB re-registration.

Full-ROM gates (watchman2 scratch, full `make VERSION=us COMPILER=ido` + `make test`, `logs/`):
- `build_prep_ai.log`: osAiSetNextBuffer slot prepared (commit 0c8867d3 state) -> `ROM matches!`
- `build_prep_both.log`: both slots prepared (commit a3df4e48 state; lib_a5f0.o `.rodata` 0x10) -> `ROM matches!`
- `build_promote.log`: + the three bodies spliced by promote_batch's own plan/splice (`--splice-out`, simulated
  lockfile) -> `ASMPROC src/rom/lib_a5f0.c`, `ASMPROC src/rom/lib_ca70.c`, `build/us/rush2049.us.z64: OK`,
  `ROM matches!`, both objects newer than their TUs, SHA-1 3f99351d7bb61656614bdb2aa1a90cfe55d1922c. Spliced
  lib_ca70.o `.data` 0x10, lib_a5f0.o `.rodata` 0x10 = `80000000 00000000 80000000 00000000`.
- `build_negative.log` (control): both fixuns bodies spliced but the companion kept -> link stops with
  `owned ROM slot address changed` (the slot ASSERTs are not vacuous).

## Commits

| commit | what | tests |
|---|---|---|
| b18257d4 | task 2 tooling: `targets.target_prelude()` - static targets take `.set noat/noreorder/gp=64` (and the float aliases) from `tools/asm-processor/prelude.inc`, i.e. the ROM build's assembler mode | `tests/conveyor/unit/test_target_prelude_gp64.py` |
| bae608f2 | task 1/2 promotion tooling: `owned_data` `co_owners`, `prepare`, helpers; `context_check.py --flags` + owned rows; `promote_batch` owned-slot units; `promote run` refuses a shared slot | `tests/conveyor/test_owned_promotion.py` |
| 0c8867d3 | task 1: osAiSetNextBuffer `.data` slot (registry row, companion, TU block, linker script) - ROM SHA-1 exact | `tests/conveyor/test_owned_slots_w19c.py` |
| a3df4e48 | task 2: `__fixunsdfdi` (+ co-owner `__fixunssfdi`) `.rodata` slot - ROM SHA-1 exact | same file |
| 6f3d279b | fix found on the builder: context_check's owned-section compare included objdump's file-name header | test added |
| (last) | this lane dir: RESULTS.md, `prove.sh`, `simlock.py`, `gp64_scan.py`, ctx rows, logs | |

Tests: the files above pass; `pytest tests/conveyor tests/cloud` in this worktree: 10 failures, all environmental
and present without these changes (empty `tools/mips_to_c` / `decomp-permuter` submodules in the worktree:
test_ido_syntax x3, test_autodecomp_population x3, test_agent_loop, test_search_cleanup x2; missing
`build/game_code.bin`: test_target_assembly::test_populate_second_pass_is_a_noop).

## Task 1: osAiSetNextBuffer + owned `.data`

Why it was manual (w17g): promote_batch refused any object with data (context_check) and never stripped a
companion; promote run strips it but splices the body without the extra declarations. Now:
- `python3 -m tools.conveyor.pipeline.owned_data prepare ROW.json` appends the row to `rom_owned_data.json`,
  writes the companion (`.section .data` / `.balign 4` / `.incbin "assets/us/data.bin", 0x4bf0, 0x10`), inserts
  `companion_block(row)` right after the owner's slot and re-renders `rush2049.us.ld` (as the build does). It
  writes nothing unless the registry validates (original bytes sha256, alignment, overlap) and every member is
  still passthrough. Row: `data/owned_osAiSetNextBuffer.json` (= w17g's).
- `context_check.py` admits data only in the section of an owned slot the function owns/co-owns (size <= slot),
  records `"owned"`, and requires those section bytes + relocations identical in the rom_tu.h context.
  Row (`ctx/ctx_w19c.jsonl`): ok, preamble = TRUE/FALSE/IO_WRITE/AI_DRAM_ADDR_REG/AI_LEN_REG defines +
  `__osAiDeviceBusy`/`osVirtualToPhysical` prototypes (the "extra declarations"), dropped = the four typedefs.
- `promote_batch.py` plans the member(s) of the slot, splices the body with its preamble, strips the companion,
  gates. Disclosed: the body keeps ultralib's function-local `static u8 hdwrBugFlag` (w16g); `__osAiNeedsAlign`
  stays defined at the slot address by `undefined_syms_auto` (0x8002C3C0, the slot ASSERT pins it); after
  promotion no ROM code references that name (the game blob uses its own `PROVIDE`).

## Task 2: __fixunsdfdi / __fixunssfdi

1. Tooling (b18257d4): `assemble_region` hard-coded `.set noreorder/noat` and took only the float aliases from the
   prelude, so GNU as (-mabi=32, gp=32) expanded 64-bit `ld`/`sd` as macros. `target_prelude()` takes the mode
   lines from `prelude.inc` (refuses a prelude without noat/noreorder). `gp64_scan.py` -> `gp64_scan.log`
   assembled all 668 static regions both ways: 649 objects byte-identical, 19 differ, all improvements:
   `__fixunsdfdi`/`__fixunssfdi` assemble_error -> gate_ok; 16 more (`__floatdidf/disf/undidf/undisf`,
   `__osCleanupThread`, `__osDispatchThread`, the 10 libgcc helpers) gate_refused (`length_mismatch`: each `ld`
   became two `lw`) -> gate_ok; `__osExceptionPreamble` stays refused (`word_mismatch@122` instead of a length
   mismatch). Existing inventory rows are untouched until something re-registers them. Not changed: the
   extracted-game path (`target_asm_from_derived`, `assemble_text`) still emits its own two mode lines; game code
   is -mips2.
2. Re-registration on a DB COPY (`sqlite3` backup API, read-only open of the live file; private empty `blobs/`):
   `targets_rename audit` -> `audit.log` (only the two stale rows). Plan (`dryrun_rename.log`, DB sha256 unchanged
   by the dry run):
   ```
   register  osCauseGet               @0x8000DBB0    3 insns seg 0xe4f0   reloc_aware o=54eac4d7b131  (new row)
   refresh   __osContAddressCrc       @0x8000F9F0   52 insns seg 0x105f0  reloc_aware o=0cdbca3b4ac5  (was 23 insns; object unchanged)
   refresh   __fixunsdfdi             @0x80009A28   40 insns seg 0xa5f0   reloc_aware o=b9227be98966  (was 40 insns; object changes, supersedes 360 matrix_entry rows)
   refresh   __fixunssfdi             @0x80009AC8   39 insns seg 0xa5f0   reloc_aware o=b81b16683e14  (was 39 insns; object changes, supersedes 360 matrix_entry rows)
   retire    __osGetCause             @0x8000DBB0  renamed to osCauseGet @0x8000DBB0; deletes n64_target 1, matrix_entry 0, function_status 1
   dry run: nothing written (5 actions; 2274 other n64_target rows untouched)
   ```
   Applied to the copy (`apply_copy.log`): `n64_target 2279 -> 2279, 720 matrix_entry rows purged`; re-audit
   empty. Objects equal w17g's in-memory patch (b9227be9..., b81b1668...).
3. Owned `.rodata`: one row, owner `__fixunsdfdi`, `co_owners: ["__fixunssfdi"]` (`data/owned___fixuns.json`),
   offset 0x6030, size 0x10 (0x8002D800, two u64 0x8000000000000000). Standalone each object has `.rodata` 16
   (padded); in the TU together it is exactly 0x10. `promote_batch` refuses either member without the other
   (`owned slot of __fixunsdfdi needs __fixunssfdi promoted in the same batch`), gates them as one unit, and
   `promote run` refuses both (`promote them together (promote_batch)`).
   Context rows: `context_check.py --flags "-g0 -O1 -mips3 -32 -G 0 -non_shared"` (replaces w16g's ctx.py
   wrapper); rows carry `flagset`, which promote_batch must find equal to the segment's.

## Task 3: stale inventory rows
Plan above (same command as w16i/w17g predicted; nothing else reported by `audit`). Applied only to the copy.

## Commands for the coordinator (main checkout, Pi)

Order matters: the fixuns locks verify against the target objects, so the DB apply comes first.
```
# 0. merge the branch (5 code/data commits + lane dir); builder gate of the two prepared slots:
#    sync per docs/BUILDING.md, then on watchman2: make VERSION=us COMPILER=ido -j16 && make test   -> ROM matches!
# 1. DB: back up, dry run (compare with cloud/work/frontier/w19c/dryrun_rename.log), apply, audit (expect no output)
python3 -c "import sqlite3,os;h=os.path.expanduser('~/.conveyor/');s=sqlite3.connect(h+'conveyor.db');d=sqlite3.connect(h+'conveyor.pre-w19c.db');s.backup(d)"
python3 -m tools.conveyor.pipeline.targets_rename apply osCauseGet __osContAddressCrc __fixunsdfdi __fixunssfdi --dry-run
python3 -m tools.conveyor.pipeline.targets_rename apply osCauseGet __osContAddressCrc __fixunsdfdi __fixunssfdi
python3 -m tools.conveyor.pipeline.targets_rename audit
# 2. locks (pinned IDO 5.3 toolkit; no --toolkit: both segments are ido5.3)
S=cloud/work/frontier/w16g/sources
python3 -m tools.conveyor.pipeline.lock add $S/osAiSetNextBuffer.c:osAiSetNextBuffer --flags "-g0 -O2 -mips2 -G 0 -non_shared"
python3 -m tools.conveyor.pipeline.lock add $S/__fixunsdfdi.c:__fixunsdfdi --flags "-g0 -O1 -mips3 -32 -G 0 -non_shared"
python3 -m tools.conveyor.pipeline.lock add $S/__fixunssfdi.c:__fixunssfdi --flags "-g0 -O1 -mips3 -32 -G 0 -non_shared"
git add matched.lock.json && git commit -m "Lock osAiSetNextBuffer, __fixunsdfdi, __fixunssfdi (score0)"
# 3. promote (one batch, both segments; owned slots handled by promote_batch)
python3 cloud/work/boot_tail_promotion/promote_batch.py cloud/work/frontier/w19c/ctx/ctx_w19c.jsonl 0xca70 0xa5f0 \
  --only osAiSetNextBuffer,__fixunsdfdi,__fixunssfdi --dry-run      # expect 3 PLAN lines as in dryrun_promote.log
python3 cloud/work/boot_tail_promotion/promote_batch.py cloud/work/frontier/w19c/ctx/ctx_w19c.jsonl 0xca70 0xa5f0 \
  --only osAiSetNextBuffer,__fixunsdfdi,__fixunssfdi
```
Expected: `PROMOTED 3 (460 B)`; static +3 functions / +460 B (recount with `make progress`). If step 1 is skipped
the fixuns `lock add` refuses (`score=10`). If the context rows need recomputing (builder, scratch repo root):
`sh cloud/work/frontier/w19c/prove.sh` (also re-runs the lock-style scores).

## Files
`prove.sh` (builder: context rows + lock-style scores) -> `prove.log`; `ctx/ctx_w19c.jsonl` (3 ok rows;
`ctx_ai.jsonl`, `ctx_fixuns.jsonl` parts); `simlock.py` (simulated lockfile for the dry run; refuses to write
matched.lock.json); `dryrun_promote.log`; `gp64_scan.py`/`.log`; `audit.log`, `dryrun_rename.log`,
`apply_copy.log`; `data/` (the two registry rows as passed to `prepare`); `logs/` (builder builds).

## What generalises
- `owned_data prepare` + `promote_batch` now carry any owned data/rodata slot: register (own commit, ROM gate),
  lock, context row (`owned`), batch. `co_owners` covers a section shared by several functions of a TU.
- `context_check.py --flags` replaces per-lane wrappers for -O1 and `-mips3 -32` TUs.
- 16 more static regions (libgcc, `__osCleanupThread`, `__osDispatchThread`, float conversions) now produce
  reloc-aware targets on re-registration; current locks/promotions are unaffected.

## Disclosures
- The session scratchpad held a `dbcopy/` dir (another session's, `blobs` symlinked to the live store). My first
  backup wrote `dbcopy/conveyor.db`; I noticed the symlink before any write to blobs, moved my copy to
  `scratchpad/w19c/db/` (private `blobs/`) and restored the `dbcopy/blobs` symlink. If `dbcopy/conveyor.db`
  existed before, it was overwritten (I did not see one listed). No blob was written to the live store.
- Read-only reads of the live DB (backup API, `mode=ro`) and live blobs (target objects copied to the builder).
- The worktree holds an ignored copy of `baserom.us.z64` and a `build/` dir created by tests.
- The w16g source headers for the three functions still say "unconverted"/"not promotable by body splice alone"
  (another lane's dir; left unchanged; promote_batch splices the body only, so the header text does not enter
  the ROM TU).
- No permission denials affected results (several compound shell commands were refused by the worktree guard and
  re-run as plain commands or scripts).
