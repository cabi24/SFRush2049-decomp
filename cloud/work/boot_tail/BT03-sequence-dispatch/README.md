# BT03-sequence-dispatch: complete sequence transition reconstruction

**COMPLETE-NONMATCH, not a matching submission.** `func_80019490` is 960 bytes,
`[0x80019490, 0x80019850)`. The best complete natural C differs in **109/240
strict relocated words**, with an actual 960-byte ELF function and the native
88-byte frame. There are no unresolved/unverified relocations, no extra nonzero
words, and no target/scorer/layout changes. No bytes are credited as matched.

- Starting commit: `cc50ad283107ee38e2eb11b6aea4e2e43bb80490`
- Central exclusive claim: `7567da75`, BT03-sequence-dispatch
- Worker branch: `dot/boot-tail-bt03-sequence-dispatch`
- Source: `nonmatch/func_80019490.c`
- Only this packet-local directory changes; central owns ledgers/publication.
- Current task explicitly leaves `func_800D1248`, its helper investigation, and
  T050-gated bodies untouched. The stale broader permission in `dot_response`
  does not broaden this packet.

## Native and ABI evidence

Both callers and the complete native target were inspected before activation.
`func_80019850` is an existing matched gated wrapper, passing `(state, result,
0)`. The call site in `func_80017D38` passes `(context + 0xFC8,
context->pendingResult, 1)`. No indirect jump, local table, or literal pool occurs
in this function. Its only global relocation is the known context array
`D_80043EB8`.

The three genuine formals are a request pointer, a 32-bit result pointer, and an
unsigned byte selecting unlocked/direct helpers versus gated wrappers. The return
value is unused and the native exit does not establish a value: source is void.

Eleven direct helper contracts were checked against native code and existing
reviewed C: translator `17644`; value/duration dispatcher `19194` and gated
`19370`; resume `18FEC` and gated `1906C`; pair setter `190AC` and gated `19144`;
16-bit value setter `18F20` and gated `18FA4`; resource/program constructor
`1558C` and its four-argument wrapper `156E8`. The latter passes an explicit
fifth byte zero, while direct construction here passes one. `1558C` is complete
research rather than a match; no matching claim depends on changing its ABI.
The reviewed `19194` source was inspected at upstream packet commit
`9543ec48f08da3577f72fb77496a83d33f0a4607` (central wave2 checkout), not invented
from a call-site guess.

### Reconstructed object fields

These are offset descriptions, not original middleware names.

- Request: 40 bytes. Identifier 0; duration 4; next identifier 8; next duration
  12; stream pointer 16; group/program 20/22; channel 24; pair 28/32; 16-bit
  value 36; flags 38. The native block copy includes all 40 bytes, including
  presently unexplained object bytes. Those object bytes are represented
  explicitly; they are not dummy frame locals.
- Context: 4088-byte stride. Request at `FC8`; its flags therefore occupy `FEE`;
  saved result pointer `FF0`; pending marker `FF4`.
- Constructor options: **32 genuine bytes**, proved from the constructor
  consumer `178B0`: flags 0, pair 4/8, value 12, duration 14, channel 16,
  map count 18, map pointer 20, channel count 24, channel pointer 28.
  Consumer inspection was read-only ABI research, not a new claim on that body.
- Translator `17644` returns `FFFFFFFF` or a valid context index 0..7 with the
  identifier's high tag retained. The final source casts the multiplied byte
  offset to `u32` before adding it to a byte pointer. Since the context stride
  is even, multiplication removes the high tag modulo 2^32 before pointer
  arithmetic. It never uses the tagged word as a direct C array subscript.

## Behavior reconstructed

1. Invalid initial identifier: no writes or subsequent calls.
2. Request flag 4: copy the whole request into its context, clear that flag in
   the stored copy, set pending, retain the result pointer, and return the
   original identifier with its high tag set through the result pointer.
   This path requires a valid non-null result pointer, as does native code.
3. Otherwise service the old identifier, selecting mode 2 for bit 1, mode 3 for
   bit 64, and mode 1 otherwise. A null result ends after that service call.
4. Bit 2 selects an existing next identifier. Translate it; on failure write
   `FFFFFFFF`; otherwise resume it, set channel/duration, apply optional pair
   and value setters, and return its current identifier through the result.
5. Otherwise construct a new sequence. Options flags always contain 4, add 16
   for request bit 8, 2 for bit 32, and 1 for bit 16. Initialize the pair/value
   only when enabled, always set duration/channel, and set channel count zero.
   Map fields are not read because option bit 8 is absent; channel pointer is
   not read because its count is zero. No invented initialization is added.
6. Successful construction with request bit 128 calls the pair setter with
   zero/zero. Flags and relevant fields are reloaded after helper calls, as the
   native code requires when helpers change the request.

## Bounded source search and residual

Exactly **21 source forms** were compiled: one complete initial form plus
**20 directed controls**. The final is the best control (`byte_offset`) with
explanatory comments only; it is not an additional source-form experiment.
`replay_variants.py` replays every retained control and records its hash.

