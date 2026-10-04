# BT02 final audio initialization and command submission

Three complete **NONMATCH** reconstructions, **2,432 native bytes**. No new match
credit. All final O2 relocations, including the five external float loads and
external halfword table, resolve with the unchanged strict scorer. No unknown
retail data value was supplied. Independent source review passes on immutable source commit
9a31055d9b4de39357c162ab0106da580a822081, tree
5eb2c5d2486acd44642d157976523006102299b4. The exact-bound receipt is
independent_review.json. Aggregate replay remains required. These are research results, not cartridge coverage or promotion.

| Function | Native bytes | Final O2 differing words | O2 ELF bytes | Final O1 |
|---|---:|---:|---:|---|
| 80011104 | 840 | 62 / 210 | 840 | 206 / 210, 22 excess nonzero words, 936-byte symbol |
| 800114C0 | 904 | 199 / 226 | 904 | 200 / 226, no excess words, 904-byte symbol |
| 800139D4 | 688 | 131 / 172, one excess nonzero word | 692 | 172 / 172, 58 excess nonzero words, 928-byte symbol, relocation error |

The 139D4 O1 control has an unpaired HI16 for D_800382EC at 0x2A8 within the
native comparison boundary. It fails the canonical scorer; no scorer change or
suppression is proposed. Every nonmatch stays in this research directory.

## Claim, scope and reproduction

- Branch: dot/boot-tail-bt02-audio-final, base b5daa44a. Parent approval and
  exclusive central activation b33647f8 preceded source edits.
- Exact starts/intervals: [80011104,8001144C), [800114C0,80011848),
  [800139D4,80013C84), respectively 840/904/688 bytes.
- claim_scan.json preserves base tracking hashes and the three prior open rows.
  There was no earlier source attempt at any selected start; earlier context
  inspection is not retried matching work.
- One clean previous worktree and the shared pinned compiler were reused. Prior
  immutable commit 99e55bf7 and its branch remain preserved. No fresh full clone,
  toolchain copy or installation was required.
- Only this packet directory changes. Central STATUS, claims, publication and D10
  belong to the aggregate owner. No protected source, tool, target, lock, layout,
  symbol, shared header, runtime image, farm state or production gate changed.

At the repository root, set IDO_DIR to the existing approved IDO 5.3 directory:

```
python3 cloud/work/boot_tail/BT02-audio-final/verify.py
python3 cloud/work/boot_tail/BT02-audio-final/test_layout.py
python3 cloud/work/boot_tail/BT02-audio-final/test_host.py
```

The verifier pins all 24 compiler files and protected inventory/scorer/setup/getter
inputs, checks the complete target manifest and all 439 extents / 99,120 bytes,
and replays the existing getter as a strict exact-size 12-byte match. It records
both final and retained baseline sources at O2 and O1. ELF function sizes are
checked separately from text-section alignment. All final O2 full-function
relocations are resolved; all full-word/exact-size equality predicates are false.
Receipts hash the actual sources and test harnesses. No source is in matches/.

For metadata-only workbench replay, set MIPS_OBJDUMP to the existing GNU MIPS
objdump, set its library path if needed, and run diagnose.py. Native words and
objects exist only in temporary files; the committed receipt contains scalar
classification/count/frame metadata, not disassembly.

## Actual ABI, dependencies and complete behavior

### 80011104

The real caller 80014198 supplies an unsigned-halfword voice count and the
unsigned frequency word. The entry homes both, uses the low halfword of the
first, and converts the full unsigned second word to float. The allocator at
80038018 takes byte size and alignment and returns a pointer; registration and
other already reconstructed callers establish the same two-argument contract.
Actual static bodies at 80008590 and 80007CA0 establish the two-argument zeroing
and data-cache writeback contracts. Return values are not consumed.

