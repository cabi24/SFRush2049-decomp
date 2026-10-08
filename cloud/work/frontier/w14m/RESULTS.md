# Wave 14, lane w14m: place_cars_in_order (continued from w14i)

Builder scratch: `watchman2:~/rush2049/scratch/frontier/w14m` (copied from base; src/blob, include, tools/cloud,
asm/us/blob, blob_matched.lock.json synced from the Pi). Trace toolkit `install.sh --reuse wtk`, fidelity 9/9 identical.
Nothing spliced, committed, pushed or written to cloud/matches (strict 0 not reached).

| Function | Bytes | State | Best candidate | Scorer output |
|---|---:|---|---|---|
| `place_cars_in_order` | 544 | 8 of 136 words differ (standalone and unit); NOT a match | `pc/best.c` (= w14i `pc/place_cars_in_order_best.c`) | see below |

Flags: `-g0 -O3 -mips2 -G 0 -non_shared`.

Standalone (builder, IDO_DIR set):
```
python3 tools/cloud/score.py fn cand/w14m_best.c place_cars_in_order --flags "-g0 -O3 -mips2 -G 0 -non_shared"
  +0x17c  want 46002124 cvt.w.s $f4,$f4   got 460094a4 cvt.w.s $f18,$f18
  +0x194  want 440a2000 mfc1 t2,$f4       got 440a9000 mfc1 t2,$f18
  +0x1ac  want 440a2000 mfc1 t2,$f4       got 440a9000 mfc1 t2,$f18
  8/136 words differ
```
Unit (`us.sh`): `FAIL place_cars_in_order: 8 of 136 words differ | words 8 ops 0 norm 0 | frame 72/72`.

## Diagnose (round 1, on pc/best.c)
`verdict: mixed(constant:5, register:8)`, `lever: none-known`, `strict_words_differing 13`, `words_differing 8`, lanes:
fp-pool slot 1, fp-temp slot 4. Owning pass unknown.

## Residual (from udiff --all)
The whole residual is one FP web family in the `(u32)` store conversion:
- retail: `add.s $f18,$f6,$f16` (sum in f18), `cvt.w.s $f4,$f18` (cast result in ring temp f4), `mtc1 at,$f4` (2^31
  constant reuses f4), `sub.s $f4,$f18,$f4`, `cvt.w.s $f4,$f4`, `mfc1 t2,$f4`.
- ours: `add.s $f0,$f6,$f16` (sum in f0), `cvt.w.s $f18,$f0` (cast result in f18), `mtc1 at,$f18`, `sub.s $f18,$f0,$f18`,
  `cvt.w.s $f18,$f18`, `mfc1 t2,$f18`.
So the sum is colored f0 instead of f18, and the cast result takes f18 where retail uses the ring temp f4. The quotient
`div.s $f16` and the constant `f20` already match.
ctrace (`w14m` ordinal 904): 19 p1 decisions, two FP webs: `w50` (the sum, save 200, bb 12) colored `$f0`; `w78` (528.0) colored
`$f20`. The cast-result web does not appear in the p1 list, and the quotient is not a web (it is an ugen temp in f16). I did not
resolve which step assigns f18 to the cast result.
Trace evidence (earlier, w14i): forcing the sum to c29 (f18) fixes the sum but moves the quotient to f4, so the residual
does not close by colour alone.

## Round log (all standalone via vbatch.sh, `--flags "-g0 -O3 -mips2 -G 0 -non_shared" --jobs 2`)
| Round | Template | Variants | Best strict (top 3) |
|---|---|---:|---|
| g1 | `pc/best.c` + `wbgen.py` (commute 4, hoist 94; v0000 = control) | 99 | 8, 8, 8 (no file beats control) |
| b1 | `pc/t1.c`: 8 axes: pass init/cond (`pass == 0`, `pass != 1`, `!pass`), `pass < 2` vs `<= 1`, `cp` before/after p, `if (pass)`/`if (p)` inert, key/sum forms (5 options, incl. named uk/f2), store forms (4) | 300 of 8064 (seed 1) | 8, 8, 8 (control reproduces 8) |
| b2 | `pc/t2.c`: b1 with 10 self-contained sum forms incl. named quotient `q` before/after key load and two-branch `q` | 300 of 11520 (seed 2) | 8, 8, 8 (named-quotient forms 14-38) |
| b3 | `pc/t3.c`: b2 + phantom pops `if ((x == -1) != 0) {}` at 4 positions (levers 14/15) | 300 of 7.2M sampled (seed 3) | 8 (control), then 20, 29 |
| b4 | `pc/t4.c`: b2 + split/copy forms of the cast (`f32 g = f`, `if (g) {}`, `(s32) g`, `r` temp) | 300 of 23040 (seed 4) | 8, 8, 8 |
Total w14m variants scored: 1299. Stopped under the 3-round rule (g1, b1-b4: no strict count below 8).

## Integration
None. No standalone or unit strict 0, so nothing goes to cloud/matches and no group changes.

## What generalises
- A hit-or-miss vgen sample of ~300 from 8k-23k spaces does not reach a sum-colour change once the control is at 8; the
  residual needs a change in the FP colour/ring event order, not statement-level forms.
- Phantom pops in the pass body (levers 14/15) made things worse (20-41), not better, on this function.
