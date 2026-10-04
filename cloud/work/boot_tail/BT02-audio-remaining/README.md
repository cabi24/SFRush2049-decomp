# BT02 audio remainder: ring consumer and setup wrapper

Two complete bounded **NONMATCH** reconstructions, **1,256 native bytes**.
There is **no new matching credit**. All final O2 references resolve under the
unchanged strict scorer, but neither ELF function extent equals its native extent.
Independent source/native/ABI review passes for research only on
`f0a047bb58b25076c90e63364cbcee27367f03e4`; the hash-bound receipt is
`independent_review.json`. No source or test changed after that review.

| Function | Native bytes | Final O2 differing words | O2 ELF bytes | Final O1 |
|---|---:|---:|---:|---|
| `80013DEC` | 780 | 164 / 195; 10 nonzero excess words | 828 | 191 / 195; 21 nonzero excess words; relocation error; 884-byte symbol |
| `80014198` | 476 | 13 / 119; zero nonzero excess words | 480 | 111 / 119; 8 nonzero excess words; relocation error; 508-byte symbol |

`14198`'s extra final word is zero, but the actual ELF function is still four
bytes too long. Zero nonzero-excess words is deliberately **not** accepted as
an exact extent. The O1 errors are retained: unpaired HI16 for `D_800382D0`
at offset `0x308` in `13DEC`, and for `D_8004F810` at offset `0x1CC` in
`14198`. Those controls fail the canonical gate; no scorer change is proposed.

## Claim and inputs

- Branch `dot/boot-tail-bt02-audio-remaining`, base
  `96b9dd1979f7c797f97a1dd2317fc0852092c0dc`.
- Exact targets are `[80013DEC,800140F8)` and `[80014198,80014374)`.
  Parent activation preceded source edits; central activation is `9a542435`,
  with the exact base corrected in `c3b07951`.
- `claim_scan.json` hashes central STATUS, claims, all live claims and prior
  status deltas. Both rows were open with no exact claim/delta hit and no
  prior source attempt. Earlier context-only inspection is not matching work.
- Reused one clean prior BT02 worktree. Its frozen audio-pair branch remains
  preserved. The existing pinned compiler and binutils were reused.
- The input replay verifies all pinned compiler hashes, protected source hashes,
  the complete target manifest, all 439 inventory/extent pairs (99,120 bytes),
  and a strict replay of the existing 12-byte getter. That getter earns no credit.

Only this packet directory changes. No edits to earlier packets, central
STATUS/claims/D10, targets, tools, locks, layout, symbols, shared types, runtime
images, farm state, or cartridge-production gates are included.

## Actual native ABI and complete behavior

### 80013DEC

This is the no-argument callback registered by already reconstructed `800140F8`.
The actual native body consumes no incoming arguments. Optional callbacks at
`80038024` and `80038020` are likewise invoked without arguments. The actual
static bodies in `asm/us/D0B0.s` prove `__osDisableInt(void)` returning the
interrupt-enable word and `__osRestoreInt(unsigned int)` consuming it.
The word is preserved across the native ring-index snapshot and restored before
other work. No hardware implementation is embedded in the candidate.

The shared published index at `80038288` is volatile, consistent both with its
native repeated loads and the previously matched producer `80010A40`.
The zero published index means the last ring slot; otherwise the target is the
published index minus one, narrowed to 16 bits. Consumer index `80038360` uses
`0xFFFF` as its uninitialized sentinel. A sentinel call only records the target,
then releases access. Normal calls prepare audio state, traverse a straight or
wrapped interval up to the current `800382CE` budget, and call actual
`80013D70(unsigned short index, unsigned short offset)` once per slot.
Index, 192-byte offset and emitted count have the native 16-bit wrap semantics.
The returned integer is ignored. A wrapped traversal still resets its local
index to zero when the first section consumes the whole budget, matching the
actual body rather than an idealized ring abstraction.

After submission it subtracts the emitted count from the signed-halfword pending
count and stores the stopping index. It then selects the opposite entry of the
two buffer-pointer and count arrays, submits those buffers, flips the active
byte, and performs the optional final runtime service. Every native call and
conditional is present, including the preparation path when no slots changed.
The native bodies of `14624`, `14650`, `1C3CC`, `1C508`, and `1DDE0` were checked
for entry arguments; all these calls consume none. The already reconstructed
small BT02 callees agree with the declarations. No blocked callee or out-of-text
helper was investigated.

Valid runtime input requires a positive ring capacity, a published index in
range, a prior consumer index in range or the sentinel, two valid buffer slots,
and valid implementations of the declared service callbacks. The known producer
uses a small ring. Large-ring host probes isolate arithmetic and wrap behavior;
they do not prove the real producer's table has 65,535 entries. Signed-halfword
assignment on underflow follows the native two's-complement truncation.

### 80014198

Both actual callers, `143C0` and `14434`, agree with four real formals:
a pointer to the unsigned frequency word, two unsigned halfwords, and one
unsigned byte. Native argument homes and low-byte/halfword loads independently
confirm them. Count is capped at 32. The initialization calls are, in order:
`11104(count, *frequency)`, `114C0(count, *frequency, mode)`,
`10DD8(*frequency, size)`, and `140F8()`.
The actual `11104` entry homes/narrows the first formal and uses the full
unsigned second word. The actual `114C0` entry narrows the first formal and
uses the third formal's low byte at its later table lookup. Their external
float constants are not needed, read, invented, or copied to prove this ABI.
The earlier complete `10DD8` reconstruction agrees with its actual native inputs.

