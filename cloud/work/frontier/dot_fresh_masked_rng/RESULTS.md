# Masked RNG: bounded O3 research, no new match

Base: `cf10b3392d7f00ae42d75c008b79fdc2541aab6b`.
Target: `func_800B23E0`, `[0x800B23E0, 0x800B24EC)`, 268 bytes / 67 words.
Status: **unaccepted nonmatch**, 13 full relocated words differ. No splice,
lock, matched-source, shared context, target, symbol, or builder changes.

## Native evidence and source

The function takes an unsigned-byte table index and rejection-samples a bit
number from 0 through 31 until that bit is enabled in `D_80123418[index]`.
Each attempt advances the seed at `D_8011735C` using the ANSI-style LCG;
its 15-bit result is converted to float, multiplied by 32, divided by 32768,
then converted through the native unsigned conversion path and narrowed to
a byte. It returns that byte. A zero mask loops indefinitely. No table contents
are needed or included in this packet.

The native body has ordinary ABI argument/return handling, no stack frame,
no calls, and no unsaved callee-saved register writes. Scanning the current
protected game targets found no direct `jal` callers. This does not rule out
indirect references or inlined uses.

The real ranged-RNG helper `func_8008B2E4` has the same LCG and float scaling
operation. The real integer RNG is `src/blob/func_8008B2B4.c`. These are genuine
donor/context leads; this packet does not claim either helper matched inside
the experimental groups. Arcade source was not installed in this cloud
checkout, so no arcade ancestry claim is made.

`best.c` retains the historical meaningful float intermediate and uses an
unsigned left-shift operand to avoid signed `1 << 31` undefined behavior.
The modulo-32-bit LCG multiplication is unsigned. The seed's signed view
preserves the observed shift, followed by the 15-bit mask. No fake parameters,
keepers, padding, inline assembly, dummy operations, register annotations,
or permissive scoring modes were introduced.

## Bounded investigation

The frontier plan and prior `game_C26` record were read before editing. Its
direct expression reproduces 24 differences at both O2/O3, and the float-local
source reproduces 13 at both levels. Workbench `diagnose` was run on a private
native target object and the float-local O3 candidate before searching. It
identified entry structure/allocation differences; its relocation-symbol
warning reflects the private absolute target fixture. The authoritative proof
here resolves actual candidate relocations and compares every complete word.

`experiments.py` deterministically replays 41 successful compiles:

- 4 historical baseline/flag combinations
- 7 table placement, local naming/reuse, bit-local, signedness, and width controls
- 17 meaningful seed/random intermediates, loop forms, and return-width controls
- 5 arithmetic/operand-expression controls
- 8 real ranged-RNG or integer-RNG donor contexts, kept/internal or ordinary O3

All remain nonmatches. Best is still 13/67, so the historical result was not
improved. One initial combined-donor probe had duplicate typedefs and did not
compile; the replay removes only that duplicate and records the successful
control. No result from the failed compile was used.

The remaining differences are confined to entry scheduling and register
allocation of the table mask, seed address, and LCG multiplier, plus the final
mask operand order. The floating calculation and complete unsigned conversion
sequence already agree. The strongest next hypothesis is authentic surrounding
module/inlining context that changes those integer webs. The tested real donor
subsets did not establish that context. Do not repeat a blind expression sweep.

## Verification

Flags: `-g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul`.
The pinned local recovery toolchain was used, without any remote builder calls.
`verification.json` binds the final source, compiler binaries, protected-target
manifest, scorer, target body, and relocated candidate body by SHA-256.

```sh
python3 cloud/work/frontier/dot_fresh_masked_rng/experiments.py
python3 cloud/work/frontier/dot_fresh_masked_rng/verify.py
python3 -m pytest -q tests/cloud/test_dot_fresh_masked_rng.py
```

Final verification: 67 full words, **13 different**; ELF `STT_FUNC` size exactly
268 bytes; no masks, unresolved symbols, unverified references, or relocation
errors. The trailing object alignment word is outside the actual ELF function
extent and earns no body credit. The runner deliberately returns a successful
research verification while recording `status: nonmatch` and
`strict_match: false`; its process exit code is not a match claim.

Three focused tests pass: source-bound negative evidence, fresh IDO replay, and
576 host executions with undefined-behavior sanitization. The host cases cover
every single-bit mask, alternating/all/high-plus-low masks, boundary and varied
seeds, and index endpoints. Both the returned choice and updated seed agree
with an independent integer rejection-sampling model. Host semantic tests do
not establish target byte identity. Zero-mask nontermination is documented,
not executed.

No image build or full-ROM gate was run; this packet adds zero accepted bytes.
Native dumps, object files, and temporary probes remain outside the commit.
