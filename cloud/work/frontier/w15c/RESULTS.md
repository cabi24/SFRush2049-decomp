# Wave 15, lane w15c: trace-only residuals (entity_process_main, func_80096130, audio_channel_setup)

Status: final.

| Function | Bytes | State | Flags | Scorer output |
|---|---:|---|---|---|
| audio_channel_setup | 332 | **strict MATCH** -> `cloud/matches/audio_channel_setup.c` | -O3 (also -O2) | `audio_channel_setup: MATCH` (score.py fn); unit `EQUAL audio_channel_setup: 83 words (kept, c_audio_channel_setup.c)` / `locked bodies that differ in this unit: 0` |
| func_80096130 | 264 | 3 of 66 words (spill home only); cause traced to a missing live-range split | -O3 | `FAIL func_80096130: 3 of 66 words differ` (now also with NO shaping devices: `f130/s1.c`) |
| entity_process_main | 1424 | 2 of 356 words (unchanged); ugen evaluation order traced | -O3 | `FAIL entity_process_main: 2 of 356 words differ` |

Integration: audio_channel_setup is a plain kept single (no rodata, no overrides, no group superseded; `frontier show`
labels it `group` only because of the `preserved: a2` signature, and it is EQUAL in the whole-program unit with all
neighbours). Disclosed shaping: one compiled-out read `if (D_801427C0[f]) {}`. No other deliverables.
Permission denials: none.

## func_80096130 (264 B): 3 of 66 words, cause traced to a live-range split (not matched)

Current unit (2026-10-10): the plain source with NO shaping devices (`w12e/f130_base.c`, also `f130/s1..s4.c`, `c1..c5.c`)
now gives the same `3 of 66 words` as w12e's best.c (`if (slot) {}` + `if (0) {}` are no longer needed: the slot web
is coloured v1 unsplit, totalsave 4 > bestcost 3). The 3 words are only the idx*8 spill home: 36(sp) vs retail 32(sp).

Trace (w13b's `uopt_al`, `altrace_remote.sh`; `spill.sh`; `force.sh`):
- `spill.sh base`: slot web (bit 5) temp 0 = sp+36, alloc web (bit 7) temp 1 = sp+32, idx*8 web (bit 23) reuses temp 0
  because it shares no block with web 5. Retail's 32 = reuse of temp 1, so in retail idx*8 shares a block with web 5.
- Alias trace: `.alias $3,$sp` is emitted at the first block start where the bb register map no longer holds the
  recorded live range (`CHK rec != map`). That is right after slot's last block. If idx*8 is in slot's last block
  (w12e d5) the alias comes after the first memset and as1 sinks the store into the jal delay slot.
- **The two constraints are satisfied together only if slot's live range is SPLIT** (the tail piece after
  osJamMesg is a different live range, so CHK fails at that block start: exactly w12e's hand-edited oracle position,
  `.alias` before the `lw $3,36($sp)` reload, while web 5 still conflicts with idx*8 -> home 32).
  Oracle: `force.sh d5 func_80096130 "p1:w5=s"` makes the whole tail (store order, homes 32, every word from
  0x08c to the end) equal to retail. The head is then wrong because the remnant piece is re-split by the same force
  entry (the force key is the web number, which the remnant keeps) and web 7 (alloc) takes v1.
- Retail therefore needs: web 5 split once (totalsave < bestcost), the remnant (head) piece coloured v1 before web 7
  (alloc -> a3). No natural source found that splits web 5 in the current unit (all of s1-s4/c1-c5 are unsplit).
- Next: a uopt force variant that can force "split once, then colour" (e.g. `p1:w5=s1` = split on the first decision
  only) would prove/refute the full hypothesis; then look for the source that lowers web 5's totalsave below 3
  (fewer occurrence blocks of the slot address) or raises its caller-saved cost, without the w12e-era colour flip.

## audio_channel_setup (332 B): strict MATCH -> `cloud/matches/audio_channel_setup.c`

```
builder: score.py fn cand/audio_channel_setup.c audio_channel_setup --flags "-g0 -O3 -mips2 -G 0 -non_shared"
  audio_channel_setup:
    MATCH                     (also MATCH at -O2)
python3 -m tools.conveyor.pipeline.blob_unit --tag w15c --jobs 2 score audio_channel_setup --with cloud/matches/audio_channel_setup.c --neighbours
  EQUAL audio_channel_setup: 83 words (kept, c_audio_channel_setup.c)
  locked bodies that differ in this unit: 0
blob_unit score: 1/1 equal; object build/blob_unit/w15c/unit.o (5.3s)
```
No rodata (0.0625f is a `lui` immediate), no unit_overrides, plain kept single. Integration: `splice_singles.py
audio_channel_setup` (flags on line 1). Shaping device (disclosed in the header): one compiled-out read
`if (D_801427C0[f]) {}` placed before `id = m->id`.

