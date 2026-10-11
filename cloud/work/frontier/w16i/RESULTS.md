# w16i - tooling: inventory re-registration and `-mips3 -32` ROM TUs

Lane worktree branch `worktree-agent-a8e9c43032cf8fa2a`, rebased onto master f0a30006. Nothing pushed; main
checkout, live `~/.conveyor/conveyor.db`, builder `~/rush2049/repo`, `/tmp/blobsplice`, `/tmp/blobgroup` untouched.
Builder work only in `~/rush2049/scratch/frontier/w16i` (`nice make -j2`). Inventory work only on scratch
copies of conveyor.db (sqlite backup API, read-only open of the live file).

## Commits

| commit | what |
|---|---|
| d5d87856 | A: `tools/conveyor/pipeline/targets_rename.py` + `tests/conveyor/test_targets_rename.py` (13 tests) |
| b4d28ac6 | B: `layout.write_opt_overrides` `-mips3 -32` rule, `tools/conveyor/pipeline/o32abi.py`, Makefile ROM-TU step, `tests/conveyor/test_mips3_32_tu.py` (14 tests) |
| 3da7cd6c | `layout convert 0xe4f0`, `layout convert 0xa5f0` (19 passthrough slots, no C). Gated clean (below). Optional. |
| (lane) | this directory: RESULTS.md, scripts, logs |

Tests: `pytest tests/conveyor/test_targets_rename.py tests/conveyor/test_mips3_32_tu.py tests/conveyor/test_debug_build_flags.py`
-> 42 passed (the one real-assembler test in test_targets_rename skips without decomp-permuter on the path).
Whole `tests/conveyor` in this worktree: the only failures are test_ido_syntax (3, `tools/mips_to_c` submodule not
checked out in the worktree) and test_search_cleanup (2, stand-in permuter), both unrelated to these files.

## A. `targets_rename` (unblocks 1,360 B)

`python3 -m tools.conveyor.pipeline.targets_rename [--data DIR] [--lockfile P] audit | apply NAME... [--retire ID...] [--dry-run]`

- Names, addresses and extents come from `layout.derive()` (never info.txt). The object is built the way
  `targets.populate` builds static targets: asm region -> `assemble_region` -> 003 `gate_target`. A failed gate
  refuses (no raw-word fallback). insn_count = the region's instruction count (the extent less alignment padding;
  the region's words must equal the ROM words).
- A row at NAME's address under another name is retired (the rename); a row named NAME elsewhere is re-addressed
  (its matrix evidence superseded via `targets._supersede_target`, fingerprint/cluster cleared); `--retire` drops
  a row that is not a layout function start. Retire = delete n64_target + matrix_entry + function_status rows.
- Refuses (nothing written) if a retired row is targeted by a matched.lock.json entry, has a promotion_record or an
  open work unit, is non-static, or is itself a current layout function name; if `--retire` names a function
  start; if NAME is missing/ambiguous in the layout or has no region. One transaction; other rows untouched.
- `audit` (read-only) on the current inventory: `audit.log` - the 8 renames, the 2 bogus rows, osYieldThread, and
  two not in the brief: `__osGetCause`@0x8000DBB0 (layout name `osCauseGet`, the 0xe4f0 tail, asm-only) and
  `__osContAddressCrc` insn_count 23 vs 52 (object is right; scores 0). Both can be added to the same command
  (`... osCauseGet __osContAddressCrc`); I left them out to keep to the brief.

Proof on a scratch copy (fresh copy of the live DB at the end of the session; `dryrun.log`, `apply_copy.diff`):

```
register  __osEPiRawWriteIo        @0x8000FE00   86 insns seg 0x10a00  reloc_aware o=1ae5d9382c22  (new row)
...
readdress __osContDataCrc          @0x8000FAC0   37 insns seg 0x105f0  reloc_aware o=1f84e0bc636d  (was 0x8000FA4C (29 insns); object changes, supersedes 578 matrix_entry rows)
refresh   osYieldThread            @0x80010110   19 insns seg 0x10d10  reloc_aware o=e6c91198258f  (was 4072 insns; object unchanged)
retire    read_controllers         @0x800140B0  mid-function: inside func_80013DEC @0x80013DEC; ...
dry run: nothing written (18 actions; 2263 other n64_target rows untouched)
applied 18 actions to <scratch copy>/conveyor.db: n64_target 2281 -> 2279, 578 matrix_entry rows purged
```
The dry run left the copy byte-for-byte unchanged (table dump compared). After apply, a full dump diff shows only
the 18 planned target rows, their function_status rows and the 578 fragment matrix rows. osYieldThread's
promotion_record is kept.

