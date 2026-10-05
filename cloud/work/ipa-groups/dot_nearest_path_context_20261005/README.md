# Nearest-path helper with genuine callers

## Result and admission boundary

`func_800E4300`, **0x800E4300–0x800E451C, 540 bytes / 135 words**, is a strict
MATCH under ordinary IDO `-g0 -O3 -mips2 -G 0 -non_shared`, using the unchanged
project compile/scoring recipe. Only this helper is registered and claimed.

The helper C is **not newly discovered**. The archived
`cloud/work/frontier/w1g/groups/func_800E4300/group.c` already contains this
zero-scoring body but depends on two stand-in calls outside its immediate caller.
The contribution here replaces that support with the complete real outer caller
and its steering sibling, corrects source/interface defects, and proves the
helper's complete body and bounded behavior. No new accepted bytes, production
integration or ROM-coverage increase is claimed.

The compiled context is:

- E4B58 (real ABI root) calls E451C at three native sites and E398C once.
- E451C calls E4300 once; its actual donor-bound parameter order is
  `(model, navigation, change_flag)`.
- E398C is the complete steering/avoidance reconstruction.
- `mp_interval_pos` is the real interval-calculation helper, inlined naturally.

No stand-ins, invented keepers, fake locals/padding, added unused declarations,
unsupported volatile, inline assembly, protected source changes, or compiler
recipe changes are used. The only kept function is the actual E4B58 root, which
saves the O32 callee-saved registers in the native body. E56F8, its external
caller, is not reconstructed or claimed in this packet.

The enclosing functions remain **NONMATCH**, including their complete extents:

- E451C: 1,576 compiled bytes versus 1,588 native; 365 differing complete-body positions
- E398C: 2,268 versus 2,396 bytes; 592 differing positions
- E4B58: 2,136 versus 2,268 bytes; 521 differing positions

These are compiler-context reconstructions, not established complete-game
implementations. The helper's exact source admission still requires the
maintainer's full shadow/image/ROM gates and independent review.

## Source provenance and changes

