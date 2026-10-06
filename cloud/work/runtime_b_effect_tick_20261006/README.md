# Image-B effect tick: complete semantic closure, one O3 nonmatch

The complete three-body source in `effect_tick.c` reconstructs `func_80390F60`
(988 native bytes), with genuine private `func_80390B10` and `func_80390D38`
(552 native bytes each). It is research source, not a strict match or production
submission. No external writes, branch publication, PR, protected-file edits,
merging, or CI monitoring were performed.

Historical context is anchored to
`6b2e9e506fe3d2267a710e41c85af5364ccd00c7`. Production context is read with
`git show BASE:path`. Receipts bind packet-owned source and complete native
function words, never mutable live production files, manifests, locks or tools.
No ROM bytes, native assembly dumps, objects, or machine-local artifacts belong
in the deliverable. `local/` and `tmp/` outside this packet are investigation aids.

## Contract and original visibility

The root is `void func_80390F60(void)`: no logical input is consumed and its caller
consumes no return. The ordinary root saves s0/s1 and paired f20/f21; the genuine
children independently initialize and clobber those registers, saving only ra.
They are not ordinary external leaves and are not replaced by stubs in source.
The caller at main `800FAA1C` is the complete `render_viewport_init` root at
`800FA9B4`. The image-B residency/lifecycle precondition established by the
viewport contract audit still applies. Image A uses the same load address and
has a different, larger function containing address `80390F60`.

`contracts.py` verifies complete target identity and the base-anchored direct
edge census: each child is called only by 0F60, at 0F70 then 0F78. The three
addresses do not appear as aligned absolute pointer literals anywhere in image
B. This supports the explicit precompile hypothesis: complete static children,
one externally visible root, with only that root kept by the canonical group
route. It does not prove original source-file identity, every computed or
indirect reference, or residency for arbitrary corrupted lifecycle state.

The three external declarations are conservative ordinary service boundaries:

- `f32 func_8008B2E4(f32 range)` is the range-float RNG.
- `void func_80090770(s16 index, u16 value)` sets the unsigned halfword at +14
  in a 68-byte scene record. Its index is the object's signed low half at +E.
- `void func_8008B0D8(s32 index, s32 mode, u32 viewports)` shows an object;
  these callers pass the full object word at +C, mode 0 and viewport mask 15.

Full helper body hashes and base source paths are in `contracts.json`. Native
caller-saved registers, HI/LO and floating condition state are conservatively
clobbered at every modeled external boundary. No pure/const memory assumptions
or hidden incoming private-register parameters are introduced.

## Data and complete behavior

The control block has signed bytes count/+0 and selected indices/+1,+2,+3;
float timers at +4,+8,+C; and an entry-table pointer at +10. An entry has an
object pointer/+0 and signed halfwords state/+4 and eligibility/+6, stride 8.
The consumed object prefix contains a flag byte/+4, scene word/+C and animation
halfword/+50. Its candidate type has 84-byte aligned size; the original complete
object allocation size is not inferred. Unknown ranges preserve evidenced
field positions, not register pressure or artificial stack layout.

Each timed selector uses -2.0f as the in-flight sentinel. At that sentinel it
waits while the selected object's flag bit 2 remains set. Once clear it chooses
a new delay, resets matching old state codes to 1, and makes its current selected
entry eligible again. Otherwise it subtracts the frame delta and, only when the
result is strictly negative, starts at a random integer slot and cycles until
eligibility==1 and the state differs from its own kind. It then sets state=1,
eligibility=kind, flag bit 2, and animation; calls setter then show; and writes
the sentinel. The table/object pointer is reloaded after the setter, while the
chosen entry offset remains fixed even if selected-index globals are mutated.

The three selectors run as follows:

- D38 first: selected +1, timer +4, kind 4, animation 353, resource 80142A82;
  reset delay Random(10)+5.
- B10 second: selected +2, timer +C, kind 3, animation 359, resource 80142A8E;
  reset delay Random(60)+90.
