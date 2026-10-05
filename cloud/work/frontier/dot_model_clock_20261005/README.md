# N64 model-clock reset: bounded source research

**NONMATCH. No claims, accepted bytes, splice or ROM-coverage increase.**

`func_800E762C`, `[0x800E762C, 0x800E7710)`, is a 228-byte/57-word model-clock
reset and synchronization leaf. The retained natural C emits **220 bytes** and
has **47/57 differing words**. Its four trailing zero alignment bytes are outside
the function; neither those bytes nor the missing final words are credited.
The old corrected A30 source emits 232 bytes and differs in 51/57 scorer words
(52 full-extent positions when its extra zero instruction is counted).

This is a source-contract and verification result, not a near-match finish.
The snapshot hypothesis did not close the function. Workbench classifies the
retained source as a structural mismatch with a two-instruction shortfall.

## Selection and source evidence

Base is freshly fetched master `cc4d5fdd`. The current game lock, wave-5 reports,
repository research and open PRs #98–102 were checked before selecting this range.
It is outside those claims and the other two current scouting lanes. Selection
and the specific snapshot hypothesis were reported before source edits. No
conclusion is drawn about unpublished work elsewhere.

The historical function name carries no reliable semantic meaning. New evidence
was assembled from these independent sources:

- Protected `render_thread_entry` calls the leaf at `0x800E786C` with argument
  zero during game-thread initialization. The other direct call is in
  `countdown`, at `0x800FC0B4`. A fresh manifest-backed census records both.
- Protected `func_800E6AF8` increments the same global integer clock, computes the
  corresponding floating time, and independently uses the adjacent per-model
  integer ticks, floating last time, and floating step at `+0x710/+0x714/+0x718`.
- Unchanged accepted `battle_mode_setup` reads the last-time field for game-task
  dead reckoning. Unchanged accepted `func_800E5D64` supplies per-replay step
  values and uses the same global tick counter for 50/60-Hz conversion.
- Authentic arcade `game/reckon.c:game_reckon_all` corroborates the relationship
  between game time, a model's last time, and its rate/fudge fields. The checked
  donor is historicalsource/rushtherock commit
  `845329d7b36f5a384c5625ed9a0aef584ab46139`, lines 159–205:
  https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/game/reckon.c#L159-L205

The clock-reset body is **N64-specific**. No complete arcade donor, original
function name, original declaration, asynchronous writer, or original source-unit
boundary has been recovered. Arcade ancestry is not inferred from the old label.
The three field names in `candidate.c` describe their observed contracts.

The older `cloud/work/near-miss/func_800E762C/base.c` contains malformed pointer
reconstruction, including replacement of the loop pointer with a field address.
It is not a valid starting point. The comparison baseline here is the later
corrected `cloud/work/tiny_A30/func_800E762C.c`, whose source hash is recorded.

## Contract and bounded source hypothesis

The leaf takes one ordinary integer argument in `a0`. It has no calls, stack
frame, incoming private register arguments, or unsaved callee-register writes.
Both native direct callers use the ordinary argument register. Tests seed and
verify all O32 callee-saved integer/FP registers plus stack/global pointers.

The function negates the 32-bit tick input, stores the global integer clock, and
computes/stores its floating time. It snapshots the global rate and mode once,
then visits six records of exactly 2,056 bytes:

- Outside mode 2, for the first record, or when a record's step is zero: replace
  ticks, last time and step with the global reset values.
- Otherwise: divide the new time by that record's existing step in binary32,
  subtract 0.5 for a negative ratio or add 0.5 otherwise in binary32, then
  truncate toward zero to update ticks. Update last time and preserve the step.
  This commonly implements nearest-integer rounding with half ties away from
  zero, but the intermediate binary32 rounding matters at large magnitudes:
  ratios of +8,388,609 and -8,388,609 produce +8,388,610 and -8,388,610.

The explicit once-per-reset snapshot was the initial source hypothesis. The
retained version computes time from the just-stored global clock; it introduces
no padding locals, dead reads, stand-ins, fabricated parameters, `volatile`,
inline assembly, compiler/scorer changes, or production keep/root changes.
Opaque byte arrays describe the real record offsets; they are not stack padding.

