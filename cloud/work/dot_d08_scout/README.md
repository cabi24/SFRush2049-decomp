# D08: current large-function reconnaissance

**Decision: NO-GO for immediate reconstruction/matching execution.** Of the four
shortlisted functions, `render_large_objects` has the smallest demonstrated real
compiler closure and a complete existing draft. It is nevertheless a broad
structural NONMATCH, not a fresh renderer or a near-match. Continue only after the
specific source/geometry audit below supplies a new hypothesis. No new matching
claims, implementation, accepted bytes, or ROM result are submitted here.

Packet D08 / owner F / branch `dot/round10-f-d08` / base
`d701b59592463d1e33bcee7011ed2a1e6c07b066`. Target and context were reserved read-only;
no production, accepted source, protected target, or group recipe was changed.

## New value over the historical scout

- Reconciles the original `cloud/work/bigfish/*.md` research on PR #9 with the
  later complete round-5 group on current master. The original recommendation to
  write the body and use stand-in callers is obsolete. The current group uses the
  twelve genuine calls and has no stand-ins.
- Freshly replays the existing source with pinned IDO and full resolved words,
  verifies its identity to the accepted group's context, and records the complete
  direct-call graph for all four candidates from manifest-verified native extents.
- Corrects candidate accounting: the root's ELF function is **5,272 bytes / 1,318
  words**, plus **12 bytes of zero alignment padding**, not a 5,284-byte / 1,321-word
  function. The scorer comparison extent includes that padding. Native remains
  **5,652 bytes / 1,413 words**. Missing full-body words cannot be borrowed from
  padding or a neighboring function.
- Workbench diagnosis of the unmodified root identifies **472-byte native versus
  448-byte candidate frame**, with equal 88-byte save areas and a 24-byte non-save
  area difference. This is a structural/frame question before allocation tuning.
- Native direct-call audit corrects the older stunt-camera list: it has **17**
  distinct direct callees / 29 calls, including `func_800D11BC` twice and
  `func_800D2C10` once. Entry ABI alone does not make its contracts established.

## Eligibility and historical overlap

Checked the current game lock and single/group source ownership, PR #1–41 inventory,
round-7/8 exclusion records, and the PR #9 bigfish/snapshot records. This is
continuation reconnaissance, explicitly not an assertion that an unlocked function
is fresh. PR #5/#6 already discuss these large targets; PR #9 preserves their
scouting. The current root source is unchanged from both PR #9 and
`src/blob/groups/render_large_objects/group.c`.

| Candidate | Native extent (exclusive end) | Current decision |
| --- | --- | --- |
| `render_large_objects` | `800F93A0–800FA9B4`, 5,652 bytes | Lowest demonstrated closure risk, but existing complete broad NONMATCH; selected for this packet |
| `entity_spawn_init` | `8008EA10–8008FFB8`, 5,544 bytes | Old switch/table reconstruction blocker, no trustworthy complete baseline newly established |
| `func_8009F058` | `8009F058–800A04C4`, 5,228 bytes | Old IPA-dependent viewport driver, broad renderer closure; parked |
| `stunt_combo_display` | `800D3B28–800D4D84`, 4,700 bytes | Ordinary-entry scripted/path camera, old seed included destructive float-to-pointer fixes; not a validated whole-body starting point |

The older `func_8009F058` report's 129-function closure is historical, not a fresh
minimal-closure proof. This packet reproduces only direct JAL adjacency. Indirect
callers, address-taken uses and transitive ABI dependencies need separate proof.

## Selected target: native purpose and actual context

`render_large_objects` is a per-car rank/distance catch-up or rubber-banding
routine, not a display-list renderer. This interpretation is supported by the
native data flow and existing typed draft: it ranks cars, separates car kinds,
builds a pairwise position-distance matrix, computes two per-car scaling outputs,
and rate-limits stores at car offsets `0x7EC/0x7F0`. It has no rendering/library
callees. A direct arcade-source identity is **not proven**. The local repository
contains no `reference/repos/rushtherock` donor checkout; the historical label is
not ancestry evidence.

The native root saves `ra`, `s0–s8`, and `f20–f30` in a 472-byte frame. Its existing
`void (void)` entry agrees with the native sole direct caller
`render_viewport_init @ 800FA9B4`; this is not proof that no indirect caller exists.
The two direct callees are:

- `func_800F92C8 @ 800F92C8`, 208 bytes, leaf: twelve call sites, and no other direct
  caller in the protected game population. Five float arguments are
  `(a, b, x, outA, outB)` in `f12/f14/f16/f18/f20`; returns `f0`. This is real IPA
  context. It clamps/interpolates between endpoints, with a small-denominator
  path using `D_80124638`. It is **already accepted** in the game lock.
- `func_800DE860 @ 800DE860`, 844 bytes, leaf: one call from this root, native
  16-byte frame saving `s0/f20`. It is an ordinary-entry reset/scale helper, not an
  IPA-argument function. The real group keeps it externally visible. Its old
  allocation plateau includes roughly 500 reported variants; do not reset that
  budget by treating it as a fresh helper.

