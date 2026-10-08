# D24C8 checkpoint crossing: complete bounded research

**NONMATCH: 60/280 differing words. No matching, accepted-byte or cartridge-coverage gain.**

The complete protected target is `race_countdown_display` / `race_mode_select`,
`[0x800D24C8, 0x800D2928)`, 1,120 bytes. The historical labels are wrong: this
updates checkpoint/lap state, records crossing times and queues finish/lap sounds.
The complete candidate is 1,116 bytes, with the native 160-byte frame. Its entry
and real helper expansion are recovered; one branch geometry difference,
field-load/temporary ordering and downstream schedule differences remain.

Base: `dea99f09ab19b1d3b324ed7097162f7b378e7096`. The newly accepted
`func_800D2054` and `car_stats_display` supplied the missing lap-recorder and
variadic event-constructor contracts. All acceptance assertions are against
that base, not a changing live tree. This packet makes no production changes.

## Genuine source and context

The arcade ancestry is `game/checkpoint.c:PassedCP` and
`get_next_checkpoint` in [rushtherock revision 845329d7](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/game/checkpoint.c).
The N64 adaptation takes `(Model *, float interpolated_crossing_time)`, uses
per-player records and native event/audio services, and wraps the next-checkpoint
index on equality rather than the arcade greater-or-equal condition.

Accepted `func_800F8EC8` at the recorded base independently identifies this
callee as PassedCP and supplies the same pointer/float prototype. The five
sound sites are complete native expansions of actual `func_800B61A8`; the
accepted direct-return wrapper is retained as real context. Explicit `__inline`
expresses this observed boundary. A genuine next-checkpoint helper supplies
its retained input/return-value topology. No unused locals, invented callers,
extra formals, volatile shaping, assembly or compiler/target changes were used.

`Model` is an accessed 0x7EC-byte prefix of the actual 0x808 record, used only
through one pointer. `GameCar` has the witnessed 952-byte stride, `Player` eight
bytes and `Track` a ten-byte accessed header prefix. Padding denotes witnessed
field gaps, not stack pressure. Original type names/TU identity are unproved.

Compiler research uses the current canonical group pipeline: IDO 5.3,
`-g0 -O3 -mips2 -G 0 -non_shared`, and its documented as1 `-r4300_mul` step.
The extra assembler step is disclosed; this is not a strict match under any
recipe. Both caller and sound wrapper are kept. The helper leaves an eight-byte
deleted-procedure stub outside either named ELF function.

## Results and verification

- Complete ELF target: **1,116 bytes**, retained sound helper **84 bytes**,
  separate deleted-helper stub **8 bytes**, and **8 zero alignment bytes**.
  Complete `.text` is 1,216 bytes. No owned data, common symbols or unresolved
  references; all **56 relocations** are validated.
- The whole unchanged object is linked independently by GNU at a placement
  giving the target its exact native address. `SUBALIGN(4)` prevents GNU from
  silently rounding that placement by four bytes. Every complete-text word
  agrees with the canonical relocator.
- All 84 bytes of the actual sound-helper ELF body remain native-equal. The
  canonical context summary reports one extra nonzero word because it also
  sees the following deleted-static stub; that warning is retained and explained,
  not suppressed or relabeled as a canonical context MATCH.
- **3,208 fixtures / 9,624 native, project-relocated and GNU-linked executions**
  agree on whole mapped backing and call-entry memory/argument snapshots.
  All **280 native and 279 candidate words**, and both outcomes of every actual
  conditional branch, execute. O32 caller-save poisoning, callee-save registers,
  restored stack and canaries are checked.
- Five explicit hook modes cover no mutation, checkpoint-helper changes to the
  finish index, lap-helper changes to lap count, finish-helper changes to place
  lock/player kind, and sound-helper changes to flags.
- The separate independent proof runs unchanged-source C with UBSan/bounds and
  strict aliasing against its own native interpreter: **3,882 cases**, all native
  words, nine elapsed-time and five clock bit patterns, and seven rejected
  meaningful compiled-source mutants. See `independent/verification.json`.

Real called-helper internals do not execute in these behavioral fixtures. They
are explicit bounded, side-effecting hooks. The actual retained sound-wrapper
body is separately compiled and byte-checked. The producer uses valid slots
0..3; independent tests add slot 5. These are synthetic backing domains, not
claims about original array bounds. The float paths only copy bit patterns;
no arithmetic/FCSR/NaN-payload behavior of external helpers is claimed.
Signed narrow writes follow the measured IDO/GCC low-bit behavior. Invalid
pointers, unrestricted aliases, concurrency, hardware, gameplay, full-image,
compression and ROM identity are outside scope.

## Bounded experiment history

`controls/ledger.json` replays eight complete source/context recipes:

1. First natural body plus ordinary accepted sound wrapper: 171/280, 72-byte frame.
2. Donor increment-style next-checkpoint helper: 269/280, 80-byte frame; rejected.
3. Conditional-return helper control: 273/280, 88-byte frame; rejected.
4. Real sound wrapper explicitly inline, with its original result local:
   114/280, 192-byte frame.
5. The already accepted direct-return sound body: 105/280, 152-byte frame.
6. Signed next-checkpoint early-return helper: 62/280, native 160-byte frame.
7. Explicit equal-remaining early return: unchanged 62/280; rejected.
8. Reorder two existing, consumed scalar declarations to reproduce the native
   lap-flag spill home while retaining the player home: 60/280.

The first broad failure was genuine missing inline context, not an allocation
problem to solve with artificial pressure. Workbench diagnosis preceded further
controls. This lane stops at the current complete source: reopen only with a
new native branch/lifetime or authentic declaration/context explanation.

## Reproduce and portability

From the repository root, with the pinned toolchain available:

```
python3 cloud/work/frontier/dot_checkpoint_crossing_20261006/verify.py --check
python3 cloud/work/frontier/dot_checkpoint_crossing_20261006/independent/verify_host.py --check
python3 -m pytest tests/cloud/test_checkpoint_crossing_packet.py -q
```

The producer loads production provenance with `git show <recorded-base>:<path>`.
Only own packet files, complete native words and proven compiled/relocation/data/
behavior evidence are frozen. No live manifest, symbols, scorer, accepted-source,
lock or test-file hash is pinned. Missing compiler/linker tests skip cleanly.
`--record` is an explicit reviewed receipt refresh, never a normal test side effect.

Focused packet tests and independent checks are separate from the mandatory full
`tests/conveyor tests/cloud` matrix. The shared current-master suite run, both
with and without IDO, is **pending at this freeze**. No full-suite/CI/ROM pass is
claimed here. Publication remains gated on that matrix and independent review.