Primary algorithm source is the clean pinned
[historicalsource/rushtherock revision 845329d7](https://github.com/historicalsource/rushtherock/tree/845329d7b36f5a384c5625ed9a0aef584ab46139),
[game/maxpath.c](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/game/maxpath.c).
The source file's SHA-256 is
`3e69bfe5017f12b8628f450029aadd4136792b4d91eab304b14a0be9c58164df`;
Git blob `812d3aa9aca84fe1de90e14f6209691addd041a0`.

- `MP_IntervalPos` begins at line 893. Its actual signature and full scoring
  algorithm ground E451C. The N64 uses four compact paths, float seconds,
  signed-half coordinates and a nearest-point window helper.
- `mp_interval_pos`, lines 1132–1165, supplies the actual inlined interval
  calculation. The N64 calls its accepted vector-normalization helper. The
  archived residual unused reciprocal and unused `nx`/`ny` declarations were
  removed; their effects on frame/register allocation are not retained.
- `avoid_areas`, beginning at line 1602, grounds E398C's purpose. The donor's
  `magnitude(rpos)` and native call preparation both confirm that the archived
  reconstruction must pass its local transformed vector, not the model base.
- E4300 itself is N64-specific; no exact whole-function arcade donor is claimed.
  Its existing native-led implementation searches a wrapped window of
  `2 * max(abs(section length difference), 5) + 1` points, returning the first
  strictly closest point in traversal order.

The earlier merged group
`cloud/work/ipa-groups/func_800E56F8/group.c` supplied the complete E4B58/E398C
reconstructions. Its obsolete `(change_flag, model, navigation)` E451C interface
is replaced with the donor-backed order in both declaration and all calls.
E451C's native count is signed halfword; E398C's native active-car count is signed
byte. Global arrays and the embedded 44-byte Nav are represented as actual
arrays/members rather than arithmetic past standalone scalar objects. Unused
3,700-line generated context and placeholder macros are absent.

The archived E451C 50-mph coefficient had incorrect binary32 rounding:
`73.33333f` is lower than the native value. The ordinary conversion expression
`50.0f * (5280.0f / 3600.0f)` yields the native coefficient. All five context
literal words, **20 bytes at 0x80124440–0x80124454**, are verified independently.
The following 12 zero alignment bytes are excluded. E4300 owns no data/literals;
its external distance sentinel at 0x8012443C is checked as binary32 `1e20`.

## Verification

`verification.json` is source-bound and reproducible with `verify.py`:

- Complete original ELF STT_FUNC extents and every word are checked, including
  short/nonmatching context extents. E4300's eight HI16/LO16 relocations agree.
- GNU MIPS ld independently resolves all four complete bodies. For this link
  fixture only, existing section-relative internal-call relocations are
  converted to named external relocations and resolved to their real native
  addresses. No instructions outside those relocation addends are changed;
  the original compiler object is unmodified. The helper has no internal calls.
- All 20 owned context literal bytes and the helper's external sentinel are
  compared with integrity-checked data. There is no writable owned data/BSS.
- Sixteen IDO O32 size/offset assertions pass and preserve the helper match.
- The protected direct-call graph and E451C call setup verify the private
  interface: position in a2, prior path t2, point t5, section a1, current path t0.
  No full E451C/E398C/E4B58 runtime execution is claimed.
- **1,536 cases / 3,072 native executions** compare protected native code,
  independently GNU-linked code, unchanged C89 host source under UBSan, and a
  separate traversal/binary32 oracle. Whole mapped nonstack memory remains
  unchanged. Exactly four caller argument-home writes are permitted; other
  stack bytes, saved private-ABI registers, floating saved registers, gp/sp/fp/ra
  are checked. s0/s1 are deliberately caller-owned in this private convention.
- Every reachable target instruction executes: **134/135**. Offset 0x208 is an
  unreachable duplicate increment: the earlier likely branch either executes
  its increment delay slot and jumps past it, or follows the wrap path whose
  unconditional branch also jumps past it. Both outcomes of all eleven
  conditional branches execute; the three unconditional branches have only
  their taken outcome.
- Five compiled semantic mutants are rejected: ignoring Y, accepting equal
  distances, omitting section lag, wrapping to zero, and shortening the window.
  Tests additionally reject a truncated ELF symbol, redirected global
  relocations, wrong literal bytes, unknown instructions and unmapped input.

The host includes `group.c` unchanged. Its unrelated context functions are
removed by ordinary function-section linker garbage collection, while IDO's
proof object contains all four genuine bodies. Host-width pointers are not used
as an O32 layout proof; the separate IDO assertions establish those layouts.

The final local packet suite passes **21 tests** and the selected scorer, integrity,
submission, guard and own-data regression suite passes **786 tests**, with no
failures or skips. All **402 static locks** pass. Initial sparse-checkout
missing-input lock errors were resolved by materializing the exact tracked
inputs; no locked source was edited. See `validation.json`.

### Receipt portability

The saved whole target-manifest, data-manifest and symbol-file hashes remain
historical provenance. The portable comparison excludes only those three global
file hashes after freshly validating the current target and data manifests. It
strictly retains every source/toolchain/scorer/object/relocation/behavior result,
plus explicit hashes of all four selected native bodies and the actual E56F8
caller, both consumed data windows, and all relocation-referenced symbol addresses.
An unrelated authenticated manifest annotation may change without invalidating
the selected proof. Unauthenticated target/data edits and authenticated changes
to selected native bodies, symbols or data are covered by rejection controls.
No whole-object, context, source, behavioral or compiler hashes are masked.

### Bounded domain

Fixtures use four accessible nonoverlapping paths with 1–64 points, five
accessible section records, valid wrap destinations, signed-half coordinates,
point/section-difference extremes, and positive path lengths. This includes tie
behavior, backward/forward initial wrap, signed narrowing and nonpositive
narrowed search-window counts. Inputs remain stable during each leaf call.

Zero-length paths, inaccessible/invalid indices, aliased caller stack/input
storage, arithmetic conversions outside the documented IDO/host signed-half
behavior, FCSR exceptions, asynchronous mutation, and gameplay are not proven.
Coordinates give finite binary32 distances; no NaN behavior is claimed. Complete
context execution and universal cross-translation-unit type compatibility are
not established. No full suite, hosted CI, shadow build, image/compression or
ROM SHA-1 result is claimed by this packet.

## Reproduce

From the repository root, with IDO and GNU MIPS tools available:

```
python3 tools/cloud/score.py group cloud/work/ipa-groups/dot_nearest_path_context_20261005 --claims
python3 cloud/work/ipa-groups/dot_nearest_path_context_20261005/verify.py --output /tmp/nearest-path-verification.json
python3 -m pytest -q tests/cloud/test_dot_nearest_path_context.py
```

Publication and merging are separate. This packet is an additive cloud
submission; all production/locked files remain unchanged.