The body records voice count, allocates count*104 bytes, aliases the current and
base record pointers, clears the shared byte, and initializes each record's
active/changed bytes to zero and bit index to 255. For the two mode branches it
computes frequency/50/192 or frequency/25/192, multiplies by the corresponding
external float at 8002D8B8 or 8002D8BC, converts to an unsigned word, adds one and
stores the low halfword. It allocates two blocks of this count*2576, clears two
halfword indexes, and allocates/zeroes/writes back 664 bytes of work storage.
The conversion FCSR sequence is compiler-generated unsigned conversion, not an
inline-assembly requirement. External floats remain declarations only.

### 800114C0

The same actual caller supplies count as an unsigned halfword, unsigned frequency,
and a mode byte. Native argument homes and low-byte loads independently agree.
No extra formal is introduced. The allocator is the same; actual 80006A00 is the
three-argument message-queue initializer. Native 80011F60 consumes a full count
word; 800123A8 narrows its real count to a halfword. Their existing source packets
are ABI context only and receive no duplicate credit.

Both mode branches compute frequency/50 or /25, multiply by the appropriate
external float (8002D8C0/C4), divide by 192, narrow the conversion to a halfword,
add one and multiply by count. The result is stored as a halfword. It sets the
working size to 784, retains the native lower-bound check against 192, multiplies
by the stored count and external float 8002D8C8, converts to unsigned, and rounds
through groups of 64, 40-byte groups and the native 0xFFF0 mask. The actual code
allocates count*2*12 bytes, initializes the one-message queue, clears the volatile
byte, then calls the two buffer/cache initializers. The second count comes from
an external unsigned-halfword table at 8002C5D0 indexed by the mode byte, multiplied
by the original count and narrowed. No table contents or actual literal values
were read, invented, copied or published.

The actual caller caps count at 32. Under that real contract every possible u16
table value times count fits signed int. Tests also use a larger synthetic count
with small fixture table values, which does not establish a wider real API.
Floating-conversion tests use finite in-range results. Unknown retail constants
are an explicit dependency, not a claimed recovered value.

### 800139D4

Already matched caller 80013D70 passes the output pointer and low-halfword sample
value. For every changed voice the body clears its byte and collects the indexed
bit. Active voices call 80012D18 with the actual record pointer, forwarded
halfword and sequential low-byte active index; the local index wraps at 256.
The external callee's genuine entry preserves a0 and homes a1/a2. It reads the
third input's low byte at its later use. Its second argument home is not read;
that genuine forwarded slot remains represented. The incoming a3 slot is never
consumed before overwrite: there is no fourth meaningful formal. No return value
is consumed. This establishes the call interface without reconstructing the
callee's boundary-blocked body or following any out-of-range call target.

The body re-reads shared pointers and indexes after callbacks. For an empty block,
it clears the first command status and sets its output; otherwise it writes the
last command output. It records work pointer, changed mask and optional loop.
With a loop it stores signed position/192 as a halfword, advances by 192 and
resets on exact length equality. Then it increments the halfword block index;
when another slot exists, it sets the aligned and unaligned command cursors and
clears that next block's command count.

The layout expresses observed storage: 104-byte voice records, 80-byte commands,
and a 16-byte header plus 32 command records per 2576-byte block. The previously
unknown byte arrays cover unexamined record fields, never stack padding. IDO
compile-time checks prove all used offsets, including status at block+60 and
output at block+84 for the first command, pointer fields 8/12, and commands at 16.
Host pointer widths differ; native sizes are independently tested.

Runtime validity requires correctly allocated records/blocks, counts fitting the
32-command block, changed-bit indexes below 32, and signed position addition
without overflow. External callbacks may mutate shared state. Host doubles
validate the caller's reads/writes/call arguments, not the unknown callee's
implementation or N64 hardware/interrupt behavior.

## Flags, bounded variants and stop condition

The same O2 flagset is retained for all sources:
-g0 -O2 -mips2 -G 0 -non_shared, with the scorer's -Wab,-r4300_mul.
Native loop/common-expression reuse, branch-likely selection and ordinary
call frames support O2; fixed O1 controls are preserved rather than swept.

