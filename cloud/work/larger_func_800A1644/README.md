# Larger-function investigation: Controller Pak text encoder

## Result

**Draft research submission: useful reconstruction, not a byte match. No acceptance, coverage, image, or ROM claim.**

`func_800A1644` at `0x800A1644` is a complete **716-byte / 179-instruction** routine. The retained ordinary C candidate has the same 179-instruction native length, original 104-byte frame, and normalization-buffer base at `sp + 32`. Strict comparison still finds **149 of 179 words different**. All relocations resolve; there are no unverified references, relocation errors, or nonzero excess instructions. The object's final four zero alignment bytes are not function or coverage credit.

The reconstruction passes **21,432 deterministic differential cases** against a small interpreter executing the protected native instruction stream. This establishes tested behavior, not a compiler match. A second reviewer independently reran the behavioral check and reviewed the interpreter's delay-slot and branch-likely handling.

## Why this larger target

This was chosen instead of another 10–33-instruction helper: it has four loops, trimming/normalization, two output encodings, and compiler loop peeling/unrolling. It is a leaf with an ordinary three-input ABI and no caller-dependent register convention. The current master had no substantive reconstruction of this target. Selection excluded accepted functions and the other workers' targets.

The code is part of N64-specific Controller Pak handling, not an identified arcade routine. Actual direct callers provide stronger naming evidence than nearby historical function labels.

## Recovered behavior and ABI

Inputs are destination pointer in `a0`, source pointer in `a1`, and a length whose low eight bits are used in `a2`.

1. Remove trailing source codes 0 and 15.
2. Copy the retained codes backward into a local workspace, replacing embedded code 0 with code 15.
3. Remember whether any retained code is at least 66.
4. For the ordinary path, map each code through unsigned-halfword table `D_8011EAEC`, emit the low byte, and append one zero byte.
5. For the extended path, emit a leading `0xff`, emit each table result as high byte then low byte, and append two zero bytes.

There is no native bounds guard or error return to invent. The copied workspace permits in-place input/output; the differential tests include exact aliasing.

All four direct call sites pass either 16 (name) or 4 (extension):

| Caller label | Call address | Length | Destination offset |
|---|---:|---:|---:|
| `track_process_main` | `0x800A33B0` | 16 | 18 |
| `track_process_main` | `0x800A33D0` | 4 | 53 |
| `track_render_process` | `0x800A431C` | 16 | 18 |
| `track_render_process` | `0x800A432C` | 4 | 53 |

The **64-byte source-level workspace capacity remains an inference**, not a recovered declaration. It is consistent with the measured frame and the candidate's consumed locals. Its declaration is never padded or resized to obtain a score. Validation covers known caller lengths and length 0/1 boundary cases, not arbitrary out-of-contract inputs. No claim is made about the actual text-table contents, external call sites, or malformed inputs beyond that scope.

## What is preserved

- `candidate.c`: best defensible ordinary C; `s32 code` temporary; same native extent, strict **149/179** residual.
- `byte_local.c`: byte-typed temporary control; one extra native instruction and strict **175/179 plus one extra**. This retains the native redundant byte-masking shape more closely but does not fix allocation.
- `group.json`: actual function only, ordinary O3 pipeline, **empty claims**; no synthetic callers or helpers.
- `compiler_result.json`: exact source/object hashes, baseline commit, strict comparison and build recipe.
- `residual.json`: all 149 differing offsets, with fully resolved native/candidate instruction disassembly.
- `verify_semantics.py`: reusable research-only differential check; loads integrity-checked words using the unmodified repository scorer, rather than embedding a target dump.
- `semantic_result.json`: final frozen candidate's deterministic behavioral result.
- `review.json`: independent final replay and review findings.
- `checks.json`: cloud tooling tests, syntax and whitespace checks.

## Reproduce

From a checkout of `cabi24/SFRush2049-decomp` with the files under `cloud/work/larger_func_800A1644`, use the repository's approved IDO setup. No ROM or LAN builder is required for these two checks.

```sh
python3 tools/cloud/score.py group cloud/work/larger_func_800A1644
python3 cloud/work/larger_func_800A1644/verify_semantics.py \
  cloud/work/larger_func_800A1644/candidate.c --repo . \
  --output /tmp/encoder-semantics.json
```

The first command should fail the matching gate with `149/179 words differ`. That is the expected honest result. The second needs Python 3 and an installed host C compiler named `cc`; it writes temporary host files outside the repository and should report PASS for 21,432 cases.

Baseline: master `2f1f30c508495db08d61170c4a9b2409461ae326`. Compiler recipe: genuine-function-only IDO 5.3 whole-program O3, `-g0 -O3 -mips2 -G 0 -non_shared`, with the scorer's normal R4300 multiply-erratum setting. Protected targets, manifest, symbols, scorer, accepted sources and locks were not edited.

## Exact remaining blocker and sensible continuation

The first remaining difference is at offset `0x010`: the compiler moves the destination from `a0` into `t0`, whereas native code keeps it in `a0`. The trim/copy induction pointer then occupies `a0` instead of native `t0`; the extended-output pointer and length webs also differ, cascading through the unrolled loops and temporary register ring. In the `s32 code` candidate, elimination of the native byte mask offsets that extra carrier move, producing the same total length without producing the same instructions. **Equal length is not a solution.**

Natural output indexing, increment spelling, byte/int local forms, signedness, real local scoping, shared trim/copy locals, declaration placement, direct versus copied pointers, and O2/O3 controls were checked. Moving the existing workspace declaration after the two consumed integer locals fixes its native stack placement without adding anything. Independent `register`/`const` parameter controls do not fix the carrier. No manufactured pressure, unused parameters, dead operations, artificial helpers, padding, target edits, or scorer masks were introduced.

Do not continue by blindly permuting declarations or reflowing lines. A useful next attempt would need fresh evidence about the original source's genuinely consumed pointer/counter lifetimes or a compiler allocation diagnostic that explains the earliest `a0` carrier choice. Preserve the complete behavior, genuine ABI, stack layout and strict gate while testing such a hypothesis. Final integration would still require an independent native match plus the maintainer's image and full-ROM verification.

## Publication scope

This research is published at the user’s request so its source, executable test and remaining differences are preserved for continued work. It remains outside accepted submissions and uses an empty claims list. The unchanged repository CI checks protected paths, accepted locks and its normal repository tests; this research directory is not an automatic matching-submission target. A green CI result must not be read as a match or as a rerun of this standalone differential harness. The explicit strict-score command above continues to fail as expected, while the independently replayed behavioral result remains a separate research check.
