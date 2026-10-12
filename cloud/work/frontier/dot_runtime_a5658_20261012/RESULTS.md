# Image A A5658: six-entry standings renderer

## Deliverable and credit

`groups/dot_runtime_a_text_helpers_20261006/` is a same-name extension of the
installed text-helper group. `func_803A5658`, native interval
`[0x803A5658,0x803A5938)`, is **184/184 strict words, 736 new candidate bytes**.
The following 16-byte region before A5948 is not part of this function and is
not claimed.

All thirteen verified bodies are both `members` and `claims`; the two broad
nonmatching bodies A4D4 and A5948 remain genuine `context`. The group keeps
C140 and A5948. No protected production source, target, tool or lock is edited.

Preserved-only: **6,512 bytes**, consisting of the eleven installed members
(5,800 bytes) plus pending PR #333's BB80 (712 bytes). BB80's exact source
corrections are inherited unchanged and reverified, not counted again. The
prior packet's source Git blob is `a650a1d0873e48ca6c4d821c21827fe6bb57a847`.
This packet must replace the same group as a whole; the checker should reconcile
PR #333 rather than installing two overlapping groups.

**New accepted image bytes: 0. New ROM-verified bytes: 0.** This is a draft for
the independent checker, who owns installation and merging.

## Meaning and genuine ABI

A5658 draws six standings entries in two columns, choosing the current or
selected results table, formatting rank/name/count strings, and drawing the
localized unavailable message when a selected rank is outside the six entries.
Its only native direct caller is the complete A5948 renderer at offset `0x1040`.
It takes no arguments and returns no value. The native private ABI saves only
`ra` and uses s0–s8, so this is delivered in its genuine whole-program group,
not falsely advertised as a standalone o32 function. Arcade ancestry is not
established. Existing external helper declarations and live buffer capacities
are retained.

## Trace-grounded source progression

Live master `b37643eec` and open PRs were checked before reserving A5658. Both
CloudHandoff sanity commands passed. The original complete source reproduced
**32/184 differences**, with no extra words, unresolved symbols or own-data
uncertainty in A5658 itself.

1. Native code caches the address of `D_803BAD90` in s8, while the draft cached
   `D_803B9A84`. Reversing the two equality operands in this function exposes the
   selector first and corrects both address-web identity and native branch
   operand ordering: **32 → 9 differences**. No executable side effect changes.
2. A natural `for (i=0; i<count; i++)` replaces the equivalent do-loop. Stock
   ugen places the initializer and loop-header address setup at the same source
   location, which restores the three entry scheduling words: **9 → 6**.
   The intermediate same-line do-loop also reached six; the readable for-loop
   is the delivered form.
3. The remaining six differences are exclusively stack frame/home immediates.
   Stock ugen reports T=16, P=4, M=140 and frame 168 without the six disclosed
   unused scalar homes. Six four-byte declarations give M=164 and frame 192,
   exactly retail: **6 → 0**. The existing `count` home moves 52 → 76, the
   unchanged 32-byte name buffer 72 → 96, and the unchanged 64-byte number buffer
   104 → 128. This reconstructs an exact measured residual, not original local
   names or purposes. No padding array or live-buffer resize is used.

Workbench `diagnose` was run before the source corrections. Its initial
unlinked-object structural classification included expected relocation-layout
noise; strict full-word scores above, rather than the masked diagnostic, drove
acceptance. Optional `trace.py` reproduces all stock-stage frame/web/location
facts with whole-object diagnostic-on/off equality, summarized in
`frame_trace.json`.

A separate diagnostic uopt trace after the source result confirmed the cause:
in both forms, address candidates w94/w95 tie at save=2.5, nocs=8, total=20.
The earlier candidate takes the last available s8 while the other splits.
Unforced traced uopt output and symbol table matched stock on both original and
equality-swapped builds. Forcing original w94 to split and w95 to s8 produced
11 differences: the six frame words, three entry schedule words and two branch
operand-order words. It is a diagnostic oracle only; no patched compiler or
object is used by any accepted proof. The extra W5D name/PRE trace mode aborted
without output and is not evidence or a replay dependency.

## Verification

```
python3 cloud/work/frontier/dot_runtime_a5658_20261012/verify.py --score
```

This authenticates current-tree image A transiently in memory and invokes the
unchanged canonical `score.py group` without `--claims` or
`--allow-unverified`. Exact member output:

```
Members:
func_8039B120:
  MATCH
func_8039B214:
  MATCH
func_8039BE48:
  MATCH
func_8039A448:
  MATCH
func_8039B00C:
  MATCH
func_8039C140:
  MATCH
func_8039B908:
  MATCH
func_8039B988:
  MATCH
func_8039BA28:
  MATCH
func_8039BAE0:
  MATCH
func_8039B560:
  MATCH
    own .rodata verified at 0x803B9594..0x803B95B4
func_8039BB80:
  MATCH
func_803A5658:
  MATCH
```

The ordinary bare group scorer correctly refuses the unavailable standalone
image-A data artifact for B560. This packet's image authentication supplies the
existing immutable current-tree data to the same own-data checker; it does not
waive uncertainty or substitute arbitrary float globals.

```
python3 cloud/work/frontier/dot_runtime_a5658_20261012/verify.py --deep
python3 -O cloud/work/frontier/dot_runtime_a5658_20261012/verify.py --deep
python3 cloud/work/frontier/dot_runtime_a5658_20261012/trace.py
python3 -m pytest tests/cloud/test_runtime_a5658.py -o addopts='' -q
```

- All thirteen full STT_FUNC sizes, next-symbol boundaries and zero inter-body
  alignment bytes checked. A5658 is the complete 736-byte function.
- GNU independently relocates all 582 member relocations, including A5658's 45,
  at exact native addresses and compares every byte with authenticated targets.
- Existing B560's complete 32-byte float pool, GNU anchor/section placement and
  four trailing alignment bytes verified. The twelve preceding bytes belong to
  unclaimed A5948 context and are not claimed as A5658-owned data.
- Missing own-data and a corrupted owned literal are rejected.
- Actual-source causal controls: original equality 24/184, original do-loop
  3/184, missing scalar homes 6/184, and original baseline 32/184. All preserve
  complete extent and have no unresolved or unverified A5658 relocations.
- Receipts bind owned source/verifiers/test and selected target/symbol rows.
  Mutable tooling, lock hashes, and whole assembly text are not pinned.
- One cheap test uses the shared missing-tool policy. Deep GNU and stage traces
  remain explicit optional research replays.

This is source-to-native instruction/data evidence. No independent gameplay,
hardware, unrestricted string-buffer safety, private compression/image build,
full-ROM hash, or original contiguous-TU placement is asserted. No ROM bytes,
raw assembly dumps, compiler objects, credentials or unrelated data are added.
