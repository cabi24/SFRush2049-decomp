# Game C36 — complete viewport scaling utility

Ready: **viewport_scale, 808 bytes / 202 words**. Exact flags: `-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul`. Both selected source compile and fresh independent replay pass canonical strict score 0 with stack differences enabled and full unmasked linked comparison: all 202 retail words exact, no extra words, unresolved/unverified references or errors. No shared acceptance files, headers, layout, lock/state or compiler/scorer settings changed; all private jobs are complete.

Two actual f32 arguments in f12/f14 supply x/y scales. The genuine unsigned byte count D_80146204 bounds an indexed loop over original D_8017A510 records of size72. Each record's pointer at offset4 leads to four unsigned16 bounds. Bounds0/2 are scaled by x, bounds1/3 by y, preserving the original unsigned32-to-f32 and f32-to-unsigned32 conversion paths (including original FCSR behavior). Existing record float fields36/44 are scaled by x, fields40/48 by y. The source retains the eight updates in retail order and the actual count reload. No calls, new data, dummy inputs, pressure variables or added work are needed.

The baseline pointer-induction loop already matches all arithmetic/conversions but differs in eleven loop-tail words (strict635). The genuine indexed-record form `record = &D_8017A510[i]` produces the original induction and branch scheduling exactly. A do-loop and moving the existing increment before final stores are unchanged nonmatches. Four bounded source compilations are retained in baseline.json and controls.json. Canonical source uses the indexed form; baseline is preserved as viewport_scale.baseline.c. Current live lock and registered accepted-address intervals were checked, as were full prior name/address reports and canonical complete-section existence. A61 B78F0 and B40 D084/D098/BC21C were excluded.

Source SHA256: `75c5ecc5f0996ad5639ddd599b7a25b13eefcc0b7c10908b744ecf061f4f6449`. Authoritative target-object SHA256: `8b729b24b20552c8d2d4f39ff7cb3ef74617c5d7d88579c46bdb26653a50193f`. Private read-only exported target: Rocky ~/agents/C/scratch/game-C36/viewport_scale.target.o. Complete authoritative target words are privately frozen in that same scratch directory as retail_words.json, SHA256 `b8ec969faf39f22210ba4e7335826c1a19daaacdd85fdfe74ed29d348fd8ef4c`; raw retail words are not committed.

Minimal frozen packet: game_C36/viewport_scale.c, verification.json, frozen_verification.json, verify.py, context_provenance.json and targets.json. Copy those and private target object to independent scratch, then replay:

```sh
python3 verify.py --repo "$HOME/agents/D/wt" --toolkit "$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5" --target viewport_scale.target.o --output independent.json
```

No ordinary numerical correctness substitute or masked comparison was used: acceptance remains exact original instructions and real global relocations.
