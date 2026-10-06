# Genuine runtime-B closure: one combined-context baseline

## Result

The first approved complete-source/private-context diagnostic produces a
**byte-exact 124-byte D328 body** within the genuine eight-function closure.
The other seven bodies are NONMATCH. **Accepted coverage gain: zero.**

This is source-derived private-body evidence, not canonical scorer admission,
an original-TU claim, or a complete image/compression/ROM result. No fake
keeper/exported E114 root, optimizer stub, parameter permutation, pressure
local, source-order sweep, tool modification or ELF mutation was used.

The one combined compilation followed 89 successful target-compiler layout
assertions. Source order, schema, complete function bodies and all narrow
reconciliations were approved first. The sole kept root is the real FCE0.
All seven private definitions are optimizer-visible statics, with the existing
logical parameter order retained. See `compatibility.md` for evidence and limits.

Base: `6b2e9e506fe3d2267a710e41c85af5364ccd00c7`.
Combined source SHA-256:
`fc80a9075c6357b1833bc53b7810f8a4ee2c2f874e1bc8ba03f47fcb10e32d95`.
Recipe: `-g0 -O3 -mips2 -G 0 -non_shared`, canonical `score.compile_group`;
the unchanged route also uses its mandatory as1 `-r4300_mul` workaround.

## Complete body accounting

| Body | Native bytes | Candidate bytes | Native/candidate frame | Full-word differences |
| --- | ---: | ---: | ---: | ---: |
| D200 | 296 | 264 | 24 / 24 | 52 / 74; 8 missing |
| D328 | 124 | 124 | 24 / 24 | **0 / 31; no missing/excess** |
| DA78 | 868 | 896 | 144 / 152 | 215 / 217; 6 excess nonzero |
| D498 | 768 | 804 | 104 / 128 | 182 / 192; 8 excess nonzero |
| E088 | 140 | 120 | 24 / 24 | 33 / 35; 5 missing |
| E114 | 5196 | 5356 | 288 / 272 | 1281 / 1299; 39 excess nonzero |
| F938 | 928 | 964 | 56 / 56 | 215 / 232; 8 excess nonzero |
| FCE0 | 3056 | 3060 | 208 / 208 | 740 / 764; 1 excess nonzero |

These comparisons use each **entire unchanged GNU-linked body**, with no
relocation masks, at its explicit research placement. Internal-call/own-data
addresses therefore differ from native placement for most bodies. They are
not silently advertised as native-placement-normalized matching scores.

All 11,588 emitted function bytes are covered by compiler-recorded extents;
the complete 11,600-byte text section additionally contains exactly 12 zero
alignment bytes. Owned rodata is 320 bytes. All 414 relocations were independently
recomputed and agree with GNU linking; every other text/rodata byte is unchanged.
The allocated .reginfo metadata is inspected and explicitly discarded by the
research executable link, alongside non-executable compiler metadata.

All private PDR records save ra only. FCE0 has a 208-byte preserving frame.
Natural optimizer output retained all eight bodies, despite hiding seven from
the ordinary ELF function-symbol table. The compiler changed DA78/D498 physical
order without any source permutation by this experiment.

## Why D328 is exact, and what remains unadmitted

The unchanged object stores D328's identity in ECOFF `.mdebug`:

- `stStaticProc` at text offset 0x108
- Matching `stEnd` length 124 bytes
- Matching procedure descriptor: address 0x108, frame 24, ra-only save mask
- Linked interval `[0x81000108,0x81000184)`
- All nine relocations use genuine external symbols: object pool, resource
  base, allocator and E398. There are no own-data references or internal-call
  placement dependencies in this body.

Both its full native target and its complete linked body have SHA-256
`5d8fc0875c56105d63e0eaee161b6ec6900d742563b85d047035984fb0fa904a`.
There are no missing/excess words or masked relocation fields. It is not a
matching subsequence taken from a neighboring function.

Canonical `score.py` currently discovers defined functions from ELF STT_FUNC;
only FCE0 is exported there. It cannot directly name this hidden D328 body and
cannot attribute its private `.text` call targets. Its original output remains
in `build.json`; `boundaries.json` explicitly supplements that limitation using
compiler metadata. Neither scorer nor object/symbol table was patched. A
matching-source admission route for this evidence requires separate independent
review and integration ownership, not a fake kept D328/E114 root.

The independent read-only body review is now recorded in
`independent-d328/review.json`: it separately decoded ECOFF symbol/end/aux/PDR
metadata, authenticated the full native target, reapplied every object
relocation and confirmed the exact D328 equality. That review confirms the
research evidence; canonical admission and production acceptance are still open.