Compiler route: use the existing three-function whole-program IDO 5.3 group,
`-g0 -O3 -mips2 -G 0 -non_shared`, `uld -kp` keeping the root and `func_800DE860`,
then `as1 -r4300_mul`. Never replace the twelve calls with fabricated callers.
The accepted interpolation body is only a read-only context/control, with no
repeat credit. Any future editing of that source requires a separate research
copy and replay of the already accepted callee.

## Fields, arrays, literals and limits

The existing typed source records car stride `0x808`: order `s16 +0x7C6`, active
`s16 +0x7C8`, kind `s8 +0x7CC`, rank `s16 +0x7E6`, output floats `+0x7EC/+0x7F0`.
The record at `player_array` has stride `0x3B8`: position `float[3] +8`, place
`s8 +0xEE`, distance `float +0x100`, assigned slot `s16 +0x356`, state `s8 +0x359`.
These are native-offset/source-layout interpretations, not recovered original
struct declarations. The source is the actual checked-in context, not new C here.

Native stack arrays and indexing must be audited as live ranges, not filled with
artificial padding: historical notes place distance rows at `sp+188` and ranking
arrays around `sp+376/392/404/416/428`. The current source has five short arrays,
a three-float temporary and a 6×6 float matrix. The new 24-byte non-save-area delta
does **not** prove which declaration or spill is wrong.

Data dependencies include `D_8014A250`, `player_array`, the signed car count
`D_80152744`, state/timer inputs, per-car table `D_80154484`, and separately named
float symbols across `80124638–801247E8`; helper scales use `80124310–8012431C`.
The existing candidate emits external address relocations, so the strict replay
has **zero unresolved/unverified relocations**. That verifies code operands, not
actual literal/table contents or complete runtime semantics. No data recovery,
ROM access or instruction archive is part of this packet.

## Fresh compiler baseline

| Body | Native bytes | Candidate ELF bytes | Padding within comparison extent | Differing words |
| --- | ---: | ---: | ---: | ---: |
| root | 5,652 | 5,272 | 12 | **1,361 / 1,413** |
| reset helper `800DE860` | 844 | 844 | 0 | **207 / 211** |
| accepted interpolation `800F92C8` | 208 | 208 | 0 | **0 / 52** |

All three comparisons have zero unresolved relocations, zero unverified local
section relocations, no relocation errors and no nonzero excess. Equal helper
size does not make its body close. Root gaps remain structural; the native root's
entry frame cannot be explained away by relocation masking.

Workbench was run before any proposed tuning, on local-only authenticated target
and compiled objects. It reports `structure-mismatch`, a 95-instruction true-size
delta, and the 24-byte frame delta above. Its raw/aligned/relocation-blind counts
are **not** used as matching scores: the target object's resolved native words
have no ELF relocation labels, unlike the candidate. The strict scorer's 1,361
is authoritative here. Raw objects, generated assembly and disassembly are not
committed.

## Concrete route and stopping conditions

1. Preserve this complete source and its 52-word accepted helper as the baseline.
   Obtain/prove any donor ancestry rather than assigning a renderer implementation
   from its old name. Confirm rank-domain invariants (0–5, permutation versus ties)
   and literal-table meanings before creating a host semantic oracle.
2. Audit the native rank/distance-array live ranges and every call-spanning local.
   Test one hypothesis: the source's stack-local representation/lifetimes account
   for the **24-byte non-save frame delta**. Require an actual native access/lifetime
   explanation before editing. A declaration-order or padding sweep is not that
   explanation. The historical missing rematerializations alone are insufficient.
3. Only after the complete frame/dataflow audit, make one natural source change
   in a research copy of the genuine group. Replay all three bodies and diagnose
   the root. Keep the 52-word helper exact. Stop after the bounded hypothesis if
   full-body geometry does not improve or semantics are not defensible.
4. A credible full route then needs the entire ranking stage, all twelve distinct
   interpolation branches and both rate-limit tails checked, plus bounded
   differential tests with external constants explicitly treated as fixtures.
   No substring, prefix or opcode-LCS metric establishes completion.

**No two-helper execution assignment is justified yet.** Return to the stocked
medium queue unless step 2 supplies genuinely new evidence. This packet's useful
outcome is the reproducible current rejection and a specific prerequisite, not a
new implementation or a claim that a match is imminent.

## Reproduction and validation

`verification.json` contains metadata only: native extent hashes, direct JAL
adjacency, accepted-lock state, source hash, compiler-tool hashes and strict
numeric replay. `audit.py` first calls the protected scorer's manifest validation.
It never writes target files, dumps native words, or invokes the ROM pipeline.

From repo root, with `IDO_DIR` pointing to the approved IDO 5.3 directory:

```sh
python3 cloud/work/dot_d08_scout/audit.py --replay > /tmp/d08-replayed.json
python3 -m pytest -q tests/conveyor/test_dot_d08_scout.py
```

The three tests cover the JAL decoder (excluding J/JALR), current native identity
and topology, and honest nonclaim/size accounting. They are **not semantic tests**
of catch-up behavior or automatic fresh compiler tests. Full compiler replay is
the separate command above. No sanitizer, gameplay, image, compressed-stream or
ROM hash gate was run for this reconnaissance-only packet.
