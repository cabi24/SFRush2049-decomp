# Sequence-start caller in accepted slot context

## Current receipt schema (2026-10-06)

Current receipts omit whole-file manifest, scorer/own-data tool, lock and redundant
production-context digests. The recorded BASE and `git show BASE:path` source reads
remain, as do native target authentication, own packet/verifier/C-harness bindings,
compiler executable identities, complete compiled extents, relocations, owned data
and behavioral evidence. Historical compatibility normalizers accept only their
explicitly listed legacy fields; unknown proof fields and changed invariants still
fail comparison. Descriptions of the earlier receipt schema below are historical.
This schema correction adds no matching or accepted bytes; fresh replay and the
aggregate test matrix are separate required checks.

Research only. `func_800979A0`, game image `[0x800979A0, 0x80097AC4)`, is a complete 292-byte / 73-word function. The type-corrected C emits 288 bytes and remains **NONMATCH: 12/73 positional words differ**. All twelve reused accepted `slot_sound` functions remain strict, complete-body matches. No matching submission, accepted-byte gain, image splice, compressed-stream or ROM result is claimed.

## New evidence and admission

Base: `dea99f09ab19b1d3b324ed7097162f7b378e7096`. Protected target identity, tool hashes, source hashes and exact recipe are in `verification.json`.

The old complete body at `cloud/work/s20261004/C/g979a0/group.c` stopped at 24/73. Its private copies of the slot helpers failed even their own accepted-context comparisons. `cloud/work/frontier/w1a/RESULTS.md` documents the subsequent genuine slot-group repair. Wave 8 `w8a/RESULTS.md` explicitly lists this caller as a remaining sibling after accepting `entity_render_mode`. The usable prerequisite is the current accepted group, not a new claim that wave 8 first discovered its ABI.

A current-base archive search found no later complete reconstruction/compile of this caller in the repaired group. It is absent from the current game locks. Open PRs checked on 2026-10-06 did not claim it. Existing siblings `func_80096CA8` and `suspension_setup` retain unresolved private saved-register caller context; `car_shadow_render` is the already-frozen A142 reconstruction. No claim is made on those functions, the particle knot or the separate heap caller work.

## Source and real ABI

The caller starts a sequence when the byte gate is zero and either there is no current sequence (`-1`) or the requested identifier differs. It loads a resource with `audio_frame_sync`, activates its slot, retrieves its payload, registers the resource, starts the sequence, and records the request identifier.

- Entry has two word arguments. The first is consumed. The second is genuinely homed by the protected native body at entry-stack `+4`, but never reread. This is witnessed native ABI, not a new pressure formal.
- Native and candidate frames are both 56 bytes. The caller saves `s0` and `ra` and uses ordinary O32 service boundaries.
- The real `slot_value_get` inlines into this caller. Its internal `func_80096288` call preserves `a3`, which carries the slot index across the call. The actual 16-byte validation hook does not write registers or memory. Treating this boundary as an arbitrary O32 clobber is incorrect.
- `display_list_alloc` can affect resource state. The index is reloaded afterward. Registry service `func_8001536C` can mutate the selected table entry or payload global; both are reloaded before the sequence-start service.
- The four-byte sequence table record has two unsigned halfwords; only the second is consumed. Original table capacity and all valid caller indices are not inferred here.
- `slot_value_get` and `D_80151ADC` have pointer types compatible with the accepted source. Registry pointer arguments and its signed integer return follow `BT03-low-registry`. Stream/options pointer semantics follow the later `BT03-low-registers` / `BT03-sequence-dispatch` transitive contract for `func_8001558C`; the direct wrapper's signed integer return is retained. The resource loader's fifth argument is a buffer pointer. Those external services are not newly reconstructed in this packet.

`group/group.c`, `group/func_800A4E58.c` and `group/entity_render_mode.c` are exact copies of current accepted source. The existing validation-hook inline-blocking quirk is inherited unchanged. No additional stand-in, pressure variable, unused local, artificial helper, volatile qualifier or dead check was introduced. Only `group/func_800979A0.c` is the new caller source; all `claims` are empty.

