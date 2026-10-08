# Independent D328 private-body equality audit

PASS as research-only private-body evidence; zero accepted coverage.

- Independently decoded ELF32 big-endian ECOFF header, file descriptor, symbol, auxiliary end, and PDR records. D328 is stStaticProc at text+0x108 with matching stEnd length 124 and a 24-byte ra-only frame. Direct instruction-field checks also confirm ra-only save/restore and s0 as its only modified saved register.
- The full 124-byte/31-word linked span at 0x81000108 equals native 0x8038D328..0x8038D3A4. Both hash to 5d8fc0875c56105d63e0eaee161b6ec6900d742563b85d047035984fb0fa904a. Native targets were checked against current protected score.targets and the pinned git-show data asset in memory. No masks, missing/excess bytes, or neighboring-code borrowing.
- Independently reapplied all 414 object relocations in memory. Complete reconstructed .text and .rodata equal existing linked sections. D328 has nine relocations, all to external object pool, resource table, allocator, or E398; no owned-data references. Eight emitted E114 calls target its actual private body, matching eight authenticated native B callers.
- All eight complete source bodies and their logged reconciliations were checked against six original research source snapshots. D328 changed only by static visibility. Full approved schema is present. Seven helpers remain real static definitions; FCE0 is the sole nonstatic definition and sole keep entry. No fake root, stub, output discard, register constraint, noinline, or source tuning was added.
- The original canonical scorer still rejects D328 by name because only FCE0 has a defined ELF STT_FUNC. No symbol table/scorer changes were made. Seven other full bodies are independently confirmed NONMATCH.

## Limits and next action

Keep the claim as private-body equality research. Do not promote to canonical matching coverage. Original translation-unit/export boundaries, source/signature order, and the accepted E398 void-return declaration conflict remain unresolved. E398 is the accepted sign_extend_call alias; its result is consumed through the research return-bearing declaration. D328 assumes successful allocation and valid resource inputs. Separate real-call closure semantic replay is outside this static audit. Historical single-compile provenance is receipt-backed; this audit performed no compiler or linker invocation.

This directory retains review metadata and notes only. No native bytes, assembly dumps, compiled objects, or integration-sensitive tool/source fingerprints are retained here. Original source, object and linked artifacts were unchanged after review.
