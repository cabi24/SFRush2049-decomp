# w14e RESULTS: entity_spawn_init (0x8008EA10, 5544 B, 1386 words)

## Status
NONMATCH. No strict match, no provisional match, nothing spliceable. Best source:
`cloud/work/frontier/w14e/entity_spawn_init/best.c` (identical to the prior lean draft `cur.c`).
Scored in the whole-program unit only; the standalone `score.py fn` numbers are noted separately below.

## Best score (verbatim, Pi `blob_unit --tag w14e`)
```
FAIL entity_spawn_init: 1356 of 1386 words differ; compiled body is 1391 words, target 1386; ...
```
udiff: `want 1386 words, got 1391; differing rows 1181 (words), 149 (--ops), 149 (--norm); frame 728/520; unverified 22`.
Command: `blob_unit --tag w14e --remote-dir rush2049/scratch/frontier/w14e score entity_spawn_init --with <file> --neighbours`.

## Variants tried (unit scores, `us.sh`; the words/ops/norm columns are aligned-row counts)
| # | variant | words | ops | norm | frame want/got | verdict |
|---|---|---|---|---|---|---|
| 0 | prior lean `candidate.c` (= best.c) | 1181 | 149 | 149 | 728/520 | baseline |
| 1 | + one dead `func_8008B2E4(1.0f)` call | 1883 | 271 | 271 | 728/504 | extra site lowers frame by 16; worse |
| 2 | every `func_8008B2E4` -> static `nrnd` | 1684 | 548 | 548 | 728/216 | INVALID for frame: unresolved calls, nothing inlined |
| 3 | static local copy of Random+rand (`rnd_l`, `rand_l`) | 1684 | 548 | 548 | 728/216 | INVALID: statics stripped, `unresolved symbol rnd_l` |
| 4 | 19 sites as blocks `{f32 rr; rr = RND(K); stmt}`, RND expands rand inline | 2098 | 630 | 630 | 728/304 | frame wrong direction; shape worse |
| 5 | 19 blocks, RND = `func_8008B2B4() & 0x7fff` call | 1233 | 264 | 264 | 728/440 | frame closer, shape worse |
| 6 | 19 blocks, RND = `func_8008B2E4(K)` call (`v_blk_e2.c`) | 1181 | 149 | 149 | 728/592 | same shape as baseline, frame +72 |

Standalone `score.py fn` (builder, `-O3`): prior draft 1376/1386 words differ (cur); static-helper variant 1376 with
`rnd_l` unresolved, so the static helper is not a usable test on the standalone path.
Call-site count: 19 `func_8008B2E4` call sites in the body (the 20th textual match is the prototype). Retail has
19 `addiu ...,12345` LCG increments, so the site count matches.

## Frame findings (measured)
- Frame = fixed part (outgoing args + saves) + named locals + inlined areas + spill temps.
- With no inlined random calls the frame is 216. Each inlined `func_8008B2E4` site adds about 16 bytes in the
  baseline (304 bytes for 19 sites).
- Retail adds about 512 bytes on top of the 216, about 27 bytes per site. Our draft adds 304, about 16 per site.
  Nothing in retail's stack is a local array: `addiu ...,sp,N` occurs once (`addiu a0,sp,696`, the `delta` argument
  to func_8008E0B8), so there is no stack array to find.
- Block-scoped `f32 rr` locals are not shared across sibling blocks: 19 of them add about 72 bytes (variant 6).
- Spill temps in ours: 4 webs, a 24-byte temp area (400 -> 424). Retail spills many more float webs (its `swc1`
  stores at 668..716 are mostly spills), which is probably the main missing part of the 208-byte gap. Not verified.

## Why not 50 variants
Only 7 variants were scored. The frame gap is not explained by any per-site model I tested, and the best-shaped
variant (baseline) still has 149 mnemonic-level differing rows, mostly in the `pos`/`delta` float sequencing and
the seed-update ordering around the `multu ...,s3` sites. I stopped to write up instead of grinding frame guesses.

## Next steps
1. Count retail's float spill webs (the `swc1` stores to 668..716 and their reloads) and match them with
   forced colours (`force.sh`) before changing source.
2. Fix the control flow at the first `li at,8` / `bne t8,at` (retail: `lh t8,666(sp)` reloads a saved `surface`
   halfword from the frame, ours recomputes it), then the `beq a2` fast path.
3. Re-check variant 6 with the spill-temp count changed, because it has the right shape at 592.

## Integration
Nothing to integrate. No group, no `cloud/matches` file, no splice, no edit outside `cloud/work/frontier/w14e/`.
