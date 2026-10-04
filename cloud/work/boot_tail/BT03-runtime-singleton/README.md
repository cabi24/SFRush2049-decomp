# BT03 runtime singleton: timed sequence-track dispatch

**COMPLETE-NONMATCH: one function,840 B; zero matching submissions.**
`func_80018634`, exact interval `[0x80018634,0x8001897C)`, is reconstructed
with its real `u8 func_80018634(void)` ABI. The full relocated O2 comparison
is **117/210 differing words**, no excess nonzero words, no unresolved or
unverified relocations, and an exact840-byte ELF function symbol. O1 is
210/210 with136 excess nonzero words and a1396-byte ELF function symbol.
No matching-byte or cartridge-coverage credit is claimed.

Claim activation: central `f9bd8dc1`, packet `BT03-runtime-singleton`, branch
`dot/boot-tail-bt03-runtime-singleton`, source base `1bf59758`. The clean former
BT02 worktree was reused, preserving the final audio packet and shared tools.
Central owns STATUS, claims and D10 updates; this packet changes only its own
research subtree.

## Full native reconstruction and ABI

The function visits64 sixteen-byte track records at context+0x128. A nonnull
cursor sets the returned activity byte even if that track terminates during
this call. Each12-byte event has an unsigned32-bit time, program/controller
bytes, a16-bit code, and a two-byte payload interpreted either as a big-endian
jump index or two signed parameters. The body has no indirect jump and calls
only17720,17824 and18B3C. Caller198C8 supplies no inputs and explicitly masks
the returned value to eight bits.

For each active track, events are due when the unsigned wrapped sum of its
whole time and context lookahead is at least the event time. AFFFF event
clears the cursor and skips clock advancement. FFFE either terminates when
stop_loop is set or jumps relative to the track's own event base, resets its
whole time to the sequence header's loop time and clears its fraction.
Only the first restart in the entire invocation calls18B3C(loop_time) and
increments the live context's16-bit restart count. Subsequent restarts still
reset their tracks but do not repeat that callback or increment.

Ordinary events select a pattern via two relative offsets from the sequence
header. The function fully initializes the corresponding40-byte playback
record at context+0x568: counters, data cursor, two nullable resource pointers,
0x2000/0 halfword values, mapped channel, two signed parameter bytes and the
track index. Program and controller callbacks are independently skipped for
FF. The former receives(context,program,mapped_channel), while17824 receives
(controller,mapped_channel). The cursor advances only after the callbacks.
The next event is reread and processed in the same invocation if still due.

All global-context reloads are retained. In particular a callback may replace
D_8004BE80: later channel reads/cursor updates and the restart-count increment
must use the replacement context, while the current event pointer remains
saved. Pointer identities and the true helper argument counts are preserved.
Reviewed17720 and17824 are ABI evidence, not matched callees or corpus credit;
18B3C is independently reconstructed elsewhere. No helper body is copied here.

When an event is pending, the track clock advances exactly once. The unsigned
fraction sum wraps to32 bits, its low16 bits are stored, and the logical high16
bits plus the whole step advance the whole clock. The native unsigned horizon
wrap is preserved. Tests do not promise termination or safety for malformed
resources, cyclic immediate restarts, invalid pattern indices, unaligned
pointers or callbacks that leave an invalid current cursor. The valid contract
requires bounded mapped resources and a terminating/future event on each
processed chain. No arbitrary retail data is included.

## Bounded source work and residual

The initial whole-body source already produced exactly210 instructions at O2.
Unchanged workbench diagnosis ran before refinement. Six directed forms were
then measured, including the initial control:

1. Initial reconstruction:121 words,72-byte frame.
2. Declaration/initialization ordering for the two genuine activity flags:
   117 words,72-byte frame. This is the retained source.
3. Reusing the genuine jump/carry scalar:120 words.
4. Offset-first pointer addition spelling:120 words.
5. Explicitly decoding the real16-bit event code:120 words.
6. Explicitly decoding the real optional relative offsets:118 words.

`replay_variants.py` reproduces all six from the retained initial control and
checks every source hash, strict residual, ELF size and frame against
`variants.json`. None adds a padding local, unused keeper, false formal,
forced register, volatile operation, assembly or dummy call.

