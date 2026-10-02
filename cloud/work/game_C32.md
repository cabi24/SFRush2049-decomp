# Game C32 — one verified complete ordinary leaf

Ready: **func_800A5560, 28 bytes / seven words**. The exact source flags are `-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul`. Both the original compile and a fresh independent replay pass strict scoring with stack differences enabled (0) and full unmasked linked comparison (all seven words exact). There are no extra words, unresolved or unverified references, or errors. Current actual game locks and prior name/address reports were checked before reservation; A56 C0294 and B37 are disjoint. No shared accepted source, header, layout, locks, state, or compiler configuration was changed.

The original assembly consumes one unsigned 16-bit color in a0, masks it with 65535, homes that real argument at 0(sp), then packs it into both halves of the original named 32-bit D_80124FC8. The canonical expression `(color << 16) | color` matches on the first compile. No new data, callees, padding, hidden arguments, or source operations are needed.

Source SHA256: `311b85532cdeb92f10f8d05f269c4f27c14e8d1b3ddf121a9ef8f9b1ee4a295c`. Current target SHA256: `9065b941eda0c539f4a35e71a3c539788fe66c500bedf309470e013d018824f1`. The read-only exported target object is Rocky `~/agents/C/scratch/game-C32/func_800A5560.target.o`.

Minimal ready files: `game_C32/func_800A5560.c`, `verification.json`, `frozen_verification.json`, `verify.py`, `context_provenance.json`, and `targets.json`. Copy those and the target object into independent scratch, then run:

```sh
python3 verify.py --repo "$HOME/agents/D/wt" --toolkit "$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5" --target func_800A5560.target.o --output independent.json
```

Two additional complete bodies remain honest nonmatches:

| Function | Retail bytes | Best strict score | Linked word differences |
|---|---:|---:|---:|
| func_800BEA3C | 28 | 200 | 4 / 7 |
| func_800A1BB4 | 184 | 930 | 32 / 46 |

BEA3C is a two-global setter. Its ordinary two-u32-input body does not reproduce the original argument-home stores. No narrowing evidence supports inventing a different ABI, extra formal, or dummy operation; no match is claimed. A1BB4 traverses actual two-level link/data pointers, tests the signed word at data+72, reads the unsigned part index at data+17, and checks the signed byte in the 772-byte car record at 133+index*40. Correcting the initial single-level pointer interpretation improves the score from 1045 to 930, but address scheduling and allocation still differ. The typed canonical source is func_800A1BB4.c (identical to .typed.c); the initial interpretation is retained as .baseline.c. All three latest results in scores.json use complete original extents without extras or unresolved references. No broad formatting sweep or stand-in caller was used. All private jobs are complete.
