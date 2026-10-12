# Image-B radius effect: 736 newly strict-matched candidate bytes

Source-first deliverable: `group.c` and `group.json`. Sole member, keep and claim:
**B:func_8038D798**, complete native `[0x8038D798,0x8038DA78)`, **736 bytes**,
all 184 words, plus eight owned literal bytes. No prior exact body is defined
or re-counted. **Zero new accepted-image or ROM bytes.**

Base master: `4f59607f7b15e252cfcbab67b9eccb6d9e6afba5`. The 14 accepted image-B
functions and pending #353 (91650) were excluded from yield. Initial PR #356 was
conservatively published as empty-claims research. The contract evidence below
was subsequently found, independently checked and accepted for source-contract
reuse; this revised packet supersedes that initial research state.

## Matching source and inherited shared-global contract

For each enabled, unblocked, non-disabled player within the strict squared
radius, the function applies the squared normalized falloff times a signed
integer magnitude. Kind 5 first transforms endpoint-origin into target-local
space, calls the genuine native angle helper, and applies the binary32 0.35
multiplier when the absolute angle is strictly below 1.35.

One genuine distance/weight local prevents the weight computation being sunk
separately into three effect-call branches. Moving the two existing pointer
declarations around the three real float vectors recovers native homes:
direction sp+152, transformed sp+140, delta sp+124. All three remain float[3].
Original names, declaration order and whole-function arcade ancestry are unknown.
No extra/padding local, capacity change, false formal, artificial helper/caller,
inline assembly or altered acceptance compiler is used.

The count view is exactly `extern volatile s16 D_801543CA;`, inherited from
**five current image-gate-accepted production sources**: EC914, 108DA8, 108F40,
109A60 and world_physics_tick. This is not an assertion of original source
qualification, a new device/asynchronous-writer theory, or admission based only
on this function's exact code. `count_contract.py` fails closed on:

- Every selected production source SHA-256 versus its current image-gate entry,
  exact declaration and signed-short typedef. The receipt pins those selected
  source identities, not the mutable whole lock file.
- Authenticated game and image-B symbol tables binding the same shared count
  to **0x801543CA**, plus D798's two exact signed-lh sites at +0x4C/+0x270 and
  both full address formations.
- Independent native EB90C evidence: first read 0x800EB9B8; BLTZL backedge
  0x800EB9F8 -> 0x800EB9BC with repeated read at 0x800EB9FC. The continuing path
  has no call or store and preserves the formed count pointer. The byte store
  at 0x800EB9F0 is on the opposite branch-likely exit path and is annulled when
  continuing. The complete witness body is input-bound.
- Fresh stock compilation and strict MATCH of accepted EC914 and 108DA8.

Independent peer review verified those current source/lock identities, both
symbol tables, the separate native witness and both accepted-source replays.
The older 108DA8 cloud report marked its contract pending; the current production
source and image-gate lock supersede that historical status. Its production
comment explicitly discloses the same accepted count qualifier.

**Limits remain:** no original N64 declaration text or asynchronous/device
writer is established. This packet reuses the already accepted project view;
it does not infer arbitrary volatile qualifications from native reloads.

## Trace-first causal controls

The initial natural source was 179/184 plus thirteen extra words. Reusing the
real distance/weight local gave 137/184 plus six. Native declaration homes gave
125/184 plus six, still nonvolatile. The accepted shared-count view closes it.

Both causal controls remain in the replay:
- Remove only the accepted qualifier: **125/184**, six extra nonzero words,
  complete 760-byte function, four own-data references unverified by the scorer.
  Independent GNU linkage resolves the entire object, including its literal
  bytes, and independently reproduces every difference and all six extra words.
- Revert only the existing pointer declaration positions: **13/184**, complete
  736-byte extent. Every residual is a real vector-home displacement.

Workbench classified the nonvolatile residual as structural, not merely renamed
registers. Stock UGEN diagnostics expose three direct count loads in the plain
source versus two count-address materializations in the matching source. Every
form has M64/T16/P20 memory-area accounting and a 168-byte frame. `trace.py`
proves that enabling stock UGEN tree/listing diagnostics changes no object byte.

