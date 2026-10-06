# w11d results (wave 11, mid-distance near-misses)

Assignment: race_countdown_display (1,120 B), sound_bank_unload (1,248 B), func_800F7F3C (1,396 B),
particle_lifetime_set (732 B), audio_doppler_full (952 B). Flags everywhere `-g0 -O3 -mips2 -G 0 -non_shared`.
All scoring was done in the whole-program unit (`blob_unit --tag w11d score … --neighbours`). No run changed a locked body.

**No strict MATCH this lane, and nothing to splice.** `cloud/matches/` has no new files and no group claims anything.
Nothing was committed, spliced or pushed. There were no permission denials.

| Function | Bytes | State | Exact unit output (`--neighbours`; every run also printed `locked bodies that differ in this unit: 0`) | Residual lane |
|---|---:|---|---|---|
| audio_doppler_full | 952 | near-miss, **improved** 153 → 119 (natural); 96 with a split-symbol spelling | `FAIL audio_doppler_full: 119 of 238 words differ; +0x1f4: .rodata+0x20: retail words encode 0x0001003c, outside the image` / `best_split_texture.c`: `FAIL audio_doppler_full: 96 of 238 words differ; compiled body is 240 words, target 238` | p1 tie between callee-saved address webs |
| race_countdown_display | 1,120 | near-miss, unchanged; as1 mechanism located | `FAIL race_countdown_display: 34 of 280 words differ; compiled body is 279 words, target 280` | as1 cross-block (xbb) hoist |
| particle_lifetime_set | 732 | near-miss, unchanged | `FAIL particle_lifetime_set: 59 of 183 words differ` | as1 macro temp choice, then ring |
| func_800F7F3C | 1,396 | near-miss, unchanged | `FAIL func_800F7F3C: 117 of 349 words differ` | p2 colour order, ring |
| sound_bank_unload | 1,248 | near-miss, unchanged; one hypothesis refuted | `FAIL sound_bank_unload: 134 of 312 words differ` (`EQUAL func_800B0EA0: 48 words`, `EQUAL func_800B0F60: 2 words`) | ring phase |

Commands (repo root, Pi):
```
python3 -m tools.conveyor.pipeline.blob_unit --tag w11d score NAME --with cloud/work/frontier/w11d/NAME/best.c --neighbours
python3 -m tools.conveyor.pipeline.blob_unit --tag w11d score sound_bank_unload func_800B0EA0 func_800B0F60 \
    --with cloud/work/frontier/w11d/sound_bank_unload/best.c --internal func_800B0EA0 --internal func_800B0F60 --neighbours
```

## Integration notes
None. Nothing here is spliceable. If `audio_doppler_full/best.c` ever closes, it has the same requirement as
race_countdown_display: it defines `func_800B61A8` as a direct-return `__inline` wrapper. Either the locked
`src/blob/func_800B61A8.c` (the `new_var` form) must change to that form, or the unit needs a
`prefer_definition` entry.

---

## audio_doppler_full: 153 → 119 words (natural), 96 (split spelling)

`audio_doppler_full/best.c` (natural) and `best_split_texture.c`.
1. **Direct-return `__inline func_800B61A8` defined in the file**, the same form race_countdown_display needs.
   The frame drops from 264+ to 240. `char name[24]` then gives frame 256 with quad at sp+208, both as retail.
   Retail has name at 176 and ours is at 184. No name size or filler gave both 176 and 256: `name[8]` plus
   `f32 pad[6]` in either order gives frame 264, and `name[32]` gives 264.
2. **The post-call `->flags &= 0x7FFF` goes through a `Poly *p` local**, or a `(Poly *)(u32)` launder; both work.
   Without it, uopt merges every load of `D_80154368[player]`, including the reload after the
   `func_8008C074` call, into one PRE web that spans the call, and colours it s0. Retail keeps it in
   a0/v0 (−12 words). Length becomes 238 = retail.
