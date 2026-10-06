# Image-A option availability: genuine caller-preservation boundary

## Current receipt schema (2026-10-06)

Current receipts omit whole-file manifest, scorer/own-data tool, lock and redundant
production-context digests. The recorded BASE and `git show BASE:path` source reads
remain, as do native target authentication, own packet/verifier/C-harness bindings,
compiler executable identities, complete compiled extents, relocations, owned data
and behavioral evidence. Historical compatibility normalizers accept only their
explicitly listed legacy fields; unknown proof fields and changed invariants still
fail comparison. Descriptions of the earlier receipt schema below are historical.
This schema correction adds no matching or accepted bytes; fresh replay and the
aggregate test matrix are separate required checks.

**Complete NONMATCH research: image A `func_8039D494`,
[0x8039D494,0x8039D6A4), 528 bytes / 132 words.**
The complete natural C differs in **18 register-only words**. Matching-candidate,
accepted-byte and ROM-coverage gain are all **zero**.

The useful result is why this source cannot safely replace the native body:
its ordinary a0/a1 input convention hides a narrower private clobber contract.
The genuine caller at `A:8039E3BC` retains t2–t5 across the call. Native D494
preserves those registers; the isolated candidate changes all four.

## Preflight and provenance

Base: `master` cd22879d40b3de443cfde047b86e75e159b6cec6, checked through
the repository connector. Current handoff, source inventory, runtime matching
paths and all 43 open PRs through #152 were checked before reserving this
function. Only the protected target/symbol inventory named it on master.
No existing source or open claim was found. The investigation began with the
native ordinary-input leaf, then widened only to read and execute its real caller.

Image A is ROM stream 0xB5C534, base 0x8038A400, image SHA-256
0d6702c320df84cc6cbc3c5967dde08d44b6e476e110667fe2a43dc2dd536667.
The complete protected target, both native callers and their entry evidence
are pinned by extent and SHA-256 in `verification.json` and `verify.py`.

This is a N64 front-end option-availability predicate. The exact labels of
its 19 option IDs are not recovered. A read-only search of arcade
[rushtherock game/select.c](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/game/select.c)
(blob 84f947813ec0def745d93a2830cbbe91d353572c) found related selection
logic but no exact whole-function donor. Category: N64-specific front-end
implementation; no original-source recovery claim.

## What the complete function does

The source evaluates a series of independent exclusion gates and returns
zero or one. IDs 12 and 15 are always excluded. IDs 13/14 depend on one
signed-byte nonzero flag; 11 depends on another flag and a signed [0,6) test;
16–18 additionally depend on an active word and mode 2. IDs 6–10 use mode,
one signed halfword, and three bits selected through a signed-byte per-player
index. Nonzero flags include negative values. Unknown signed32 option IDs
are accepted without reading the indexed attribute table.

The fixture's C-valid domain is player 0–3 and selected attribute index
0–127. Signed-byte/halfword and signed32 gate extremes are covered. This is
a stated bounded test domain, not proof of the actual asset table length or
all reachable game values. Another 384 native/GNU cases exercise negative
signed index addressing using explicitly mapped memory; these are not C
array-validity claims.

There are no external helpers, owned literals or storage, hidden formal
parameters, fake callers, pressure variables, forced volatility, inline
assembly or compiler policy changes in the candidate.

## Compiler evidence and the blocker

The first complete straightforward C body immediately produced the final
18/132 residual. Ordinary isolated O2 and O3 produce the same complete
executable body, at the exact 528-byte extent, with no alignment tail.
The workbench diagnosed register allocation before further work. Its raw
object report also sees unresolved address fields; the final proof resolves
all 22 relocations and independently links the unchanged object with GNU ld
at an explicitly stated .text address. Both paths agree on every byte and
all 18 residual offsets. No source-shape sweep was attempted.

The native call sites are:

