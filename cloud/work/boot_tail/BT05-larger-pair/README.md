# BT05 larger scaling and random-note research

**Two complete NONMATCHs, 696 B; zero matching bodies or verified bytes.**
`232A4` is7/67 O2 words; `23564` is66/107 with two nonzero extras. Neither is
eligible for a matching submission or promotion.

- Activation `20621601`: `800232A4+268`, `80023564+428`.
- Branch `dot/boot-tail-bt05-larger-pair`, base
  `301d9e7552ad4fd7f54a38796db84671e1000d35`.
- Fresh work beyond the completed BT05 under256 population. Earlier sources
  stay frozen; only this owned research packet changes.

## Whole native behavior and genuine contracts

Dispatcher `23E9C+0x474/+0x554` supplies genuine state/command pointers and consumes
byte results. The command is an aligned two-word record. Packed state prefixes
model only observed offsets; unknown object gaps are not local padding.

`232A4` selects current volume+0x30 or original volume+0x34 according to command
word1 bits8..15. It multiplies by word0 bits8..15 with unsigned low32 arithmetic,
shifts right7, adds bits16..23 shifted16 and caps the result at0x7F0000. It forms
a u16 curve identifier from word0's high byte and word1's low byte, calls genuine
`2321C(u32,u16)` and stores its returned word. The cap proves index0..127 for
this curve call. Native packed writes/reloads, shift7 and absence of a newer
version-specific flag update are retained.

`23564` sorts two raw command byte bounds or computes relative bounds from
packed note+0x50. Relative sum/difference truncate to signed16 **before** the
0..127 clamps. A newer public source's signed32 locals would change this native
behavior. Both bounds and the swap temporary are meaningful used locals.

A real command flag chooses a detune byte or `(RNG % 201)-100`; another draw
chooses a note within the computed range. The handler rebuilds word0 with
opcode0x19, note and detune, zeros word1, invokes genuine
`225FC(state,command)`, ignores its result and returns zero. The selected source
reads the randomization flag before bound construction; no intervening store or
call changes that input. No artificial live range or stack slot is added.

`1E790(void)` explicitly masks its return to16 bits. Its u16 prototype and
signed-int promotion for remainder are preserved. Relative narrowing follows
the N64 signed16 conversion convention. No safe fallback is fabricated for an
invalid zero range divisor; the source contract excludes that native trap case.
Ordinary valid note ranges are nonzero. This does not claim arbitrary malformed
bytecode safe. Command construction stays within positive int range; volume
arithmetic is unsigned, avoiding signed multiplication/left-shift overflow.

All callees are declared only and available in the canonical symbols. The
previously reviewed2321C and225FC sources are unchanged. These caller bodies
require no local rodata, fake float input, hidden parameter or copied callee.

## Bounded controls and source-family context

All seeds used O2 then O1. Before refinement, the unchanged workbench diagnosed
fresh relocated candidates and canonical targets held only in temporary storage.
The native frames24/64 bytes are correct.

- `232A4`: initial7/67 concentrates in curve construction/argument lowering.
  Explicit full-word low-byte extraction gives16/67; a two-step curve assembly
  gives15/67. The original complete source is retained.
- `23564`: initial73/107 plus three extras. Reordering disjoint output bitfields
  does not move it. Reading the real randomization flag early improves to66/107
  plus two extras. Narrow-local conversion/allocation residuals then stop.

Three natural forms per target. No repeated prototype, SDK-word, register, K&R,
volatile, padding, fake-formal or flag sweeps. `initial_controls.json` and
`directed_controls.json` retain hash-bound numeric outcomes and sanitized
workbench counts, without native instruction dumps.

Public CC0 MusyX `mcmdScaleVolume`/`mcmdRandomKey` provide source-family context:
[revision78d2e16](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/src/musyx/runtime/synthmacros.c),
source blob `a3203f9e0032fa5e9d08fa5e51da80acf4cab0c0`, license blob
`0e259d42c996742e9e3cba14c677129b2c1b6311`. This is not an authenticated N64 source
release. Native shift/division, identifier assembly, signed16 bounds and omitted
modern flags remain authoritative.

| Function | Final O2 | Final O1 |
|---|---:|---:|
| 232A4 |7/67|67/67 +9 extras|
| 23564 |66/107 +2 extras|107/107 +18 extras|

All rows have zero unresolved symbols, unverified relocations and errors.
Frames, numerical closeness and host tests do not create match credit.

## Tests, replay and preservation

Two strict-C89 ASan/UBSan tests compile the actual final sources:350 unsigned
scaling/curve cases and1,920 random-note cases covering fixed/relative bounds,
signed16 wrap, RNG extremes, exact outgoing command words, ordered calls and
ignored callee results. Synthetic helpers express contracts, not native bodies.
LeakSanitizer alone is disabled under ptrace; neither harness allocates heap.

```sh
python3 cloud/work/boot_tail/BT05-larger-pair/verify.py
python3 -m unittest discover -s cloud/work/boot_tail/BT05-larger-pair -p 'test_*.py' -v
```

The normal pinned IDO setup is required; `IDO_DIR` can point to an existing
verified installation. `verification.json` binds four final source/control rows.
`preflight.json` records protected hashes, all439 extents/99,120B, the existing
getter and unchanged compiler hashes. All161 static locks and whitespace pass.

A temporary byte-capacity failure prevented the first README write and Git
commit; HEAD stayed at the base. Only duplicate untracked compiler copies were
reclaimed after hash verification against the preserved pinned toolchain. All
candidate/source/receipt hashes stayed unchanged; the completed packet was then
replayed before committing. No shared source reset or object pruning occurred.

Independent source/ABI review precedes central integration and exact aggregate
CI. No matching submission, target, symbol, lock, layout, scorer, compiler,
runtime image, farm, spec or production gate is changed. Accepted800D1248 and
restricted helper work remain untouched. No ROM, raw assembly, object or
credentials are published.

Independent paired review PASS binds source commit
`fe77bfa63502ff69316415265d5899d030d6edc5`, tree
`b1da961f7feb8a9018034aaa285c7d095d0a177e`. The reviewer compiled temporary copies
of exact immutable source blobs, reproduced all four rows, inspected both full
native/source bodies and real helper ABIs, and reran both sanitizer groups.
`independent_review.json` records the complete696-byte nonmatching research;
zero matching credit remains unchanged.