The accepted E398 wrapper's void declaration still conflicts with consumed
native v0. The research contract preserves the returned scene index; accepted
production source is unchanged. Original source object/array/TU identity and
the original private formal order remain unproven.

## Genuine whole-closure semantic verification

The final bounded replay executes all real native and candidate private bodies
from FCE0, including nested calls and return/stack traffic. Private functions
are never intercepted by semantic helper hooks.

- 552 paired fixtures
- 856 complete FCE0 invocations per image
- 2,815 / 2,844 native instruction positions executed
- 2,852 / 2,897 candidate instruction positions executed
- Zero state/ordered-external-call mismatches
- Ten fail-closed/negative controls passed

The coordinating worker separately reran the complete 552-case suite and all ten
controls. The fresh proof equals the producer receipt after removing invocation
arguments only; behavior and all source/ELF/boundary bindings are identical.
Those replay receipts are `semantic-replay.json` and
`semantic-controls-replay.json`.

Every writable nonstack byte and ordered external boundary snapshot is compared.
The models preserve exact global byte aliases, live/free pool lists, signed hit
masks, pointer-valued quad lifecycle, F938 owner0..3 intersection, and root
nonvolatile-register restoration. D054's genuine argument-home write is included.
Candidate D200/D328/E088 have complete instruction coverage; all eight members
are exercised. Unexecuted offsets remain explicit, with no omitted source/target
bytes in the boundary or equality comparisons.

External services remain bounded deterministic side-effect models. Trigonometry,
rendering/scene lifetime and whole-game state are not reimplemented. Fixtures
require aligned initialized storage, finite normal-or-zero binary32 arithmetic,
default rounding, valid indices, successful required allocations, and nonzero
D498 distance on impulse paths. Full details are in `semantics/README.md`.

## Files and reproduction

- `shared_schema.proposed.h`, `compatibility.md`: approved evidence map/schema
- `closure.c`, `group.json`: complete single-source diagnostic and exact recipe
- `assembly.json`, `assemble.py`: mechanically recorded, source-preserving
  reconciliation of the six original source packets
- `inventory.json`, `audit.py`: authenticated eight-target identities, source
  snapshots and complete B/main direct-call census; compiler-free
- `build_once.py`, `build.json`: layout-first canonical one-shot build and
  full object/linked inventory; no private kept roots
- `inspect_baseline.py`, `boundaries.json`: read-only ECOFF/PDR bounds, complete
  body scores and independent all-relocation/unchanged-byte check
- `toolchain-provenance.json`: actual selected IDO and GNU linker binary
  fingerprints; no integration-sensitive tool/source/manifest hashes
- `independent-d328/`: independent compiler-boundary/relocation/body review
- `semantics/`: source/ELF-bound genuine-root replay and fail-closed controls

The original compiler work directory is temporary and is removed after the
required independent review/replay. No compiled objects, native bytes, raw
assembly or ROM assets are publication artifacts.
That default cleanup is complete and recorded in `cleanup.json`; source,
metadata and final verification reports remain.

For an authorized identical-source reproduction in a checkout with the pinned
IDO and MIPS GNU linker, use a fresh unique TMPDIR and default cleanup. This is
an exact replay, not permission for another source/recipe experiment:

    TMPDIR=$(mktemp -d)
    export TMPDIR
    trap 'rm -rf "$TMPDIR"' EXIT
    python3 build_once.py --reference-root /path/to/repository --work-dir "$TMPDIR"
    python3 inspect_baseline.py --reference-root /path/to/repository
    python3 semantics/verify.py --reference-root /path/to/repository \
      --linked-elf "$TMPDIR/closure.elf" --boundaries boundaries.json \
      --source closure.c --output semantics/closure-verification.json
    python3 semantics/controls.py --reference-root /path/to/repository \
      --output semantics/controls.json

Read-only inspection/replay can instead use the original ELF while its temporary
directory exists. `semantics/README.md` supplies those commands. No full source
clone is needed. No current portability-matrix file or protected repository
file was edited. Publication and integration gates were not run here.

## Stopping condition

The approved single-context baseline is finished once independent D328 and
semantic replays are recorded. No further source tuning is authorized by this
result. Preserve the private-body equality and the seven structural NONMATCH
results as evidence. Source matching, canonical private-body admission,
whole-image, compression and ROM gates remain with the coordinator and
independent checker.