Per-hypothesis counts, totaling 20:

- Condition topology: 5
- Pointer/index topology: 4
- Declaration scope: 2
- Integer representation: 4
- Identifier-only negative control: 1
- Statement layout/store order: 2
- Aggregate copy form: 1
- Result-value carrier: 1

The initial O2 source has 137/240 differing words, exact 960-byte extent and
88-byte frame. The byte-offset expression improves that to 109 and reproduces
the early branch scheduling. Direct masked-index forms add instructions and
are rejected. The library-copy control is also rejected; no control is silently
promoted on a scalar score. No ABI trick, fake keeper, extra formal, explicit
unroll, inline assembly, or synthetic local padding is used in the retained C.

O2/O1 are separate flag replays, not additional source forms: both initial and
final were freshly compiled at both levels. O1 is 239/240 differing words,
1156-byte ELF extent, and 43 extra nonzero words. Native branch-likely control,
CSE, and saved-register usage support O2. The unchanged scorer always adds
`-Wab,-r4300_mul` to `-g0 -O2 -mips2 -G 0 -non_shared`.

Workbench diagnosis was run before directed tuning and again on the final.
The final has matching frame/count but five aligned insertions/deletions,
99 aligned register differences, and 14 aligned structural differences.
Routing is `structural`, owning pass `cfe-spelling`, **heuristic** evidence.
Residuals concentrate around native aggregate-copy temporary allocation and
constructor-result reload/control scheduling, with downstream temporary-ring
phase differences. This is not proven to be an impossible compiler shape.
No further tuning is justified within this packet's measured bound. A future
attempt should first obtain independent source/translation-unit evidence for
the aggregate-copy and result-value expression topology, rather than more
unmeasured register or declaration sweeps.

## Verification

All reports are generated from the retained source and unchanged targets:

- `verification.json`: all target hashes pass; all 439 starts/sizes agree with
  the inventory (99,120 bytes); known getter strict exact match; pinned 24-file
  IDO toolchain; fresh initial/final O2/O1 comparisons and actual ELF extents.
- `layout_verification.json`: 29 compile-time IDO O32 size/offset assertions.
- `native_verification.json`: 3,142 independently expected cases, each executed
  against canonical target words and freshly relocated compiled C (6,284
  executions). Covers all 256 flags, byte values 0/1/255, tagged and plain IDs,
  null results where valid, both translator failures, creation failure, result
  aliases, helper-mutated flags/identifiers, and embedded context requests.
  Helpers poison caller-saved registers; stack reads are tracked; no uninitialized
  stack read is accepted, including guarded constructor option consumption.
- `host_verification.json`: actual retained C, 3,072 cases each under strict C89
  and ASan/UBSan (6,144 host executions). Synthetic helper stubs assert call
  order, widths, values, options, result effects, and untouched contexts.
- `controls_verification.json`: all 20 directed O2 controls, separately labeled.
- `diagnosis.json`: metadata-only workbench diagnostics; native words/objects
  exist only in temporary local files and are never part of this packet.

Native replay uses bounded integer MIPS-II execution with fail-closed unsupported
instructions and explicit helper contracts. Host pointers/layout are host-sized;
IDO assertions and native replay establish O32 layouts separately. These tests
cover synthetic valid, aligned input objects. Aggregate request copies must be
disjoint or exact self-copy; arbitrary partial overlap is outside this C domain.
They do not prove real helper
bodies, game integration, ROM equality, or cartridge coverage. `make test`, ROM,
splice/promotion, and production gates are out of scope and NOT RUN.

## Independent review

`independent_review.json` approves frozen source commit
`d01d9d5ef563f7846870da53faea44d37152458c` / tree
`5ab885a4953c00ac0f3f3c0191cc11187daa0e7a` as COMPLETE-NONMATCH only.
The independent constructor-pair reviewer inspected the full target, both
callers, all eleven direct helper interfaces and the option consumer, and
reproduced all six reports. Three additional actual-source defect controls
reject a missing 32-bit tag-wrap cast, nonzero channel count, and incorrect
mode 3. `peer_additional.py` reproduces those controls in temporary copies;
they are test-pressure mutations, not more matching-search variants.
The review adds no matching or integration credit and requests no C changes.

## Reproduce

Use the official pinned toolchain from `tools/cloud/setup.sh`, or set `IDO_DIR`
to an existing pinned installation. `input_pins.json` records its hashes.
Workbench diagnosis additionally requires `MIPS_OBJDUMP` and its matching MIPS
assembler; supply their normal library path if using a local extracted package.

```sh
P=cloud/work/boot_tail/BT03-sequence-dispatch
python3 "$P/verify.py"
python3 "$P/test_layout.py"
python3 "$P/replay_variants.py"
python3 "$P/test_native.py"
python3 "$P/test_host.py"
python3 "$P/diagnose.py"
python3 "$P/peer_additional.py"
```

No ROM, raw native assembly, object, credential, lock, protected path, shared
header, global ledger, or unrelated artifact is published by this packet.
