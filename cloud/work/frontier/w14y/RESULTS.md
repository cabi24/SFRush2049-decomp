# w14y: brake_light_update and world_effect_update (wave 14)

Result: **no match**. No file in `cloud/matches/` or group dirs. Nothing spliced, nothing edited outside this lane.

## brake_light_update (0x800A6244, 448 B / 112 words)

Semantics (checked line by line against the retail asm): not a brake light. It is a world-to-screen projection for
view `idx`: `d = pos - view->pos`; `r = func_800A61B0(d, view)`; `r[2]` clamped to >= 2.5f; `inv = 1/r[2]`;
`scr[0] = r0*inv*p->1C*p->24 + p->2C`, `scr[1] = p->30 - r1*inv*p->20*p->28`; both saturated to s16 into `sout`
(-32768/32767, else truncated); optional copy of `r` into `vout` when non-null. The frame is 72 bytes with
`ra` at 20 and `a0` at 72. The prior draft (`cloud/work/frontier/w9d/brake_light_update/best.c`) is already a
complete, semantically correct draft, so no new draft was needed.

Scorer (standalone, IDO -O3, the only verdict that counts):
- Prior best, w9d: `score.py fn cand/brake_prior.c brake_light_update --flags "-g0 -O3 -mips2 -G 0 -non_shared"`
  -> `74/112 words differ`.
- Diagnose (prior best): `verdict mixed(structural:23, register:33)`, lever `none-known`, `strict_words_differing 75`
  (words 113 vs 112, one extra word).

Batch rounds (`vbatch.sh`, standalone bscore, `--sort mnem`/`strict`):
| Round | Template | Variants | Best strict | Best mnem-missing |
|---|---|---|---|---|
| 1 | `b1/` t1.c, 8 axes (decl order, pad, clamp form, inv/p order, scr0/scr1 operand order, clamp form, vout form) | 300 (of 1024) | 74 | 13 |
| 2 | `b2/` t2.c, 10 choice points (adds clamp-block move, inline 1.0f/r[2], direct `D_8017A510[idx]`, pad2) | 300 (of 3840) | 74 | 13 |
| 3 | `g1/` wbgen one-edit neighbourhood of the 74 control (`best74.c`) | 279 | 74 (no strict improvement) | 13 |

Round-3 sanity: `g1/v0000.c` reproduces 74 (the control). The first round-2 control was accidentally 96 because
`pad2` was in option 0; fixed, and the control is `best74.c` (= `b2/v0001.c`).

Residual (from the word diff, `tools/wdiff.py`, mnemonic-level): the FP register colouring is the main problem.
Retail hoists `-32768.0f` into `$f2` and `1.0f` into `$f16` before the clamp, and uses `$f0` for `inv` and `$f10`
for the reloaded `r[2]`. Ours colours `r[2]` into `$f2` and the constants into `$f14`/`$f10`. Retail keeps `scr[0]`
and `r[2]` in memory after the call, which matches the w9d note (forcing `p1:w22=s,p1:w38=s` gave 27 rows).
The rest is as1 order at the clamp blocks. Stop rule reached (3 rounds, no strict improvement).

No variant beat 74 on strict, and the best mnemonic structure is 13 missing words. The function is structurally close
but blocked by colouring. Next: `tools/trace/force.sh` with `p1:w22=s,p1:w38=s` (w9d's lever) plus a
`$f2`-holding `-32768` web, and a source shape that puts both constants before the first clamp.

## world_effect_update (0x800EE8B4, 456 B / 114 words)

Unit scoring (the verdict that counts for this function; standalone compiles put the prologue at word 14 in ours,
which is a real scheduling difference, not a harness bug):
- w10f best (`cloud/work/frontier/w10f/world_effect_update/best.c`, no flags header, scored with `--flags` set via
  `/* flags: ... */` on line 1 in the w14y copy): `us.sh` -> `words 49 ops 31 norm 31 | frame 24/24`.
- `blob_unit score` on the same file: `FAIL world_effect_update: 100 of 114 words differ`.
- Diagnose (round-2 best): `verdict mixed(structural:34, register:20)`.

Structure (the dominant residual): the retail prologue (`addiu sp,-24; sw ra,20(sp)`) stays at entry. Ours schedules
the four zero stores, the `90.0f` store and the `while` loop head before the prologue, which costs ~14 words of shift
in the strict count. Mnemonic-level residual is 14 to 16 words.

Draft 2 (`d2.c`): all stores in retail order (zero stores, 90.0f, `while`, viewport, audio, the three aligned stores,
buffers, kinds, resources, `D_8015B250`/`260`/`C8`/`F4`). Unit: `words 51 ops 33`, worse than w10f.

Batch rounds:
| Round | Template | Variants | Best (bscore strict / mnem) | Unit check |
|---|---|---|---|---|
| 1 | `b1/` t1.c, 6 choice points (align block order X/Y/Z, kinds/resources order, `D_8015B250` placement) | 6 | not scored (only 6 variants) | (not run) |
| 2 | `b1/` t1.c regenerated with 10 choice points (zero-store order, `tv` local, `90.0f` placement, `bb` linked reorder of `D_8015B250`/`B260`, `0x22620u` literal, `~63u` vs `-64`) | 300 (of 512) | 96 / 16 (19 variants tie at 16) | `best_r2.c` (= `b1/v0071.c`): `words 54 ops 33` (w10f: 49/31) |
| 3 | wbgen on `best_r2.c` | 0 (no one-edit candidates generated) | - | - |

Notes: the first batch of 96 unit-scored round-1 variants was stopped (about 50 s per unit build on the shared Pi).
One variant at that stage (`v0010`) had `D_8015B260` computed before `D_8015B250` was set. That is a semantic error
caused by unlinked choice names, and it is discarded. The `bb` link fixes it.

Stop: the 3-round minimum is not fully met (round 3 produced no variants). Best kept: w10f's 49/31, unchanged.

## Integration
Nothing to splice. No match, so no `cloud/matches/` file, no group, no overrides.
Drafts and scoring tools: `cloud/work/frontier/w14y/{brake_light_update,world_effect_update}/`, `tools/wdiff.py`
(mnemonic word diff on the builder).

## What generalises
- Standalone bscore is only trustworthy when the prologue placement matches. A word-14 offset means the scheduler moved
  the entry stores above `addiu sp`. Read the diff with `wdiff.py`, not the strict number, before judging a draft.
- vgen choice names must be linked when two choices must move together (a reorder that split into two names produced
  a use-before-set program that still scored well: 14 mnem-missing). Check every top variant for order semantics.
- wbgen gives 0 candidates for functions made of store/load chains with no commutative operands. Do not expect a climb there.
