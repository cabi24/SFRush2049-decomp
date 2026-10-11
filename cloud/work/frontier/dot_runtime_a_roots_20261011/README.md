# Genuine availability-root dependencies: frame-only research checkpoint

**No new match, accepted-image bytes, or cartridge coverage.**

The complete 1,292-byte A:80394058 root improves from the previous packet's
317/323 differing words to **2/323**, with no excess or relocation uncertainty.
Only its stack adjustment and restoration differ: 120 bytes instead of 128.
All 321 other instructions match, including every integer/FP save and restore,
all 84 GNU-resolved relocation records, both signed-byte spills at stack offsets
119/118, and the full ordered static call sequence. No padding or pressure
variable was added to close the remaining gap.

## Genuine context

New complete source reconstructs A:803923B0 (2,936 native bytes) and A:80393B3C
(1,308 native bytes) from their protected image-A extents. The renderer supplies
all six naturally used callee-saved FP pairs, reproducing the root's previously
missing 48 bytes of FP saves. An ordinary local captures the root's witnessed
mode load and resolves four register differences. Its two original byte locals
and every other root statement are unchanged.

The five earlier helper matches, 92F28/93004/930B8/931C0/932E4, remain complete
strict matches. They receive no duplicate credit. The 9338C body is unchanged
and remains 475/490, 1,952 generated versus 1,960 native bytes. The original
translation unit is not known; 915E8 remains an external renderer dependency.

The new FP bodies are **nonmatching research**, with identical ordered static
call sequences but unresolved positional literal comparisons. Canonical scores
and full ELF sizes are in verification.json. The root has no owned-data
references, and its complete relocations are independently resolved with GNU.
No source-body or full behavior match is asserted for the two FP dependencies.

## Storage and literal ownership

The former 64-byte record view is refined from sixteen words to its witnessed
fields: two words, angle at +8, 3x3 matrix at +12, three-position vector at +48,
alpha byte at +60 and three opaque trailing bytes. Cleanup still accesses its
unchanged word member. The renderer's flag row is signed, matching native lb.
The initializer's local scale variable carries its witnessed constant-one FP
value; writing literal *=1.0f instead folds away the native multiplications.

The **entire compiler .rodata section, 48 bytes**, independently equals the
unique non-code image-A interval **803B9440..803B9470**. The authenticated
compressed asset, loader pointer, decompressed image hash, full target manifest,
unique section location and absence of overlap with function extents are checked.
Only its hash and interval are published. No literal payload is published.

This ownership check does not override canonical acceptance. Because the FP
bodies are structurally nonmatching, positional own-data inference reports
unverified references or differing content at unrelated candidate offsets.
Those failures remain visible in the receipt; no masked or approximate score is
presented as a match.

## Bounded source experiments

- Full initializer alone: root gains four FP save pairs; still nonmatching
- Full renderer plus initializer: root reaches 6/323, all six FP pairs present
- Capture the root's actual mode load: 2/323, retained
- Capture input bits locally: 9/323, reverted
- Rewrite 9338C's empty first branch as a positive enclosing guard: unchanged,
  reverted; source retains the archived body
- Initializer literal scale versus local scale, pointer versus bounded index
  record traversal: complete reconstructions, no match claim; indexed local-scale
  version retained with all original helper matches intact

Replayed context ablations show that removing the renderer regresses the root
to 321/323; removing the initializer alone leaves 2/323, because the renderer
already supplies the full FP clobber set. Removing mode capture gives 6/323.
The initializer remains included as genuine source/dependency and data research,
not as a claimed necessary root-allocation control.

## Verification and ABI limits

- All six selected complete ELF bodies independently extracted and GNU linked;
  all inherited helpers exact, root exactly the documented two words off
- 1,024 cases on protected native root and independent GNU-linked candidate
- Identical non-stack memory, ordered callback arguments and saved-register
  results; root restores s0-s7/fp and all six callee-saved FP pairs
- 307/323 root instructions and 75 branch outcomes execute; every one of its
  14 distinct callback targets is reached. This is bounded, not full coverage
- Explicit helper-boundary fixtures exercise clobbers and selected state effects,
  including a callback rewriting the handle before the root reloads it
- C89 syntax check, fail-closed decoders, source/scope guards, import isolation,
  normal and optimized-Python replay

The fixtures do not execute the reconstructed initializer or renderer, nor prove
arbitrary child side effects, invalid-state handling, concurrent mutation, or
whole-caller/gameplay semantics. Root temporary stack writes differ because the
frame is not exact. Private helpers are not standalone O32 replacements. No
image splice, compression, ROM SHA-1, hardware, or gameplay gate was run.

## Reproduce

From repository root with stock IDO 5.3 and GNU MIPS tools configured:

```
python3 cloud/work/frontier/dot_runtime_a_roots_20261011/verify.py
python3 -O cloud/work/frontier/dot_runtime_a_roots_20261011/verify.py
REQUIRE_TOOLCHAIN=1 python3 -m pytest tests/conveyor/test_runtime_a_roots.py -q
```

The frozen baseline is 84bdadc830834bde962aa532874a114b48353a06, whose tree
matches draft PR310. GitHub preflight at 14:44 UTC, 2026-10-11 found master
0119270d89538f38195e8a36ad66269c04ebb4de and open PRs 163,305–310. No selected
new claim overlaps those branches. PR295's separate 998C0 initializer is
analogous provenance, not a 93B3C result. No moving-baseline chase occurred.

Wave-one investigation started 14:43 UTC. Full dependency source reached the
six-word root at 14:48; mode capture reached the two-word checkpoint at 14:49.
Source, tests and metadata only. Protected inputs, scorer and production source
are unchanged. No ROM bytes, assembly dumps, binaries, credentials, or unrelated
private information belong to this packet. Nothing is merged.

## Wave two: independently inspect the remaining frame

A separate worker replayed the complete source and confirmed that native frame
bytes 120..127 are unused by every native instruction. All save slots and both
byte homes are already exact. Six bounded captures of actual values were tried
independently (repeat flags, pressed flags, held flags, record count, level bound,
selected handle); they either preserve the two-word residual or alter real code.
None is adopted and no frame padding is introduced.

The stock compiler diagnostic in compiler_evidence.py captures UGEN's input
metadata using only its existing -e/-l diagnostics and proves the entire object
is identical to the unchanged canonical pipeline. The root is external with
memory definitions T=16, P=4, M=2. Its two byte locals survive optimization;
ordinary integer captures can be removed from the memory-area budget. The same
script independently compiles and checks all refined record/color field offsets.

Existing compiler research in cloud/work/frontier/tools/trace/README.md explains
that coloured expression webs can receive homes even when never spilled, with
reuse determined by basic-block conflicts. That is a grounded investigation
avenue, **not evidence of the original retail frame's missing ownership**. A
parallel renderer family also has an eight-byte top-of-frame gap; this similarity
does not establish a common original declaration or permit invented padding.

The source checkpoint remains nonmatching. Reproduce stock metadata with:

```
python3 cloud/work/frontier/dot_runtime_a_roots_20261011/compiler_evidence.py
```
