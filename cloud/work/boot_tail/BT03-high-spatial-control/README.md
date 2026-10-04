# BT03-high spatial control

Exclusive central activation `ec7a6718` assigns `8001DC08/472`,
`8001D1F4/508` and `80019C8C/580`, totaling 1,560 bytes. This packet uses
`dot/boot-tail-bt03-high-spatial-control` from master
`301d9e7552ad4fd7f54a38796db84671e1000d35` in the reused isolated worktree.
The previous runtime-followon packet is frozen separately.

**One strict O2 match: 19C8C, 145 words / 580 bytes.** The independently
relocated body is wholly equal, with one correctly named function at offset zero,
exact 580-byte ELF function size, no masks, unresolved/unverified references,
relocation errors or nonzero excess. Two complete nonmatches total 980 bytes:
D1F4 is 46/127 words and DC08 is 3/118. Their ELF sizes are exactly 508 and 472,
but neither receives match credit. Section alignment does not count as body size.

## Genuine ABI and retained semantics

### 19C8C: active-voice portamento chain

The three real byte inputs are note, channel and controller set. The complete
19ED0 caller narrows the note to seven bits; the source preserves the actual
byte ABI. A packed 416-byte voice has child/parent words at 16/20, flags at 36,
channel/set bytes at 74/75, original/current note halfwords at 78/80, identifier
at 96, portamento time/pitch words at 140/148 and signed detune/saved-note bytes
at 192/193. Native widths and every used offset are separately IDO-asserted.

The loop uses the live byte voice count. It tests identifier/channel/set and the
actual low flag bits, calls the genuine word-index active query, then reads the
current fields after that call. Pitch uses the old unsigned halfword shifted
as unsigned32, plus signed-byte times 65536 divided by 100. The signed product
fits int; the final addition wraps as unsigned32. No negative signed shift or
signed-overflow reconstruction is used. Note truncation remains explicit through
the real halfword/byte fields.

After detaching a voice, the first successful chain allocation establishes the
returned key. Child/parent words, the previous identifier and the last selected
pointer are live at their native points. A failed allocation can leave the key
sentinel and permit a later first-member attempt. The final last-started helper
runs before the channel/set/note reload for the three-byte publication helper.
All six complete native bodies and relevant helper ABIs were inspected; helpers
are declarations only. No newer family-source extra allocator call or arguments
were imported.

Supported execution requires allocated registered voice storage, a live count
within that storage, and each chain identifier's low byte indexing a valid
voice. Helper mutations must preserve valid objects and eventual bounded traversal.
The full-byte arithmetic fixtures establish arithmetic behavior, not malformed
engine-state or downstream playback safety.

### D1F4: emitter setup and synchronous start, complete nonmatch

All ten inputs are genuine: emitter pointer, two whole vector pointers, range
float, curve float, word flags, halfword identifier, word group/context and two
byte levels. Incoming stack slots are +16 float, +20 word, +24 halfword, +28 word,
+32 byte and +36 byte. The previously reviewed D3F0/D460 callers corroborate them.
The 68-byte native emitter has next/previous at 0/4, flags at 8, vectors at 12/24,
float fields at 36/40/44/48, handle/context at 52/56, halfword ID/counter at 60/62
and fade at 64.

The no-argument synchronization helper runs before input reloads. Null input
selects the actual external scratch emitter. The real six-input calculator
receives emitter plus five float output pointers; output order and the subsequent
emitter-plus-five-float call were checked against complete native callees.
Zero first output returns immediately without reading the other potentially
unwritten outputs. An allocation failure skips parameter application. Success
returns the live handle after the final synchronization call. A supplied emitter
is linked at the current head, receives sentinel handle/counter/flags, and returns
the sentinel. Unwritten fields are preserved.

Vectors must be valid whole objects. Self-copy and sequential cross-alias between
the two vector members are tested; arbitrary partial byte overlap is not claimed.
For the null path, a nonzero first calculator output requires all remaining
outputs initialized under the actual calculator contract. Valid downstream
floating domains and ordinary FP behavior remain required. No default outputs,
extra frame locals or fake formals were introduced to close the frame mismatch.

### DC08: queued emitter starts, complete nonmatch

There are no incoming arguments. Native groups are 12 bytes and entry nodes are
28 bytes, with next pointer, five real float parameters and emitter pointer.
The unsigned byte group count and both list pointers remain live across helpers.
The two externally stored thresholds at `8002D904` and `8002D908` are loaded once
for a nonempty invocation and retained across all calls. The final source makes
that snapshot explicit. Their original float values are unknown and are never
substituted with the public family's literals or synthetic fixture values.
`abi_proof.json` binds each native and compiled load to its exact external address.

