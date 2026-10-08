# w14q RESULTS (partial: no function matched)

Builder scratch: watchman2:~/rush2049/scratch/frontier/w14q (base copy, src/blob, include, tools/cloud, asm/us/blob and
blob_matched.lock.json synced from the Pi, trace toolkit installed; fidelity.sh: 9 identical, 0 different).
Flags everywhere: `-g0 -O3 -mips2 -G 0 -non_shared`. Unit metric = `tools/trace/us.sh` (`words` aligned rows, `ops` mnemonic rows).
Standalone `vbatch` ranking is NOT a reliable proxy for these functions (strict counts ~41 for all 800E7A98 drafts vs 14 in the unit); rankings below use the unit.

## func_800E7A98 (43 words, frame 32/32): 13 words / 1 mnemonic row, NOT matched
- Drafts re-scored in the unit (b0): w11a 14/43 (ops 1), w12e 14/43 (ops 1), w10f 28/43, w7c 27/43, w8c 17/43.
  Standalone vbatch (`b0`) gave 41-43 strict for all five: not used for ranking.
- Round 1 (`b1`, gen800E.py, 60 of 1536 combos, unit-scored): best words 13 / ops 1 (v0026, v0029, v0046, v0050).
- Round 2 (`b2`, gen800E2.py, 60 of 1440 combos, unit-scored): best words 13 / ops 1 (v0010, v0011, v0023). No strict improvement over round 1.
- Residual (v0026 ops diff): retail's else arm keeps a real copy `move v0,a0` (h = next, one lw of D_801527C8, h in v0 in both arms,
  next in a0). Ours coalesces the copy, puts h in a1 in the then arm and next in v0. Body is 42 words vs 43.
- Stopped after 2 rounds (below the 3-round rule). Next: force next=a0, h=v0 with force.sh, then target the else-arm copy.
- Diagnose (flags passed explicitly): verdict mixed(constant:5, structural:7, register:13), lever declare-the-pair-later. This is standalone (strict 42), not the unit residual.

## func_8010FBE0 (32 words, frame 24/24): 5 mnemonic rows at best, NOT matched
- Drafts unit-scored (b0): w9d 6 ops rows, c_sep 6, b_vol 5, others 6-17.
- Round 1 (`b1`, gen8010.py, 300 combos sampled from 1920, standalone-ranked, then unit-scored 43 of them): best ops 5 (v0000 control, v0084 same source). No improvement.
- Residual: retail shares one `lui at,0x8015` between stores to D_80155288 and D_8015528C (see the field-symbol findings in w13f/RESULTS.md). No source variant in round 1 changed the lui count.
- Stopped after 1 round (below the 3-round rule). Not run: `wbgen` hill-climb, `force.sh`.
- Note: `b_vol.c` uses `extern volatile OSScTask` (a volatile global): disclose if used.

## func_800A7480 (34 words): 17/34 unit (w9d best), NOT matched
- Unit re-score: `w9d.c: FAIL func_800A7480: 17 of 34 words differ | words 18 ops 8`. Diagnose (explicit flags): lever none-known.
- No vgen rounds run in this lane (time). Next: vgen on the temp-ring LRU residual described in w13f/RESULTS.md.

## Integration
Nothing to integrate: no matches, no groups, no cloud/matches entries. Scripts: gen800E.py, gen800E2.py, gen8010.py (lane dir).
