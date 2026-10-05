# Vector blend: 3 words to 1, no matching claim

Pinned baseline: `cf10b339`. Target `func_800E8D50`, native interval
`0x800E8D50..0x800E8F10`, 448 bytes / 112 words. It is unlocked and absent from
the accepted, provisional, and `cloud/matches` sets at this baseline. It became
callee-ready when `vector_normalize_length` was accepted in frontier wave 2.
The prior `tiny_A76` work is preserved unchanged.

## Result

`best.c` has exactly the complete 448-byte ELF function extent and differs in
one fully relocated word, at offset `+0x160`: the floating multiply's two input
operands are reversed. This is **NON-MATCHING RESEARCH**, with no claim, no
splice and no coverage credit. All 112 words were compared without masks;
there are no unresolved, error, or extra-word entries. The unchanged scorer
reports two own-rodata sites as unverified; the existing own-data verifier
proves both by content, and the replay relocates them before its independent
unmasked comparison. No uncertainty is silently ignored.

The archived source freshly reproduces 3/112 at both O2 and O3. Expressing the
actual weight read inside its consumed inverse calculation,
`inverse = 1.0f - (weight = D_80152708[slot]);`, closes the two scheduling
residuals at `+0x114/+0x118`, without adding any value, operation, argument,
spill, padding, or invented context. The one remaining residual is unchanged
at O2/O3 and with the two genuine callee definitions visible.

Exact command (after sourcing the pinned local recovery environment):

```
python3 tools/cloud/score.py fn cloud/work/frontier/dot_vector_blend/best.c func_800E8D50 --flags "-g0 -O3 -mips2 -G 0 -non_shared"
```

Scorer summary:
`1/112 words differ (2 section-relative relocations unverified: .rodata+0x0 at +0x78, .rodata+0x0 at +0x7c)`.
The separate content proof covers exactly these two sites. A negative control
using 0.7f instead of 0.6f is refused before any full-word result can be accepted.

## Callee and data contracts

The target has four consumed native pointer parameters: an object, a three-float
position, a nullable 3x3 matrix, and a three-float delta. The optional matrix is
forwarded unchanged with the object and position to the accepted `func_800E8CB8`.
Its true implementation copies nine floats, conditionally scales a matrix
column, and invokes the signed-half-index matrix updater.

The final `vector_normalize_length` call consumes the three-float residual and
writes all nine floats of an orientation basis. Its historical name is
misleading. `best.c` declares its matrix parameter as the same pointer-to-row
type as the accepted definition, rather than the archived pointer-to-whole-
matrix spelling. The first helper's declaration agrees with its accepted
opaque pointer form. Both are ordinary native call contracts; no extra formal
or stand-in was introduced.

The source retains signed-byte car/slot indexes at object offsets 859/860,
a signed-half car status with 2056-byte stride, and 152-byte slot records with
matrix at 96..128 and position at 132..140. Mode reads retain the archived
volatile view. The archive's `D_801244C0` declaration was found to name this function's own
0.6f literal, as identified by `type_model/rodata_owners.json` and authenticated
`asm/us/blob_data`. `best.c` corrects that inherited external-global assumption
by writing 0.6f naturally at both uses. The produced own bytes are verified at
0x801244C0..0x801244C4. Generated declarations elsewhere are not evidence of a
shared runtime global.

The accepted normalization source is copied byte-for-byte into scratch context.
The complete genuine `func_800E8CB8` body is extracted unchanged from its
accepted group. Only those two actual callees are included. Their compiled raw
ELF bodies are identical between the archived and improved setter builds; this
is preservation evidence, not a fresh target-match claim for the callees.
The normalizer's own rodata verification remains the existing acceptance proof.

## Bounded work and stopping point

Workbench diagnosis ran before editing: exact frame/register lanes, two
scheduling differences and one commutative FP multiply difference. Its target
object was made from authenticated target words for diagnostics only; it has no
relocation records, so the diagnosis's relocation warnings are not acceptance
evidence. All published comparisons use the unchanged authenticated scorer plus
an independent exact-ELF-size and complete relocated-word comparison.

Twenty-nine bounded source-form controls on the archived source tested literal spelling, actual
weight/inverse evaluation shape, product/addition order, consumed intermediate
values, ordinary declaration ordering and loop spelling. Counts/hashes are in
`controls.json`; exploratory files remain untracked scratch, and these controls
are not acceptance receipts. The best improvement and archive control are fully
replayable by `replay.py`. Double precision and added intermediates regressed;
reversing the multiply's source operands did not change the remaining word.
After the literal ownership correction, four natural spellings of the same
0.6f value (including shortening the adjacent 0.75f/0.5f literals) each retained
the same one-word residual. The pass stopped rather than introducing artificial
pressure or a scorer rule.

Next useful evidence: an authentic original expression/declaration pattern or
compiler lowering explanation that changes this multiply's operand order while
preserving the measured frame and all other words. Repeating the same algebraic
operand swap is already a negative control. The arcade checkout is unavailable
in this environment; the available mapping documents did not identify an
ancestor. No new arcade provenance is claimed.

## Reproduction and verification

```
python3 cloud/work/frontier/dot_vector_blend/replay.py
python3 -m pytest -q tests/cloud/test_dot_vector_blend_packet.py
```

Six full-extent replays, one wrong-literal refusal and 38 focused tests passed.
The replays are: archive and best at O2/O3, then both in genuine two-callee
context. `verification.json` binds the source inputs, protected manifest, scorer,
target bytes and relocated body hashes. Raw target bytes, assembly dumps,
compiled objects and diagnostic output remain under ignored `build/` only.

No source under `src/blob`, locks, symbols, target assembly, scorer, or shared
context was edited. No remote builder was used. Image, full-unit shadow and
ROM gates were not run because nothing is proposed for acceptance.
