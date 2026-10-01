# Workbench pilot C1: n64-decomp-workbench `diagnose` on 5 near-miss functions

Agent C1, 2026-10-01. Everything scored with `~/agents/wb/loop.sh` (strict score plus `diagnose`) on Rocky, flags
`-g0 -O2 -mips2 -G 0 -non_shared` unless stated. A "run" below is one loop compile of one source variant; the counts
are approximate (I batched variants with a small driver that calls the same loop and keeps the score and lane lines).
Matches were copied to `cloud/matches/` and re-scored from that exact file with `tools/cloud/score.py fn`: bare MATCH.

## Result table

| Function | Start | End | Runs | What moved it | Tool verdict |
|---|---|---|---|---|---|
| `func_800EAFDC` | 8 (seed) / 4 (hand notes) | **MATCH** | ~18 | `f32 t = 0.0f;` then `if (D_80124518) {}` before the real code, plus `(0x75C + 0x760)` operand order | lanes useful, no lever named |
| `func_800D0B14` | 4 | **MATCH** | ~45 | `f32 d = 0.0f;` then `if (D_80124150) {}`, then `d = A - D154` as a named local | lanes useful, lever list empty |
| `func_8008A704` | 2 | 2 (not matched) | ~25 | nothing | diagnosis right, "-g0" lever vacuous, L79 explains why it is stuck |
| `func_8008C680` | 4 | 4 (not matched) | ~35 | nothing | misleading lever (phase levers for a ring-width problem) |
| `state_update_global` | 7 (5 with hand-note types) | 3 (not matched) | ~55 | `(v = arg0->f1A) != (t = D_80149D98 != 0)` in the `if` condition | right family, lever 17/19 gave nothing |

Matched (flags `-g0 -O2 -mips2 -G 0 -non_shared` for both): `func_800EAFDC`, `func_800D0B14`.
Files: `cloud/matches/func_800EAFDC.c`, `cloud/matches/func_800D0B14.c`.

## func_800EAFDC (MATCH)

Start: seed 8/19 words, `verdict=mixed(structural:1, register:8)`, lever `none-known`. Lanes:
`fp-pool target f12 f2 f0 / candidate f0 f2 f14 f12 f0` (diverges at slot 0), `fp-temp target f4 f6 f8 / candidate f4 f6 f8`.
Reading: the target has three pool webs, ours has five; the seed's named `a`, `b` are what create the extra webs.

Levers / steps:
1. Drop the named `a`,`b` locals, keep `t` (webs now f12 f2 f0, matching the lane): 8 -> 17 words, because the `lwc1 D` is now
   emitted after the `add.s`. Lane view said the allocation was right and only the schedule was off, which is useful.
2. Lever 7 (dead web, `if (D_80124518) {}` at the top): 17 -> 6 words, `D` loaded early but pool order flips to `f2 f12 f0`
   (the dead-read web is coloured first and takes f2).
3. Same with a dead read of `t` first (`if (t) {}` before `t` is assigned): 5 words, lane `f12 f2 f0` correct, but reading
   an uninitialised local adds `addiu sp,sp,-8` (frame 8).
4. Replace the uninitialised dead read by an initialiser, `f32 t = 0.0f;` then `if (D_80124518) {}`: 2 words, only the order of
   the two `lwc1 1884/1888` differs.
5. Swap the `+` operands in the inline expression (`0x75C + 0x760` where the target adds 0x760 + 0x75C): MATCH. (ugen evaluates the
   inline `+` right operand first; the named-locals form is source order.)

Most important single move: step 4, an initialised local ahead of the dead read, which fixes the pool colour order without
adding a frame slot.
Honest verdict: diagnosis correct (pool-position, not temp ring); no lever text applied directly. The lane view is what let me see
the target has only 3 pool webs, so the hand-note form with `a`,`b`,`d` locals was the wrong population. Lever 7's text
("dead web takes the next free pool slot") is right but incomplete: it does not say which web is coloured first, and it did not say
that the order of the dead read relative to a real initialiser decides the colours.
At the true MATCH `diagnose` still printed `structure-mismatch words=1` for the trailing padding `nop` (16-byte padded
`.text`); only the strict scorer says MATCH. The tool prints a warning about this, but the headline verdict is misleading.

## func_800D0B14 (MATCH)

Start: seed 4/35 words (diagnose `words=8` because of the 36-vs-34 padded count), `verdict=mixed(structural:4, register:4)`,
lever `none-known`, web substitution `w1 f2->f8, w2 f10->f2, w3 f8->f10`, `fp-pool` identical `f12 f2 f16 f0`.
Reading: same four pool webs, but the f2 web is the *difference* in the target and the *product* `t` in ours.

