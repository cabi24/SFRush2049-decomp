# BT02 medium packet

Eight strict relocated matches, **680 native bytes**, plus two complete nonmatches,
**272 bytes**. All eight matches use `-g0 -O2 -mips2 -G 0 -non_shared` with the
mandatory `-Wab,-r4300_mul`. Local strict replay, immutable input checks, fixed O1
controls and host behavior checks pass. Independent source/ABI review and exact
aggregate-head CI remain required. These are matching-source results, not cartridge
coverage or promotion.

## Claim and boundaries

- D10 / Packet 4 / BT02-medium; branch `dot/boot-tail-p4-bt02-medium`.
- Fresh master base: `301d9e7552ad4fd7f54a38796db84671e1000d35`.
- Central active claim: `a28c244e`, in the lead's wave2 claim registry.
- Exact claimed starts and sizes: `80010DD8` (168), `8001144C` (116), `80011848`
  (76), `80011C1C` (104), `80011D24` (80), `800121BC` (68), `8001261C` (68),
  `80013964` (112), `800140F8` (72), `80014140` (88): ten disjoint extents, 952 B.
- Edited paths: the eight matching sources and this packet directory only. Central
  ledger, D10 and prior frozen packet sources belong to the aggregate lead.
- Read-only research dependency: [Packet 1 PR #59](https://github.com/cabi24/SFRush2049-decomp/pull/59)
  at `21e104a22575cf4d639261d2f6d9535913074e6a`; Packet 2 checkpoint
  `76780b3a1b3e26c54b86e1f153344e92dac15b20`. Neither branch was merged.
- Prior reviewed source context was inspected at first-wave aggregate checkpoint
  `de8652c39d3bba59a45d3f552666cf5e435a3dc3` without modifying or re-crediting it.
- No queued packet. No protected target, scorer, symbol, shared header, lock,
  layout, runtime-image, D11 or production-gate edits.

## Reproduction and integrity

After normal compiler setup, run from the repository root:

```sh
python3 cloud/work/boot_tail/BT02-medium/verify.py
python3 cloud/work/boot_tail/BT02-medium/test_host.py
```

`IDO_DIR` can point to the existing pinned compiler installation. `input_pins.json`
preserves the Packet 2 compiler and input digests. The verifier checks all compiler
files, protected target manifest members, all 439 inventory starts/sizes (99,120 B),
the original getter, the actual final source hashes and both flag levels. It
reproduces the archived nonmatch residuals instead of treating them as submissions.
The source-binding receipt is `verification.json`; no object or native dump is
archived. `initial_scores.json` is the initial natural-source O2/O1 experiment.

## Actual bodies and ABI audit

Roles and historical names remain hypotheses. The inventory's empty direct-call
list did not mean these were leaves: eight bodies use `jalr`. Inspection therefore
covered each actual indirect call's arguments, return usage, stack frame and
relevant native caller/helper context. No guessed extra callback arguments are
introduced from incidental live registers.

| Function | Bytes | Reconstructed behavior and ABI evidence |
|---|---:|---|
| `8001144C` | 116 | Invoke callback `8003801C` on four pointer slots at `800382D8`, `800382DC`, `80038298`, `800382E4`, in that order. The callback is reloaded for each call and receives only the loaded pointer in `a0`. No incoming argument reads; normal 24-byte O32 frame. |
| `80011848` | 76 | Wait until volatile byte `800382CC` is zero, then invoke the same one-pointer callback on `8003802C`. Native branch-likely loop repeatedly loads the flag. No incoming arguments or returned result consumed. |
| `80011D24` | 80 | If unsigned halfword `80038028` is nonzero, set byte `800382CC` to one and call callback `80038010` with buffer `8003802C`, that count, and address `800382B0`. Native `a0/a1/a2` establish three arguments; result unused. `80013DEC` calls without arguments. |
| `800121BC` | 68 | Invoke one-pointer callback `8003801C` on slots `8003833C` then `80038338`. No incoming arguments, two callback reloads, ordinary O32 frame. |
| `8001261C` | 68 | Corresponding release pair: slots `80038350` then `8003834C`. Same callback ABI and frame. |
| `80013964` | 112 | Select the unsigned-halfword counter and buffer using byte index `8003829E`; save buffer+16 and its 16-byte-aligned floor, then clear the selected counter and first buffer halfword. All pointer arithmetic/widths follow native shifts and stores. Native reloads the saved pointers before the final clears. No calls or incoming arguments. |
| `800140F8` | 72 | Set halfword `80038360` to 65535, clear callback slots `80038020/24`, and register `func_80013DEC` through `80038008`. `80014198` calls with no arguments; the indirect registration call has the callback address in `a0`. Existing native `80013DEC` calls the cleared slots without arguments. |
| `80014140` | 88 | Copy unsigned halfword `80038292` to volatile signed halfword `80038362`; wait while positive; pass `func_80013DEC` through callback `8003800C`. Native signed `lh` and branch-likely reload establish the signed waiting condition. Ordinary O32 frame and one callback argument. |

The buffer-pointer array at `800382D8` spans the individual slots `800382D8/DC`.
The release source expresses those as individual pointer slots; the selector
expresses the actual indexed array. This is the same native storage. No new
shared-header definition or storage owner is proposed. Host tests deliberately use
separate executables for these two views rather than pretending independent host
symbols alias at native addresses. The address-like unsigned-word cursor values
retain only 32 bits on the N64; host tests explicitly compare the same truncation.

## Compiler evidence and fixed controls

All eight matched the first natural O2 form. No matching source underwent a
schedule or allocator search. The volatile loops, shared indexed loads and
frameless selector discriminate O2 for four bodies. Four wrappers also match O1,
so those bodies do not independently identify their historical optimization level.

| Function | O1 differing/total words | O1 excess nonzero words |
|---|---:|---:|
| `8001144C` | 0/29 | 0 |
| `80011848` | 19/19 | 0 |
| `80011D24` | 17/20 | 0 |
| `800121BC` | 0/17 | 0 |
| `8001261C` | 0/17 | 0 |
| `80013964` | 27/28 | 1 |
| `800140F8` | 0/18 | 0 |
| `80014140` | 18/22 | 1 |

## Complete nonmatches and bounded diagnosis

`nonmatch/func_80010DD8.c`: **NONMATCH**, O2 **40/42** words; O1 **40/42**.
The complete behavior allocates a duration-dependent halfword buffer: multiply
unsigned rate by unsigned-16 duration, divide by 1000, advance to the next 192-unit
boundary (including advancing a full block when already aligned), save the count,
clear one pointer, and call the allocator callback with doubled count and zero.
Zero duration clears the output pointer. Caller `80014198` supplies a rate loaded
through its incoming pointer and a halfword duration. Other allocator calls in
`80011104` support the two-argument pointer-returning callback declaration.
Workbench diagnosis on the original O2 source found a structural lowering mismatch:
42 native instructions versus 36 candidate instructions, the same 24-byte frame,
and divergence at narrow-argument normalization and constant-remainder lowering.
Native also retains a register-divisor check for the 192-unit block, while the
candidate folds the constant division and rearranges the addition.

Rejected directed controls: K&R definition, an explicit meaningful 192-unit quantum
local (ANSI and K&R), register-qualified narrow/all formals, explicit self-narrowing
of the narrow formal: all retain 40/42. A full-width duration formal with explicit
cast or mask reaches 38/42 but loses the naturally supported narrow-formal ABI
spelling and does not repair the structure, so it was not retained. Nine O2 source
forms including the baseline, plus the fixed baseline O1 control, were sufficient
to establish the blocker; no padding, forced homes or dummy uses were attempted.
Next hypothesis: obtain the original callback/API declaration and native block-size
expression or a measured IDO front-end explanation before another source search.

`nonmatch/func_80011C1C.c`: **NONMATCH**, O2 **14/26** words; O1 **26/26** with
13 excess words. The complete body selects a 104-byte audio record by unsigned-16
index and initializes its two bytes, flag/gain halfwords and two four-halfword
ranges. Caller `800146B4` passes its narrowed index and zero flags; its later record
accesses corroborate 104-byte stride. The unknown-offset arrays represent real
unexamined record fields, not stack locals or artificial frame padding. They are
compatible with the earlier packet's partial AudioState fields.
Workbench diagnosis found equal 26-instruction lengths and a frameless leaf, but
the native homes `a0` then narrows it in place. The candidate narrows into a temp
before homing `a0`, cascading the temp ring and scheduling. K&R, prior exact
prototype, register-qualified index/all formals, and redundant explicit narrowing
all reproduce 14/26; full-width explicit cast/mask controls worsen to 26/26. Eight
O2 forms including the baseline, plus its fixed O1 control, were tried. The final
archive retains the natural narrow signature. A separate BT03-low-B worker reported
the same normalization obstacle independently. Next hypothesis: establish the
original narrow-formal declaration/front-end convention before more variants;
do not force a spill or add fake formals.

## Tests and limits

The host-only drivers link actual matching sources as separate translation units.
They check release order and pointer forwarding, zero/max-count submission paths,
flag-before-call ordering, registration callback identity and cleared state,
zero/negative signed wait exits, both buffer indices, aligned-cursor math, and
preserved neighboring counters/buffer fields. Two bounded child-process controls
confirm that positive/nonzero wait flags do not fall through. They do not test
concurrent release of those flags or prove native concurrency/hardware behavior.
Callback doubles exist only in the host drivers, never in a matching source.

Native ground truth remains read-only `asm/us/boot_tail`. No third-party source was
copied, no local literal pool or switch table is involved, and every selected
submission relocation resolves. No ROM, raw assembly dump, object or credential is
part of this packet. Independent review and exact aggregate-head CI are pending;
merging and cartridge integration remain with the owner's independent checker.
