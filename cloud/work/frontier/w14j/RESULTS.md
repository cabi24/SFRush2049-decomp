# w14j results (round 2 mass variant testing)

Builder scratch: `~/rush2049/scratch/frontier/w14j` (base copy; src/blob, include, tools/cloud, asm, lock synced from the Pi).
Flags for all runs: `-g0 -O3 -mips2 -G 0 -non_shared`. No function reached MATCH. No cloud/matches or group files written.

## Results

| function | bytes | state | best draft | standalone scorer line (verbatim) | residual |
|---|---|---|---|---|---|
| func_8010D680 | 476 | 82/119 words differ, 2 own-rodata relocs unverified (not a match) | `func_8010D680/best.c` (= b1/v0000, w10f b.c) | `82/119 words differ (2 section-relative relocations unverified: .rodata+0x0 at +0xe8, .rodata+0x0 at +0xf0)` | see below |
| func_8010D3C0 | 704 | 61/176 words differ, 8 own-rodata relocs unverified (not a match) | `func_8010D3C0/best.c` (= b1/v0016, default placed first) | `61/176 words differ (8 section-relative relocations unverified: ...)` | default block placement |

Prior bests re-scored first (bscore, strict words differing): func_8010D680 w6d best 86, w10f b.c 82 (chosen as start);
func_8010D3C0 w7b best 72, w10b best 72 (identical), w7b b 112, w7b a 99 (w7b best chosen as start).

## Round log (strict = bscore differing words; ranking by strict, not by mnem column)

| fn | round | template | variants | best strict | notes |
|---|---|---|---|---|---|
| 8010D680 | 1 | t/t2.c (ok flag) + t/t2on.c (on as flag), 14 choice points | 400 (200+200) | 82 | 82 reached by both ok-flag and on-flag files; nothing below 82 |
| 8010D680 | 2 | t/t3.c (+ sign-ext `on=(s16)on;`, `i` type s16/s32, `(s16)` cast on model_visible) | 500 | 82 | no movement |
| 8010D680 | 3 | t/t4.c (+ 0 vs 0U in call args and ok=0, ok u32 vs s32) | 500 | 82 | no movement |
| 8010D3C0 | 1 | t/t1.c from w7b best: default placed start/end (plain, volatile), shaping `if(amount){}`, decl order, `cmp` order, v351 volatile | 64 | 61 | default at START gives 61 (end placement 72, volatile end 130) |
| 8010D3C0 | 2 | t/t2.c: default start/end, kind 1 vs 1U in default and case 351, decl order | 16 | 61 | no movement |
| 8010D3C0 | 3 | t/t3.c: 11 points (decl order of car/vehicle, `&x[i]` vs pointer arithmetic, `&=~6` forms, `(car->flags&16)!=0`, flags order, extra_flags order) | 320 | 61 | no movement |
| 8010D3C0 | 4 | t/t4.c: whole switch vs if-chain (`else if` chain) on top of t3 | 320 | 72 | if-chain alone is worse (76 floor); switch kept |

Total scored: func_8010D680 ~1400 variants (3 rounds), func_8010D3C0 ~720 variants (4 rounds). Both stopped under the
3-rounds-without-improvement rule. Batch scoring used `cloud/work/frontier/tools/vbatch/vbatch.sh`; ranking note: its
`score.txt` sorts by mnemonic-missing first, so re-sort by the strict column before reading the best.

## func_8010D680 residual (from the -O3 dump of the 82 draft, vs retail)

- Entry: retail keeps `on` sign-extended in a1 (`sll t6,a1; sra a1,t6; bnez a1`), ours sign-extends into t7 and
  hoists the sll/sra above the prologue. No variant moved this.
- Constant hoist: ours executes `move a1,zero` (the `0` arg of `model_data_load`) above the `bgez` in the flags test,
  so the `bgezl` branch-likely with `move a0,t0` in its delay slot is not produced (ours: `bgez` + `nop`). Retail's
  else path has `move a0,t0; move a1,zero; jal model_data_load` in the taken block. The `0`/`0U` variants (round 3)
  did not change this, so the sharing is not the `0` literal alone.
- `lui v1,0x8003` (D_8002EB94 address) is hoisted above the `lbu/andi/beqzl` test; retail has it after the branch.
  The volatile qualifier on D_8002EB94 is kept as in the source; its effect on the hoist was not tested.
- Jump-table reloc (`lui at`/`lw t6,0(at)`) is the unverified `.rodata` pair; the switch body (350..360, default -1)
  matches retail instruction for instruction.

## func_8010D3C0 residual

- Only region that differs: the default block (`lui v1,0x8012; li a0,1; lw v1,7524(v1)`), which retail places after
  the case-360 block and reached by the table's `beqz at`. Ours with default last is hoisted above the `sltiu` (PRE
  moves the shared `D_80121D64` load/`li a0,1` into the switch head, same as case 351). Ours with default first
  matches the 351-shape but is in the wrong position (61). Round 1 per-option floors: default end plain 72, end
  volatile 130, start plain 61, start volatile 132; the volatile case-351 choice changed nothing.
- Remaining mismatch is the 8 own-rodata relocation pairs (jump table and float literals), unverified, not a code diff.
- Next hypothesis: the default block in retail is not a `kind=1; amount=D_80121D64` duplicate of case 351 at the
  source level. Test a default that writes through a different expression (e.g. a `kind` set from a separate
  table or `amount` read through another global) only if the default value is confirmed from the arcade ancestor.

## Shaping / disclosures

- func_8010D680 best (w10f base): `static s32 model_visible(s16 id)` helper inlined; `s32 pad;` unused local present in
  the w10f base, kept as a frame residual candidate; not a standalone match.
- func_8010D3C0 best: inherits w7b shaping: two `if(amount) {}` statements (retail-shape devices) and the
  "consumed view" header; default placed first in source.

## Integration

Nothing to integrate: no strict or group match. No overrides, no groups, no splice. Do not promote either `best.c`.

## Generalises

- Choice points that only reorder declarations, compare orders, or change literal types do not move the default-block
  or the delay-slot shape; those are as1/PRE effects. Check the diff region first (udiff/full.py) before spending a
  round on source-level forms.
- Score sorting: `score.txt` sorts by mnemonic-missing first; sort by strict for the real best.
