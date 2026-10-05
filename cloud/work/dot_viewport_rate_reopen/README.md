# B83 viewport-rate setter: bounded evidence-based reopen

Status: **COMPLETE-NONMATCH**, no claims, no added accepted bytes. The native
interval is `arb_rate_set@0x800A5908..0x800A5A40` (312 bytes, 78 words).
The historical name does not describe the viewport-initialization behavior.

Baseline: `55ddfc6b53d96d4c5dcaaa8891dedafb037b3994`, fetched on 2026-10-05.
This superseded the assigned `aac35341` baseline before any experiments began.
The caller is absent from the acceptance lock; the real direct callee is accepted.
No shared source, context, symbols, protected targets, locks, scorer, build flags,
or remote builder were modified.

## Why this frozen packet was reopened

[B83](../near_miss_B83.md) stopped at a 316-byte candidate with 73/78 differing
words and one excess word. Its genuine SDK `GPACK_RGBA5551` source is the seed.
The newly accepted [`exhaust_smoke_effect`](../../../src/blob/exhaust_smoke_effect.c)
demonstrates a specific compiler-source fact in this same viewport family:
integer literal `* 2` preserves single-precision multiplication, while `* 2.0f`
becomes addition. Its complete seven-argument View72 contract is now available.
The SDK macro is retained verbatim from B83's recorded donor; that donor checkout
is not present here, so this packet does not claim a new independent donor audit.

The new candidate changes all four viewport conversions to integer-literal
multiplication. It also passes ordinary opaque alpha `1` to the SDK macro;
retail has an immediate alpha bit and no reload of the stored 255 alpha byte.
Neither change introduces fabricated shift work or opaque constants.

## Results

Four literal controls were compiled alone and with the unchanged, complete
accepted callee. The caller results are the same in both contexts.

| Spelling / source | ELF function bytes | Full-relocation differing words | Excess bytes |
|---|---:|---:|---:|
| B83 `* 2.0f` | 316 | 73/78 | 4 |
| Integer `* 2` | 324 | 35/78 | 12 |
| Double `* 2.0` | 344 | 76/78 | 32 |
| Short float `* 2.f` | 316 | 73/78 | 4 |
| Integer `* 2`, opaque SDK alpha | **312** | **31/78** | **0** |

Nine further ordinary source controls tested the alpha member/cast, chained
RGB assignment, a consumed viewport pointer, the real callee's returned pointer,
packed-variable width, and alpha-statement placement. Together with the opaque
alpha candidate and the original four controls there are 14 distinct sources,
18 control compilations, then one final real-callee group build. No permutation
or line-layout search was run. `replay.py` reproduces the complete bounded set.

The stock scorer's integer control reports 36 differing words and an unpaired
HI16 because its target-sized comparison stops before that overlong candidate's
matching LO16. The full-STT_FUNC relocation resolves that pair and reports 35
different target words **plus 12 excess bytes**. Both observations are retained;
the full-extent result does not turn an overlong candidate into a match.

`candidate.c` uses the 312-byte opaque-alpha source. Its final stock scorer result
is 31/78 differing words, zero excess words, and no unresolved, unverified, or
relocation-error sites. Its separate full-function relocation result is identical.

## Independent checks

- Fresh canonical target and symbol hash verification, with exact ELF symbol sizes.
- Complete function relocation through each symbol's full extent, including the
  part after the native interval for overlong controls.
- Independent GNU link of the real contiguous 452-byte callee + 312-byte caller
  at their native addresses; its caller bytes agree with canonical relocation.
- Unchanged accepted callee: all 113 words equal, all three owned float literal
  references (six relocation sites, 12 literal bytes) verified against the
  manifest-checked existing data artifact. No literal bytes are copied here.
- Four zero bytes after the group's function extents are identified as section
  alignment, never counted as part of a function.
- 4,160 finite-input cases compare native execution, freshly compiled candidate
  execution, and host-compiled C: 8,320 native/candidate executions and 4,160 host
  comparisons. They check all seven callee arguments, all four view records,
  viewport fields, fog, fill color, stack restoration, and callee-saved registers.
- Negative tests reject a wrong callee destination and detect a changed alpha bit.

The semantic test uses a deliberately bounded model of the accepted callee's
write range and return value. It does not independently validate trigonometry,
projection math, invalid indices, NaNs, infinities, or out-of-range conversions.
Behavioral equality does not establish a compiler match.

## Residual and stopping condition

Workbench diagnosis was run before source controls and again on the best result.
The best result has the correct 56-byte frame and complete instruction count.
Its FP register sequences and temp-register sequence agree. The view-record and
viewport-address webs use opposite `v0`/`v1` assignments before the call; the
packed-color expression is evaluated and scheduled differently afterward.
No change among the bounded real-source controls closed these residuals.

Freeze again pending actual packed-color source/typing evidence or an authentic
complete caller context that explains those expression webs. Do not add fake
shifts, padding variables, unused formals, invented helpers, or changed flags.
The large `car_render_full` caller was not claimed or edited by this packet.

## Reproduce

Use the repository's pinned local IDO environment and GNU MIPS binutils:

```sh
python3 cloud/work/dot_viewport_rate_reopen/replay.py --output /tmp/viewport-rate.json
python3 -m pytest -q tests/conveyor/test_dot_viewport_rate_reopen.py
```

The replay exits successfully only when the documented nonmatch, callee proof,
and semantic checks reproduce. It does not splice or promote. Toolchain,
candidate, context, target, data-manifest, object, and relocated-body hashes are
recorded in `verification.json`; no raw objects, ROM bytes, or assembly dumps are
published. Local tools were used throughout.

The focused suite passes 9 tests with IDO available. Frontier `show` cannot run
from this fallback checkout because `build/blob_layout.json` is absent; canonical
target and lock inspection supplies the bounded identity checks instead. No
whole-program shadow, image, compression, or full-ROM gate is claimed. Unrelated
current Conveyor CI failures are outside this task.