Scores against the NEW rows' objects (builder, w16g `check.py`, `run_scores.sh` -> `scores.log`;
`collect_targets.py` copies the objects out of the scratch DB): `RESULT PASS count: 24 of 24`. The 8 renamed ones:
```
__osEPiRawWriteIo LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (88 rows)   TU: same
__osEPiRawReadIo  (89 rows)  __osContDataCrc (37)  __osSiDeviceBusy (8)                 all 0/0/0 identical
__osResetGlobalIntMask (27)  __osSetGlobalIntMask (24)  osDestroyThread (79)  osYieldThread (25)  all 0/0/0 identical
```
`__osContDataCrc` also passes in the real current `src/rom/lib_105f0.c` (after the promoted __osContAddressCrc):
`TU: true=0 strict=0 reloc_blind=0 objdump-dr identical (37 rows)`. I did not run `lock add` (it submits to the
live pool); `check.py` replays its `verify_entry` compile+score.
Preconditions (`precheck.py` on the scratch DB, `precheck.log`): `23/23 preconditions OK`. For each renamed
function the segment flagset is pinned when the lock names the current id, and is `None` when it names the old id
(the w16g refusal):
`0x10a00 __osEPiRawWriteIo PRECONDITIONS OK (after layout convert): ... flagset '-g0 -O2 -mips2 -G 0 -non_shared' (lock on old id osEPiRawStartDma: None)`.

## B. `-mips3 -32` (unblocks 916 B)

1. `write_opt_overrides`: a converted segment whose flagset has `-mips3 -32` emits
   `$(BUILD_DIR)/src/rom/lib_e4f0.o: CFLAGS := $(subst -mips2,-mips3 -32,$(subst -O2,-O1,$(CFLAGS)))`
   and is not skipped as default (also at -O2). Any other ISA (`-mips3` without `-32`, `-mips1`, two ISAs)
   raises SystemExit. -mips2 segments: output unchanged (the committed opt_overrides.mk regenerates identically).
2. `o32abi.py`, run in the ROM-TU rule after asm-processor and before `owned_data object`, only when CFLAGS
   contain `-mips3` (`$(if $(filter -mips3,$(CFLAGS)),...)`); the script also requires both `-mips3` and `-32`
   (otherwise it does nothing). It refuses anything that is not a big-endian ELF32 MIPS relocatable at MIPS III,
   and changes only e_flags. Result: `lib_e4f0 Flags: 0x20000101, noreorder, 32bitmode, mips3`; -mips2 TUs stay
   `0x10000000, mips2`.

Builder proof (scratch, rebased tree; `builds.log`):
- conversion only (commit 3da7cd6c): `build/us/rush2049.us.z64: OK` / `ROM matches!` (make and make test exit 0).
- 16 libgcc bodies spliced by promote_batch's own `plan()`+`splice()` with `ctx/ctx_ll.jsonl` into the converted
  TUs (`splice_libgcc.py`, run in a Pi scratch copy with the 16 locks injected), opt_overrides regenerated:
  `ASMPROC src/rom/lib_e4f0.c` ... `build/us/rush2049.us.z64: OK` / `ROM matches!` / `test exit 0`, objects newer
  than sources, SHA-1 3f99351d7bb61656614bdb2aa1a90cfe55d1922c.
- Negative control (same tree, Makefile without the o32abi line):
  `mips-linux-gnu-ld.bfd: build/us/src/rom/lib_e4f0.o: linking 32-bit code with 64-bit code`, Error 1.
- Lock-style scores, 16 libgcc functions at `-g0 -O1 -mips3 -32 -G 0 -non_shared`: all LOCK and TU
  `true=0 strict=0 reloc_blind=0 objdump-dr identical` (`scores.log`). __fixunsdfdi/__fixunssfdi stay
  passthrough (data-bound, w16g).

## Commands for the coordinator (main checkout)

0. Take commits d5d87856 and b4d28ac6 (they rebase cleanly on master f0a30006). Commit 3da7cd6c is optional:
   step 4 below converts after the locks, which is how opt_overrides.mk picks up the flagset. If you take 3da7cd6c,
   regenerate it after the locks with
   `python3 -c "from tools.conveyor.pipeline import layout; layout.write_opt_overrides()"`.