## Bounded compiler result and stopping decision

One natural current-context transplantation recovered the private ABI and all twelve accepted context bodies. Correcting the historical scalar/void helper declarations to real pointer/return contracts preserves the same generated caller and residual.

The difference is one final address materialization: native uses a separate `v1` address for `D_8011EAA0`, whereas the natural C uses a direct store through `at`. The missing instruction shortens the body by four bytes; it also changes two forward branch displacements and shifts the epilogue. The exact differing offsets are `0x18`, `0x3C`, and `0xFC..0x120` inclusive at four-byte intervals. There are no nonzero excess words or unresolved/unverified references.

The required workbench diagnosis was run on the full baseline and a separate relocated comparison view. It classifies the mismatch as a one-instruction structural/address-register divergence with the same frame. Its ownership diagnosis is heuristic; it does not establish an original source expression. Native references to the sequence-handle global in `func_80096238`, `func_800A4CA8`, and `car_shadow_render` use ordinary direct stores and do not justify adding volatility. The independent reviewer likewise found no grounded repair. No shaping sweep was attempted.

Reopen only with authentic source, a genuine omitted helper/caller contract or a native-supported value/address lifetime explaining the final address. Repeating the old private leaf group, adding a dead result check, introducing a filler local or making the global volatile solely for code generation is not a justified next step.

## Reproduction and checks

From a complete checkout with the recorded IDO toolchain available:

    python3 cloud/work/frontier/dot_sequence_start_979a0_20261006/verify.py --check
    python3 -m pytest -q cloud/work/frontier/dot_sequence_start_979a0_20261006/test_verification.py

`--repo /path/to/repo` explicitly selects the current protected targets/tools/accepted baseline. Replay also works from a different working directory and with optimized Python; verification guards do not rely on assertions.

Fresh checks:

- Complete 13-function ELF membership and exact emitted extents. The twelve accepted bodies equal their full native extents.
- All 125 whole-group text relocations resolved, with zero masks, unresolved symbols, unverified references or errors. Group text is 2,064 bytes; zero owned data/storage.
- Source, metadata, roots, file order and flags pinned; all accepted source copies checked byte for byte before compilation.
- Frozen receipt replays exactly. Five scoped tests pass, including optimized-Python portable replay and rejection of claims, changed accepted source, and altered kept-root order.
- Caller source passes 32-bit C89 pedantic syntax/type checks with all warnings treated as errors except the witnessed unused parameter.

Independent review reports PASS for bounded NONMATCH research in `independent/review.json`:

- GNU linking checks each complete ELF function extent separately, including all twelve accepted bodies and all 35 caller relocations. This is not a complete production-unit link. The thirteen complete bodies total 2,060 bytes, followed by four verified alignment bytes.
- 960 byte-offset-oracle/native/candidate cases, 1,920 machine executions, cover all 73 native and 72 candidate instructions and both outcomes of every caller conditional branch. The actual `display_list_alloc` and `func_80096288` bodies execute. External services remain effectful contract hooks with hostile caller-save clobbers.
- An unchanged C89 source host harness passes 960 UBSan cases. Saved registers, the witnessed second-argument home, effectful global/table reloads and nonstack reads/writes/calls are checked in the machine tests.
- Negative controls reject keeping the validator as public ABI (35-word residual), deleting the genuinely homed second argument (68), compiling without real context (51), and resolving a global to the wrong address.

The producer receipt itself claims only full compile/relocation/body verification; the independent behavioral evidence has the finite scope above. No broad suite, real hardware, unrestricted alias/concurrency, full image or ROM gate was run. Generated objects, raw native assembly and diagnostic dumps remain outside this packet and must not be published.

## Integration-portable replay (2026-10-06)

Scorer, whole-manifest and accepted-context digests are historical provenance,
not live-tree requirements. The verifier normalizes only enumerated provenance
fields on both receipt sides. Packet source and verifier bindings, compiler
identity/actual flags, selected native bodies and addresses, complete emitted
extents, relocations, owned data and behavioral checks remain binding.
Production context is read with `git show BASE:path` at the recorded base,
never from the current production tree.
