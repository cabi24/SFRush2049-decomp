# w23b - static boot tail: func_8001F13C (voiceAllocate) + func_8001A658 (synthHandle) (wave 23)

IDO 7.1, `-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul`, toolkit 4586d230. Builder scratch
watchman2:~/rush2049/scratch/frontier/w23b (copy of `base` + rsync of src/{blob,rom}, include, tools/{cloud,conveyor,
asm-processor,workbench.py}, asm/us/{blob,boot_tail,nonmatchings/rom}, cloud/work/boot_tail_promotion, locks, symbol
files, baserom, assets/us/data.bin; `tools/ido-static-recomp/build71/out` symlinked to the 4586d230 `ido/`). trace71
installed with `install71.sh ... --reuse w22c`, the 5.3 toolkit with `install.sh ... --reuse wtk`. Targets = reloc-aware
inventory objects (w17k fetch_targets.py). Nothing locked, promoted, spliced, or committed. Static numbers only.

## Headline

No match. Both near misses moved a long way, each by traced, device-free levers.

| function | B | state | flags | scorer line (check.py = lock add verify_entry) |
|---|---:|---|---|---|
| func_8001F13C | 1456 | near miss, `func_8001F13C/best.c` | -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul | `LOCK: true=2045 strict=2069 reloc_blind=130 objdump-dr DIFFERS(201 rows) (436 rows)` (w22a: true=2420) |
| func_8001A658 | 2812 | near miss, `func_8001A658/best.c` | -g0 -O2 -mips2 -G 0 -non_shared | `LOCK: true=2770 strict=2770 reloc_blind=83 objdump-dr DIFFERS(319 rows) (749 rows)` (w22b: true=7725 strict=7983 reloc_blind=270, 364 rows) |

Because neither function matches, there is no sources/, ctx71 jsonl, prove.log, or promote_batch line. The
integration notes for func_8001A658 still hold for when it closes (see below).

## func_8001F13C (voiceAllocate): 2420 -> 2045

**Fixed: the fxFlag hoist.** 5.3 pretrace (single-file snapshot, `tools/pp.sh` + `snap.sh`): bit 84 =
`cvt.J(fxFlag)` had antloc 27,71,104, DELETE 104, INSERT 92. The final `if (fxFlag != 0)` compiles to the
converted value, which is also computed in the two search loops (`fxFlag == ...external4C`). That made it partially
available and anticipated after `voice == -1`, so PRE inserted the narrowed copy (`move v1,s1`). Writing the test
as `if (fxFlag)` gives the raw-parameter test (`neq.L(varP,0)`, the same form as the entry test), and the hoist goes
away. The whole unlink/tail block then matches retail. It is natural C, not a device.

**Open: the mpy(voice,416) hoist** (w22a item 3). Pretrace vectors for bit 95 `mpy(voice,416)`: antloc
{31,75,88,101}, alters {16,32,34,58,60,76,78} (the voice stores), DELETE 88, INSERT 60 (the D_800504C0 path) and 87
(the split edge after the `voice == -1` return). Findings:
- The insertion needs partial availability at `_update`. The ternary-kill oracle (`voice = cond ? i : voice`, v3) and
  a separate best variable (v7/v8) both remove the hoist (multu at `_update`), so PAV is the condition. uopt
  splits critical edges itself: `if (voice != -1) goto _update; return -1;` still hoists (v1).
- A self-assignment `voice = voice;` before the exit check is removed before code motion, so it kills nothing (o2 ==
  v5). A `(u32)voice` cast at `_update` changes only the ixa, not the mpy (o1). Loop shapes (for, while+next, else-if,
  `||` forms: e1-e4, v2) change nothing.