3. Residual (traced uopt, proc 501): the callee-saved address webs. Retail has s7 = quad (sp+208), s8 =
   &D_801140F4 (colour), and the texture &D_80154398 is materialised at every use. In ours the texture web is
   one web with tot 50 (2 `&` uses and 3 loads), so it beats quad (tot 30) and takes s7. Quad then takes s8 and
   the colour is split.
   - If the `&` and the loads are separate symbols (`best_split_texture.c` loads it as
     `*(u16 *)((char *)D_80154368 + 0x30)`), the texture web drops to tot 30. Quad takes s7 (−23 words). The
     loads web then **ties** with the colour web (both tot 30, save 2.3077, numintf 27) and wins by lower web
     number, so it takes s8.
   - Web numbering follows first appearance in the procedure. A diagnostic `if (D_801140F4[0]) {}` early in
     the `show` arm flips colour ahead of texture. Moving or reordering extern declarations does nothing. A
     colour local (`u8 *color = D_801140F4`) is copy-propagated, so the order does not change.
   - `force.sh` with `p1:w154=s,p1:w155=c22` (texture split, colour to s8) was declined (`forbidden` includes s8).
   - Tried without movement: volatile texture; `s16` texture; colour as a `Color` struct passed by address;
     four spellings of the flag-clear line (only the local or the launder move it).
   Next hypothesis: retail references the texture through two different symbols or bases, and the colour
   appears first in the procedure. Look for an N64 struct that spans 0x80154368..0x801543A4 (poly pointers at
   +0, texture at +0x30, shown count at +0x3C) and check how the other locked users of D_80154398 spell it.

## race_countdown_display: 34/280 (unchanged); the as1 mechanism is now located

Built the printf-patched traced as1 (`as1 -R`) in my scratch (`watchman2:…/w11d/as1/as1`, from w6d's
`patch_as1_printf.py`). I also wrote `tools/asx_remote.sh`, which reassembles a hand-edited
**single-function** ugen listing with as0+as1. The whole-unit `out.s` does not reassemble, because of
duplicate symbols. The single-function listing reproduces our unit bytes exactly.
- Editing our listing so the beq block reads `lb $2; li $1,1; beq $6,$2; … bne $2,$1` (`as1_listing/li_before.s`)
  gives exactly retail's tail: `lb v0; li at,1; beql a2,v0,epi+4; lw ra; bne v0,at`. Retail's `li at,1` was
  therefore **inside the beq block when as1 scheduled it**. It had been moved there across blocks before
  scheduling, not by delay-slot filling.
- That move is as1's cross-basic-block pass (`do_xbb_opt`, followed by a reschedule pass). The trace shows
  the same hoist happening in the locked `func_800F8EC8` (`lbu; li at,6; beqzl; …; bne v0,at`). It also
  happens in our own function at line 107 (the `D_8014A110 != 1` block, rescheduled 4 times), but our line-157
  beq block is never retried.
- The hoist in func_800F8EC8 survives every mutation I tried: beq target changed to the epilogue, register
  instead of zero compare, a label plus `b` at block start, one or two extra branch predecessors, and a bne target
  that is hoistable or blocked. In our listing it never fires. Changes tried there: removing
  `.alias/.noalias`; removing, merging or relabelling `.loc`s; a label after the beq; an inverted branch; a
  different immediate; different compare registers (a2, a3, v1, s0, t5); lb/lbu/lw/global loads; extra
  instructions in the block; a blocked bne target; removing the inlined `D_8010FFC0` test.
- Conclusion: the trigger is in xbb's candidate selection (`func_4287c8/428fc4/428a2c/429534/42aa0c/42b51c`
  in the recompiled `as1.c`, called from `do_xbb_opt`). It is not in the local shape of these three blocks.
  Next step: read those routines, or bisect our listing by deleting earlier blocks until the hoist fires.

## particle_lifetime_set: 59/183 (unchanged)

The open cause is narrowed to as1. Our ugen emits the macro `lw $9, D_801145D4($25)`, and as1 expands it with
the destination as its temporary (`lui t1; addu t1,t1,t9; lw t1,lo(t1)`). Retail has temp t1 and destination
t3 (`lw t3,17876(t1)`), and the temp ring is then +2 for the rest of the function.

