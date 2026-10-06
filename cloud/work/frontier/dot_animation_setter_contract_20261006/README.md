# Animation setter boundary: verified unchanged three-word nonmatch

**Research only.** `audio_channel_setup`, [0x80094888, 0x800949D4), is a
332-byte animation callback despite its historical audio name. The complete
candidate still differs in **3/83 words**, exactly as the archived w7b source.
There are no claims, accepted bytes or ROM-coverage gains.

The new evidence is a genuine source boundary: replace the archive's anonymous
static object setter with the complete, unchanged accepted `func_80090770` in
an ordinary kept O3 translation unit. Its entire **44-byte / 11-word body stays
exact**, and the callback's independently linked full body is byte-identical
to the archived nonmatch. Missing authentic setter context does not explain
this residual. Stop this route here; do not expand it into a spelling search.

Base master is `cd22879d40b3de443cfde047b86e75e159b6cec6`. Locks, archived
reports and open PRs through #130 were checked before reservation. This does
not establish absence of unpublished work elsewhere.

## Source and contract evidence

Pinned arcade ancestor:
[targets.c::AnimateFlag, lines 1391–1445](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/game/targets.c#L1391-L1445).
The GitHub file blob ID agrees with the independently hashed complete donor
file; hashes and exact source identities are in `provenance.json`.

The donor has a short operation parameter, cleanup on zero, a 1/16-second
frame cadence, signed frame index, wrap at the frame count, and a real
`ZOID_SetObjectDef(object, mapping[frame])` call before recording the new frame.
The N64 implementation adds its pause flag, floating countdown and two
metadata-driven cleanup choices. These differences are retained, not replaced
by the arcade logic. The donor's unused `flags` local is not imported.

The native setter has a signed-half index, unsigned-half value, 68-byte record
stride and halfword output at +20. `group/setter.c` is byte-for-byte equal to
accepted `src/blob/func_80090770.c`; original arcade implementation, N64
function naming, source linkage and inline annotations are not recovered.
Only the two actual entry points are kept; there are no stand-ins or keepers.

A regression test proves `group/caller.c` differs from the archive only by
using that real function declaration/call and removing the unused local setter
and its record declaration. There are no added guards, locals, computations,
stack padding or register-pressure devices. Separate O32 compiles verify 14
layout facts, including the actual object/model/metadata and setter fields.

The volatile frame-clock view is inherited from the archive. An independent
protected witness, `func_8010E694`, reads the same `D_8002EB94` location three
times with no intervening call or store. The verifier binds the full witness
hash and checks its address formation and observation offsets. This supports
a volatile observation view; it does not establish the original qualifier,
asynchronous writers or interleavings. No concurrency behavior is tested.

## Complete compiler result

- Archive and real-setter context both have a 332-byte ELF function and the
  same three differences at +0xFC, +0x118 and +0x11C. They concern the scratch
  register used to form the halfword mapping address; no instructions are
  removed from the comparison.
- The accepted setter's 44-byte ELF body is exact in that context.
- A single ordinary-clock control emits 328 bytes and has 69 scorer differences and **70 complete-body differing
  positions**, including its missing final word. The four missing native bytes are retained in the whole-body
  comparison; scorer output alone is not used as extent proof.
- GNU readelf independently verifies each ELF symbol size. GNU ld resolves
  all 12 caller and two setter relocations and agrees with the project's relocation
  result. Every non-function text byte is checked as zero alignment.
- Neither caller nor setter owns literal/data bytes. The 1/16 constant is
  materialized in code. No literal pool, copied data or masked relocation is
  used.

The whole-object per-body linking layout is a verification device, not evidence
of original contiguous translation-unit placement. No production file, scorer,
compiler stage, target, symbol map or lock is edited.

## Behavioral verification

2,400 directed and seeded cases compare a separately expressed scalar oracle,
the protected native caller, the complete GNU-linked caller and the unchanged
host C89 source with UBSan. That is **4,800 bounded MIPS executions** plus
2,400 ordinary host executions. The unchanged accepted setter runs in the host
fixture rather than being replaced by a setter hook.

All 83 native instructions execute, and all seven conditional branches take
both outcomes. Cases cover cleanup priority when both flags are set, the pause
path, positive/zero/negative timer residuals, frame equality, count boundaries,
signed short mode narrowing including nonzero upper argument bits, all eight
fixture resources and metadata records, and callback mutations.

The native interpreter compares every mapped nonstack byte and callback-visible
snapshot with the oracle. It checks saved integer/FP registers, return address,
restored stack, exact allowed stack-store addresses and surrounding canaries.
The host fixture checks all object/metadata/resource gaps and mapping canaries.
External removal and sound callbacks remain explicit O32 contract hooks with
caller-save clobbers and deliberate bounded mutations; their real internals
are not executed.

Five compiled semantic mutants fail: wrong cadence, double frame advance,
reversed cleanup priority, wrong mapped resource and omitted current-frame
update. Unknown opcode, redirected stack save, wrong callback target and
truncated native body controls fail. Tests also reject an unmapped clock read
and a changed witness clock address.

The original local checks passed **787 selected tests**: 13 packet tests and 774
scorer, integrity, guard, submission and owned-data tests. All 402 static source
locks remain intact. The submission scanner schedules zero matching
submissions for this research-only directory. Early sparse-checkout test
failures were missing baseline source inputs, resolved by materializing those
unchanged inputs and rerunning; they are not candidate regressions.

The linker-provenance follow-up passes **14 focused packet tests** and repeats
the full native/host proof. A broader local attempt encountered scratch inode
exhaustion and was interrupted; no broader pass or hosted-CI rerun is claimed.

## Reproduction and limits

With the pinned IDO and GNU MIPS tools configured, from repository root:

    python3 cloud/work/frontier/dot_animation_setter_contract_20261006/verify.py
    python3 -m pytest -q tests/conveyor/test_animation_setter_contract.py

`verify.py --write` refreshes the local receipt after a deliberate source change;
normal replay requires equality of portable evidence. The GNU linker binary hash
is retained as run provenance rather than a cross-host equality requirement;
complete GNU-linked bytes and extent checks, all relocations, IDO stage hashes,
source/native bindings and behavioral results remain equal. A regression accepts
a changed linker identity while rejecting changes to each verification result.
Current native manifests are always verified.
The whole-manifest digest is historical provenance for receipt portability;
selected caller/setter/witness body hashes, all consumed relocation symbol
addresses, source/recipe hashes and all behavioral evidence remain bound.
The three selected entry addresses are also asserted against their intended
native placement and recorded in the receipt; address-only drift is rejected.
The caller interval is derived from its verified entry and complete target size.

Finite binary32 samples use round-to-nearest. NaNs/infinities, FCSR exception
flags, invalid pointers, unrestricted aliasing, asynchronous clock mutation,
actual callback runtime, whole-game behavior and gameplay are outside scope.
There is no shadow-unit, image, compression, full-ROM, production-admission or
hosted CI claim. Merging and any later admission belong to the independent
checker. This packet does not start CI monitoring.
