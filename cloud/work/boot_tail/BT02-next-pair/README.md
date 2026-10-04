# BT02 next pair: envelope and sample-position updates

**Two COMPLETE-NONMATCH bodies, 932 native bytes; zero matches and zero matched
bytes.** Both full natural C89 sources are retained under `nonmatch/`, never in
`cloud/matches/`. Pinned-input strict replay and C89/ASan/UBSan host checks pass as
research verification, not matching acceptance. This packet does not establish
cartridge coverage, promotion, CI acceptance or merge readiness.

## Claim and scope

- Canonical master base: `301d9e7552ad4fd7f54a38796db84671e1000d35`.
- Parent activated exact targets before candidate edits: `80011A64`, 440 B,
  `[80011A64,80011C1C)`; `80011D74`, 492 B, `[80011D74,80011F60)`.
- Fresh 48-file central claim/status/research/delta/manifest snapshot SHA-256:
  `2aa8e44716a6e0ad5b31f2486e50de20ebe4c61657d975c35f26d58263289302`.
  Existing text mentions were end-exclusive boundaries of earlier claims, with
  zero target-start overlaps. Both central statuses were open and no matching
  source existed. `claim_scan.json` preserves the hashed input list.
- Only this packet is authored. The aggregate lead owns central status, claims
  and D10; `status_delta.json` is proposed input, not a copied central ledger.
- No previous small nonmatch was edited or re-scored. No Git store writes were
  made during the storage hold. Existing read-only canonical targets and pinned
  IDO 5.3 were used; temporary target/candidate objects are not publication inputs.

## Native-supported contracts

`80011A64(AudioState *, unsigned short samples, unsigned char *active)` updates
an envelope. Actual caller `80012D18` passes its existing audio record, 192 samples,
and the address of a writable stack byte at the call ending at `80013534`. The
third argument is consumed only by the release-completed path. No return is used.
The function computes an unsigned fixed-point increment from the global unsigned
sample rate `8003828C`. A released note enters stage 3 and saves its current value;
expired stage 0 enters decay, expired stage 1 settles at sustain, and expired stage
3 clears the caller's active byte. Other stages and unfinished stages interpolate
between stored endpoints and advance the unsigned accumulator. A zero duration
selects ratio 1.0. Division by zero is not bypassed: native has the compiler's
unsigned-division trap and valid host tests use a nonzero global rate.

`80011D74(AudioState *, unsigned short samples)` advances a double sample
position, wrapping inside an enabled loop or clamping at `length - 1`. Caller
`80012D18` passes the same record and 192 at the call ending at `80013944`; no return
is used. Its only callee `8001E688` takes one double in the FP argument pair and
returns a double. Read-only inspection of that entire canonical 172-byte helper
shows unsigned truncation followed by conversion back to double, including IDO's
conversion exception paths. The source declares this external function and does
not copy or define it. The first helper call tests truncated current position;
the optional second helper call obtains the number of whole loop lengths to
subtract. Loop-end addition is unsigned 32-bit addition before conversion.

The 104-byte `AudioState` storage view agrees with previously reconstructed fields
and stride. Newly exposed fields are: held byte at 1; rate halfword at 4; decay
halfword at 0x22; sustain float at 0x24; length/start/loop-length/stop-loop words at
0x30/34/38/3C. Existing fields remain position 0x08, current/previous words 0x10/14,
initial/release/count halfwords 0x20/28/48, and value/step/scale/saved-value/state at
0x4C/50/54/58/5C. Unknown byte arrays describe actual record storage, not stack
padding. Names describe inferred roles; native offsets and access widths are the
proof. No shared header is changed.

## Flags, bounded experiments and stopping condition

All flags are `-g0 -O2 -mips2 -G 0 -non_shared`, followed by the same complete
source at O1. The unchanged scorer adds mandatory `-Wab,-r4300_mul`.