Listing probes on the builder:
- `lw $11, D($25)` gives `lui t3 … lw t3`: as1 uses rd.
- `lw $9…; move $11,$9` gives `lw t1`: as1 copy-propagates into the temp.
- `la` plus `lw 0(...)` forms leave `addiu t1,t1,lo` unfolded, which is not retail.

So retail's `lw` is the macro with rd = t3, and as1 picked a different temp. A named `u32 fl` local
(`v1.c`, `v2.c`) makes fl a uopt web in v0 (61 words). Next step: as1's temp choice for `lw rd, sym(rs)`
(`f_setup_tempreg`). Find the case in which it does not reuse rd.

## func_800F7F3C: 117/349 (unchanged)

Traced (proc 909) and confirmed w10b. In p2, colouring goes in web-number order. The arm-1 sort end pointer
(w60) takes t4 in ours. In retail it takes t5, so some web numbered before 60 that interferes with the sort loop
holds t4 in retail. `&D_80152038`/120 (w201/w202) are p2 webs and take t5/s0, where retail has s0/s1. Tried:
- a `s8 *tie = D_80150B68` local, initialised at the start of arm 1 or before the tie loop: 287 words;
- an `s32 *cnt = &D_80150B60` local: 281 words.

Both are worse. Stopped (w1c already scored 2,187 scope combinations).

## sound_bank_unload: 134/312 (unchanged)

Tested w10e's next hypothesis: the ramp is an inlined helper `(dst, colour, end)`, as an `__inline` static
called three times (`r1.c`). umerge then inlines `func_800B0EA0` into the helper, because the helper is its
only caller at that point. The result is 182 words with a 385-word body, and the retail `jal func_800B0EA0`
calls disappear. **Refuted**: any helper that wraps the three ramps cannot keep `func_800B0EA0` out of line.

## What generalises
1. **A value read before and after a call is one PRE web unless the second read is spelled differently.** A local
   (`p = g[i]; p->f &= …`) or a `(T *)(u32)` launder keeps the post-call read as a fresh caller-saved temp. This
   fits retail when it shows `lw a0,0(sN)` at the test and `lw v0,0(sN)` after the call.
2. **Address-web ties (p1, same tot/save/numintf) go to the lower web number, which is first appearance in the
   procedure.** Declaration order is irrelevant, and a local copy of the address is propagated away.
3. **One global referenced by `&` and by loads is one address web whose tot is the sum of both.** A retail
   function that rematerialises such an address while keeping lesser addresses in s-registers points to two
   spellings or bases for that address.
4. **as1 has a cross-basic-block pass (`do_xbb_opt`) plus a reschedule.** A `li at,K` that sits *above* a
   branch-likely in retail, with the slot filled from the target, was hoisted out of the fall-through block by
   that pass. Delay-slot filling is not the cause. To test a hypothesis, edit the ugen listing and reassemble it
   with `tools/asx_remote.sh FN.s FN`, which works on single-function listings only.
5. The direct-return `__inline func_800B61A8` form is needed by a second function (audio_doppler_full, frame).
   That is more evidence that the locked `new_var` source of func_800B61A8 is not the original.

## Tools (tools/, all tag w11d; builder scratch `watchman2:~/rush2049/scratch/frontier/w11d`)
- `u.sh`, `q.sh`, `udiff.py`, `ulist.sh`, `ctrace.sh`, `force.sh`, `pdiff.sh`, `sum.sh`, `tr.sh` (w10e/w10b kits, retagged)
- `fr.sh NAME FILE…`: unit verdict, frame and sp-relative `addiu` rows per file
- `ugu.sh NAME CAND.c`: unit score, then traced ugen on the unit stage; that function's free-list events
- `asx_remote.sh FN.s FN` (builder): as0+as1 a single-function ugen listing and print the disassembly
- `asr_remote.sh FN.s LOG` (builder): the same with the traced `as1 -R`; checks the object is identical to stock
- traced uopt, ugen and as1 builds live in the builder scratch (`uopt/`, `ugen/`, `as1/`); `build_uopt.sh` and
  `patch_as1_printf.py` describe the build.
