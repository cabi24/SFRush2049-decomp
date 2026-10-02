# Static packet C23 — canonical inflate helpers, no matches

Four current unpromoted 0x5610 helpers were independently compiled at the actual shared O2 pin. No strict zero was found; nothing is ready for lock, promotion, coverage credit, or a flag change.

| Function | Retail body bytes | Baseline O2 strict/raw words | Best directed strict/raw words |
|---|---:|---:|---:|
| inflate_fixed | 456 | 6472/105 | 1991/75 |
| inflate_block | 420 | 2083/93 | 2083/93 |
| inflate_dynamic | 1936 | 12336/482 | 11936/482 |
| huft_build | 1740 | 9969/426 | 9969/426 |

Exact common flags: `-g0 -O2 -mips2 -G 0 -non_shared`. O1 controls were worse and remain diagnostic only. Scores enable stack differences; raw comparisons include complete target ELF text alignment. Retail body bytes above exclude standalone assembler padding (fixed8, block12, huft4).

`targets.json` records privately assembled current authoritative target objects and successful original-word assembly gates. This packet did not mutate DB targets, evidence, headers, sources, locks, layout or coordinator state. The objects are private at `~/agents/C/scratch/static-C23/*.target.o`. Current source header snapshot is copied from the C22 current-root snapshot; no synthesized replacement header was used.

Canonical sources come from local Perfect Dark `src/inflate/inflate.c` and Banjo Kazooie `src/core1/inflate.c`; retail adaptations follow the real assembly. The length arrays are genuinely heap-allocated (fixed288 words, dynamic316, huft288). The refill consumes little-endian16-bit words, not canonical gzip bytes. Border entries are u32, length/distance extras are u16, and all tables remain named external readonly data. The actual target retains gzip oversubscription and decoder length checks removed by Rare's variants. Dynamic construction preserves the 1264-byte length-array allocation while discarding its first temporary code table. No invented initialization, anonymous ROM data, assembly stub or ABI replacement was used.

The useful fixed-block source lever is reusing canonical `i` for loop indices and returned errors: frame72 and the first18 instructions match exactly, compared with baseline frame80. The frozen best body is `inflate_fixed.best.c`. Remaining differences include speculative branch-delay scheduling and allocator-state access. Accepted `inflate_flush_window` already uses a nonvolatile cast of the existing volatile allocator global; testing that same access in fixed-block reduced its object size but worsened strict score. Four small physical-layout controls and explicit error spelling also failed. Volatile input-pointer and canonical ternary refill controls failed for block/dynamic, so neither assumption is accepted.

Evidence: `baseline_scores.json` contains eight O1/O2 baseline builds; `fixed_controls.json` contains three allocation/error-variable controls; `fixed_guided.json` two allocator-access/error controls; `fixed_format.json` four small source-format controls; `refill_controls.json` six meaningful refill controls. Total23 successful compile/score controls; no giant formatting search or changed compiler driver. One initial scorer script mistakenly included variant filenames without matching target filenames and refused at a missing target; the packet checker now lists the four canonical names explicitly. No result from that refusal was accepted.

No linked/full-ROM proof is claimed for these nonzero candidates. Shared 0x5610 flags, existing accepted allocator/flush bodies and all mutable storage stay untouched. No remote search remains running.