1. Back up and re-register (no sqlite3 CLI on the Pi):
```
python3 -c "import sqlite3,os;h=os.path.expanduser('~/.conveyor/');s=sqlite3.connect(h+'conveyor.db');d=sqlite3.connect(h+'conveyor.pre-w16i.db');s.backup(d)"
R="__osEPiRawWriteIo __osEPiRawReadIo __osResetGlobalIntMask __osSetGlobalIntMask __osSiDeviceBusy osDestroyThread __osContDataCrc __osProbeTLB osYieldThread"
python3 -m tools.conveyor.pipeline.targets_rename apply $R --retire read_controllers --dry-run   # compare with dryrun.log
python3 -m tools.conveyor.pipeline.targets_rename apply $R --retire read_controllers
python3 -m tools.conveyor.pipeline.targets_rename audit    # expect only __osGetCause and __osContAddressCrc
```
2. Locks under the current names (no `--target`):
```
for f in __osEPiRawWriteIo __osEPiRawReadIo __osContDataCrc __osSiDeviceBusy; do
  python3 -m tools.conveyor.pipeline.lock add cloud/work/frontier/w16g/sources/$f.c:$f --flags "-g0 -O2 -mips2 -G 0 -non_shared"; done
for f in __osResetGlobalIntMask __osSetGlobalIntMask osDestroyThread; do
  python3 -m tools.conveyor.pipeline.lock add cloud/work/frontier/w16g/sources/$f.c:$f --flags "-g0 -O1 -mips2 -G 0 -non_shared"; done
L="__lshrdi3 __umoddi3 __udivdi3 __ashldi3 __umoddi3_alt __divdi3 __muldi3 __qdivrem __moddi3 __ashrdi3 __fixdfdi __fixsfdi __floatdidf __floatdisf __floatundidf __floatundisf"
for f in $L; do
  python3 -m tools.conveyor.pipeline.lock add cloud/work/frontier/w16g/sources/$f.c:$f --flags "-g0 -O1 -mips3 -32 -G 0 -non_shared"; done
```
3. Convert (0x105f0 is already converted on master):
   `for s in 0x10a00 0x10b60 0x10970 0x109a0 0x10cc0 0x107a0 0xe4f0 0xa5f0; do python3 -m tools.conveyor.pipeline.layout convert $s; done`
   Check `src/rom/opt_overrides.mk` has -O1 lines for lib_109a0, lib_10cc0, lib_107a0 and
   `$(subst -mips2,-mips3 -32,$(subst -O2,-O1,$(CFLAGS)))` for lib_e4f0 and lib_a5f0. Builder gate
   (`make COMPILER=ido && make test`), then commit locks + conversions. promote_batch refuses a dirty `src/rom`
   or `matched.lock.json`.
4. Promote:
```
python3 cloud/work/boot_tail_promotion/promote_batch.py cloud/work/frontier/w16g/ctx/ctx_O2.jsonl 0x105f0 0x10970 0x10a00 0x10b60 --only __osContDataCrc,__osSiDeviceBusy,__osEPiRawWriteIo,__osEPiRawReadIo
python3 cloud/work/boot_tail_promotion/promote_batch.py cloud/work/frontier/w16g/ctx/ctx_O1.jsonl 0x107a0 0x109a0 0x10cc0 --only osDestroyThread,__osResetGlobalIntMask,__osSetGlobalIntMask
python3 cloud/work/boot_tail_promotion/promote_batch.py cloud/work/frontier/w16g/ctx/ctx_ll.jsonl 0xe4f0 0xa5f0 --only $(echo $L | tr ' ' ,)
```
   Sizes: 352+352+160+48 + 96+80+272 = 1,360 B; libgcc 704 + 212 = 916 B (static only).

## Caveats

- The O2/O1 promote_batch runs for the 7 renamed functions were not replayed in full-ROM context here (only
  LOCK/TU scores, and real-TU context for __osContDataCrc); the libgcc one was replayed in full.
- `__osProbeTLB` is registered under its current name but has no C (asm-only); registering it changes nothing else.
- The live DB may change before you run step 1. The dry run prints the plan; refusals stop it before any write.
- `targets_rename` needs `tools/decomp-permuter` importable (the scorer), like `matrix extract`. The main checkout has it.
- Builder scratch tree: `~/rush2049/scratch/frontier/w16i` (build3/4/5 logs, scores.log). IDO there is a symlink
  to the toolkit cache d4c39cbc.../ido; game blob = the Pi's current `build/blob/game_code.deflate`.
