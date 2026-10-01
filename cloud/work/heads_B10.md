# Worker B10 — six fresh registered heads

Selected six smallest ordinary eligible heads after excluding B5/A2 and refused overlaps. Authority is current protected blob sections plus read-only n64_target extraction record (addresses, instruction counts, assembly/object hashes retained in `heads_B10/provenance.json`). All population `extracted`. Root acceptance found E828 was already pilot-locked (`db8a62ab`); the worker selection missed that lock. It is a reverified alternative and adds zero coverage. Only D9CC and DF90 add new coverage: 664 bytes. Private Rocky B scratch/repo; one compiler/scorer at a time. No source, assembly, locks, farm state, tooling, integration or commits by worker. Coordinator performs independent strict/image/ROM acceptance.

| Head/address | Trusted extent | Baseline | Final strict | Flags |
|---|---:|---|---|---|
| func_8010E828 @8010E828 |132 B /33 words|27/33 O2|MATCH (already locked; +0 coverage)|-g0 -O2 -mips2 -G 0 -non_shared|
| func_8010D9CC @8010D9CC |300 B /75 words|72/75 O2;50/75 O3|MATCH|-g0 -O3 -mips2 -G 0 -non_shared|
| func_8010DF90 @8010DF90 |364 B /91 words|22/91 signed prior repair|MATCH|-g0 -O2 -mips2 -G 0 -non_shared|
| func_8010DBB8 @8010DBB8 |324 B /81 words|77/81 O2;64/81 O3|14/81 NONMATCH|-g0 -O3 -mips2 -G 0 -non_shared|
| func_8010C448 @8010C448 |320 B /80 words|65/80 clean prior repair|55/80 NONMATCH|-g0 -O2 -mips2 -G 0 -non_shared|
| func_8010C588 @8010C588 |320 B /80 words|65/80 clean prior repair|55/80 NONMATCH|-g0 -O2 -mips2 -G 0 -non_shared|

The three full candidate TUs were rescored after literal first-line flags comments were added; plain MATCH, exit0, no unresolved relocations. Final SHA256:

- `cloud/work/heads_B10/func_8010E828_reverified.c`: `18a94c7a0d7314e6e9027dcff37188b25f024f3ec1e76706357b605e78c72613`.
- `cloud/matches/func_8010D9CC.c`: `b90d7374a2b0975810c16d912c38e462fc2e314b5bf1f87fad463ed40076edcf`.
- `cloud/matches/func_8010DF90.c`: `754af3cb0fe13ff64c05598ff09dbb32381476b1dd143dc4c6b39eda7219f7d1` (supersedes earlier c843 hash, after obsolete unused temp_t5 declaration removed and exact TU rescored).

## E828 — reverified, already locked

Root restored the original cloud match, promoted source, and lock. The alternative below is archived only; no new coverage is claimed.

Seed's initial entity_transform_apply((void*)1,0) was wrong: target keeps original arg0 in a0 and sets a1=1. Corrected it, removed dead sp1C alias, moved existing node pointer declaration before short status carrier to obtain real home28. Explicitly preloaded node+108 position pointer before status branch, exactly as assembly reads it unconditionally. Final two-pointer call and zero-status handling match. No dummy padding/dead declaration retained.

## D9CC

Final statistic read is a word, not the seed's byte. Reconstructed actual vector[3] passed to vector_normalize_length, rather than three unrelated scalar stack locals. Moved real existing pointer declarations before vector to obtain vector home28. Assigned node statistic before node owner pointer, yielding target table load before owner store and statistic store after it.

The last six residuals came from carried node home40 vs52 and global head address materialization. Made actual returned node the first declaration/carrier, eliminated discarded sp34 alias, and introduced a genuinely used head pointer to D_801391F0 in the available declaration slot. Added proper linked-node structure (next at0, short active4, owner12, float timer16, word stat20) and pointer-return allocator declaration. Exact target frame56 and node/table/vector homes52/48/28. No padding or unused locals retained. Three ordinary spelling variants match; delivered typed node/head version is the reviewable one.

## DF90

Started prior `registered-heads/hand-variants/func_8010DF90_signed.c`, which had already repaired the signed per-player byte. Fixed final statistic word. Replaced separate mask calculation/store/check with natural compound update in the condition: `(child_mask &= ~(1 << signed_byte)) == 0`. This removes a persistent temporary web and restores the target instruction/register order (21/91→MATCH). Deleted obsolete unused temp_t5 and verified unchanged strict MATCH. No padding or hidden-group context needed.

## DBB8, nonmatch

Fixed statistic word and table-first node field assignment. Named first two dot products' sum (preserving add association) removes severe initial scheduling differences,64→18. Existing pointer declaration order gives target table home28, reducing16. Commuting the two first products within their sum yields14, but FP operand demand/coloring around the dot product and branch-dependent node/argument save homes still differ. Explicit product/load carriers, complete-sum expression forms and node declaration reuse worsen14–48. About15 bounded source-shape probes. Retained `heads_B10/func_8010DBB8_best.c` is not spliceable. No evidence justifies invented caller context.

## C448/C588, nonmatches

Started prior clean best65 rather than repeating old width/stride repairs. Target copies a real3-vector to stack0/4/8, computes another at12/16/20, and uses frame32 with all relevant reloads. Actual loops did not recreate that straight-line shape: they retain loops, larger frames and extra words. Vector structs plus volatile only input position produce55/80 without extra words. Both real bodies independently rescored; their upper vertical limits differ (18 vs4), preserved in their respective sources. No new match. Existing both-array volatile candidate53 had two nonzero extra words and was not treated as better acceptance. Abouteight meaningful probes on representative and transferred/rechecked controls on its actual paired body. Retained best files explicitly keep volatile position quirk; it is a lead, not demonstrated original C.
