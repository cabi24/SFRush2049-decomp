# lib_5610 whole-module control

Status: frozen, unaccepted. No coverage credit.

The fourteen original function slots occupy 8,176 bytes. This is the first complete genuine source closure in this research lane, extending the earlier four-function C21 context. The source combines existing native reconstructions in original order. No synthetic caller, extra allocation, padding, assembly, or initialized table was added.

## Recipe and entry audit

One g0/O3 compile was run on Rocky with toolkit 796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5. `compile_recipe.json` records every real cc-j/uld/usplit/umerge/uopt/ugen/as1 stage. `external_calls.json` identifies real outside callers of lzss_decode, inflate_entry and inflate_entry_alt. Those three entries were kept. Existing accepted inflate_flush_window and huft_alloc were also kept to audit preservation of their locked boundaries. All other procedures participated in the genuine module call graph.

`callgraph.json` records each native direct call and address. Native inflate_io_wait returns through t1 and uses unsaved s0–s4. The complete source closure still returns through v0, reproducing the previously known ABI discrepancy rather than solving it. Native DMA, cache, queue and allocator calls retain their actual arguments.

## Strict outcome

Both existing helpers retain byte-for-byte identical C bodies and match every native word after actual relocation: 7/7 words for inflate_flush_window and 11/11 for huft_alloc. None of the twelve remaining members matches. In particular, O3 reduces inflate_block to a two-word stub while expanding inflate_loop from 23 native words to 109 generated words. The native block/loop split is not reproduced. inflate_io_wait has 43 full-word differences and lzss_decode has 138 when length differences are included.

Every relocation resolves to actual named image addresses. There are zero unresolved, unverified or masked sites. The generated object contains 7,904 text bytes and no owned data/rodata/float-pool section. `full_closure_proof.json` compares exact ELF function extents with actual original native slot bytes; GNU assembler container alignment is excluded. Numeric opcode alignment is diagnostic only.

The current normal static TU uses g0/O2 and GLOBAL_ASM slot preservation. This failed full IPA result cannot replace that TU, and the normal static promotion framework has no general mixed static IPA publication mechanism. No infrastructure changes are warranted by this outcome. Existing source, locks, flags and build files were untouched.

## Provenance

`provenance.json` records hashes of every reused complete source, the two algorithm ancestors and all adaptations. `header_hashes.json` freezes the current private header snapshot. `preserved_helpers.json` verifies exact source-body identity for both accepted helpers. The native target assembly SHA256 and byte SHA256 are in `targets.json`. Raw objects, target words, disassembly and compiler intermediates are kept only in ignored build storage.

## Coordinator state caveat

The supplied priorities snapshot (private_head17211976) omits huft_build and inflate_dynamic, whereas both visible current TUs still list those functions as GLOBAL_ASM. This packet compares the visible accepted helper bodies and uses the historical C23 bodies for those two functions. Reconcile the snapshot eligibility before assigning coverage to this family; no new credit is claimed.
