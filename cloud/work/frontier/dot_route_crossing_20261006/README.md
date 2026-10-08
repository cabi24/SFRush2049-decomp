# Revived route-crossing helper submission

`audio_priority_find`, `[0x800BA46C, 0x800BA61C)`: **432-byte / 108-word strict MATCH** under the canonical `-g0 -O3 -mips2 -G 0 -non_shared` group pipeline, including its mandatory `-r4300_mul` backend flag. Only this helper is claimed. Accepted-byte and ROM-coverage gain remain zero.

This is a **revived verified submission, not a new discovery**. The earlier repaired route-tracker research already verified this helper with a complete genuine caller, but left it provisional because the caller was NONMATCH. The new matching source preserves every declaration and both complete bodies from `6b2e9e506fe3d2267a710e41c85af5364ccd00c7:cloud/work/frontier/dot_route_tracker/group.c`; only the introductory comment changes. That base has no accepted lock or submission under `cloud/matches` for this target. No admission policy is changed.

The older `cloud/work/frontier/w2b/groups/frontier_route_cross/group.json` does contain a historical `claims: [audio_priority_find]` entry. Its accompanying RESULTS calls the result provisional and leaves the claim to the integrator. That precursor used artificial caller padding, an invalid seventeen-entry index array accessed through entry nineteen, and a separate prefix-based length overlay. The later `dot_route_tracker` packet repaired that storage and removed padding, kept its own claims empty, and verified the unchanged genuine helper with the corrected caller. This submission revives that repaired lineage; it neither conceals nor deletes the legacy claim/source/evidence.

The source belongs in `cloud/matches/dot_route_crossing_20261006/`. It requires the complete actual `audio_mixer_main` caller in the same O3 unit, with only the caller in `keep`. No other context, inlined helper, deleted-static stub, fabricated caller, dead read, padding, pressure expression, assembly or volatile qualification is added. The canonical scorer compiles the genuine two-body group directly.

## What the helper does

The historical audio names are misleading. This routine returns the first route-point index whose direction-line side differs from that of the first qualifying point, or -1 when no crossing is found. Eligibility compares squared XZ distance to the signed integer range converted to float; range is not assumed to be an unsquared geometric radius. A type-2 route returns zero for tracker zero and -1 otherwise. The signed-halfword route argument is checked against the unsigned route count.

The real caller invokes it at `0x800BA8A8` and `0x800BA944`. Native O3 argument allocation reverses source order: route arrives in `a0`, tracker in `a1`. Both source parameters are genuinely consumed. The native initial read of an unspecified local stack halfword is never used to decide a result before the first qualifying point initializes the anchor. The host C source reads `prev` only after that initialization.

The repaired storage model is a 12-byte header followed by ten 80-byte tracker records, with twenty halfword indices at +34 and segment length at +76. The unchanged earlier packet's `layout.json`, read at the same base commit, documents its independent 812-byte initializer/copy and consumer witnesses. The present proof compiles the same layout assertions and executes against those explicit native offsets. The native route record is 16 bytes with a 12-byte scalar prefix; host route pointers have the host ABI and are not mistaken for N64 pointers.

## Complete proof

- ELF `STT_FUNC` size is exactly 432 bytes, with zero excess target words. All 108 relocated words equal the protected native target; no masks, unresolved references or unverified data remain.
- All four target relocations are checked: HI16/LO16 pairs for the two actual globals. All 23 relocations in the whole unchanged object resolve, with seven external address bindings.
- Independent GNU linking places the helper exactly at `0x800BA46C`, using `SUBALIGN(4)` to avoid silently rounding it to the input section's 16-byte alignment. The complete target and the entire object's relocated text agree with the project relocation result. The NONMATCH caller is laid out contiguously for this linker cross-check; this does not claim its native placement or image integration.
- No owned data, literals, BSS, jump tables or alignment tail exist in this two-body object. Text is 1,168 bytes: target 432 and caller 736.
- 4,159 deterministic fixtures compare an independent rounded-binary32 oracle, all native target instructions, the GNU-linked target, and the unchanged C under C89 UBSan/bounds/float-cast-overflow checks. This is 8,318 native/GNU executions. All 108 native offsets execute; all nine conditional branches take both outcomes.
- Cases cover every tracker 0–9, route counts 0–15, out-of-range nonnegative routes, high raw argument bits narrowed to valid halfwords, empty/single/multiple point lists, range and side boundaries, negative range, signed point extrema, type-2 shortcuts, and the final safe signed-loop count 32,767. Non-Y geometry varies independently of Y extrema.
- The unchanged prior host regression runs against this exact matching file: 395,264 caller cases and 23,094 helper cases, checking the complete 812-byte state, call order/arguments, totals, and read-only routes. Its three external caller helpers are explicitly test doubles, never compiler context or native-execution replacements.
- Five compiled wrong-source mutants are rejected: range-bound check, type-2 classification, zero-side comparison, distance inclusivity, and crossing equality. Three native controls reject an unsupported instruction, truncated extent and corrupted return.

`audio_mixer_main` remains **NONMATCH, 64/183 native words different**, with one nonzero excess word and 736-byte ELF extent versus 732 native bytes. It supplies genuine context only and earns no claim. Its source-supported three position stores remain unchanged, including their otherwise unused result; no new stores are introduced to shape compilation.

## Reproduction and portability

From a full-history repository with pinned IDO and MIPS GNU binutils available:

```sh
python3 tools/cloud/score.py group cloud/matches/dot_route_crossing_20261006 --claims
python3 cloud/work/frontier/dot_route_crossing_20261006/verify.py --check
python3 -m pytest tests/conveyor/test_dot_route_crossing_20261006.py -q
```

For a separate materialized packet, pass `--history-repo /path/to/full-history-repo`; focused tests accept `RUSH_ROUTE_HISTORY_REPO` for that same purpose. Compiler-dependent pytest checks skip cleanly when IDO or the GNU MIPS linker is unavailable. Native/source-only checks remain active.

The receipt binds only this packet's matching source, recipe, verifier and host harnesses, plus native target words and the base commit. Historical caller declarations/bodies, native caller census/comparison and base lock status are read with `git show`. No live lock, scorer, ownership tool, symbol manifest, production source or test-file hash is frozen. Later integration is not expected to invalidate the packet.

## Limits

The native fixture interpreter is a fail-closed instruction subset with finite binary32, default round-to-nearest arithmetic. It is not hardware emulation. NaN/Inf/FCSR variations, invalid pointers, aliases, negative tracker/route indexes, tracker counts above ten, route counts above fifteen, point counts above 32,767, concurrent mutation, and real gameplay are outside the behavioral proof. The caller's external helpers are not executed in native differential cases.

No full-repository aggregate, image, compressed-stream or ROM gate is claimed here. The parent must run the required with/without-IDO aggregate before publication; the independent checker owns merging and production acceptance. The existing changed-submission selector does not discover nested `cloud/matches` group directories; the added focused test explicitly replays the unchanged canonical group compiler and complete proof. No protected tooling is changed to broaden selection.
