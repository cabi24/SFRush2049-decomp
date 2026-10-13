# func_8001558C: strict static match

Deliverable: [`cloud/matches/boot_tail/func_8001558C.c`](../../../matches/boot_tail/func_8001558C.c).

One function, static boot tail, **348 bytes / 87 words**, exact under stock IDO 7.1.
Source and verification base: master `55e17941390a06b2e31101b45c503d8a15fd119b`.
No other functions were changed; no targets, locks, layouts or promoted sources were edited.
This is an installable source match, not a ROM hash, gameplay or cartridge-coverage claim.
The independent checker owns merging and the coordinator owns locking/promotion/ROM gates.

```
IDO_DIR=tools/cloud/ido71 python3 tools/cloud/score.py fn \
  cloud/matches/boot_tail/func_8001558C.c func_8001558C --targets asm/us/boot_tail
func_8001558C:
  MATCH
```

The verifier selects `cc` with `tools.conveyor.compilers.tu_cc('lib_15a10', ...)`,
checks its stock `ido7.1` identity, and uses `-g0 -O2 -mips2 -G 0 -non_shared`
plus the canonical scorer's `-Wab,-r4300_mul`. The mandatory stock 5.3 getter
and resource-slot group sanity controls also passed before matching work.

## Semantic and ABI contract

The routine finds the first resource with the requested unsigned 16-bit ID.
Only resource type zero is supported. It scans 132-byte packed program records
until the 65535 sentinel, dispatches the first matching program, and returns the
handle word. An unsuitable first matching resource stops the search rather than
falling through to later resources. Disabled operation, a nonpositive signed
resource count, or a missing program returns all-one bits.

The real callers (`func_800156E8` and `func_80019490`) establish two halfword IDs,
two address words, and the fifth unsigned-byte `unlocked` argument. Zero means
lock, dispatch, unlock; nonzero means dispatch directly. The callee receives the
two bank pointers, packed MIDI setup pointer, arrangement pointer and optional
play-parameters pointer in ordinary o32, including its fifth stack argument.
The guarded path preserves the handle across unlock, whose return registers
may be overwritten.

The candidate uses the current TU's exact `ResourceGroup *D_80038398[]`
declaration and casts each entry to its fuller `ResourceHeader` access view.
The `func_800178B0` declaration uses its promoted `unsigned int` return and
Page/MidiSetup/Arr/PlayPara structure tags. The wrapper retains the signed int
return established by its callers. IDO's target conversion preserves all 32
handle bits; tests include zero, positive, `0x80000000`, `0xDEADBEEF` and
`0xFFFFFFFF`. This is a target-specific implementation-defined signed conversion,
not a claim that every C implementation chooses that behavior.

Program records are read bytewise. Odd program offsets are exercised as target
machine behavior; the promoted callee's completed MIDI type naturally has
2-byte alignment, so successful odd-address dispatch is not asserted to be
portable ISO-C behavior. Host hooks never dereference that opaque callee view.
Backing headers/tables must be valid, counts within backed storage, and program
lists sentinel-terminated. No unbounded-invalid-input or gameplay claim is made.
Original source ancestry is unknown; MusyX sequence-play is a behavior hypothesis.

## Traced source lever and disclosure

The retained old source was a genuine 4/87 near-match with exact 56-byte frame
and full 348-byte extent. Only guard/preheader setup order differed.

The final source initializes the existing counter to zero, then performs the
otherwise dead `result = id`, and enters the same loop. Every path returning
`result` first overwrites it with the dispatch result. This deliberately retained
source-shaping assignment adds no local, frame bytes, arguments, calls or memory
reads. It is not asserted to be an original-source statement.

Observed compiler evidence, captured by `trace.py`:

- Without the dead initialization, the promoted-ID expression is PRE bit 15,
  after the table-address expression (bit 10). Both are inserted in preheader
  node 2; ugen emits table-base setup before the ID copy.
- With the initialization, ID promotion is bit 8 and the table address is bit 16.
  PRE still inserts in node 2. Ugen emits the ID copy first, and stock as1 moves
  it into the count-guard setup, giving the native final order.
- A compiled-out `if (id != 0) {}` before the loop moves the promotion into an
  earlier block but leaves counter/copy order swapped: exactly 2/87 differences.