| Function | Final O2 difference | O2 excess nonzero words | Final O1 difference | O1 excess |
|---|---:|---:|---:|---:|
| 80011A64 | 107/110 | 1 | 104/110 | 5 |
| 80011D74 | 24/123 | 0 | 122/123 | 14 |

Every final row has zero unresolved symbols, unverified relocations and scorer
errors. Both functions materialize all numerical constants as instructions;
neither needs a literal pool or jump table. Branch-likely selection, expression
CSE, the frameless envelope, and the 32-byte position frame support O2 as the
closer lowering. O1 remains a rejected full-body control, not a claimed match.

The original position source was 97/123 at O2, 123/123 at O1 with 21 excess words.
Naming the actual loop-end double after the first helper/comparison reproduces
native FP materialization and moves O2 to 24/123, exactly 123 native and generated
instructions. The remaining 24 words are register differences associated with
incoming narrow-formal normalization and downstream temporary-register numbering.
The native keeps the normalized samples in a1; IDO places them in a temporary.
The envelope has the same initial normalization difference and two additional
instructions (112 generated versus 110 native; the strict excess count excludes
zero padding).

Unmodified workbench diagnosis was run before directed controls and again for the
retained source. Its raw-object call/address relocation warnings do not replace
strict relocated `score.py`; target objects deliberately contain canonical words
without relocation metadata. The final position diagnosis reports 24 aligned
register differences. The envelope has structural normalization/scheduling and
register differences. No compiler instrumentation or scorer change was used.

Four follow-up variants per body, all within the 20-variant bound:
- Both: register-qualified real samples parameter and C89 K&R definition, unchanged.
- Envelope: split increment computation and signed 32000 multiplier, unchanged.
- Position: multiply-to-divide constant spelling, unchanged; semantic loop-end
  local, retained for the measured 97-to-24 improvement.

The same unsigned-short normalization class is documented in the earlier
`BT02-medium` packet. Once recognized, no wider-formal workaround, forced spill,
fake formal, keeper, padding, assembly or exhaustive sweep was attempted.
Next hypothesis: independently establish the original narrow-formal front-end
convention before further source variants. The packet is frozen at this evidence
boundary; reopening needs new native/compiler evidence.

## Reproduction and checks

From the repository root with `IDO_DIR` set to the existing approved IDO directory:

```sh
python3 cloud/work/boot_tail/BT02-next-pair/verify.py
python3 cloud/work/boot_tail/BT02-next-pair/test_host.py
```

For a private review packet outside a checkout, give `verify.py --root PATH`.
The verifier checks all compiler-file hashes against the existing Packet 2 pin,
protected input hashes, all target manifest members, all 439 extent/inventory rows
(99,120 bytes), the original getter MATCH, both full-source O2/O1 residuals, and
nine rejected/baseline O2 controls. Its PASS explicitly means expected NONMATCH
residuals reproduced. `verification.json` binds results to source/harness hashes.

Host checks compile the actual complete sources as separate C89 translation units,
then repeat under ASan/UBSan. They cover 1,600 envelope cases and 48 position cases:
all envelope stages, note-held/released paths, duration zero, step threshold and
unsigned wrap, sample counts 0/1/192/65535, four nonzero rates, release output-byte
preservation, loop enable/stop combinations, exact-end and multi-wrap transitions,
truncated-position boundaries, no-loop clamp, zero-length unsigned underflow,
large unsigned positions, exact helper call arguments/order/count, field offsets
and untouched record bytes.

Host tests stay within defined finite double-to-unsigned and signed-product input
ranges. They do not model N64 FCSR exception behavior, NaN/infinity, overflowing
signed products or hardware/audio concurrency, and they cannot prove matching.
Leak detection is disabled for the existing ptrace restriction; no heap allocation
is used by the harness. No external reference source was copied or vendored.

No ROM bytes, raw assembly, objects, credentials or unrelated data are published.
Targets, scorer, symbols, locks, layout, shared types, runtime images, farm config,
central status, D10 and cartridge gates remain unchanged. Publication, if useful,
is through the central aggregate; merging remains with the independent checker.