- The root's final selector: selected +3, timer +8, kind 2, animation 354,
  resource 80142A84; reset delay Random(60)+90.

Between the second and third selectors, the root walks the live signed-half
player count at 8014A108, using 952-byte records at 80152818. It handles status
bit 0/+38C, signed countdown/+3A0, unsigned alpha/+3A1, signed phase/+3A2 and
float timer/+3A8. Phase 3 indexes an eight-byte local alpha table. Phase 1 clears
countdown, subtracts alpha modulo 256, clamps results <=16 to 16 and advances to
phase 3, then subtracts delta while the old timer was positive. An already
nonpositive timer advances phase without changing alpha. Other phases with a
positive timer add 8 to alpha with saturation at 255 and subtract delta; otherwise
they clear phase and only status bit 0. The signed countdown decreases only if
positive, even when the status bit was initially clear.

This group performs no allocation, linked-list traversal, indirect callback call
or callback-pointer store. No arcade whole-function donor was established;
existing Random and show-object lineage is service context, not a donor claim.

### Proof domain

Storage is initialized, aligned, live and large enough for all dereferences.
The player count may be nonpositive, otherwise it fits the supplied player array.
A status-active phase-3 countdown is in 0..7. Selections used at a sentinel are
nonnegative valid table indices. When an expiry search runs, count is positive,
the RNG's truncated return is in range, and at least one currently qualifying
entry exists. This last condition is required at each of the three ordered
searches; an earlier selector can consume eligibility. Without it the original
native code can loop forever. No defensive guard has been invented.

The tests use up to eight players/entries, including shared pointed objects,
finite normal-or-zero binary32 with default rounding, valid scene indices, and
nonaliasing global/control/player/scene storage. They are not an unrestricted
memory-safety, NaN/conversion, concurrent mutation, full gameplay or loader proof.
The intentional setter/RNG mutation probes widen the services' real effects to
test reload obligations; they are not claims that those services normally change
control counts, selections, tables, player fields or delta.

## Verification before and after the one baseline

Before target compilation, host C89 source versus authenticated full native
bodies passed 1,784 fixtures / 1,989 root invocations. Coverage reaches all 517
CFG-reachable native instructions. The six unexecuted native slots are duplicate
load/store remnants unreachable under the decoded branch/delay-slot graph.
The interpreter fails closed on unsupported instructions, bad pointers,
unaligned reads/writes, unknown calls, and exhausted search/step budgets.
Non-field bytes are checked unchanged. All semantic fields, external scene/RNG
state, and full ordered boundary snapshots are compared, not only final output.

All 28 target O32 size/offset facts passed before the sole group compilation.
The unchanged canonical `score.compile_group` used exactly the source's bare
`-g0 -O3 -mips2 -G 0 -non_shared` flags and its mandatory `as1 -r4300_mul` backend
flag. `group.json` keeps only 0F60. Every real body is present in source. There
are no pressure locals, unused arguments, inline asm, artificial padding/dead
reads, volatile shaping, fake ABI leaves, fake source stubs or extra keepers.

The resulting full ELF passes the same 1,784 fixtures / 1,989 root invocations,
using only its own linked constant data. Every root run preserves ordinary
nonvolatile registers. All 53 relocations and 1,932 other text/data bytes are
independently checked against the GNU-linked image. The sole 8-byte initializer
payload equals the native lookup; its 16-byte data section has eight trailing
zero alignment bytes. Payload equality does not prove native reference placement.

Fifteen wrong host-source variants are rejected, including signedness, alpha
boundaries, selector order, timer sentinel/expiry, search kind/eligibility,
resource/animation identity and reuse of mutated selection after a call. An
unknown native opcode is also rejected. These controls do not invoke IDO.

Independent audit additionally passed 2,498 host/native fixtures / 2,528 root
invocations, 120 direct-private calls with randomized incoming registers, 1,800
independent expected player-field oracles, and 2,528 unchanged candidate/native
root invocations. It verifies every reachable native instruction and the complete
object. Those counts belong to the independent audit, not this packet's own suite.

## Honest baseline result and stopping condition