- A one-operation diagnostic oracle moving only the baseline promotion's PRE
  insertion from node 2 to node 1 also gives zero differences. Oracle objects
  are not used as source evidence or submitted artifacts.

The causal interpretation is that the earlier expression numbering changes
preheader emission order and therefore the scheduler's eligible instruction.
The stock 4/2/0 comparisons and observed IR/listing order establish the concrete
changes; no general rule for every IDO function is claimed. Trace-on, trace-off
and stock optimized ucode, binary assembler input and ELF objects are byte-identical
for each of the three source controls. No forced compiler generated the deliverable.
The initial boolean-only dead read failed because it did not create the same
integer-promotion expression. An explicit early 65535 comparison introduced an
unwanted constant web. Splitting the counter init across an empty condition
lengthened its live range and disrupted strength reduction. Those rejected
source-shape diagnostics were not retained as alternate deliverables.

## Verification

`verification.json` is protocol v2: it binds the source, verifier, behavior tests,
trace driver/summary and cheap test, plus the exact target word hashes/extent
and relevant symbol addresses. It does not pin mutable pipeline tools, locks,
Makefile or assembler text. `claims` contains only this new function.

- Canonical strict comparison: **0/87**, no unresolved/unverified references,
  errors or excess nonzero words.
- Independent GNU link: all **10 original ELF relocations** resolved at native
  symbol addresses; exact **348-byte STT_FUNC**, **352-byte .text**, exactly
  **4 zero alignment bytes**, no allocated owned data. GNU uses `SUBALIGN(4)`
  because this native function begins at an address that is not 16-byte aligned.
- IDO layout assertions verify header size 40, program stride 132, ID/type at
  +4/+6, and relative bank/program offsets at +28/+32/+36.
- Official context fit and temporary full-TU splice with `--prefix-dedup
  rom/lib_15a10`: new function strict, all **22 existing defined functions**
  remain strict. This includes every compiled body preserved in that TU.
  No repository TU is modified by the replay.
- Bounded native and GNU-linked MIPS-II replay: **1,152 fixtures each**, **2,304
  executions** total, varied signed counts, high IDs/home-slot bits, packed
  alignment, sentinels, first hits, callback return bits and guard order.
  Every o32 volatile general register is clobbered by hooks; callee-saved
  registers and stack restoration are checked.
- Dynamic instruction coverage is **86/87** for each body. The +0x11c duplicate
  first-byte load has no incoming execution path: branch-likely supplies the
  load in its delay slot and both successful-dispatch paths leave the loop.
  All **87** words, including that unreachable instruction, are covered by
  full static byte equality.
- Actual-source C89 host runs at O0, O2 and ASan/UBSan/bounds each pass **2,400
  cases** (7,200 total). LeakSanitizer is disabled because of executor ptrace;
  allocation/leak behavior is not relevant to this allocation-free harness.
- Removing the dead initialization is rejected at 4/87; early promoted read at
  2/87. A wrong bank offset is rejected by strict comparison and machine ABI
  fixtures. Unknown machine opcodes fail closed.

Run from the current repository root with IDO 5.3/7.1 and MIPS GNU binutils:

```
python3 cloud/work/frontier/dot_static_1558c_20261013/verify.py --deep
python3 -m pytest tests/cloud/test_static_1558c_match.py -q
```

One inexpensive default test only; full behavioral/compiler experiments remain
opt-in. Missing IDO is handled by the repository's shared test skip policy.
The deep TU splice requires the still-unpromoted slot; after coordinator landing,
use the current repository's lock/ROM verification workflow for that installed TU.

Optional compiler-causality replay (local, no network/download): provide a v1.2
`ido-static-recomp` checkout after `make setup`, or an existing PRE/CDX trace build:

```
python3 cloud/work/frontier/dot_static_1558c_20261013/trace.py \
  --recompiler /path/to/ido-static-recomp --out build/1558c-trace
# or: --trace-ido /path/to/pre71/ido --out build/1558c-trace
```

The script uses the already committed 7.1 trace/PRE patches, verifies fidelity,
and writes a safe summary plus local instruction-bearing scratch files. Only
`trace_summary.json` is published. ROM bytes, raw assembly/ucode, compiler binaries,
objects, credentials and private data are not included.
