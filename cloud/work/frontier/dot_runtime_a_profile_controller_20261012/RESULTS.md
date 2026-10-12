# Image-A profile controller: 31-word research residual

Fresh master baseline: `46780d3e8`.
Source-first deliverable: `groups/runtime_a_profile_menu_20261006/`.

**`func_80396FA0` remains NONMATCH: 31/718 differing words over the complete
2,872-byte interval `[0x80396FA0,0x80397AD8)`.** No extra words, unresolved
symbols, unverified sites, or owned-data errors. Its complete 56-byte native
switch table verifies. Zero new strict, accepted-image, or ROM bytes.

Eight prior exact bodies total **7,152 preserved bytes**, excluded from new
credit: 95E24 104; 96EE8 92; 96F44 92; A7B58 3,196; A87D4 920; A8B74 1,300;
94564 720; 94B00 728. A7B58 is draft #366 and 94B00 is draft #359, not newly
accepted coverage. Other bodies are already accepted. The ordinary group has
all eight as members and claims. The same profile-group identity and all its
prior members remain. A9088, A9EEC, 96FA0, and genuine 9842C remain context.

This is a research packet, **not an installation request**. The two 92-byte
helpers remain separately owned; 94564 belongs to the availability group, and
94B00 has its existing draft owner. The checker must reconcile ownership before
any future combined installation. Do not automatically unlock, migrate, merge,
or count these duplicated preservation bodies.

## Native-led reconstruction

The earlier #366 controller, when kept by itself, was 717/718 with 39 extra
words. Its native entry instead consumes `player` in s0 under its sole real
parent, 9842C+0x1c0. The complete real parent is reused from the current menu
cleanup source; genuine 94564 and 94B00 restore their actual private clobbers.
Neither synthetic callers nor hand-assigned argument registers are present.
The parent remains independently NONMATCH: it is useful ABI context, not a
claim that the entire menu root is reconstructed exactly.

The remaining structural defects were causal and inspectable:

1. Native loads list head/tail from the shared root at 8012E6D8 +8/+12. A typed
   two-prefix-word root view recovers that reuse. Original type names are unknown.
2. Native owns a full fourteen-entry action switch. Cases 1 through 10 perform
   no action; explicit inactive case labels recover its jump table and keep
   integer constants from being given incorrect long-lived registers.
3. The old overlay declaration narrowed the active flag of 800A3424 to s8.
   The accepted game source `src/blob/func_800A3424.c` declares and implements
   s32. Correcting the declaration removes two spurious narrowing sequences.
4. A consumed `Profile **owner` local for the two selected-profile assignments
   recovers the native v0 value and all active expression-home offsets.
5. Reused 94B00 is expressed with this group's pointer-to-pointer handle view.
   Its equality operand order is adjusted for that view; all 182 words remain
   exact. This does not change either loaded value or the comparison result.

In the final genuine context, replayed controls preserve all eight exact bodies:
old controller 631/718 (2,844 emitted bytes); sparse switch 631/718 (2,844 bytes);
without the consumed owner local 284/718 (full extent); stale s8 prototype
489/718 with four extra nonzero words (2,892 bytes).

## Exact stopping point

The final stock function has 56-byte frame and native-active homes at sp+28,
+32, +36, +40, and +44. Native frame is 88 bytes, with its player argument home
at sp+88 rather than +56. Twenty-six differences are the two frame adjustments
and corresponding argument-home accesses. The five other differing words are
at +0x274 and +0x474..+0x480: two default-profile address paths rematerialize the
four-byte player stride instead of loading its saved sp+40 value.

The full native sp-access census is receipt-bound. There are no address-taken
stack objects in this controller and no buffer whose size is being guessed.
Stock UGEN reports M28/T16/P4 and frame56. A diagnostic-only eight-scalar control
reports M60/T16/P4 and frame88, preserves all eight prior bodies, and leaves
exactly the five index words. **None of those unused scalars is retained or
counted as a match.** Original inactive locals and their source meaning remain
unknown; merely filling the gap would not resolve the remaining CSE difference.

