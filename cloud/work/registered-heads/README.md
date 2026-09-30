# Registered-head matching round (master 05935a7e)

All 45 newly registered heads were triaged with `amatch.triage.features` and `ipakit.deps.Model`. Each has an m2c seed, a seed-generation record, and a strict compile result under `seeds/<name>/`. ABI candidates were tried at `-O2`, `-O1`, and `-O3`; IPA candidates were tried as whole-program `-O3` groups. These are work products, not coverage or match claims.

## Matches

- `func_8010C7CC`: six words, four-argument handler stub returning zero.
- `func_8010FD60`: seven words, address masking/mapping helper.

Both minimal submissions in `cloud/matches/` were independently checked by the canonical `tools/cloud/score.py fn` on watchman2: plain `MATCH`, no unverified relocations, exit 0. Their first lines contain only the exact flags comment. They have **not** been spliced in this round; the existing game lock remains at 490 functions / 55,652 bytes (8.60%).

## Classification discrepancy

The conveyor scan gives the maintainer’s 22 IPA / 23 ABI. The cloud dependency model gives 20 IPA / 25 ABI (16 IPA-caller, 4 IPA-both). This difference also exists with the old concatenating loader: it is not caused by deduplicating stale sections.

| function | conveyor | ipakit | evidence |
|---|---|---|---|
| `func_8010DCFC` | preserver `a2` | ABI | no live-across site in the cloud CFG model |
| `func_80108F40` | preserver `a1,a2` | ABI | no live-across site in the cloud CFG model |
| `func_8010C974` | preserver `a2` | ABI | no live-across site in the cloud CFG model |
| `func_8010E0FC` | ABI | IPA-caller | passes `t2` to `string_copy_format`; preserves `at,t2` across `math_utility` |

All 20 cloud-model IPA candidates call at least one non-ABI callee. None qualifies for the suggested “IPA caller with only ABI callees” shortcut. Both classifications and the individual callee classes are retained in `triage.json`; no conveyor population/status was changed.

## Tooling corrections

Five stale region files remain in the protected manifest. The canonical scorer concatenates repeated sections for 15 functions, doubling their apparent extents; this also hides the refused `func_80104B14` under an oversized `highscore_entry_anim` body. The cloud corpus loader now verifies the manifest/file set through the canonical scorer, reads identical repeated sections once, and rejects conflicting copies. Protected assembly and the canonical scorer were not edited. The current audit now finds exactly the seven refused heads.

The group-generator disassembly previously let a historical alias replace the canonical section name. m2c then missed the callee signature/IPA map. Canonical function names now take precedence over aliases; a synthetic call regression test checks this. Regenerating the `func_80105DA8` seed reduced its -O2 diff from 64 to 35 words.

This checkout already has a patched, dirty m2c submodule. The default group generator tries to apply the same patches again and falls back to stubs. Seeds here were generated read-only with `GROUPGEN_M2C=$PWD/tools/mips_to_c`; all 45 target bodies produced m2c output. Some discovered context bodies remain stubs, explicitly recorded in `seed-info.json`. A compiled seed may still contain inference mistakes and is not a match.

## Results