Levers / steps:
1. Name the difference instead of the product (`f32 d = A - D154;` and fold the product into the return): all registers right
   (f2 sub, f12 D150, f10 mul, f16 sum, f0), but `lwc1 D150` is emitted just before its use instead of at the top, which shifts the
   `lui at` pairs and the reloc order: 22 words.
2. Lever 14-style named locals (`d`+`t`, `d`+`s`, all three): 22-30 words, loads reorder further.
3. Lever 8/9 (expression dead read / read-count dial, `if (D_80124150) {}` x1,x2,x3 and in several positions): D150 loads early
   but takes the *first* pool colour (f2) and `d` gets f12: 18 words, then 4 words with `k = D_80124150;` (lanes `f2 f12 f16 f0`,
   the swapped pair; the diff is then `f2<->f12` and `mul.s` operand order).
4. `register f32 d`, dead reads of `d` (1..8, `!=0`, `>0`, `&&`): inert (4 words, identical lanes).
5. Initialise first: `f32 d = 0.0f; if (D_80124150) {} d = A - D154;` : MATCH.

Most important single move: step 5 (the same init-then-dead-read trick as EAFDC, found on EAFDC first and then re-applied).
Honest verdict: diagnosis correct as far as it went (webs, pool lanes), but `lever: none-known` and the suggested see-also
(pointer-add order) was irrelevant. The guide has the right family (levers 7-9) but not the rule that decides the order.
`register` and dead reads of the *named* fp local do nothing here; dead reads of an fp global that has other real uses work.

## func_8008A704 (not matched, best 2 words)

Start: seed 2/28, `verdict=schedule-mismatch`, identical pool and temp lanes (15/15, 2/2), `playbook=g0-schedule-probe`, lever
`line-order`. Target: `li t7,1` before `bnez`, `sw ra` in the delay slot; ours the reverse.

Levers / steps (all stay at 2 words unless noted):
1. Lever 3 (`-g0`): vacuous, the loop already uses `-g0` (the guide says so itself).
2. Lever 23/25 (line layout): already exhausted by the caller (1,024 layouts), not repeated.
3. L79 (read, not a variant): in a pre-branch block the leftover node wins the delay slot; as1 only fills from the fall-through when
   the branch is the last node picked. Ours has 5 nodes before the branch (lui, addiu, lb, addiu sp, sw ra) and steals `li` from the
   successor, the target has 6 nodes (the `li` is a node of the block). So it is a node-count, not placement, difference: the `li 1`
   has to be emitted before the branch by uopt/ugen, which no line change can do.
4. 23 source shapes aimed at getting the constant 1 emitted in the first block: `s8 v = 1` stored later, `!D`, else-form, goto,
   `(s8)1`, do/while(0), ternary and comma forms, assignment-in-condition, a local passed as the three `1` arguments, loop-once form,
   `if (D) {}` dead reads at four positions, `s8 one = 1; if (one) {}`: all 2, 5, 7, 8 or 24 words. In the `s8 v = 1` forms the `li` stays
   in the delay slot and the address web for `D_8011194C` is lost (8 words).
Final: 2 words.
Honest verdict: diagnosis correct (schedule, equal multiset, identical lanes). The listed lever (`line-order`, then L80's
"join the initialiser to the loop header") has no loop here; L79 is the useful part and says the residual needs an extra node, which I
could not produce. The tool is better at telling me to stop than at moving it.

## func_8008C680 (not matched, best 4 words)

Start: seed 4/40, `verdict=allocation-mismatch`/`register-ring-only`, `playbook=temp-fifo-phase`, lever `temp-ring`.
Lanes: `fp-pool target f14 f12 f0 f12 f0 / candidate f14 f12 f0 f16 f12 f18 f0`, `fp-temp target f4 f6 f8 f10 f4 f6 / candidate f4 f6 f8 f10 - -`.
The four diff words are `add.s f4` vs `f16`, the `div` source, `lwc1 f6` vs `f18`, `add.s` source.

What I found: the `cc -K` listing shows ugen itself writes `add.s $f16` and `l.s $f18`, i.e. in ours the ring continues
`f4 f6 f8 f10 f16 f18` (six wide); in the target it wraps `f10, f4, f6` (four wide). So the residual is the ring *width* for this
procedure, which L13 says is decided by uopt withdrawing f16/f18 (per procedure). A probe function with no variables
(`return (A*B)+((C*D)+((E*F)+(G*H)));`) hands out f16 and f18 as ordinary temps, so L13's "never handed out" is conditional, not a constant.
In `mulrecip` (see below) real pool webs landed in f16/f18 and the ring wrapped at four, which fits "uopt used them, so ugen lost them".

