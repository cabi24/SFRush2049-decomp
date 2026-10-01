# A7: real nearest-path caller closure — NONMATCH

Frozen 2026-10-01. No claims, no spliceable result. Four actual functions, no synthetic callers or callee stubs. This packet replaces the stand-in-containing historical test bed with a bounded real caller module and records unsuccessful controls; it does not certify the full navigation module's semantics.

Exact flags: `-g0 -O3 -mips2 -G 0 -non_shared`. Private Rocky A; sequential compilation, no integration edits.

| Function | Role | Strict differing words | Compiled / retail words |
|---|---|---:|---:|
| func_800E4300 | proposed target | 80 | 135 / 135 |
| func_800E451C | actual sole caller | 389 + 3 nonzero extra | 400 / 397 |
| func_800E398C | real sibling IPA callee | 592 | 567 / 599 |
| func_800E4B58 | actual ABI parent | 528 | 533 / 567 |

No compile errors or unresolved relocations. `scores.json` records strict builder results. Canonical `--claims` emits “No claims: work in progress, reported only” and exits 0, which is not a match verdict; its complete output is `score_claims.txt`. The default scorer still reports NONMATCH.

## Real closure and inputs

The direct E4300 + E451C closure is 532 words, but E451C has hidden car/nav inputs. Including its sole real parent E4B58 (567 words) and that parent's other real IPA callee E398C (599) gives a 1,698-word module. E4B58 saves s0–s8 and f20–f30 in retail and takes car in a0, so it is the only retained ABI root. E56F8/E6AF8 are unnecessary callers of that ABI root and were removed, together with their historical synthetic wrapper.

E4300 original formal homes prove `(point, previousPath, pointIndex, row, currentPath)`: a2, t2/home4, t5/home8, a1/home12, t0/home16. E451C loads previousPath from nav+40, pointIndex from nav+36, row from car+2018, and computes currentPath in the 0..3 loop. Its actual logical signature is `(flag, car, nav)`: E4B58 has three genuine calls, first with flag=1, then flag=0 after either forward or backward cursor adjustment. Parent stores car at incoming home304 and nav in local296; the retail s5/s6 carriers are established at these actual calls, not fabricated formals.

The target E4300 body follows the already hand-written real rows/points implementation: Section stride80, starts+0x30; Track stride8 with unsigned16 count and point pointer+4; TrackPt stride8 with signed16 XYZ. The other-row branch reads `ent[1].start[currentPath]` (current row+0x80), rather than treating D_8012E668 as a constant regardless of index. B8's standalone raw seed was used only for controls, after repairing that indexed read and making the wrapping index s32. Its original signed16 index otherwise introduces extra truncations.

## Source audit and limitations

Source context comes from `func_800E56F8/group.c`'s hand-written real bodies. Preserved actual cursor calls, fields, global tables, vector outputs, interpolation and smoothing operations. E451C local vectors are genuine arrays: direction[3], point[3], and v[2]. Removed the inherited unused `volatile s32 padv[12]` that was used solely to force a frame. The delivered E451C frame is 184 bytes versus retail232; no arbitrary padding remains.

Found and repaired a real historical context bug: E398C's call to `func_8008B3C8` takes transformed local vector `l`, not car `rec`. Retail s6 is sp+248, established before func_800A61B0 writes that vector, and remains the a0 argument at +0x30C. Both transform calls use actual input/output/matrix pointer triples; camera_blend_between receives car and the real scale local pointer.

The broader E398C/E4B58 first drafts remain far from matching. This packet audits the hidden input provenance, shared field layouts and selected real call semantics; it does not establish instruction-by-instruction semantic equivalence for all their arithmetic and branch paths. Their local home layouts, FP allocation and source control shapes remain unresolved. Context is informational, not accepted or claimed.

## Bounded controls and exact blocker

24 sequential controls are recorded in controls1/2/3.json. Four placements of the genuine loop-counter increment, invariant point/coordinate carriers, copy of loop bound, hoisted last-point index, caller integer carriers, indexed track walk, direct coordinate conversion spellings and removal of historical padding leave the best 80/135 unchanged. Coordinate carriers worsen to81; loading points before the guard worsens to87. Repaired raw m2c body unrolls to276–280 words; explicitly rolled variants give116/135 plus one extra. No new helper/getter, arbitrary extra argument or register-pressure caller was invented.

The initial hidden parameter pointIndex lands in compiled s0 instead of retail t5. Subsequent loop invariants rotate: retail points=t3, position XYZ=t4/t5/s0, lastpoint=s1; compiled coordinates=t3/t4/t5 and later points/lastpoint occupy s-register carriers differently. Source target size is correct; mere inclusion of the real caller and ABI parent does not fix allocation. Caller itself still uses compiled car/nav=s4/s5 instead of retail s5/s6 and has substantial home/shape differences. A new attempt needs authentic parent/caller source reconstruction, rather than another standalone ABI or formatting sweep.

Reproduce:

```sh
rsync -a cloud/work/ipa-groups/codex_path_search_a7/ Rocky:agents/A/wt/cloud/work/ipa-groups/codex_path_search_a7/
ssh Rocky 'cd ~/agents/A/wt && python3 tools/cloud/score.py group cloud/work/ipa-groups/codex_path_search_a7 --claims'
ssh Rocky 'cd ~/agents/A/wt && python3 cloud/work/tools/amatch/builder.py group cloud/work/ipa-groups/codex_path_search_a7 --json'
```

Frozen group.c SHA256: `df7a83b24512e14455513a9dae505b5ff91993dd66cef9a0ab55037c8183f54f`.