| function | cloud class | words | mode | best strict diff | result |
|---|---|---:|---|---:|---|
| `func_8010E72C` | ABI | 63 | single -O2 | 49 | unmatched |
| `func_80105DA8` | ABI | 64 | single -O2 | 35 | unmatched |
| `func_8010DAF8` | ABI | 48 | single -O3 | 13 | unmatched |
| `func_8010D9CC` | ABI | 75 | single -O3 | 50 | unmatched |
| `func_8010C448` | ABI | 80 | single -O2 | 76 | unmatched |
| `func_8010C588` | ABI | 80 | single -O2 | 76 | unmatched |
| `func_8010DBB8` | ABI | 81 | single -O3 | 64 | unmatched |
| `func_8010E694` | ABI | 38 | single -O2 | 33 | unmatched |
| `func_8010C2E4` | ABI | 89 | single -O2 | 89 | unmatched |
| `func_8010DF90` | ABI | 91 | single -O2 | 25 | unmatched |
| `func_8010D85C` | ABI | 92 | single -O2 | — | compile blocked |
| `func_8010E828` | ABI | 33 | single -O2 | 27 | unmatched |
| `func_8010C7F4` | ABI | 96 | single -O3 | 78 | unmatched |
| `func_80108DA8` | ABI | 102 | single -O2 | 94 | unmatched |
| `func_8010E4E4` | ABI | 108 | single -O3 | 105 | unmatched |
| `func_80105B74` | ABI | 141 | single -O2 | — | compile blocked |
| `func_8010DCFC` | ABI | 165 | single -O2 | 153 | unmatched |
| `func_8010C02C` | ABI | 174 | single -O2 | 169 | unmatched |
| `func_8010FD60` | ABI | 7 | single -O2 | 0 | strict MATCH |
| `func_8010C7CC` | ABI | 6 | single -O2 | 0 | strict MATCH |
| `func_80109A60` | ABI | 317 | single -O3 | 312 | unmatched |
| `func_80108F40` | ABI | 330 | single -O2 | 323 | unmatched |
| `func_80109468` | ABI | 382 | single -O2 | — | compile blocked |
| `func_80106D94` | ABI | 401 | single -O2 | 364 | unmatched |
| `func_8010C974` | ABI | 659 | single -O2 | — | compile blocked |
| `func_80107EDC` | IPA-caller | 158 | group -O3 | 145 | unmatched |
| `func_8010BC84` | IPA-caller | 232 | group -O3 | 217 | unmatched |
| `func_8010E8B4` | IPA-caller | 85 | group -O3 | 83 | unmatched |
| `func_8010B7FC` | IPA-caller | 115 | group -O3 | 113 | unmatched |
| `func_8010B5D0` | IPA-caller | 139 | group -O3 | — | compile blocked |
| `func_80106B3C` | IPA-caller | 150 | group -O3 | 127 | unmatched |
| `func_8010A53C` | IPA-caller | 154 | group -O3 | — | compile blocked |
| `func_8010B9C8` | IPA-caller | 175 | group -O3 | — | compile blocked |
| `func_80106874` | IPA-caller | 178 | group -O3 | 173 | unmatched |
| `func_80108AB0` | IPA-caller | 190 | group -O3 | 187 | unmatched |
| `func_801084D4` | IPA-caller | 375 | group -O3 | 351 | unmatched |
| `func_80108154` | IPA-caller | 224 | group -O3 | 204 | unmatched |
| `func_8010E0FC` | IPA-caller | 250 | group -O3 | 248 | unmatched |
| `func_80109F54` | IPA-caller | 376 | group -O3 | 363 | unmatched |
| `func_8010F218` | IPA-caller | 611 | group -O3 | — | compile blocked |
| `func_80105EA8` | IPA-caller | 627 | group -O3 | 623 | unmatched |
| `func_80102448` | IPA-both | 334 | group -O3 | 327 | unmatched |
| `func_80102980` | IPA-both | 364 | group -O3 | 345 | unmatched |
| `func_80103D28` | IPA-both | 629 | group -O3 | — | compile blocked |
| `func_8010EA08` | IPA-both | 514 | group -O3 | — | compile blocked |

Additional hand probes changed `func_8010DF90` from 25 to 22 words by correcting a signed-byte access, and the paired `func_8010C448`/`func_8010C588` from 76 to 65 words by using stack arrays and byte strides, then 53 differing words with volatile arrays (82 emitted versus 80 target words). These remain partial; sources and results are in `hand-variants/`. The remaining large diffs and failed compiles need signature/type/structure repair, not an estimated 15–30% autopilot yield. This round’s measured initial strict yield is two of 45 targets (two of 25 single-function starts); the two matches are exceptionally small.

## Verification

The required full local pytest invocation selected 1,203 tests (748 passed, 455 skipped) and exited 0; its exit was saved separately in `/tmp/heads-next-complete-pytest.exit`. The focused heads/groupgen/diagnostic tests also pass. IDO ran only on watchman2, in `~/rush2049/tmp/heads-05935a7e`, using its existing IDO 5.3 installation. No ROM input, lock, locked C source, canonical scorer, builder checkout, or m2c submodule was changed.

The register-rotation mechanism and controlled replay are documented in [register-ring/README.md](../register-ring/README.md).

## Regenerate seeds

To create a fresh scratch set without overwriting the curated candidates:

```bash
GROUPGEN_M2C="$PWD/tools/mips_to_c" \
  python3 cloud/work/registered-heads/prepare.py --out build/heads-next-scratch
```

The tool reclassifies the 45 listed targets with the current cloud model, emits
singles/groups with empty group claims, and records decompiler output status.
Compilation/scoring must be run on watchman2 using the recorded flags. For
ordinary registered targets use `tools/cloud/score.py fn` or `group`; groups
containing discovered context targets additionally need their original image
extents, as the existing `amatch.builder` extra-target mechanism specifies.
