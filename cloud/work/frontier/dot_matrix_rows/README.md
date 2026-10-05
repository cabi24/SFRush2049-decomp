# Two complete matrix-row rotation matches

Date: 2026-10-05. Base: `cf10b3392d7f00ae42d75c008b79fdc2541aab6b`.

## Result

- `func_800ACFF8`: complete `[0x800ACFF8, 0x800AD090)`, 152 bytes.
- `func_800AD090`: complete `[0x800AD090, 0x800AD128)`, 152 bytes.
- Both print strict `MATCH` using the unchanged `tools/cloud/score.py`.
- Both also have exact 152-byte ELF `STT_FUNC` sizes and independently compared
  complete relocated byte streams. There are no masks, unresolved symbols,
  unverified relocations, errors or extra nonzero words.
- All eight replay cases pass: each function alone at `-O3`, alone at `-O2`,
  together with the genuine two-member `-O3` pipeline, and added to the unchanged
  real `frontier_traction_control` caller group.

This is **304 bytes of new object-match evidence, not cartridge coverage**.
No splice, lock, target/context update, shared type change, ROM build or merge
was performed. The whole-program shadow gate and source-built image/ROM gates
remain integration requirements. This packet does not certify unrelated CI.

## Native semantics and source recovery

These are leaf functions with the ordinary float/float/pointer calling
convention. They take already-computed sine and cosine coefficients and update
two rows of a row-major 3x3 float matrix, one column at a time. ACFF8 rotates rows
1 and 2; AD090 rotates rows 0 and 1. The untouched row is preserved.

For the old components `a` and `b`, the outputs are
`a * cosine - b * sine` and `b * cosine + a * sine`. A single real temporary
preserves the first result until the original component has been consumed by
the second result. There is no dummy local, fake formal, compiler register
constraint, inline assembly, stand-in caller or context helper.

The authenticated native call graph has one direct caller, `steering_sensitivity`,
with three JAL sites to each target. Existing reconstructed caller context is in
`cloud/work/frontier/w2g/groups/traction_control/group.c`. Its coefficient
calculations and matrix pointer use are consistent with this interpretation;
this work makes no match claim for that caller. No arcade ancestor is asserted.

The real-caller probe compiles `src/blob/groups/frontier_traction_control` first
unchanged, then with the two separate new leaf files added and kept. All five
existing functions' complete relocated ELF bodies are byte-identical before
and after. The four formerly strict members/context helpers stay strict, and
the retained `steering_sensitivity` candidate remains a 1008-byte nonmatch
(230/232 differing target words, 19 extra nonzero words). The leaves remain
exact with real caller context and expose no changed caller/clobber behavior
in this probe. This does not turn that near-miss into a matched caller closure
or replace the whole-program integration gate.

## What changed from the previous attempts

The old `cloud/work/game_C24/` complete seeds use six spill/register-derived
locals. At both O2 and O3 those seeds give 37/38 differing positional words,
four extra nonzero words and an oversized function. Earlier register and
temporary-reuse controls are retained in that packet; they were not repeated
as a search strategy here.

After reading the 2026-10-04 frontier plan and native complete extents, a bounded
set of natural source forms tested scalar input pairs, a single output
temporary, a row temporary, a matrix copy and fixed-trip-count loops. The
single output temporary immediately reproduced the 152-byte function and
16-byte frame, leaving six words different.

Workbench diagnosis showed identical opcode sequence, no alignment gaps and
identical destination-register lanes. Inspection localized all six residuals
to the two multiplication inputs in each addition. Writing the addition as
`b * cosine + a * sine` instead of `a * sine + b * cosine` closed all six at
both O3 and O2. This is expression evaluation order, not an unexplained global
register-coloring workaround. Applying the same natural form to the second
row pair produced the second strict match immediately. No broad permutation
search was needed.

## Reproduce

Use the repository's pinned IDO toolchain, with `IDO_DIR` pointing at its IDO
binary directory. From the repository root:

```sh
python3 cloud/work/frontier/dot_matrix_rows/verify.py
python3 -m pytest -q tests/conveyor/test_dot_matrix_rows.py
python3 tools/cloud/score.py fn cloud/matches/func_800ACFF8.c func_800ACFF8 \
  --flags '-g0 -O3 -mips2 -G 0 -non_shared'
python3 tools/cloud/score.py fn cloud/matches/func_800AD090.c func_800AD090 \
  --flags '-g0 -O3 -mips2 -G 0 -non_shared'
```

The verifier explicitly uses `-Wab,-r4300_mul`, also supplied automatically by
the normal scoring command. It writes binaries and a fresh receipt only under
`build/dot_matrix_rows_verify/`; `--output` can select another private directory.
The checked-in `verification.json` contains source, toolchain, authenticated
target-manifest, target-byte and relocated-byte SHA-256 identities, sizes and
strict comparison results. It contains no raw target bytes or assembly dump.

Five focused tests cover both functions on an independently calculated float32
reference (244 coefficient/matrix cases per function), untouched rows and
surrounding storage guards, saved receipt/source/target consistency, rejection
of an ELF size that falsely claims alignment zeros as code, and rejection of a
changed instruction. The last two tests require IDO and skip explicitly when
it is unavailable. Host behavior tests require a host C compiler. Neither kind
of test substitutes for the fixed-IDO complete-byte acceptance proof.

The optional broader `tests/conveyor/test_cloud_score.py` run encountered 15
failures in untouched locked singles whose own `.rodata`/`.data` references
are reported as unverified by the existing scorer. None involved these two
new leaves. No unrelated CI, locked source or scorer change was attempted.

## Independent review and integration

Independently rerun the verifier and the five focused tests on the frozen
commit. Check both natural source formulas against the complete native target
and inspect the two negative controls. The ordinary scorer permits trailing
section-alignment zeros; the explicit ELF-size assertion intentionally closes
that gap for this packet.

The parent integration lane owns draft-PR publication and any later source
integration. Leave merging to the owner's independent checker. Before any
coverage claim, use the normal splice workflow, whole-program shadow gate and
source-built image/ROM acceptance gates without weakening their checks.