Workbench diagnosis ran on all initial bodies before source variants and again
on final sources. Frames agree (24,32,56 bytes), but structure/register lowering
differs. No artificial local, added formal, forced register, dummy call, asm block,
helper body, target edit or context shim was used.

- 11104: baseline 66/210. Using the actual stored count for allocation and unsigned
  byte-size arithmetic reaches 62/210 with the exact 840-byte symbol. Eleven directed
  controls cover narrow local/register/prefix/assignment/do/while/cast forms,
  allocator-size signedness and a meaningful intermediate block count. No match.
- 114C0: baseline 222/226 with a 900-byte symbol. Preserving the native two-times
  twelve allocation expression reaches 199/226 and 904 bytes. Eight directed
  controls cover genuine count declaration/carrier forms and typed size/unsigned
  multiplication alternatives. A 167-word residual is rejected because its 892-byte
  symbol loses more native structure; score alone is not a faithful-source rank.
- 139D4: baseline 131/172 with a 692-byte symbol. Eight directed counter/register/
  prefix/assignment/do/while/masked-counter controls fail to improve it. Native
  narrows incremented counters in their assigned registers; the compiler inserts
  temp copies that alter subsequent scheduling/allocation.

The single malformed exploratory declaration (register accidentally applied to a
struct field by a textual probe) did not compile and was corrected without source
adoption. variants.json records the successful directed builds; the baseline
and fixed O1 controls remain separately reproducible. All hypotheses are below
the 20-variant bound. Stop this packet here: obtain new original narrow-carrier/
front-end declaration evidence before repeating those spelling families. There
is no scorer, rodata or callee-ABI blocker to hide behind.

## Host controls and their limits

Both strict C89 and ASan/UBSan configurations pass **6,408 actual-source calls**:
120 initializer cases, 6,144 setup cases and 144 submission cases. These cover
all 256 mode-table indexes, zero/small/max halfwords, unsigned frequency conversion,
two float-factor branches, four allocator calls/order and exact sizes, zero/cache
ordering, queue-before-flag-clear ordering, both downstream calls and counts,
active-index wrap, changed masks including bit 31, empty/full command branches,
loop absent/present/negative position/end wrap, and next-slot cursor/clear behavior.

Fixtures define arbitrary documented external values for testing only. A first
sanitized fixture combined a 65535 synthetic count with a large table value,
exceeding the real caller's capped-count domain and triggering signed overflow.
The fixture table was reduced to 0..255 for that out-of-contract count probe;
production source was not changed or padded to hide it. Tests do not claim
correct behavior for invalid floating conversions, invalid shift counts, signed
overflow or invalid allocations. No retail data values are inferred from a pass.

Leak detection is disabled for the established sandbox ptrace limitation.
Pointer/integer-cast warning exceptions cover the actual 32-bit target cursor
conversion on the 64-bit host; the truncated aligned cursor is observed as an
integer and never dereferenced. Address/undefined sanitizers remain enabled.

No third-party source was copied. No ROM, raw assembly, object, credential or
unrelated private data is included. Publication remains through the aggregate
owner; merging and cartridge integration remain with the independent checker.

## Independent review checkpoint

The BT06 math-trio peer replayed all twelve controls, six diagnosis checkpoints,
all pinned inputs, and both original host modes on the immutable source commit
above. It independently checked the actual caller/callee interfaces and static
helpers. No source or original test changed after review. The additional
peer_mutation.py fixture changes the voice pointer/count and block/index/work/
loop/position/limit globals inside the synthetic callback; the caller correctly
re-reads them. Both C89 and sanitized modes pass 6,409 total calls including that
one added mutation case. peer_mutation_verification.json records the harness
hash, which matches the independently reviewed fixture. Only temporary-file
paths were made repository-relative for reproducibility. The fixture makes no
claim about the real boundary-blocked callee.