The lower comparison skips without increment; the upper comparison increments
the actual halfword counter, including wrap, and skips below 20. When an active comparison satisfies neither
less-than-or-equal predicate, the counter is cleared. Failure only updates the documented flag bits when bit2
is absent. Success clears fade, sets the start flag, forwards all five entry
floats, then clears the live post-call flag and advances the live active head.
The next pending link is read after callbacks. Valid bounded group/entry/emitter
objects and normal FP comparison semantics are assumed. No protection against
malformed cyclic lists, concurrent mutation or exceptional FP settings is claimed.

## Bounded research and provenance

Every initial O2 body was diagnosed before refinement; improved and five-word
checkpoints are separately recorded. O1 follows O2 for every form. There are
15 total source forms / 30 archived compiler rows: five for 19C8C, four for D1F4
and six for DC08, well below the per-hypothesis bound.

- 19C8C: 43 -> 8 words by expressing pitch before note mutation and independent
  chain stores; then 5 by recording the original note before changing it. A
  lexical key-local scope control stayed at 5. A pinned family-source adaptation
  of the genuine cursor/result/chain locals and direct field expressions closed
  all words. There was no arbitrary local-order or padding search.
- D1F4: 120 -> 55 -> 46 using natural fallback expression/explicit arms. The pinned
  family's real output-local organization stayed at 46. The native 72-byte frame
  versus 64-byte candidate frame and aggregate-copy temporary differences remain;
  no unused local or artificial source construct was added.
- DC08: 112 -> 106 -> 5 by explicitly retaining the threshold snapshots and using
  a guarded counted loop. External const qualification and a two-real-local
  ordering control made no improvement. Lower-before-upper snapshot evaluation
  reached 3; the remaining constant-load scheduling sites are archived, with no
  claim that a nearly identical function is a match. Earlier controls that
  reread thresholds are numerical research and are not the retained live-mutation
  semantics.

Public CC0 sources are pinned in `references.json`: revision
`78d2e16e4905fc675952162d331c24d5198b2687` of
[AxioDL/musyx](https://github.com/AxioDL/musyx/tree/78d2e16e4905fc675952162d331c24d5198b2687),
`synth.c` (`do_voice_portamento`) and `snd3d.c` (`AddEmitter`). The license was
checked. Names and real expressions are adapted to independently proved N64
layouts/ABIs. Newer additional arguments, fields, flag values and helper behavior
are excluded unless present in this native body. Provenance is not an original
source identity claim and does not replace strict verification.

O1 HI16 errors on the accepted portamento source and retained emitter residual
are explicitly reproduced and rejected. No control row is hidden or credited
as an additional function.

## Verification

Three actual-source strict-C89 ASan/UBSan/float-cast-overflow groups pass
**134,124 calls**: 65,536 note/detune combinations plus 640 live-chain/filter
scenarios; 65,536 emitter gain/minimum combinations plus 12 exits/alias cases;
and 2,400 synthetic-threshold/counter/failure/live-list scenarios. Callback doubles
exercise declared external contracts and reload ordering, not full callee
implementations. Original threshold values and runtime image bytes are not used.
Only LeakSanitizer is disabled under ptrace. Host LP64 tests establish logical
behavior; separate IDO assertions establish native pointer widths/layouts.

Run `verify.py`, `verify_controls.py`, `abi_proof.py` and unittest discovery for
`test_*.py`. The canonical one-submission gate, protected preflight, 161 locks and
whitespace checks pass before handoff. Paired actual-source/ABI review precedes
central integration and exact-head CI. Central alone edits the shared ledger;
the independent checker alone merges. No target/scorer/lock/layout/symbol/runtime/
farm changes, gated callee source work, ROM bytes, raw native dumps, objects,
credentials or unrelated private information are included.

## Independent review

A fresh reviewer approved exact source head `86d3898a912884f32c71e6ed9a95e3cc65fe114b` / tree `2862c2bb6bad0805c454638501756028eb288672`. All six final rows, 30 archived controls, layout/address proofs and 134,124 sanitizer calls reproduce. The reviewer inspected all three retained bodies and 15 complete native callers/helpers, and independently verified the pinned CC0 license and source blobs. `independent_review.json` binds the unchanged source hashes.