The final workbench result is **allocation-mismatch**, with zero aligned
insertions/deletions,115 register differences and two frame constants. Native
and candidate frames are88 and72 bytes respectively; the saved registers,
activity-byte home, instruction count, branch destinations and operation
sequence align. This is not a match. The diagnosis's CFE/stack-home ownership
is heuristic. No unexplained local storage is invented to force the frame.

**Next hypothesis:** seek independently supported original declaration/type
or translation-unit context that explains the larger genuine frame and extra
colored register web. Then reproduce that evidence with the current strict
scorer. Merely increasing a declaration count is not evidence or an authorized
matching technique. No further blind flag/declaration sweep is warranted.

## Reproducible verification

Use the pinned IDO5.3 directory via IDO_DIR, the shared MIPS objdump via
MIPS_OBJDUMP and its normal library path via LD_LIBRARY_PATH. Paths are local
configuration, not required source changes.

```sh
python3 cloud/work/boot_tail/BT03-runtime-singleton/verify.py
python3 cloud/work/boot_tail/BT03-runtime-singleton/replay_variants.py
python3 cloud/work/boot_tail/BT03-runtime-singleton/diagnose.py
python3 cloud/work/boot_tail/BT03-runtime-singleton/test_layout.py
python3 cloud/work/boot_tail/BT03-runtime-singleton/test_host.py
python3 cloud/work/boot_tail/BT03-runtime-singleton/test_native.py
```

- Pinned compiler hashes, protected manifest,439 extents/99,120 B and the
  existing getter's strict exact12-byte match pass. Target/scorer contents
  remain unchanged. O2 and O1 are both retained, not selectively hidden.
-43 IDO compile-time layout checks establish real32-bit sizes and offsets.
  The unknown byte spans represent actual unexamined context fields.
- Host tests compile the actual source in strict C89 with warnings as errors,
  then with ASan/UBSan.26 synthetic scenarios plus two inactive follow-through
  checks cover all64 tracks, optional-pointer/callback combinations, signed
  parameters, unsigned clock wrap, restart-count wrap and global replacement.
  Whole-context comparisons detect unintended writes. Wider host pointers
  model behavior; they do not independently establish O32 offsets.
-42 native/candidate replay scenarios execute the protected target words and
  freshly relocated compiled C against independently constructed complete
  expected memory snapshots. These include multiple immediately due events,
  termination without ticking and wrapped lookahead. The bounded MIPS-II
  interpreter fails closed on unknown instructions, bad alignment, unmapped
  memory or excessive steps; poisons caller-clobbered registers after each
  synthetic helper; and checks callee-saved registers, stack restoration and
  uninitialized stack reads. It is a focused integer execution check, not a
  full N64/game integration test. Native words/objects exist only temporarily.

No target, scorer, header, symbol, layout, lock or pipeline changes are made.
No ROM, raw native dump, object, credential or unrelated private data is
published. func_800D1248/helper work and specT050-gated large bodies are untouched.

## Frozen-source independent review

The independent chain-dispatch reviewer approved research-only integration of
source commit `6a2d716b33a8361269404fb956b18d3d44bf8317`, tree
`f436515c6608217733a9e604ba33fb20974547b5`. `independent_review.json` binds the
reviewed source and all original packet files to that exact commit. The reviewer
reread the full native body, caller and three helper ABIs and independently
replayed all six scripts without a source change.

Four additional native/candidate whole-memory cases pass: a program callback
changes the saved event's controller byte; a controller-only callback replaces
the live context and cursor; a big-endian jump index of 256 is interpreted as a
halfword; and two tracks restart then terminate without ticking, with only one
restart callback. The unchanged reviewer script is retained as
`peer_additional.py`; its SHA-256 is pinned in the review receipt.

```sh
python3 cloud/work/boot_tail/BT03-runtime-singleton/peer_additional.py .
```

The original C source remains frozen at SHA-256
`874ff77bec17f4f1cc25b6a4ec939497a8180c932541106bb121f742dac5211b`.
Review adds no matching credit; aggregate-head CI and publication remain
separate central checks.
