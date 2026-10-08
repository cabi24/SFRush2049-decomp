# Independent FA9B4 source and semantic review

## Verdict

No source-fidelity defect found in the bounded ordinary-boundary reconstruction of `render_viewport_init` at `0x800FA9B4`.

This is **semantic research, not a matching result**. The owner's single existing canonical O3 object independently re-compares as NONMATCH: 912 bytes versus 924 native bytes, 227 of 231 positional words different; no unresolved relocations, unverified references, masks, comparison errors, or excess words. Native frame size is 192; the existing candidate frame is 128. No additional target compilation or instruction/register shaping was performed.

Frozen source SHA-256: `e843f3500c6a952dd067949a7d5f894febfb8d635d77baa480778360dc95f4ee`.

Existing object SHA-256: `03cd24df3ce24448c2f8dc2e91e2f0553565b8dff60113b0da27c72ec361e9b5`.

Historical context: `6b2e9e506fe3d2267a710e41c85af5364ccd00c7`. Native words are authenticated against this historical commit and the current target, without pinning changing production files.

## Independent checks

- 2,548 three-way fixtures: 548 focused threshold/control-flow/callback cases plus 2,000 separately seeded randomized cases.
- 2,145 schedules mutate state at multiple external boundaries. 22,729 mutations were actually applied.
- Target native words, the literal frozen C source compiled for the host, and the owner's existing O3 object agree on ordered call arguments and complete modeled nonstack state at every external boundary and at return. Host data are normalized to target byte order and 32-bit object-pointer layout.
- All 20 external call identities, 229 reachable native words, and 54 branch outcomes are exercised. The two untouched setup words at offsets 0x78 and 0x224 have no reachable predecessor; no words are masked from strict comparison.
- Every native external boundary poisons caller-saved integer and floating registers. The independent harness additionally overwrites all four outgoing argument homes, and all ten argument homes for positional sound. Stack/nonvolatile preservation remains checked by the underlying interpreter.
- The complete 2,548-case suite was repeated with the literal source host compiled under UBSan with nonrecovering errors; it passed.
- Sixteen source mutants are rejected by changed boundary behavior/state or layout assertions. These include unsigned event sign, excluding auxiliary index zero, stale versus overwritten captured slot, cached loop count, inclusive statistic threshold, saturating rather than wrapping counter, missing third coordinate copy, wrong state restore, positive-only sound enable, reversed object sign, wrong flag mask, missing mode-2 slot gate, swapped B-root calls, wrong float scale, and wrong model stride.
- A separate no-compile static check verifies every native/existing-object SP adjustment and paired floating load/store offset is 8-byte aligned. Both deliberately misaligned doubleword-store controls are rejected. This does not claim the producer interpreter itself checks doubleword alignment dynamically.

## Source/ABI findings

- Player stride 0x3B8; signed event at +ED, signed auxiliary selector +35C, state bytes +35D/+35E. Model stride 0x808; float position +22C, signed short slot +7C6, signed model mode +7CC. Statistic and auxiliary records have native 76/152-byte strides, +40 unsigned half counter, and float triples at +24/+84.
- The table chain is exactly table[slot] -> pointer at +0 -> pointer at +28 -> pointer at +0 -> signed byte +5. Only the table element is null-checked. The additional +0 dereference must not be removed.
- The initial signed-short slot is captured per iteration. Sound cleanup and later callbacks can change model slot/mode; sound dispatch reloads those fields while C3578, B0180 and BEAA0 retain the captured short. State snapshots catch ordering before each helper rather than checking only final values.
- The live signed-byte loop bound is reloaded after callbacks. It is not the nearby signed-half human-player count. The statistic condition has no added lower-bound test and increments modulo 65536.
- Entry mode dispatch, B:FCE0 then B:0F60 ordering, effect snapshot/state restoration, zero-event behavior, sound gating, status 0/6 logic, mode-2 captured-slot-zero gate, and final helper order all match the native root.
- AF06C receives the address of the short local, word 1, float 1.0 in a2 under O32, and word 0. The prior authenticated service audit establishes that this pointer is read/copied and does not escape. It is not a float-vector argument for this call.
- The two sound calls have the observed native carriers: `(45, current_slot, 1, 1)` and `(model_position, D_801141B0, 400.0f, 0.0f, 1.0f, 0.0f, 45, current_slot, 0, 128)`. The latter's second/fourth carriers are passed but not consumed by the inspected callee. Treating the second as an opaque pointer does not assert an unsupported original type.
- Signed-short declarations for F8E90/B0180/BEAA0 agree with native entry narrowing/homing. C3578 consumes a word index. The inspected sound roots save/restore their conventional nonvolatile state and return an unused handle. The prior identity/contract audit supports ordinary B:FCE0/B:0F60/AF06C boundaries; the producer's additional explicit-write/save-pair audit is corroboration, not transitive execution proof.

## Scope and limitations

The reviewer independently inspected source/native control flow, created fixtures, callback schedules, memory inspection, source mutants, argument-home poison, and the static alignment check. It deliberately reuses the producer's fail-closed MIPS interpreter. It is not a second independent CPU emulator.

The modeled callbacks can change mode, signed loop and human counts, option flags, model slots/modes, event/effect selectors and state bytes, statistic counters, per-slot flags, status bytes, sound/final gates, nullable object references/header signs, and float payloads at multiple calls. Their actual bodies are not executed.

Valid initialized arrays and indices 0..3, valid nonnull secondary object links, disjoint global/stack storage, finite normal-or-zero float payloads, an aligned conventional stack, and B-image residency through the audited lifecycle remain assumptions. Negative slot safety, corrupted pointers, concurrent mutation, whole-game behavior, original C identity, full-ROM identity, original translation-unit identity, and strict matching are not proved.

No production edits, external writes, publication, protected-tool changes, target recompilation, CI watching, or matching claims occurred. No native byte dumps, assembly dumps, objects or ROM assets belong in a deliverable.

## Artifacts and replay

- `review.json`: independent fixture receipt. Raw replay logs remain outside the source packet.
- `mutations.json`: all sixteen negative controls and their first rejecting observation.
- `static-review.json`: existing-object comparison and stack/doubleword alignment evidence.
- `frozen_candidate.c`: exact frozen source; `host_review.c`: literal-source inclusion plus observation hook.
- `review.py`, `mutations.py`, `static_review.py`, `build_review_host.py`: source-only checks.

The scripts support `RUSH_REFERENCE_ROOT`, `RUSH_TOOL_ROOT`, `RUSH_PACKET_ROOT`,
and `RUSH_VIEWPORT_OBJECT`. By default the tool/history root is this repository
and the packet is the parent directory. Set `RUSH_VIEWPORT_OBJECT` explicitly to
the already-built canonical O3 object; review scripts never compile the target.
`REVIEW_HOST_LIBRARY` selects a sanitizer-instrumented host library.

Set `TMPDIR` to writable workspace storage. `RUSH_VIEWPORT_REVIEW_WORK` defaults
to `viewport-source-review` under that directory and contains generated binaries
and host-only mutants. `RUSH_REVIEW_OUTPUT` defaults to the same work directory.
The host-builder checks the archived frozen source and harness instead of
rewriting either C file. Run `build_review_host.py`, `review.py`, `mutations.py`
and `static_review.py` in that order from any working directory.

The additive batch's bindings pin these actual Python verifiers and C harnesses,
without pinning test files or mutable production context. The original source,
object comparison, normal/UBSan fixtures, mutants and alignment receipts remain
semantically unchanged. Scratch output, native dumps, binaries and raw logs are
not packet deliverables.