Fewer than thirty directed source experiments were used, with no permutation,
layout, register-forcing or compiler-flag sweep. The verifier reproduces the
important controls from complete source:

- Corrected A30 baseline, O3: 51/57; ELF 232 bytes.
- Explicit local clock snapshot: 53/57; ELF 216 bytes.
- Retained stored-clock source, O3 and O2: 47/57; ELF 220 bytes.
- One common last-time store: 47/57; ELF 212 bytes, still less complete.
- Unconditional per-record step cache: same retained output.
- Hypothetical storage-owner TU: defining the real tick/model objects locally
  and leaving mode reads in the loop gives 38/57 and the native 228-byte extent.
  **This is an unadopted source-ownership question, not an improved admitted
  candidate.** No evidence establishes this original owning TU; the objects are
  not added to the submitted C or production context. Related pointer/loop/global
  forms did not produce equality.

The last control demonstrates that storage/declaration context can change code
shape. It does not justify adding data definitions merely to obtain registers.
Stop here until genuine owning declarations or a new source/ABI boundary supports
one specific change. Broad allocation or qualifier searches are unwarranted.

## Verification

From the repository root with the existing pinned IDO/GNU MIPS environment:

```sh
python3 cloud/work/frontier/dot_model_clock_20261005/verify.py
REQUIRE_TOOLCHAIN=1 python3 -m pytest tests/cloud/test_dot_model_clock.py -q -o addopts=''
```

The receipt and fresh replay establish:

- Full ELF symbol extents; all ten HI16/LO16 relocations; independent GNU linking
  of the entire retained function at its native address. GNU and the project
  relocator agree on every emitted byte and the full-extent residual. The
  candidate owns no literals, jump tables, data or BSS.
- A genuine O3 context of the candidate and three unchanged accepted sources:
  `func_800E5D64` (1,296 bytes), `battle_mode_setup` (348), and `func_800EC914`
  (596). Every accepted body remains exact, including its verified own literal
  references. The candidate remains 47/57 and 220 bytes.
- **3,494 deterministic cases** compare manifest-backed native instructions and
  GNU-linked candidate instructions through a small explicit MIPS interpreter,
  an independently written float32 oracle, and unchanged host C with UBSan.
  Full model memory, both global outputs, ordered writes, single global rate/mode
  reads, and O32 preservation are checked.
- All **55 reachable native instruction offsets** execute. A separate
  conservative branch/delay-slot CFG establishes that the two remaining native
  sites, `+0x90/+0xB8`, are unreachable duplicate arithmetic copies. This is
  instruction-level bounded testing, not N64 hardware/gameplay execution.
- Cases include all branch regimes, positive/negative zero steps, positive and
  negative rates, sign and half-tie rounding, six-record boundaries, mode values,
  and 32-bit tick-negation wrap boundaries. Floating values are finite and every
  tested float-to-signed-int conversion is defined. NaN, infinity, conversion
  overflow, asynchronous mutation and overlapping global objects are outside
  the proven domain.
- Five wrong-contract controls are rejected: wrong tick sign, removal of the
  first-record exception, wrong negative rounding, overwriting retained rates,
  and visiting only five records. Witness cases are recorded.

Seven focused tests pass, including fresh compilation, linking, behavior and
receipt replay. The packet plus existing scorer/guard/submission regression suites pass
**707 tests**, with no failures or skips. The verifier itself reconstructs all research controls;
none relies on saved compiler output. Target-manifest hashes record provenance;
unrelated acceptance annotations may change, while selected native hashes and
all source/body/extent/relocation evidence must still reproduce exactly.

No protected targets, symbols, shared headers, accepted source, locks, global
recipes or tooling were changed. No ROM bytes, raw assembly, objects, compiler
binaries, credentials or unrelated data are included. Whole-program shadow,
image splice, compression, cartridge hash, full repository suite and CI are not
claimed. Merging and final integration remain with the independent checker.
