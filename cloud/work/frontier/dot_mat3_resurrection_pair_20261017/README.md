# Resurrection camera pair: natural-source research

Base: 58425072b49e95f0fbe5c4c239988fb4885f7c5c, game population, stock IDO5.3
`-g0 -O3 -mips2 -G 0 -non_shared` and standard `as1 -r4300_mul`.

This is **NONMATCH research with empty claims**, not an installable new-coverage
packet. The primary source contains no unused-local frame filler.

- camera_follow_path / FindSurfaceUvsFromResPoint, 0x800C46D0: strict MATCH,
  1,484 bytes / 371 words, frame104. This is a private-ABI helper proved only
  inside genuine caller context; its unmatched caller prevents a standalone
  new-coverage claim.
- func_800C4F68 / SetResurrectionData, 0x800C4F68: 17/354 differing words,
  exact 1,416-byte extent, frame80 versus native104. The residual is stack-related.
- All 12 already-locked source members, including the real rand and Random,
  remain strict MATCH. Every compiled context function is also scored. Existing
  nonmatching Resurrect/FindHumanResPt/renderer context is retained transparently,
  including its pre-existing differing words and parent own-data errors.

Replay:

    python3 cloud/work/frontier/dot_mat3_resurrection_pair_20261017/verify.py [current-checkout]

The replay checks source/target-row/verify inputs, compiles in fresh temporary
folders, and verifies every inherited locked member and the surface helper.
It also reproduces the separately labeled inherited-six-unused diagnostic.
The replay passed against current-master tools and inputs. No source promotion,
whole-program/ROM verification, publication, or new coverage is claimed here.

## Accepted donor and source choices

The accepted MAT3 donor source SHA256 is
321ed3676364877f2d43038cf96bf663930a10caa035af5a73d877f9066a6d00.
Saved donor proofs report FindSurfaceUvsFromResPoint 1,764 B at0x800356B8 and
SetResurrectionData 1,328 B at0x80036344, exact at their original layout with
33/43 verified relocations and no proof errors. These are reconstructed donors,
not recovered original N64 spelling.

The full surface-helper body transfers successfully when preserving native
6-byte resurrection points, 8-byte drone points, 16-byte alternate descriptors,
and the actual 12-byte checkpoint header / 80-byte checkpoint entries. Four
float locals (sine, cosine, yaw, angle), three vectors (snapPos, pt, pos), then
the surface pointer replace the old draft's extra pointer/scalar intermediates.
Sine/cosine are reused in the fallback matrix. Its actual kept fasin wrapper is
camera_update_c, already part of the genuine particle-knot group; invoking the
real wrapper instead of flattening func_8009C3F8's flag-zero call fixes both the
frame and FP value lifetimes. The GC-specific square-root adapter is not copied:
N64 uses its native sqrtf intrinsic.

The donor's formal order res/path/idx is retained with all genuine call sites
updated consistently, while the compiler produces the measured private N64 ABI.
The RoadPosition result is correctly pointer-typed. Direct indexed point and
checkpoint expressions remove the four operand-order mismatches of the old
pointer-local draft without byte-offset tricks. Native matrix fallback ordering
stores -sine immediately after the first 1.0f store, then the zero/remaining
basis fields; the separated sine/cosine reloads retain the observed schedule.
That native order closes the last five words. All points, modes, symbols, and
helper identities remain native rather than substituting PPC layouts or IDs.

For SetResurrectionData, the donor's zero-before-negation order and cross-product
operand order close the eleven body differences. The real rand definition
func_8008B2B4 replaces the research copy's static rand helper: that copy left an
unnamed `jr ra` stub after C4F68, which the strict scorer correctly rejected as
an extra word. Keeping and scoring the actual native rand removes the artifact;
rand and Random both remain exact.

## Explicit diagnostic boundary

The w27d baseline inherited these six unused function-scope locals in C4F68:
`s32 xv, yv, zv, last_cp_index; f32 *dp; void *chkpnt;`.
Reintroducing them into the primary natural source makes both functions strict
MATCH. They account for exactly six four-byte slots: frame80 becomes104, with
body length unchanged. That result is preserved only as a diagnostic in
verify.py, not as a claim that these were original source declarations.

The accepted SetResurrectionData donor has only `int p` and an unused third
abort parameter. A faithful one-local adaptation with the actual kept
func_800B930C GetNextCheckpoint still has frame80 and additional structural
differences; dropping its explicit empty case1 is a separate native control-flow
question. Thus the donor does not establish the missing 24 bytes. The native
call inventory still calls CopyUV, two NormalVector sites, SetDroneResPathIndex,
and SyncMaxpathToLastCheckpoint; those real calls cannot be replaced by invented
inline wrappers just to enlarge the frame. The cross product is a real source
macro in current vecmath.h, not evidence for an inlined three-pointer helper.

The observation-only uopt trace of the four-float intermediate was identical to
its complete stock object, SHA256
560158924701d87083b25376a10c420848041c9d089ec4bea094d6d23ca96f00.
No forced colors were used. Do not reopen the frame search without genuine
native local/helper evidence. The exact surface reconstruction and old-member
proofs are reusable when the caller is resolved.

No raw assembly, ROM data, objects, credentials, or private traces are included.

## Protocol-v2 recovery verification (2026-10-17)

The replay now checks the explicit empty-claim declaration, all twelve preserved
locked members, all seventeen target rows, the unchanged full group sources,
and the replay script digest. Every variant compiles in a fresh temporary
directory and scores every member and every context function. Positive variants
also assert emitted function extent and frame size. No compiler, scorer, whole
assembly, lock, or build-system digest is pinned.

Five bounded negative controls all reject the surface helper while preserving
all twelve locked functions:

- Wrong resurrection-point stride (8 rather than 6 bytes): 359/371 words differ
- Wrong checkpoint-record stride (84 rather than 80 bytes): 197/371, with 3 extra words
- Flattened kept fasin wrapper: 57/371, frame96 rather than104
- Sequential fallback matrix stores: 5/371
- Wrong fasin input axis: 11/371

The wrong-stride controls also expose unverified own-data references because
the body is shifted; these are rejection evidence, never masked into success.
The six-unused-local case remains a separately named diagnostic. Neither it
nor the exact helper is promoted to new coverage. Source, replay, and target-row
mutation guards were also exercised without changing protected repository inputs.

A clean staging replay using current-master tools and target inputs passes the
bounded pytest with REQUIRE_TOOLCHAIN=1 (1 passed, no skips).