A diagnostic, stock-identical UOPT run separates initial source/merged local
area8 from eight four-byte spill-home allocations ending at area40. UGEN's
final M28 includes home reuse and compaction. PRE shares the ordinary index
multiply as an expression bit, but the default-profile address remains an IXA
operation with an implicit scale, so it recomputes that scale. This narrows the
next useful hypothesis to native address-expression representation, not coloring
all registers or sweeping unrelated frame declarations. Equivalent pointer/
byte-index spellings did not close it; byte-offset forms perturb other constant
sharing. These diagnostic records are not acceptance/compiler oracles.

## Verification

Canonical ordinary group score, without `--claims` or a waiver:

```
Members:
func_80395E24: MATCH
func_803A87D4: MATCH
func_803A8B74: MATCH
func_80396EE8: MATCH
func_80396F44: MATCH
func_803A7B58: MATCH
  own .rodata verified at 0x803B9848..0x803B986C
func_80394564: MATCH
func_80394B00: MATCH
Context:
func_80396FA0: 31/718 words differ
```

Other context diagnostics remain explicit in the receipt: A9088 15/917;
A9EEC 281/315 with its prior unmatched table; 9842C 449/451 with its unmatched
table. They receive no GNU-match or installation claim.

- Independent original-REL GNU reconstruction resolves **469 text relocations
  and 20 table relocations**. It checks every unmasked word, all nine full
  checked extents, native placements, both complete owned regions (92 bytes),
  no residual relocations/undefined symbols, and no unexpected allocated data.
- The complete ELF census includes all twelve emitted functions, their actual
  sizes, ordering, gaps and final zero suffix. No neighbor supplies missing
  tail words. The 31 research differences are verified again by GNU.
- Both owned regions independently reject missing and corrupted native data.
- The current `packet_spec -> group_bodies -> mismatch` route extracts exactly
  the eight native-equal prior slices, excludes every context, and does not
  change any production source or lock. This demonstrates extraction only;
  it does not override the ownership reconciliation gate above.
- **54,880 actual-C state/event comparisons** cover every valid player/count,
  all fourteen initial selections, seven action masks, seven navigation masks,
  owner collisions, controller choices, eligibility states and both save-check
  outcomes. The complete controller-visible mutable state and ordered helper
  events agree with the earlier reconstructed controller in the same genuine
  context. Real 94564/94B00 bodies run in both versions.
- The same host matrix passes ASan/UBSan. Only LeakSanitizer is disabled. The
  native-width memset parameter uses an explicit host-only adapter. Other
  external helpers are deterministic synchronous doubles. This is finite
  baseline equivalence, not retail helper or gameplay execution.
- Normal and optimized-Python deep replays pass. Stock diagnostic-on/off full
  compiler objects are identical. Focused regression passes in 1.07 seconds;
  missing IDO cleanly skips in 0.52 seconds after checking input bindings.
  Protected-path and whitespace checks pass.

Protocol-v2 receipts bind owned source/recipe/proof/test/harness, current-tree
baseline/helper sources, complete authenticated target words and caller census,
all referenced addresses, native owned data and eight stock compiler identities.
They do not pin shared tools, lock files, Makefiles or assembly-text formatting.
No replay requires an unmerged commit or a private ROM; no static ROM TU is rebuilt.

## Replay and limits

```
P=cloud/work/frontier/dot_runtime_a_profile_controller_20261012
python3 "$P/verify.py" --deep
python3 -O "$P/verify.py" --deep
python3 "$P/verify.py" --sanitize
python3 "$P/verify.py" --score
REQUIRE_TOOLCHAIN=1 python3 -m pytest tests/cloud/test_runtime_a_profile_controller.py -q -o addopts=''
```

Valid player indices, backed acyclic consistent profile lists, at least one
eligible option, stable storage and synchronous helper contracts are assumed.
The inherited renderers retain their earlier buffer assumptions. Arbitrary
aliases, concurrent mutation, malformed profile storage, every external helper,
original source identity, whole gameplay, image composition/recompression and
full-ROM identity remain unproved. No ROM bytes, native/raw assembly dumps,
compiled objects, secrets, protected edits or unrelated data are deliverables.

Independent review and final-head hosted CI are reported at publication. Keep
the PR draft; the independent checker owns acceptance, installation and merging.