- `A:8039D6A4` at +0x920 and +0x95c (read-only extent/hash and call-site proof)
- `A:8039E3BC` at +0x30 (complete 136-byte native caller executed)

E3BC keeps t2 as the 0–18 option index, t3 as the output-count pointer,
t4 as the player, and t5 as the output-array base. It appends accepted IDs
to a player-specific 19-word row, then increments the count. Its own entry
uses private t4 and it changes s0 without saving it. It is native read-only
context, not a reconstructed or claimed function.

The candidate changes t5 in a branch delay slot even for option 0. In the
real caller, the first expected output write is redirected into the selector
area (or an unaligned address for other players). Thus isolated output
agreement is insufficient for safe substitution. Reopening requires genuine
compiler context that restores the witnessed preservation contract, with
all actual context bodies reconstructed and verified. Extra ABI arguments,
artificial keepers or arbitrary register exclusions would not close it.

## Verification

- Independent ELF32 parser: complete 528-byte STT_FUNC, zero trailing
  alignment bytes, zero owned data, all 22 HI/LO relocations, eight addresses.
- Full project relocation and explicit-address GNU whole-object link agree;
  all 18 differing words contain only register-field changes. No masks,
  unresolved sites, unverified data, errors or excess words remain.
- Native types checked as 8/16/32 bits under a 32-bit C ABI.
- 8,962 native/GNU/unchanged-host-C89+UBSan+bounds/oracle cases; project
  relocation bytes are proven equal to the GNU route. Native and linked
  results and complete load traces agree, with no memory writes.
- 384 additional negative-selector native/GNU cases; 18,692 total leaf
  machine executions. All 132 leaf instruction offsets and 70 branch
  outcomes execute; O32 saved registers are preserved.
- Complete E3BC native caller passes 128 flag/player cases against the
  independent option-list oracle; all 34 caller instructions execute.
  Substituting the isolated candidate is rejected in all 128 cases.
- Four compiled wrong-logic controls are rejected, including player-index,
  attribute-mask, negative-track and mode errors. Unknown instruction and
  escaped-code controls fail closed.
- Eight packet tests pass. Scoped scorer/guard/submission/packet tests:
  **60 passed, 664 deselected** with `-k 'not locked'`.

An attempted unfiltered scorer run could not replay locked functions because
this intentionally minimal checkout omits their source files: 647 failed,
77 passed. Those results are not a repository regression or whole-suite pass
claim. No protected file or infrastructure source was changed to bypass it.

## Reproduce

Use the current repository's IDO_DIR, GNU MIPS binutils, GCC, Python and pytest.
From the repository root:

    python3 cloud/work/frontier/dot_runtime_a_availability_boundary_20261006/verify.py --check
    python3 -m pytest -q tests/cloud/test_runtime_a_availability_boundary.py
    python3 -m pytest -q tests/conveyor/test_cloud_score.py tests/conveyor/test_cloud_guard.py tests/conveyor/test_cloud_submissions.py tests/cloud/test_runtime_a_availability_boundary.py -k 'not locked'

`--repo` and `--tools-repo` optionally select protected target and scorer
roots. The verifier restores all modified scorer module state in `finally`.
The receipt is JSON-canonicalized and compares portable source/executable
facts. Raw debug-path-sensitive object hashes and tool identities are kept
separately in local `build/availability_boundary/local_provenance.json`.

No native words, assembly dumps, objects, ROM bytes, credentials or unrelated
private data are published. Source-shadow, image, compression, ROM SHA-1,
hardware/gameplay validation and merging remain with the independent checker.

## Integration-portable replay (2026-10-06)

Scorer, whole-manifest and accepted-context digests are historical provenance,
not live-tree requirements. The verifier normalizes only enumerated provenance
fields on both receipt sides. Packet source and verifier bindings, compiler
identity/actual flags, selected native bodies and addresses, complete emitted
extents, relocations, owned data and behavioral checks remain binding.
