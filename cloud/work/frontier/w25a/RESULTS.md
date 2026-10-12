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
