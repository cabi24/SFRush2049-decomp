# D498: complete runtime-B radial hit and impulse source

Research only: **COMPLETE-SEMANTIC-SOURCE / NONMATCH**. No accepted replacement,
original-TU reconstruction, private-context match, coverage increase, or ROM claim.

Base: `6b2e9e506fe3d2267a710e41c85af5364ccd00c7`.
Native interval B `[8038D498,8038D798)`: 768 bytes / 192 words.
Target SHA-256: `fc6f18de1f301b86d8a9708aa066babbedc370b22067858a2248578597aa2d6e`.
Native image and consumed constants are authenticated in memory from the base
asset. This packet contains no ROM bytes, raw assembly, or compiled objects.

## Source and real boundary

`visual.c` supplies the complete natural semantic body, with actual field views,
a player loop, distance/tier calculations, ordered helper calls, hit-mask updates,
and two impulse additions. It contains no synthetic caller, padding formal,
clobber mask, pressure-only variable, matching sweep, or assembly wrapper.

- Capture source-player pointer, full-word-indexed scene scale, and primary
  object position before scanning. Primary must be valid even when count is zero.
- Scale <= 8 uses damage 800 / strength 1; <= 20 uses 400 / 0.6; larger uses
  200 / 0.5. All comparisons and arithmetic preserve native binary32 order.
- Reload signed player count and signed owner bytes. Exclude the same owner,
  loop-indexed vehicle state other than -1, inactive players, and mask hits.
  The signed hit-mask byte is sign-extended; native variable shifts use the low
  five owner bits, expressed explicitly in C.
- Accept distance squared <= (scale * 4)^2. Set the hit bit before damage.
  Damage may change later-reloaded fields, but captured center/scale/distance
  and the original source-player pointer remain the current call's inputs.
- Normalize radial impulse, scale by 330000 and tier strength, add 66000 to the
  vertical component before tier scaling, then apply the player's current basis.
- Reload player.owner after transform to select the output vehicle. This differs
  from the loop index used by the eligibility test. Add xyz into vehicle+0x124;
  then set y to positive zero and add xyz into vehicle+0x13C. The y addition is
  still performed, including its observable effect on an existing negative zero.

The native entry consumes record pointer s2. It saves only ra in a 104-byte
frame, preserves s2, and writes unsaved s0/s1/s3-s8 and f20/f22/f24/f26/f28/f30.
The sole direct caller in the authenticated B image is E114+0x111C, which spills
s5/s6/f22 and reconstructs several constants afterward. Indirect/other-image
callers are not excluded. The ordinary-ABI signature here is a readable testing
interface, not evidence of an original exported/kept compiler root.

D3A4 is a conventional three-input boundary: owner player, target player, damage.
Its authenticated 244-byte native body has no unsaved callee-save writes.
A61B0 is the conventional source-vector, destination-vector, basis transform.
Neither helper is fabricated as another private register-pressure requirement.
The record/player/vehicle/scene views agree with the companion E114/DA78 packets
at the consumed offsets. Record byte+5 is named signed hit_mask here.

D498 is the final previously missing named semantic body in the known minimum
FCE0/F938/E114/D200/D328/E088/DA78/D498 private-context closure, totaling 11,376
native bytes. This does not establish original TU visibility, compilation roots,
whole-context matching, genuine private calling conventions in compiled C, or
integration eligibility. Those gates belong to separate closure work.

## Verification

From a checkout with pinned IDO and MIPS GNU binutils:

    python3 cloud/work/runtime_b_d498_visual_20261006/verify.py --check

For this source-only overlay, add `--reference-root /path/to/repository`.
Temporary compiler/linker products are removed automatically. A private TMPDIR
can be used; no persistent shared build outputs are needed.

The required bare O3 header goes through unchanged canonical score.compile_single,
including its mandatory `-Wab,-r4300_mul` backend workaround. The whole compiled
function is 920 bytes and is **NONMATCH**. All allocated sections, full function
extent, relocations, and external bindings are checked; independent GNU linking
must resolve every referenced symbol. Linked research placement does not imply
native data placement or instruction equality. The IDO layout object validates
18 field/size facts.

The receipt binds only packet-owned implementation/verifier/layout/contracts,
authenticated target words/constants, compiled extents/bytes/relocations/data,
and bounded behavior. Production context is obtained with git show at the base.
No live source/manifest/scorer/lock hash, live lock-state assertion, or own-test
hash is used. Compiler/linker provenance is recorded separately from stable
replay comparison.

The complete replay covers 1,658 paired fixtures: all 256 mask bytes, tier and
radius boundaries, signed nonpositive counts, activation/state exclusions,
repeated owners, distinct eligibility/output indices, full-word scene indices,
nontrivial matrices, callback mutations, and deterministic randomized cases.
Each pair compares full mapped nonstack state and ordered helper calls with
state snapshots. Ordinary helpers poison all caller-save registers. Fourteen
compiled wrong-source controls and three fail-closed controls must be rejected.

190/192 native words and 229/230 candidate words are exercised. Native +0x88 and
+0xB0 and candidate +0xF8 are statically unreachable duplicate mtc1 landing
operations after unconditional branches; the preceding likely branches skip
those duplicates. The receipt reports exact unexecuted offsets and branch edges.
No unreachable instruction is counted as executed.

## Explicit domains and exclusions

Records, object, tables, players, and vehicles are aligned, initialized, live,
disjoint and single-threaded. Fixtures use signed count <=4, mapped initial owner
0..3, and mapped output-owner slots through31. Out-of-range signed owners occur
only on rejected routes without an invalid indexed dereference. Individually
mapped full-word scene indices are examples, not a recovered table bound.

Accepted hits require positive finite distance. Native admits coincident points
through the radius test and then divides 0/0; no guard is invented in C. Both
replays fail closed on that excluded case. Float tests cover finite normal/zero
operands and results under default rounding. NaNs, infinities, subnormals,
overflow/underflow, exception/FCSR effects and arbitrary aliases are excluded.
Null primary is deliberately rejected even for nonpositive player counts.

The transform model uses sequential rounded binary32 row dots from the accepted
base-commit A61B0 source. Damage uses a deterministic side-effecting boundary
model; its team/shield/health/scoring implementation is not executed. Separate
widened mutation probes test capture/reload ordering; they do not assert that
real helpers write all such fields. Full gameplay, real assets/lifetimes, private
ABI, original TU, strict match, accepted bytes, compression and ROM gates remain
outside this packet.

Focused tests skip replay cleanly without IDO or GNU linker. The coordinator owns
aggregate tests on current master with/without IDO and any later publication,
after the packet is frozen and independent review concludes.
