# C18 — SDK initialization storage lead; no accepted strict match

Selected current passthroughs: dma_wait, lzss_decompress and inflate_decompress in 3140 (no accepted flag pin), plus __osInitialize_common in 8a80 (accepted __osPiReadDeviceType pins O1). No shared source, declarations, layout, lock, target metadata or state were changed by this worker. Frozen sources and proof artifacts are in static_C18/.

The useful result is a genuine complete initialization module: full_module.c compiles to all 848 original text bytes and all 32 original initialized-data bytes exactly after real linking. Its initializer contributes 680 function bytes; the already accepted 156-byte __osPiReadDeviceType body is unchanged, followed by the original 12 alignment bytes. The standalone initializer has different final padding; complete-module evidence determines the actual boundary without artificial runtime padding. Current target remains strict 40 / raw-word 6, so this is an ownership and guarded-metadata proposal, not a ready lock or promotion.

## Exact source and physical evidence

init_ownership_manifest.json records source SHA256 7606f20fc4e2459c6de43e5be046a9389030c5c7a93db6548bf9729003e5fe80 and compiler-object SHA256 a73bb742b270f19701a7b8377c3704befe99431c965c3829e98d20bddef07db9. The object is private on Rocky at ~/agents/C/scratch/static-C18/full_module.o. Source flags are -g0 -O1 -mips2 -G 0 -non_shared; actual complete-module invocation additionally uses the existing production -Xcpluscomm. No arithmetic requiring the floating-point errata flag is present.

| Real section | Original placement | Size | Linked original-byte differences |
| --- | --- | --- | --- |
| .text | VRAM 80007E80 / ROM 8A80 | 350 hex / 848 bytes | 0 |
| .data | VRAM 8002C360 / ROM 2CF60; data.bin offset 1CF60 | 20 hex / 32 bytes | 0 |
| .bss | VRAM 800367D0 | 10 hex / 16 bytes | allocated section; no COMMON |

linked_full.json records matching text SHA256 5f9eb949fa43d9b8212d4337ae14a0b02b4e6302d09c8a39d15c86a8fdafe8f7 and data SHA256 00eff670001b7953977665e4e87a8f811e3928fc8cce0f7ff5e8bf01d41559c6. Readelf artifacts retain actual section-bound symbols: clock counter at data+0 (OSTime/u64), VI clock at +8 (s32), shutdown at +C (u32), global interrupt mask at +10 (u32), and final-ROM flag at BSS+0 (u32). Section alignment supplies the remaining genuine bytes. These owned symbols were explicitly removed from the model's absolute external-symbol definitions.

These are the original SDK initialize.c storage definitions, mapped to actual historical ROM labels: clock 62500000, VI_NTSC_CLOCK 48681812, shutdown zero, OS_IM_ALL 003FFF01, and __osFinalrom. The genuine old SDK unused clock local is retained because the original function stores it; it is not a guessed seed. SDK source/header hashes and real retail helper assembly provenance are recorded in sdk_authorities.json and the manifest. Code copies the actual four-word exception preamble to the four SDK fixed exception addresses, performs the original SI/PIF loops, cache and TLB initialization, clock scaling, reset/NMI handling, TV clock choice, and AI register setup.

For 64-bit scaling, direct C operators produce IDO names __ll_mul/__ull_div with no authoritative root bindings. The frozen source calls the actual ABI-compatible unsigned64 helpers __muldi3/__udivdi3 already present at 8000DA58/8000D958. Original asm/us/E4F0.s and SDK libc/ll.c establish the paired argument/result ABI and dmultu/ddivu operations. No fabricated absolute helper aliases are required.

## Honest metadata residual and minimum review

Current authoritative __osInitialize_common target object SHA256 is 52a1df4288ba5644b96e6e7ea5958b22a2133fdef2a468eb5739ebc9cad53da9 (reloc-aware, no fallback). Six differing object words correspond to address representation; the fully linked retail instructions all agree. A supported normalization would need independently checked live/SDK types and symbol addresses for these exact relationships:

- gAudioDmaState is the low word of the real OSTime gAudioDmaCounter, at counter+4 (8002C360/8002C364).
- g_tlb_exception_vector and its three instruction aliases denote fixed SDK UT_VEC addresses 80000000, 80000004, 80000008 and 8000000C. SDK R4300.h defines UT_VEC as K0BASE; the other three vector destinations already use literal fixed addresses in the target.

Replacing the literal first destination with a named source extern changes compiler allocation substantially (strict 1660 / raw124); that source workaround was rejected. No scorer masks or changes are proposed. The metadata review must retain full unmasked linked-word equality and refusal on wrong/missing addresses or mismatched types.

Minimum context is already in the frozen TU. It declares the 16-byte ExceptionVector object, genuine OS low-boot variables, the actual SR/FPC/cache/TLB/SI functions, and the two u64 helpers. Root currently declares gAudioDmaBufferPtr as s32 and __osShutdown as u32, matching these definitions. Existing accepted neighbor body and O1 flag must remain unchanged. Production activation would require one coherent transaction owning both real data32 and BSS16, replacing their original physical slot and excluding their absolute symbol assignments. Existing generic composed-data machinery was used privately; no production ownership/lifecycle route is claimed here. Coordinate existing-O1 complete-module activation with the separately reviewed timer ownership transaction rather than adding another implementation.

## Private ROM, regeneration and rollback proof

ownership_proof.json and verify_private_ownership.py record a private model using the reviewed generic owned_data splitter/rewrite API and the genuine compiler object. The prior PI table owner and source-built game composition were retained. Actual BSS was renamed and allocated as NOLOAD at 800367D0, with exact size/address assertions. All owned globals remained real section definitions.

Candidate and restored baseline both produce original ROM SHA1 3f99351d7bb61656614bdb2aa1a90cfe55d1922c. Linker regeneration is idempotent. Moving BSS to 800367E0 refuses at link; changing the actual clock initializer links but fails the full-ROM gate (SHA1 a80d3c82ba92a8a97eae1a451b581b17eba17680). Rollback restores the original registry/linker/data/TU/symbol files byte for byte and passes the original gate.

This builder is the older genuine-ROM C13 private baseline, not the current shared root snapshot. It establishes physical ownership and byte identity; current-header standalone scoring, supported target refresh, current source-built game regeneration, production lifecycle and the final forced current-ROM gate remain parent integration requirements. Private workspace: watchman2 ~/agents/C/scratch/static-C18/api-model2, now rolled back. The verifier requires a fresh private C scratch workspace; inputs and exact hashes are recorded. Current owned_data imports owned_text and extract_candidates, so all reviewed dependency copies were supplied privately. No ROM bytes or binary objects are checked into this packet.

## Remaining ordinary wrappers

| Function | Best bounded baseline flags | Strict / raw-word residual |
| --- | --- | --- |
| dma_wait | O0 canonical | 580 / 28 |
| lzss_decompress | O1 canonical | 580 / 13 |
| inflate_decompress | O1 canonical | 729 / 16 |

initial_results.json and wrapper_controls.json record O0/O1/O2 baselines and eleven directed source controls at O0/O1. Volatile arguments/results, explicit failure branches and register spelling did not reproduce the original boot-code scheduling and branch structure. dma_wait's volatile-parameter control correctly refuses against its real prototype; it is not an accepted signature change. No large formatting sweep or assembler-driver changes were attempted. These remain genuine nonmatches and are excluded from coverage.