- A separate best variable cannot coalesce with voice: forcing best and voice both to a0 is declined because the copy
  makes them interfere (force71 on v10). Reusing `num` gives v1 (num's web), not a0. Both also cost frame slots.
- The maxVoices home (sp+24, retail sp+28) is home 1 behind the coloured D_8004FA1A web (w8, home 0). Dropping `sfv`
  (5 declared words) puts it at 28 but recolours everything (v9: 6755). So it is either one local fewer in retail or a
  different web order. Not resolved.

Next hypothesis: retail kills voice's availability on the skip-loop exit with a store that disappears after code
motion. Two candidates: a dead store that survives "removal of redundant stores", or a store in a block that ugen
merges. An instrumented 7.1 uopt with a PRE delete/insert oracle (the 5.3 `nopre` cannot clear insertions) would show
whether clearing INSERT 60/87 alone gives retail. Stop rule reached (~2 h here on top of w22a's 3 h).

## func_8001A658 (synthHandle): 7725 -> 2770

Traced with ctrace71 (`[CDX] calleecost`, `CUPLOG=1`) and the 5.3 pretrace on a single-file snapshot. All levers are
device-free, and each moved the colouring the traced way:
1. **Callee-saved cost.** Base: 159 blocks, cost 39.75. midi (tot 40.0 > 39.75) took s3, which left no s-register for
   the 0x7F0000 web (w727: best 310, split, rematerialised `lui`). The pan clamp written as one nested ternary
   `pan = pan < 0 ? 0 : pan > 0x7F0000 ? 0x7F0000 : pan;` (the same form w22b uses for the ramps) adds exactly one
   block. That gives 160 blocks and cost 40.0. midi ties, and a tie goes to split, so 0x7F0001/0x7F0000 get s7/s8 and
   the s-register map (s3 address, s4 2, s5 255, s6 i) becomes retail's. A compiled-out `do {} while (0);` showed the
   same effect (b1, oracle only).
2. **span.** The w22b top `span = 0;` is PRE'd as a store: 5.3 bit `str(span,0)` DELETE 84,113 INSERT 10. It is hoisted
   to the loop top, partially available through the back edge. span then lives across about 25 calls (a3 cost 250) and
   is split. GameCube writes `lastSPan = 0` in the D_8004F2F8 != 0 arm. Writing `span = 0;` there and in the
   midi == 0xFF arm, with no top store, gives span in a3 (cost 20, tot 50) with the 104(sp) saves around the two calls.
   The behaviour is the same.
3. **midi.** `if ((midi = sv->midi4A) != 0xFF)` in each D_8004F2F8 arm makes midi one web with tot 50 > 40. It is
   coloured a1 and saved around the four inpGet calls, as in retail.
4. **Loop shape.** `for (i = 0, sv = D_8004BEB8; i < D_8004FA18; i++, sv++)`: the sv increment moves into retail's
   latch and the prologue order then matches.
5. **Volume products.** `vol = sv->volume30; pan = sv->panning38;` come before the three fader products (retail loads
   them straight into s0/s1). This was the big drop: 220 -> 112 differing instructions.
6. **cpitch.** `cpitch = sv->sweepOff40[1] + (sv->sweepOff40[0] + convert(...))` gives retail's operand order (load [0]
   first, `addu v0+[0]`). Seven orders were tried. The GameCube two-statement form does not reproduce it.
7. **midi's home.** modulation/portamento are declared `s32` (getter results), so midi's home is sp+0x48 (-64) as in
   retail. The 16 declared locals are otherwise w22b's order.

**Remaining (about 112 differing instruction texts, mostly the knock-on temp ring):**
- Retail has `move a3,zero` (`span = 0`) at the envelope test and keeps the midi == 0xFF arm store. Our top-store
  variants are hoisted to the loop top (PRE), and the arm store is deleted as redundant. A dead `span = 1;` after
  func_80014A74 kills the back-edge availability and puts the store at retail's spot (q1, oracle only). It does not
  bring back the arm store, so retail behaves as if no store-PRE ran on span. The cause is unexplained.
- midi: retail loads `lbu a0,74(s2)` once in the volume block and does `move a1,a0` in each arm: split pieces a0, then
  a1 with saves. Our arms reload the byte into a1. Forcing the top-assigned midi to a1 (force71 `p1:w274=c4` on
  c2/d2.c) gives 75 rows against 80 for the best, but the load goes straight to a1. No source form made 7.1 colour the
  split pieces. In d2 the split re-queues no piece.
- Fader loop: retail ends the &D_8004BE90 range (s3) at the voice loop and rematerialises `lui a0/lw a0` in the fader
  loop, with the end pointer D_8004F800 in s3. Ours extends the s3 address range into the fader loop, so the end pointer
  goes to s5. The GameCube `smf++` for-header form is worse (g1/g2: 144 vs 112).

**Integration (when it closes; unchanged from w22b):** TU patch moving lib_1a660's `ChannelState` typedef and
`extern ChannelState D_8004F300[32];` above the func_8001A658 slot (w21c pattern, `tools/tu_edits.py`). Add a
reloc_addrs entry for the fader end pointer `D_8004F800` (= D_8004F300 + 0x500, the `addiu s3,s3,0` in retail; ours
emits `D_8004F300+1280`) via `reloc_split derive` (w20c/w21b kind). No owned data.

## What generalises

- **A store is a PRE expression.** A loop-body `x = 0;` that is redone on a path reaching the back edge is hoisted to
  the loop-body entry. The variable then lives across every call in the loop and is split (5.3 pretrace:
  `str(var,const)` DELETE/INSERT). Assigning the constant in the arms where it is needed avoids it (synthHandle span).
- **A raw `if (u8param)` and `if (u8param != 0)` are different ucode.** The latter is `cvt(param)`, which PRE can hoist
  when the same cvt appears in a loop compare (voiceAllocate fxFlag).
- **One block can decide a callee-saved tie.** cost = nblocks/4, and a tie goes to split. A nested ternary clamp
  instead of `if/else` plus ternary is a natural +1 block (159 -> 160 here).
- **Retail loading a field straight into a variable's s-register** (`lwl s0,48(s2)`) means a separate
  `vol = sv->field;` statement precedes the arithmetic (synthHandle vol/pan).
- trace71's `ctrace71` + `CUPLOG` explained every colouring change here. The 5.3 pretrace (`tools/pp.sh` + `snap.sh` +
  `pretrace.sh`) is still the only PRE view for 7.1 functions and agreed with 7.1 on both.

## Disclosures

- Neither best.c uses devices, unused locals, pad arrays, false prototypes, or implicit int.
- synthHandle declares modulation/portamento as `s32` (getter results narrowed from u16) for midi's slot. It keeps
  w22b's private SynthVoice_/SynthFader_/SynthLfo_8001A658 views and the TU's ChannelState lines.
- voiceAllocate keeps w21c/w22a's `volatile D_8004BE94` (as lib_113e0 declares it) and the private packed views.
- These were used as oracles only and appear in no delivered source: `do {} while (0)` (b1), `if (span) {}` (o1/o2),
  dead `span = 1;` (q1/u*), `voice = voice;` (o2), `(u32)voice` (o1), and force71 runs.
- No permission denials. Builder: only `~/rush2049/scratch/frontier/w23b` (plus mktemp under the builder /tmp from
  check.py/dif.sh, as earlier lanes). MusyX GameCube sources (synth.c, synthvoice.c from the w21c scratch copy) were
  read as reference only.

## Files

`func_8001F13C/best.c`, `func_8001A658/best.c` (near misses). `tools/`: w22a's set renamed to w23b, plus w22b's
`rows.sh`/`ops.sh`/`vbo.sh`, and new `ins.sh`/`vbi.sh`, an instruction-text diff count with branch targets stripped,
batch over a directory. Variants and traces are under `*/v/` (ignored).