The root is **NONMATCH**. ECOFF, matching stEnd records and PDRs prove:

- B10: 8-byte naturally deleted-static stub; its genuine body was inlined into
  0F60. The source did not contain a fake stub. This emitted entry cannot replace
  the native standalone private entry.
- D38: 548 candidate bytes versus 552 native; 24-byte ra-only frame.
- 0F60: 1,560 candidate bytes versus 988 native; 80-byte versus 88-byte frame.
- Total complete procedure bytes: 2,116, with 12 zero text-alignment bytes.

Canonical root comparison reports 245/247 differing words, 141 nonzero excess
words, one unresolved private `.text` call reference and two unverified own-data
references. It is neither a strict match nor relocation-only equality. Candidate
size, kept-symbol status and semantic equality do not authorize splicing.

D38's complete unmasked residual is 131/138 different words, including one
missing word, no excess. The one-word reduction is delta address formation:
native materializes the complete global address in a temporary and uses a
branch-likely delay-slot load; the candidate uses a direct LUI+offset load on
the else path with an ordinary branch/NOP. Scheduling, one-word displacement
and subsequent temporary-register rotation account for broad positional
mismatch. The global address, arithmetic order, strict-negative test, scene
arguments and observable writes agree. No new source-contract, type, alias or
arithmetic fact was found that justifies another candidate or visibility sweep.

The exact closure visibility hypothesis therefore yields complete semantics but
does not reproduce original inlining/address-generation choices. Stop here.
No source variant or additional target compile was attempted. Metadata inspection
was repaired once to allow the genuine `.data` initializer instead of assuming
`.rodata`; the original object was reused unchanged throughout.

## Reproduction

Run from any directory with a full-history repository as REFERENCE and the
canonical scorer/target tree as SCORE_ROOT. Set TMPDIR to a writable workspace.

- Compiler-free contracts: `python3 contracts.py --reference-root REFERENCE --output contracts-replay.json`
- Host semantics: `python3 semantic.py --reference-root REFERENCE --output host-replay.json`
- Host falsification: `python3 controls.py --reference-root REFERENCE --output controls-replay.json`
- Fresh unchanged one-shot baseline: `python3 baseline.py --reference-root REFERENCE --score-root SCORE_ROOT --work-dir FRESH_WORK_DIR --output baseline-replay.json`
- Reinspect an existing baseline without IDO calls: use the same command with
  `--inspect-existing` and its original work directory. This validates source
  identity, relinks the same object, checks every relocation and reruns semantics.

The baseline command cleanly prints SKIP if pinned IDO or the GNU MIPS linker is
missing. Set IDO_DIR as usual; the scorer is not edited or bypassed. Its work
directory refuses a second group compile. Fresh replay comparisons should use
source, target words, extents, bytes, relocations, own data and behavior; compiler
paths or mutable production metadata are not acceptance keys.

For pytest, set SFRUSH_REFERENCE_ROOT and SFRUSH_SCORE_ROOT as needed, then run
`python3 -m pytest PATH_TO_PACKET/test_packet.py -q`. Supplying
SFRUSH_EFFECT_TICK_ARTIFACTS enables inspection/replay of the already built
object. Without artifacts that single test skips. Without IDO/linker it also
skips cleanly. The suite never secretly recompiles the target. Host/compiler-free
checks remain active when IDO is absent. No test-file hash is bound in receipts.

Final focused checks: 5 passed with the existing IDO artifact; from a foreign
working directory with IDO deliberately absent, 4 passed and the target-artifact
check skipped. The baseline entry point itself also returned the documented
missing-toolchain SKIP before invoking IDO. Repository-wide aggregate suites were
not run for this unpublished, isolated research packet.

A separate evidence-only residual inspection resolved both candidate delta
address/load locations through retained ECOFF/PDR line metadata to the existing
timer-subtraction statement (effect_tick.c:104). No optimizer or scheduler trace
survived the canonical temporary build. Workbench diagnosis supplied no known
source edit. This localizes the residual without adding a new source hypothesis.
