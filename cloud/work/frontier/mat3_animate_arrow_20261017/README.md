# AnimateArrow: source-backed exact N64 candidate

`entity_render_setup` at `0x8008C884` is AnimateArrow. This isolated source and
proof packet reproduces its complete 2,048-byte body, all 512 instructions, and
184-byte frame. The main claim is exactly this function. Seven existing locked
transitive callees also reproduce their complete extents, with every member and
context explicitly claimed in the closure manifests. They add no new bytes.
No canonical integration, ROM build, or runtime result is claimed here.

## Why the match is credible

The accepted MAT3 AnimateArrow body (1,516 bytes, task-11 PASS) and original DWARF
local order supplied the source evidence. The N64 callback retains `(visual,
s16 operation)`, its native dimensions, four-by-four signed fade array, five-entry
RGBA palette, native external white value, and world-arrow flags `0x3600`.
The GameCube flags and one-argument callback do not override the N64 evidence.

The decisive new context was the genuine `ZOID_HidePoly` helper. Then removing a
reconstructed constant-only local and restoring the original x/y/z-before-vscale/
voffset declaration order removed the remaining stack differences. The meaningful
19-unit border local remains: the accepted donor uses it and the native negation
uses its loaded value. This local is a meaningful reconstruction and is absent
from the original DWARF. Only the recovered x/y/z-before-vscale/voffset ordering
is authenticated by that original local evidence. No unused scalar, artificial operation read, pointer
laundering, split palette alias, or padding holder is present. Negative controls
compile the direct hide store and the reconstructed local order separately. The
latter loses seven instruction words while preserving the function extent.

The source uses a full 24-byte visual and 88-byte polygon rather than undersized
prefix views. The three-component arrays and RGBA union are compatible across
caller/callee declarations. The polygon and world-vector closure copies retain
exact bytes after those type clarifications. A data-only IDO probe checks 32
layout/type constants and rejects three wrong layouts by reading the emitted
values, rather than assuming compiler success proves a typedef assertion.

## Complete closure

- `8008B3C8`: magnitude, 44 bytes, extracted genuine body at its locked O2 flags
- `8008C074`: polygon update, 1,232 bytes, complete eight-case texture switch
- `8008C544`: world vector, 156 bytes, locked O2
- `8008C768`: atan2, 268 bytes
- `8008C720`: signed atan, 72 bytes, extracted genuine body at locked O2
- `8008C680`: argument reduction, 160 bytes
- `8008C5E0`: rational approximation, 160 bytes, real internal group context

The extracted magnitude excludes its inherited unrelated stand-in caller. The
rational approximation uses the already locked clean call-group body, not the
older standalone source's artificial declarations. Every listed function is
strictly checked for all instructions, relocations, complete symbol extent and
absence of extra words. Native direct/tail-call destinations must equal the
explicit graph. There is no unmatched callable boundary in this closure.

The deleted private HidePoly helper has an eight-byte pre-entry linker stub. It
is outside AnimateArrow's ELF extent, contributes no match credit, and is not a
claimed image function. The entry has no trailing extra instructions.

## Proof and limits

`verify.py` binds all source/proof files, target instruction digests, emitted
objects, trace receipt, claims and extents. Strict own-section verification is
required. An additional comparison checks the entire 48-byte quad template,
including zero coordinates omitted by trimmed relocation windows. The native
five-color palette, seven alpha levels and white value are checked independently.
The packet contains reconstructed semantic constants, not image dumps.

The separately expressed host model checks 7,210 valid-domain visibility, color,
alpha, view and geometry cases. The real magnitude and world-vector C bodies run;
atan is a deterministic injected boundary and the polygon call captures arguments.
The camera matrices are identity fixtures, the view radius and scale are fixed
positive values, and the car defense flags are always 1. Thus the host fixtures
do not exercise every alpha-gating or geometry input. These tests do not claim native MIPS
execution, general floating-point equivalence, arbitrary cameras, invalid-index
behavior, or full-game semantics. Native correspondence comes from the independent
strict instruction/data proof and complete locked closure.

Four compiled semantic negatives change default palette selection, the alpha cap,
world-arrow flags and one zero coordinate. Each loses strict correspondence and
fails the field/geometry model for a specific behavioral reason. Two additional
controls isolate real-helper context and local ordering. Focused tests run the full
proof and eleven integrity refusals under optimized Python, including missing
claims, changed model/targets, unverified relocation, truncated extent, and trace
receipt corruption. Observation-only traced uopt must reproduce the complete
stock object byte for byte. All raw traces and binaries stay under ignored build/.

Run with the repository's IDO and MIPS toolchain configured:

```
python cloud/work/frontier/mat3_animate_arrow_20261017/verify.py
python -O cloud/work/frontier/mat3_animate_arrow_20261017/verify.py
python -m pytest -q tests/cloud/test_mat3_animate_arrow_20261017.py
```

The baseline is commit `58425072b49e95f0fbe5c4c239988fb4885f7c5c`. `closure.json`
binds the original source provenance; the proof uses the baseline's integrity-
checked target artifacts and locked-function inventory. Independent review is
recorded separately and is required before publishing or counting this candidate.

## Clean-checkout portability correction

The first publication passed the complete strict proof but its final sealed
receipt was path-dependent: four single-file closure compiles recorded absolute
source filenames in IDO's `.mdebug` section, even at `-g0`. Only `.mdebug` changed
between checkouts; all instruction and relocation sections were identical. The
verifier now invokes those same sources from their directory using the stable
relative filename `source.c`. Complete object hashes remain required. No C source,
flags, target bytes, match result or function extent changed. A relocated-packet
regression and a fresh clean-checkout replay cover this correction.
