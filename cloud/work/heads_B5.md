# Worker B5 — fresh registered heads

Read-only target/lock check: all three are registered, unaccepted heads. Current DB target object hashes were copied privately into the workbench targets; strict scoring used the protected assembly and symbols in private Rocky B checkout. No source, assembly, lock, farm state, commits or integration changes by this worker.

| Target | Strict baseline | Delivered result | Flags |
|---|---|---|---|
| func_8010E694 | 33/38 words differ | MATCH, 152 bytes | -g0 -O2 -mips2 -G 0 -non_shared |
| func_8010DAF8 | 13/48 at seed O3; 42/48 O2 | MATCH, 192 bytes | -g0 -O3 -mips2 -G 0 -non_shared |
| func_8010E72C | 49/63 words differ | NONMATCH, best 24/63 | -g0 -O2 -mips2 -G 0 -non_shared |

Exact full delivery files (rescored after flags comment was added):

- `cloud/matches/func_8010E694.c`, SHA256 `325eb06acb0286c1ec9428e83747849b54b54963c26b290dba6f3fae2ae7bdb7`.
- `cloud/matches/func_8010DAF8.c`, SHA256 `7a39ba68fd984100ac9f516bab276f9513fbd7a29413bbff0fa1c17b137cc857`.

E694: corrected the seed's reversed entity_transform_apply arguments (target uses original arg0 in a0 and literal 1 in a1). Marked the conversion factor volatile: assembly loads it three times. Replaced incorrectly inferred separate float locals with actual three-float vector. Declaration order is two temporary pointers, one unused s32, then vector; this gives vector at sp24 in the target 48-byte frame. The unused s32 is a compiler-home reproduction quirk and must remain visible to reviewers.

DAF8: final statistic was guessed as a byte by m2c; target loads a word. Placed node temporary before table pointer declaration to obtain table pointer home sp24. Moved statistic assignment before node owner assignment: this yields the target's table load before owner store and statistic store after it. Natural O3 compilation then matches; no artificial helper/predecessor context.

E72C: both table reads are words, not bytes. Scalar scratch was wrong: vector helper writes to a larger stack buffer; current best uses 13 floats to obtain frame96. This is not validated as the genuine vector size, so no match claim. Two-carrier seed, one-carrier rewrites, typed nodes/table arrays/48-byte rows, proper pointers, table-first stores, explicit preloads, stack homes, O3 and debug flags were bounded at about 40 probes. Best24 uses explicit word preload before owner store; it colors the table value in v0 rather than target t8, so later temporary webs shift. Target also wants player address cached before the float load and a distinct v0 address result before argument setup. Remaining discrepancy is compiler shape, not a one-word lead. Best source retained at `cloud/work/heads_B5/func_8010E72C_best.c`; do not splice it. Remote variants remain in private B scratch.

Coordinator must independently accept image and ROM gates for the two strict deliveries. Workbench masked diagnostics were never counted as acceptance.
