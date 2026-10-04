# BT07 stream pair: complete reconstructions, no matches

**Two COMPLETE-NONMATCH bodies, 988 native bytes. Zero matching credit.**
Both reconstructed C89 functions pass actual-source host contract tests and
32-bit layout checks, but neither is a strict native match. They remain under
this research directory, with no new file under `cloud/matches/`.

- Base: `301d9e7552ad4fd7f54a38796db84671e1000d35`.
- Branch: `dot/boot-tail-bt07-stream-pair`.
- Exclusive central claim: `b5723bfa`, acknowledged before editing.
- Exact intervals: `80025AB4–80025C68` (436 B), `800252AC–800254D4` (552 B).
- The lead owns central STATUS, claims and D10. This packet returns a status delta.
- Approved IDO 5.3 is reused read-only. Its 24 file hashes match the earlier
  pinned receipt (archive SHA-256
  `ab5c741561f80913d58c8b074771f23941a3edd312505a8ebed6d1dfeb65e506`).
- All three target-manifest members pass, all 439 extents/99,120 B agree with the
  inventory, and the existing getter still strictly matches.

## Whole-body and actual ABI evidence

`reconstruction.md` records region-by-region observations written before source
creation. The reconstruction comes solely from protected native targets and
actual earlier reconstructed helper sources. No external source was imported.
Behavior names and original source spelling remain hypotheses.

`80025AB4` is a two-address input, boolean-result table binding/loading routine.
Its high-nibble-8 path borrows an input table; other inputs are translated,
synchronously copied into allocated storage, and owned. It invokes exactly the
actual one-address translation hook, two-word allocator and one-address release
hook, and the genuine three-input `80010628(destination, source, unsigned size)`.
The hooks' +20/+24/+28 projections agree with earlier stream packets. Count and
allocation/transfer sizes are unsigned words in both retained files. No helper
is defined or stubbed inside either candidate.

Native failure side effects are retained: the old owned table is released before
either branch; a first allocation failure leaves the previous table pointer and
ownership byte untouched; a second allocation failure stores a null table but
leaves ownership unchanged. The retained byte-count local is recomputed after the
allocator, so callbacks that change the count are not silently ignored. Borrowed
binding clears ownership only on success. There is no attempt to reconstruct or
publish the table contents.

`800252AC` consumes seven genuine input words: unsigned index/rate and five byte
options. It checks enable/range, selects the first free of two 4,648-byte records,
creates a two-message queue, selects the default rate from the table's high byte,
computes rounded buffer count, binds or allocates a buffer, invalidates cache,
then enters the protected initialization sequence and returns a stream token.
The source preserves unsigned 32-bit wrap in the rate/count arithmetic. Allocation
failure still triggers the native cache-invalidation call before returning -1.
The initializer has five real inputs and the callback has the actual four-input
source/destination/count/queue contract. The queue token is passed unchanged to
unlock. Original parameter typedef spelling is not uniquely recoverable.

The stream view is a consistent refinement of earlier packet views. It exposes
the real second queue at +4576 and its two message slots at +4600, with busy +4608,
option +4612, rate +4616, handle +4620, buffer +4624, buffer count +4628, mode +4633,
processed +4636, float +4640 and token +4644. Unknown bytes represent actual global
record storage. No shared header, fake stack local or artificial formal is used.
Neither function uses a local switch table or floating literal relocation.

## Compiler results and bounded diagnosis

| Function | Retained O2 | Retained O1 |
|---|---:|---:|
| 80025AB4 | 45/109 different, 0 excess nonzero words | 94/109, 11 excess |
| 800252AC | 44/138 different, 0 excess nonzero words | 136/138, 28 excess |

All trials use `-g0 -O2/-O1 -mips2 -G 0 -non_shared` plus the scorer's mandatory
`-Wab,-r4300_mul`. O2 native cues are shared indexed addresses, branch-likely
outer gates, two-register stream loop, immediate return handling and call-crossing
homes. O1 expands substantially. The retained 252AC O1 trial ends the native
comparison window with one unpaired HI16 for D_80058688; this is an overlong
rejected control, not a successful relocation proof. All retained O2 relocations
have zero masks, unresolved symbols, unverified references and errors. Workbench
counts 110 true instructions for retained 25AB4 versus 109 native; its extra
trailing instruction is zero. Zero excess nonzero words therefore does not imply
boundary equality. Retained 252AC has 138 instructions, equal to its native count.

Unmodified workbench diagnosis was run before refinement and after intermediate
near-matches. Initial positive-control-flow changes removed extra return blocks;
putting stream-pointer advancement in the loop body matches the native loop.
Genuine frequency/count locals and declaration ordering reproduce the 72-byte
frame and actual token/size home offsets. Residuals remain in option/formula
scheduling, hook-call expression allocation and temp-register assignment.
The native-object diagnosis's relocation-symbol warnings arise from temporary
word-only target objects; strict scoring is the final authority.

`experiments.json` records 13 directed source forms for each body, in O2 then O1
order. These cover positive gates; equivalent count expressions; loop forms;
byte versus full-word options; a signed-word and unsigned-long diagnostic;
separate hook projections; rate-selection forms; assignment order; and genuine
byte-count lifetimes. Unsupported signed extern views and volatile-count controls
were rejected and are not present in final sources. One intermediate automation
accidentally added a local frequency declaration to the record view; that layout
was immediately corrected and is not retained. This did not change any target.
Only initial and final sources are archived for replay; intermediate residuals
are historical observations, not independent reproducibility claims.

The final size-lifetime control for 25AB4 improved 48/109 plus one excess to
45/109 with no excess; splitting it into two separate size locals regressed.
Further equivalent arithmetic and hook projection controls were inert. 252AC's
best 44/138 result did not improve across later natural forms. Work stops here
within the 20-form bound. The next useful step is an authentic source/type lead
or measured IDO scheduling explanation, not more blind equivalent-expression
variants. No scorer, flag sweep, target or ABI workaround was attempted.

## Verification and limits

With `IDO_DIR` set to the approved compiler directory, run from repository root:

```sh
python3 cloud/work/boot_tail/BT07-stream-pair/verify.py
python3 cloud/work/boot_tail/BT07-stream-pair/test_semantics.py
```

`verify.py` pins compiler, scorer, inventory, generated-target inputs and getter;
it hashes final sources, scripts and notes, strictly replays O2 then O1 and the
initial controls, and explicitly requires both final results to remain NONMATCH.
`verification.json` is the source/target-bound receipt.

The actual-source host harness covers **334 scenarios**, each at strict C89 O2
and ASan/UBSan O1. Coverage includes enable/index rejection, both busy slots,
all 256 option byte values, both ownership modes, optional-rate and high-byte
selection, unsigned arithmetic boundaries, null allocators/buffers, unchanged
other records, queue/hook/callback ordering, count mutation across allocation,
owned-table failure states and borrowed binding. A mapped host pointer whose
low 32 bits are 0x80000000 exercises the borrowed branch without putting memory
inside ASan's shadow gap. It is not evidence of native address mapping.
Separate 32-bit C89 assertions check every exposed native offset and both object
sizes. LeakSanitizer is disabled under ptrace; ASan and UBSan remain enabled.

These checks support source behavior and layout, not instruction equality,
cartridge integration, promotion, production runtime, ROM hash or coverage.
Raw words, disassembly, objects and compiler intermediates remain private.