Levers / steps:
1. Lever 14-style locals (named numerator/denominator/result/constant): the locals are copy-propagated away, 4 words, identical output.
2. Operand swaps (`D + f(..)`, `1.0f + arg0`): 4 words, identical.
3. `1.0f / x` reciprocal form: ring wraps at 4 but structure changes (13 words).
4. Lever 7/8 dead reads of every global and of arg0, and expression dead reads (`arg0+1.0f`, `arg0-1.0f`, `1/arg0`, ratio): single globals are folded
   (4 words); dead reads of E4+E8 or of expressions hoist loads and change structure (35-39 words).
5. Parameter copies (`x = arg0`, late copy): 4 words, identical.
6. Flags -O1 (40), -O3 (4), -loopunroll 0 (4), -g1 (40): no change.
Final: 4 words, every variant that keeps the structure gives the same four words.
Honest verdict: partly misleading. `diagnose` classified it `register-ring-only`, "web-existence problem", and listed
levers 14-16 (phase and phantom-pop levers) plus "the era's coloring pass never hands these out". Those levers shift the *phase*; this
is a ring *width* difference (6 vs 4), which no phase lever reaches. The lane view's pool column also mixes in f16/f18 for the candidate,
which hides that they are ring temps in our build. I could not find the source construct that makes uopt withdraw f16/f18 without
emitting a web.

## state_update_global (not matched, best 3 words)

Start: seed 7/28, `verdict=register-permutation`, `playbook=forced-color-oracle`, web substitution `v0->v1 x3, v1->v0 x4`, pool lane
`target v0 v0 v1 a0 v1 v0 a0 v0 / candidate v1 v1 v0 a0 v0 v0 a0 v0`, temp lanes identical.
Reading: two webs swap colours: the unnamed global load `D_80149D98` (v0 in target) and the named `v` (v1 in target).

Levers / steps:
1. Lever 17 (K&R/implicit-int or `s32` return for `Input_ApplyPadConfig`, header edited and definition consistent): 7 -> 7, inert.
2. Statement order of `t`/`v`, `register v`, dead read of `v`, types u8/s8/u32: 7 (same as hand notes), s8/u8 on `t` blow up the structure.
3. Lever 7/9 (dead read of `D_80149D98`, 1 or 2 times, many positions and forms: `D+1`, `!D`, `D>5`, `D&4`): with the dead read after both
   assignments D gets v0 and `v` gets v1 (colours right) but `t` becomes a pool web `a1` instead of ugen temp `t6`: 6 words.
4. Named `u = D_80149D98` first: same 6 words.
5. Assignment-in-condition: `if ((v = arg0->f1A) != (t = D_80149D98 != 0))` gives the correct temps (`t6 t7 t7`), `v` in v1, but the
   `D` web lands in `a1`: 3 words (`lui/lw/sltu` register only). Declaration order, `u32 t`, `if (v)` do not change it; adding any dead read
   of D in this form breaks it (11 words).
Final: 3 words (best form in step 5).
Honest verdict: family right (pool colour order), lever text not sufficient. Lever 19 says source cannot reach this class; that is too
pessimistic: the dead-read and ordering moves reach the colours (step 3) and one more construct decides whether `t` stays a temp.
Lever 17 as written (gate: void callee after a direct call) did not apply here and had no effect.

## Where `diagnose` helped, where it did not, and what is not in the guide

`diagnose` was right about the mechanism in all five cases and its lane view (pool vs temp, with the slot where the sequences first
differ) was what turned two 4-word register problems into matches: it showed that the target had fewer pool webs than the hand-note
sources and that the wrong web was colouring first. It was less useful as a lever source: three of five reports ended at `lever: none-known`,
and for `func_8008C680` the temp-ring levers it printed address phase, whereas the real difference is ring width. Not in the guide, and
worth adding: (1) in IDO 5.3 fp and int pool colouring the *first-coloured* web gets the lowest register (f2 before f12, v0 before v1), a named
local with equal reference count outranks an unnamed global web, and an initialised local followed by a dead read of the global
(`f32 t = 0.0f; if (G) {}`) both pulls the global's load to the top of the block and keeps the local first, without adding a stack slot (an
uninitialised dead read adds an 8-byte frame); (2) the f16/f18 withdrawal of L13 is per procedure and conditional: a variable-free function
hands them out as ring temps, so a "ring width 6 vs 4" residual exists and none of levers 14-16 addresses it; (3) the `+` operands of an
inline float expression are evaluated right-first by ugen while named locals are source order, which decides a load-pair order that no
register lever fixes; (4) at a true MATCH `diagnose` still reports a nonzero word count from the trailing 16-byte padding `nop`, so only the
strict scorer is the gate.