The wrapper clears the shared volatile flag, computes per-frame samples from the
current frequency, rounds the 192-sample block count with `+0.5f`, initializes
its two counters and sample total, and writes frequency/mode/sample-bit fields
at offsets 0/4/5 of the output record. A signed-byte copy uses `8002D890` as the
termination test and **distinct** `8002D8A4` as the byte source. These distinct
addresses are preserved, without assuming the actual strings are equal.
The output starts at record offset `0x106`, and the byte cursor wraps at 256.

Every floating literal in this wrapper is immediate in the native body (60.0,
192.0 and 0.5); no local literal/table relocation is fabricated. External byte
arrays are retained as arrays with symbolic loads; their contents are not needed
for body reconstruction. The typed record names only accessed fields and the
observed unaccessed span. Its 256-byte text view covers exactly the byte-indexed
access domain, not a claim about the original declaration. Pinned IDO assertions
check native widths and all consumed field offsets.

Valid input requires a readable frequency word, valid external services, a zero
terminator within the first 256 bytes of the test array, readable source bytes
through that position, and writable output storage. A source byte may itself be
zero because termination comes from the other array. Missing termination retains
the native wrap/nontermination and is not silently made safe.

## Flags, diagnosis and bounded controls

Primary flags are `-g0 -O2 -mips2 -G 0 -non_shared`; the unchanged scorer adds
`-Wab,-r4300_mul`. Native common-expression reuse, saved loop carriers and
branch-likely paths support O2. O1 was tested on both baselines and final sources;
it is larger, worse and rejected as recorded above. No blind flag sweep occurred.

Workbench diagnosis ran before refinements. The first `13DEC` body had an
oversized 56-byte frame. Native volatile access, genuine cursor/target lifetime
and postincrement-call variants recover the native 48-byte frame, but its final
body is still 12 instructions too long. Workbench reports structure mismatch,
17 aligned insertions, 5 deletions, 49 register differences and no constant
mismatch. Its exact compiler-pass cause is unknown; this is not a near-zero match.

For `14198`, actual signed byte types, flag volatility, the consumed float
intermediate and observed store order reduce 97 differing words to 13. Its frame
is exactly 32 bytes. Workbench finds one inserted instruction, six aligned
register differences, one structural difference and no constant mismatch.
The byte-cursor reduction uses a separate temporary and copy instead of the
native same-carrier mask; subsequent offsets and registers shift. Exact pass
ownership is not established by the heuristic report.

`variants.json` and `controls/` retain **12 directed refinements for 13DEC** and
**15 for 14198**, plus both initial baselines. Trials test actual narrow/wide
carrier forms, masked arithmetic, pre/postincrement placement, cursor lifetime,
consumed pointer/byte temporaries, loop form and declaration order. Equivalent
byte-loop spellings plateau. C89 `register` probes are ordinary declarations,
not forced-register variables, and did not help. No fake formals, padding locals,
dummy calls, assembly, manual unrolling, target edits or context bodies were used.
The final sources remain honest complete research. A next attempt needs measured
frontend/allocator evidence explaining the narrow-carrier copy web; merely
replaying these spellings is not a new hypothesis.

## Reproduction and tests

With `IDO_DIR` pointing at the existing pinned compiler:

```sh
python3 cloud/work/boot_tail/BT02-audio-remaining/verify.py
python3 cloud/work/boot_tail/BT02-audio-remaining/test_layout.py
python3 cloud/work/boot_tail/BT02-audio-remaining/test_host.py
```

`verify.py` replays every control, exact symbol length, O2 final and fixed O1
control, all pins, target census, and the historical getter. `diagnose.py` uses
`MIPS_OBJDUMP` and its adjacent GNU assembler, with its runtime library path if
needed. Temporary raw words/objects never become packet files.

The actual final sources are compiled as separate translation units under strict
host C89 and again under ASan/UBSan. Both runs pass **36,931 ring cases** and
**18,433 setup cases**. Ring tests cover all small straight/wrapped/equal/sentinel
cases, budgets including zero, both buffer slots, every optional callback/service
combination, exact call ordering and arguments, pending-count truncation, and
large offset-wrap probes. Setup tests cover clamp boundaries, every string length
0–255, arbitrary signed source bytes including internal zeros, preserved record
storage, high-bit frequencies, rounding thresholds and full-byte mode/halfword
size values. An additional case changes the frequency through mocked initialization
services and verifies the wrapper reloads it at each actual use.

These host tests verify reconstructed source behavior with controlled external
services; they do not execute hardware interrupts or the original machine code.
Host pointer width and endianness differ. Pinned native layout and strict score
results are separate proof. ASan leak detection is disabled for the existing
ptrace-runtime limitation, not to suppress memory-access diagnostics.

No third-party implementation is copied. No ROM/image bytes, raw native dumps,
objects, credentials or unrelated private data are included. No cartridge
coverage, promotion, maintainer acceptance or merge is claimed.