Trace (method that closed it):
1. w12e's mechanism: retail `lui a2; addu a2,a2,t2; lhu a1,%lo(a2)` = as1 copy coalescing of ugen `lhu $6,sym($10)`
   + copy into `$5`. So retail has the loaded value in a COLOURED web (a2) and a separate variable web (a1).
2. A coloured *expression* web for the load appears when `D_801427C0[f]` occurs twice. Of four placements of a
   compiled-out second read, only "before `tex = ...`" (`acs/h3.c`) produced the shape: ugen
   `lhu $5,D_801427C0($10); and $3,$5,65535` (the u16 copy), coalesced by as1 -- 7 words, all colours.
3. `force.sh h3 audio_channel_setup "p1:w48=c2,p1:w54=c4,p1:w53=c5"` -> `differing rows 0`: colour-only.
4. Lever: tex (save 2, one block) was beating id (save 1, two blocks). Moving the read before `id = m->id` (`i4.c`)
   makes id a one-block web with save 2 and a lower web number than tex; the tie goes to id -> v1, tex -> a1,
   expression -> a2. EQUAL.

## entity_process_main (1,424 B): 2 of 356 words (unchanged; stopped)

```
TAG=w15c tools/trace/us.sh entity_process_main cloud/work/frontier/w11a/entity_process_main/best.c
best.c: FAIL entity_process_main: 2 of 356 words differ | words 4 ops 0 norm 0 | frame 280/280
```
(`words 4` = the 2 real rows + the 2 own-rodata rows `lui/lwc1 14648(at)` that the unit scorer leaves unverified.)

Trace (post-uopt ucode of line 177 from the snapshot's `opt.u`, parsed with the workbench `parse_ucode`; ugen
FREELIST/EMIT trace `ugt.sh SCHED=1`):
- ugen receives `lod v0; ldc 1; lod i; shl; not; lod v0; ilod(u16); cvt; and; cvt; istr`. It emits strictly in that
  postfix order and allocates ring temps least-recently-freed: li t8, sllv t9, nor -> t6, lhu -> t7.
- Retail (li t8, sllv t9, lhu t6, nor t7, and t8) is exactly the allocation sequence of `shl, ilod, not, and`: the
  NOT is emitted AFTER the load but applies to the shift. No plain postfix tree has that order (NOT always applies
  to the top of stack), so retail's ucode had the load between the SHL and the NOT on a different stack path.
- The free-list alternative (same order, different free history) is ruled out: the preceding statement
  (`lh t6; multu; mflo t7; addu v0`) is identical in retail and frees t6 before t7.
- Tried this lane (all measured in the unit): algebraic forms `~((1<<i)|~flags)` (f1: 12 words; it gives retail's
  exact allocation sequence but nor-of-load + nor ops), `~(~flags|(1<<i))` (16), `(-1^(1<<i))&flags` and
  `((1<<i)^-1)&flags` (357 words), `& 0xFFFF` forms (17), u16/s32/u32 casts on the load (2, unchanged), direct
  `D_8015B268[v->objnum].flags` both sides (18), mask in a separate earlier statement through `pad1` (2, copy-
  propagated), load-first `flags & ~(1<<i)` (18: lhu t8, li t9, sllv t6, nor t7). w14oa's unfinished t_dup/t_dup2/
  t_ld/t_ld2: 42/41/53/53 words (larger bodies).
- Best next hypothesis: find which uopt re-emission puts an operand between a value and its unary consumer. Candidates:
  a ucode `Uswp`/`Udup` path (uopt emits these for some PRE/CSE re-materialisations within one statement), or a
  NOT that uopt creates itself (e.g. from a `!=`/`==`-derived mask or from an `&` with a constant that uopt folds).
  Dump retail-like sibling functions whose matched source has `~(1 << x) & y` with this allocation pattern, if any,
  and read their opt.u (`grep -l "~(1 <<" src/blob/*.c`).

## What generalises

- **as1 copy-coalescing residuals (`lui X; addu X; l* Y,%lo(X)` with X != Y) need a COLOURED source web.** ugen
  emits `l* $X,sym($i)` + copy into `$Y` only when the loaded value is a coloured web distinct from the variable.
  A second (compiled-out) occurrence of the load expression turns it into a coloured expression web; then fix the
  colours with the usual force-oracle + web-number tie (here: place the read so the competing variable becomes a
  one-block web with equal save and a lower number). Applies to the other "inlined accessor" temp patterns
  (w10g note) when no call follows.
- **A live-range split is visible in the alias trace:** `.alias reg,$sp` at a block start where the register is
  used again in that block means the bb map holds a different live range there (a split piece), not a block
  boundary after the last use. Spill-home reuse (`HOME reused=1`) is per web, so the split also keeps the
  CONFL/home layout. `force.sh "p1:wN=s"` is an imperfect oracle for this (the remnant keeps web number N and is
  re-split); a "split once" force mode would make it exact.
- **Re-score drafts in the current unit**: func_80096130's plain source now equals w12e's shaped best (IPA changes
  since wave 12 removed the slot-web split that the `if (slot) {}` lever was fixing).
