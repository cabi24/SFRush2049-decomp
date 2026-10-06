# Image B: complete FCE0 root semantic reconstruction

**Research only. Complete ordinary-root source and bounded native behavior.
No private-ABI match, accepted bytes, image/compression or ROM claim.**

Base: `6b2e9e506fe3d2267a710e41c85af5364ccd00c7`.
Target: B `[0x8038FCE0,0x803908D0)`, 3,056 bytes / 764 words.
Native SHA-256: `3574383f8241f87f1aa201bc4a7265e49cd818b8b6f7e6564ecca4131b6682e2`.

## Result and corrected closure

`root.c` reconstructs the entire genuine outer root. It expires and advances
short-lived debris, iterates live player state, handles reset/fire conditions,
initializes every kind-specific record branch, optionally emits secondary
debris and attached effects, drains the release queue, then calls E114 and
CB20 in order. Native root entry consumes no incoming formal. Its conventional
208-byte frame preserves s0..s8 and f20..f31.

The first complete source exposed another real private child:
F938 `[0x8038F938,0x8038FCD8)`, 928 bytes. It consumes incoming s0 (status),
s2 (player), s3 (vehicle), and s5 (owner), and does not preserve all ordinary
O32 nonvolatile state. The minimum known closure therefore increases from
10,448 to **11,376 native function bytes**. Original TU/export visibility is
still unproven; this is a lower bound, not a matching assignment.

The prior E114/D200/D328/E088 research packet remains unchanged. This packet
claims only FCE0 source and its contracts. F938, E114, F568, D054 and CB20
remain read-only dependencies; their complete C is not supplied here.
`private_F938` and `private_E114` are clearly labeled semantic external
interfaces for root-only testing. The compiler sees ordinary-ABI external
declarations and no private callee implementation; this establishes no actual
private call compatibility. They are not optimizer-visible stand-ins,
kept fake roots, native ABI claims or a substitute for real closure source.
No compiler shaping, declaration permutations, pressure locals, assembly,
volatile or artificial input parameters were used.

D054 has a real homed third argument, although its body does not otherwise
consume it. FCE0 passes the record position in both a1 and a2; the source
preserves that actual call. Its selector is always 0, 1 or 2 in this root.
The accepted 8008E398 wrapper's void declaration remains inconsistent with
native scene-index consumption. The research declaration returns that index;
no accepted declaration is changed.

## Compiler and behavioral evidence

The primary source header and recipe are exactly
`-g0 -O3 -mips2 -G 0 -non_shared`; unchanged canonical `score.compile_single`
also inserts the mandatory `-Wab,-r4300_mul` backend flag. The source is one
ordinary-ABI semantic unit, all callees external, no group keep list.
The first O2 compile was a diagnostic smoke check; it is not O3 evidence.

Canonical O3 emits one complete 3,048-byte function, with eight separately
checked zero text-alignment bytes and 80 bytes of owned rodata. The whole
unmodified ELF is GNU-linked at research text/rodata addresses. Every external
binding and object relocation is recorded; no executable neighbor is borrowed.
Unused intrinsic sqrtf has no relocation and emits native sqrt.s.

The research-placed full-word comparison differs in 735/764 positions,
including two words absent from the function extent. This count includes
candidate-owned data at research placement and is not a native-placement
owned-data proof or a matching baseline. Broad ordinary/private convention
differences are expected. No tuning is justified until real closure source
and original visibility evidence exist.

`verification.json` records complete source/verifier hashes, selected native
words, object/linked allocated sections, relocations, addresses, 25 native
layout facts, **1,419 paired whole-function fixtures (2,838 executions)**,
759/764 native and 751/762 candidate instruction offsets, and six
compiled wrong-source controls. The fail-closed interpreter compares all
mapped nonstack state and its digest at every ordered external call, under
caller-save register poisoning. Native private hooks use conservative clobber models that are supersets
of the observed nonvolatile writes; the conventional root must
restore its saved state. Models are explicit and do not execute real helpers.

The five native offsets not executed in the bounded valid-input domain are
+0xBC/+0x260/+0x310 (duplicate unreachable compiler blocks), +0x2E4 (ammo zero
after the earlier zero-ammo reset sets -1), and +0x31C (a late null input after
an earlier unconditional input dereference). These are retained in the full
764-word authentication; no bytes are omitted from target or source extent.

## Reproduce

From a normal checkout with full Git history, pinned IDO and MIPS GNU linker:

    python3 cloud/work/runtime_b_fce0_root_20261006/verify.py --check
    python3 -m pytest tests/cloud/test_runtime_b_fce0_root.py -q

A source-only overlay may use `--reference-root /path/to/repository` for the
historical asset, or `RUSH_REFERENCE_ROOT` for the pytest replay. The asset
is read through `git show BASE:path` and decompressed only in memory. Tests
skip compiler-dependent work cleanly without IDO or the linker. Receipts
bind this packet's source/model/verifier/layout, selected native words,
allocated content, relocations and behavior. They do not bind live locks,
production source, scorer versions, manifests or their own test file.

The coordinator must run the required current-master full suite both with
and without IDO before a PR. Independent frozen-source review is required
before publication; merging and acceptance belong to the independent checker.

## Domains and limitations

- Acyclic debris/release lists have 0..4 nodes. Live player count is signed;
  nonpositive entry counts skip the region, and tested positive or
  callback-modified counts stay 0..4.
- Player owner and model must select valid vehicle/table entries. Producer
  owner cases use 0..3, models 0..12; independent review additionally probes
  owners 0..12. Player kind stays 0..8 at player-indexed table accesses.
  A separately mutated record kind tests the default record switch.
- The first reset read requires a valid nonnull input pointer. The later
  null check does not make initial null input safe. Tables, records, pools,
  player/vehicle storage and returned allocations are aligned and disjoint.
- Allocation may fail at either allocation site. The kind-3 latch is set
  before primary allocation and is not rolled back on failure. Successful
  allocations in tests have initialized sufficient storage; this is not a
  proof of real allocator capacity or gameplay lifetime.
- B-image table/constant slices are authenticated. The main-image bounds
  table uses synthetic finite fixtures. All external service bodies,
  including E114/F938/F568/CB20, remain explicit bounded effect models.
- Zero/normal finite binary32 arithmetic, default rounding, no concurrent changes or
  unrestricted aliases. NaNs/infinities, denormal operands or results,
  overflow/underflow/exception behavior, nondefault FCSR, arbitrary invalid
  pointers, hidden register dependencies of unimplemented callees, original
  TU identity and whole-game execution are outside this proof.

Next useful work is complete genuine F938/E114/DA78/D498 source and helper
contract reconciliation, then a justified whole-context compile. This root
milestone adds no matching coverage and does not reopen allocator sweeps.