An optional instrumented UOPT build with force controls disabled also emitted
whole objects identical to stock, exposing ordinary ILOD/endpoint PRE versus
qualified ILDV. Integer pointer round-tripping was investigated but rejected:
it creates a converted-address web (save 12, cost 5), takes s4, displaces the
vector pointer and expands the frame to 176. Forced splitting does not close
it; disabling endpoint PRE fails with UOPT internal error 1115, from which no
score or conclusion is taken. Genuine kept D3A4 context preserved its old exact
244-byte body but did not improve D798, so that body is not duplicated. There
was no blind variant sweep or accepted group/lock migration.

## Native ABI and nearby scope

Direct callers are game C974+0x950 and image-B E114 at
+0xBCC/+0xFC8/+0xFE8/+0x10AC/+0x10C8. The complete selected callers are
input-bound. They establish ordinary O32: two vector pointers, signed source
index, float squared radius in the fourth slot, integer magnitude in stack
slot zero. C974 passes the same pointer for both vectors, 400.0f and integer
1600. Known caller paths proceed to another call without consuming a D798
result. The submitted void result describes that consumed contract, not every
possible indirect caller or original declaration. The accepted D3A4 helper
remains a genuine external ordinary-O32 boundary.

F938 scouting found private incoming s0/s2/s3/s5 and unsaved scratch s1/s4/s6.
It needs genuine FCE0 context and receives no ordinary-ABI replacement or claim.
D200/D328/E088/D498/DA78 likewise remain dependent on private E114/FCE0 context.
F568 and C910 were not retuned.

## Reproduce

    python3 cloud/work/frontier/dot_runtime_b_radius_effect_20261012/verify.py --score

Authenticated current-tree image B and unchanged canonical scorer, without
`--claims` or `--allow-unverified`:

    Members:
    func_8038D798:
      MATCH
        own .rodata verified at 0x80394DD4..0x80394DDC

Deep replay, optional trace and one focused test:

    python3 cloud/work/frontier/dot_runtime_b_radius_effect_20261012/verify.py
    python3 -O cloud/work/frontier/dot_runtime_b_radius_effect_20261012/verify.py
    python3 cloud/work/frontier/dot_runtime_b_radius_effect_20261012/trace.py
    python3 -m pytest tests/cloud/test_runtime_b_radius_effect.py -q

Independent GNU linkage resolves all 25 matching/old-home relocations and all
27 plain-control relocations from their unchanged complete objects. Full ELF
function extents, native next boundary, every instruction, external bindings,
allocated sections and zero suffixes are checked. Matching text is exactly
736 bytes; plain text is 760 plus eight zero alignment bytes. All objects own
eight actual literal bytes followed by eight zero alignment bytes, not eight
more native literals. MIPS2, big-endian ELF identity and the expected normal
EF_MIPS_NOREORDER flag are checked. Final links have no undefined symbols or
remaining relocations. Unreferenced intrinsic fabsf is dropped by GNU; unknown
referenced externals fail closed. Missing data and corruption of either literal
are rejected independently.

Receipts bind packet sources, recipe, verifier, trace, focused test, selected
full native targets/callers/helpers/shared-contract witnesses and eight stock
compiler executables. No shared-tool, whole-lock, Makefile or asm-text pin.
Normal and Python -O replays, stock trace and focused test pass; missing IDO
cleanly skips through the shared conftest. No static ROM TU is rebuilt.

This is complete compiler/native evidence, not a newly executed behavioral
oracle. Valid backed storage/indices, genuine helper contracts and finite
in-range conversions are assumed. Arbitrary partial aliases, concurrency,
exceptional FCSR, actual helper execution, gameplay, image composition and
recompression, and full-ROM identity remain unproved.

Text source, tests and receipts only; no ROM bytes, raw native/assembly dumps,
objects, compiler binaries, protected edits, credentials or unrelated data.
Keep draft; installation and merging remain with the independent checker.
