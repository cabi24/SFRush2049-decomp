# BT06 matrix and vector math trio

Three first-form natural C89 bodies are strict local O2 matches: **388 bytes /
97 words**, with exact ELF function sizes **172, 104 and 112 bytes**. No candidate
variants were needed. Independent review PASS binds source commit
`bedc216cc9157395c533ba8a16b7fc5a1b3ad360` and tree
`7d4fd9399a11e57ac02f6084607d525af478283e`; see `independent_review.json`.
The reviewer replayed the native scores, exact extents, layouts, pinned inputs,
and both 6,007-call host modes, and independently fetched the pinned source and
license. Exact aggregate-head CI remains pending. These are source matching
results, not cartridge coverage or promotion.

- Owner: `/root/match_boot_tail_low_final`.
- Branch: `dot/boot-tail-bt06-math-trio`.
- Base: `b5daa44a` (full identity in `preflight.json`).
- Exclusive central activation: `b33647f8`.
- Exact ranges: `[80024BF0,80024C9C)`, `[80024C9C,80024D04)`,
  `[80024D04,80024D74)`, respectively 172/104/112 bytes.
- `claim_scan.json` records the fresh base STATUS rows and claim hashes. All three
  rows were open and absent from existing exact claims before editing.
- Changes are only the three named match files and this directory. Central STATUS,
  D10, specs, symbols, targets, scorer, shared types, locks and layouts are unchanged.

## Provenance and actual ABI

The source lead is AxioDL/musyx at pinned revision
`78d2e16e4905fc675952162d331c24d5198b2687`,
[`src/musyx/runtime/snd_math.c`](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/src/musyx/runtime/snd_math.c),
under the pinned [CC0-1.0 LICENSE](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/LICENSE).
Both files were read for this packet. The three named definitions are
`salApplyMatrix`, `salNormalizeVector`, and `salCrossProduct`. Adaptations use
address-spelled function names, self-contained native-layout types, and a C89
local declaration. This does not establish exact original N64 translation-unit
provenance or rename repository symbols.

Actual protected native bodies and callers were inspected before editing:

- `24BF0` is `void(const Matrix *, const Vector *, Vector *)`, with matrix in a0,
  input in a1, and output in a2. Caller `1C860` at `+284` passes state+72,
  input+12, and its actual output vector. The matrix comprises three 3-float rows
  and three translation floats at offsets 36/40/44. Output components store
  sequentially, and input components reload between outputs; input/output
  aliasing therefore has sequential rather than simultaneous-transform behavior.
- `24C9C` is `float(Vector *)`, and caller `1C860` at `+378` supplies its actual
  vector pointer. Three square products are added in native single-precision
  order and passed in f12 to static `sqrtf` at `8000E3C0`. The tracked
  `asm/us/EFC0.s` proves the callee returns `sqrt.s` in f0. After the call, the
  body reloads all three components and divides them by that returned length;
  length remains the function return. The native 24-byte frame and a0 home
  store follow directly from the genuine call and pointer use.
- `24D04` is `void(Vector *, const Vector *, const Vector *)`. Real caller
  `1D5C0` at `+18` passes output state+48 and inputs state+60/state+36. Paired
  products/subtractions follow the standard cross-product cyclic component
  indices. Stores are sequential, with input reloads between components, so
  outputs aliasing either input retain the actual sequential semantics.

The native vector has three floats at offsets 0/4/8 and size 12. The matrix has
size 48. IDO compile-time assertions independently verify all these sizes and
field offsets. No artificial padding, invented formals, fake calls, assembly,
forced registers, context helpers or shared-type edits are present.

All floating operands come from actual input fields. There are no constant-pool,
local-rodata, jump-table or runtime-global references. The only relocation is the
normal call to proven static `sqrtf`; the unmodified scorer resolves it through
the protected symbol map. No literal/table content was guessed.

## Compiler evidence and exact geometry

| Function | O2 strict comparison / ELF size | O1 control |
|---|---|---|
| `80024BF0` | MATCH 43/43 words, 172 B | Also MATCH, 172 B |
| `80024C9C` | MATCH 26/26 words, 104 B | 26/26 differ, 8 nonzero excess words, 140 B |
| `80024D04` | MATCH 28/28 words, 112 B | Also MATCH, 112 B |

Every control uses `-g0 -O2` or `-g0 -O1`, then `-mips2 -G 0 -non_shared`;
the unchanged scorer adds `-Wab,-r4300_mul`. Both arithmetic leaves are
frameless; O1/O2 cannot be distinguished from those two bodies alone. The
normalizer's matching O2 call frame/schedule and unsuccessful O1 body support O2
for this contiguous source-led cohort. No flag sweep was performed.

`verify.py` requires strict full relocated word equality **and** exact ELF
function-symbol extent for each selected O2 source. Zero section-alignment
padding does not establish a match. The 172/104-byte functions reside in
176/112-byte text sections; their symbol sizes remain exactly native. All
selected sources have zero differing/excess words and no unresolved,
unverified or erroneous relocations. Since all first bodies matched, there was
no near-match source refinement and no workbench-driven variant phase.

## Reproduction and tests

`preflight.json` pins the target manifest, inventory, static sqrt helper,
scorer/setup/getter, and all 24 already-approved IDO files. Manifest verification,
all 439 extents / 99,120 bytes, and strict getter replay passed. No compiler was
downloaded or copied. `verification.json` binds the exact source hashes and all
six flag controls, full relocated comparisons, ELF function sizes, and native
layout assertions.

From the repository root, using the existing pinned IDO installation:

```
python3 cloud/work/boot_tail/BT06-math-trio/verify.py --ido-dir "$IDO_DIR"
python3 cloud/work/boot_tail/BT06-math-trio/test_host.py
python3 cloud/work/boot_tail/BT06-math-trio/test_host.py --sanitize
```

The test script compiles the actual three source files as strict C89 with
floating contraction and fast-math disabled. It checks 6,007 calls: disjoint and
in-place transforms, cross products with disjoint outputs and outputs aliasing
either input, finite normalization/returned lengths, identity plus translation,
oriented unit basis, zero, signed zero, infinity and NaN. Oracles use indexed
loops and explicit sequential stores; bit equality is checked except for NaN
payloads. No invented zero-length guard is introduced. ASan/UBSan passed the
same controls. LeakSanitizer itself is unavailable under the sandbox's tracing;
leak detection is explicitly disabled for that run, with address/undefined
checks retained. The harness performs no dynamic allocation.

Host behavior does not prove cartridge execution, native exception-mode behavior
or NaN payload propagation. Native float instruction order and ABI are instead
covered by exact MIPS matching. No raw target bytes, disassembly dumps or objects
are committed. `status_delta.csv` requests only local-match claims until the
central lead has independent review and exact aggregate-head CI evidence.
