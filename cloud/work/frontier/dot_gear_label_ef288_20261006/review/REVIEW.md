# Independent EF288 source and behavior review

Result: pass for the bounded behavior reconstruction. This is not a matching,
original-source, or complete private-ABI closure claim.

Reviewed base: `6b2e9e506fe3d2267a710e41c85af5364ccd00c7`.
Function: `func_800EF288`, 808 bytes, 202 words.
Frozen candidate SHA-256:
`a7ef198e3c87750594e19658e97bab7ddf135053a3475bf1d7fd525b15c4e02d`.
The review copy of candidate.c is byte-for-byte unchanged from the producer.

## Findings

- The callback is an ordinary entry: the native prologue/epilogue saves and
  restores s0-s7 and s8, its stack, and return address. Its first argument is
  homed but otherwise unobserved. The source's u32 formal models one uninterpreted
  ABI slot; it does not establish original signedness or a pointer type.
- Slot selection is a private boundary: native selection 10, 11, or 12 enters
  slot_state_setup in s2, and the selector may overwrite s0, s1, and s3. Its
  called bank-refresh path has further nonstandard scratch-register contracts.
  The callback's queue pointer in s4 must survive. The independent fixture
  clobbers s0/s1/s3 at this hook and all ordinary caller scratch at every hook;
  argument-home memory and f0-f19 are clobbered too.
- The source uses genuine selector semantics. Its context exactly matches
  `git show <base>:cloud/work/dot_selector_d9058_20261006/context.c` after moving
  the exact O3 header to line 1. No stale dead-switch or empty-read scaffold was
  substituted. This does not make the context matching or prove its TU boundary.
- Player count, enable, hidden, gear, car index, and car-kind access widths agree
  with native loads. Model stride is 0x808, car stride 0x3B8, gear +0x730,
  hidden +0x0A, car index +0x7C6, car kind +0xEF. The host fixture statically
  asserts those observed scalar layouts. Complete original capacities and types
  remain unproved. Host pointer fields use native host pointer width intentionally.
- X is loaded as a 32-bit word then offset by minus/plus four and narrowed to s16;
  Y is read as a signed halfword at position +6. Draw arguments are sign-extended
  s16 values. Width arithmetic deliberately wraps at 32 bits before narrowing.
- Gear is captured before per-slot gating and remains a snapshot across callbacks.
  The label pointer is loaded afresh after each width callback and after color
  callbacks. Glyph positions and player count are read again after the label pair.
  Loop bounds reload after a rendered slot; skipped slots contain no callbacks.
- Rendering is ordered: render state 0, lock/select/unlock, label shadow color 0
  with +1/+1, foreground color 1, glyph shadow color 0 with +1/+1, foreground
  color 14, render state -1, return 1. Glyph byte is R for -1, N for zero, otherwise
  narrowed gear+'0', followed by a NUL byte. Arbitrary signed gear bytes are not
  silently limited to decimal digits in the tests.

## Independent tests

7,281 additional host-C/native contract cases passed:

- 440 signed count/enable/coordinate edge cases
- 1,536 hidden/type mask and signed-byte cases
- 256 cases covering all gear byte values
- 2,024 callback mutation cases
- 24 transitions to nonpositive counts at valid callback boundaries
- 3,000 deterministic random cases, plus one baseline

Mutations separately change count, localized table entry, outer localization
pointer, gear, positions, hidden, type, and global enable at callback boundaries.
All 202 native instruction words were reached. Both outcomes were reached for
all meaningful conditional branches; the redundant inner loop guard cannot be
true after the preceding identical loop test without an intervening callback.
Unconditional branches naturally have one outcome. Full sets are in review.json.

A separate ASan/UBSan host executable passed 10,000 source cases. No target
compilation or tuning was performed by this review. The producer separately owns
its canonical O3 measurement and compiled-contract replay.

Eight negative controls were rejected:

1. Reload gear after callbacks instead of retaining its snapshot.
2. Cache the localized pointer across a width callback.
3. Omit the glyph position-row reload.
4. Read gear as unsigned.
5. Cache the loop count across rendering callbacks.
6. Use a0 rather than native s2 for the selector.
7. Present an unsupported opcode to the native interpreter.
8. Truncate the native extent.

The five deliberately wrong sources were host-only controls in tmp; none altered
or replaced the frozen candidate. Results are in negative_controls.json.

## Reproduction and limits

This review uses the producer's fail-closed MIPS engine after independent source
inspection, with independently written memory initialization and callback hooks.
It is not a second independent emulator. Native words are loaded at replay time
through the unchanged canonical scorer; no native words are embedded in the
publishable review files. Test hooks do not execute queues, font loading, selector
internals, width internals, drawing, scheduling, or the game. These finite cases
are not unrestricted equivalence or proof of arbitrary index validity.

Run with an unchanged canonical tool/target root. Host artifacts and regenerated
receipts go to a writable workspace directory, outside this source-only packet:

```
export TMPDIR=/absolute/writable/workspace/tmp
export RUSH_GEAR_REVIEW_WORK="$TMPDIR/gear-source-review"
mkdir -p "$RUSH_GEAR_REVIEW_WORK"
REVIEW=/absolute/repository/cloud/work/frontier/dot_gear_label_ef288_20261006/review
cc -std=c99 -O2 -fPIC -shared "$REVIEW/host_audit.c" -o "$RUSH_GEAR_REVIEW_WORK/host_audit.so"
python3 "$REVIEW/audit.py" --source-root /absolute/tool/root
cc -std=c99 -O1 -g -fsanitize=address,undefined -DAUDIT_MAIN "$REVIEW/host_audit.c" -o "$RUSH_GEAR_REVIEW_WORK/sanitized"
ASAN_OPTIONS=detect_leaks=0 "$RUSH_GEAR_REVIEW_WORK/sanitized"
python3 "$REVIEW/negative.py" /absolute/tool/root
```

`RUSH_REVIEW_OUTPUT` optionally redirects regenerated JSON independently of the
scratch directory. It defaults to `RUSH_GEAR_REVIEW_WORK`. The historical receipt
in this directory is unchanged; the batch bindings also pin the actual review
verifier and host harness. The reviewed interpreter snapshot differs from the
producer's snapshot only in the producer selector scratch-register list; this
review replaces that hook with its independent `AuditMachine.hook` in either
case. Both snapshots are preserved, without claiming byte identity between them.

The source-root and review directory names above are local examples, not required
repository destinations. review.json binds only the reviewed packet source and
native target, never live integration manifests, locks, scorer, or production C.
No protected repository edits, external writes, publication, or CI monitoring
were performed. Exclude tmp, audit.log, and __pycache__ from any later packet;
tmp contains private target extracts and compiled diagnostic artifacts.
