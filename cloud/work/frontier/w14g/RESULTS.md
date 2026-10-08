# w14g results (matching wave 14, lane w14g: mass variant testing)

No function reached a strict MATCH. No `cloud/matches/` files, no groups, no overrides, no unit_overrides.
Nothing to integrate. Working drafts are the `b*/v0000.c` files named below.

## Tools used (this lane)
- `cloud/work/frontier/tools/vbatch/vgen.py` (choice-point expansion) and `vbatch.sh` (builder scoring,
  `--top 400` to keep every row). Note: `vbatch.sh` passes its args unquoted, so `--flags` with spaces cannot be
  given through it; the default flags are the stated `-g0 -O3 -mips2 -G 0 -non_shared`.
- `w14g/vsum.py DIR`: per-round summary (strict histogram, option frequencies among the best).
- `w14g/cdis.sh FILE.c FN`: compiles a candidate standalone on the builder and disassembles it with
  mips-linux-gnu-objdump (diagnostic only).
- Scratch: `~/rush2049/scratch/frontier/w14g` (base copy plus src/blob, include, tools/cloud, asm/us/blob and
  blob_matched.lock.json synced from the Pi).

## Scorer lines (this session, verbatim)

| Function | Bytes | Best draft | State | Scorer line |
|---|---|---|---|---|
| wheel_torque_apply | 140 | `wheel_torque_apply/b3/v0000.c` | 6 of 35 words off (strict 6) | `blob_unit score wheel_torque_apply --with .../b3/v0000.c` -> `blob_unit score: 0/1 equal; object build/blob_unit/w14g/unit.o (14.7s)` |
| func_800A1BB4 | 184 | `func_800A1BB4/b3/v0000.c` | strict 28 standalone (23 aligned missing) | `blob_unit score: 0/1 equal; object build/blob_unit/w14g/unit.o (13.2s)` |
| func_8008AD6C | 164 | `func_8008AD6C/b2/v0000.c` | strict 34 standalone (33 aligned missing) | `blob_unit score: 0/1 equal; object build/blob_unit/w14g/unit.o (13.8s)` |

Standalone builder lines (bscore, `-g0 -O3 -mips2 -G 0 -non_shared`): wheel `mnem-missing 3 aligned-missing 6 strict 6`;
800A1BB4 `mnem-missing 6 aligned-missing 23 strict 28 size +0 v0000.c`; 8AD6C `mnem-missing 7 aligned-missing 33 strict 34`.
The wheel `-O2` check (b3 best) gives the same `strict 6`.

## Round log (variants scored with the builder; best strict count)

### wheel_torque_apply (target 0x800AC668)
- r1 `b1` (144 variants, template `t1.c`: 5 choice points: declaration of `key`, store form and position, strcpy
  first argument, entity_name_copy call cast/volatile, return form). Best 6 (48 variants). Option 1 of the store
  (store after the strcpy call) is worse (11+). Other axes tie.
- r2 `b2` (216 variants, `t2.c`: store forms incl. `*(volatile s16*)&D_80149D90`, a temp block, `(s16) -1`;
  strcpy argument forms; reload forms with `volatile` pointer local). Best 6 (48). No improvement.
- r3 `b3` (400 of 1296, `t3.c`: adds `0xFFFF` and cast forms of the entity_name_copy arguments, the function-pointer cast
  forms, and `differential_output((s32) D_80149B80)`). Best 6 (92). No improvement.
- `-O2` on the r3 best: `strict 6`.
Residual (unchanged in every variant): (a) the `sh` to D_80149D90 is the instruction before `addiu a0,sp,36`
(retail has the `addiu` before and `sh` in the jal delay slot); (b) ours shares one `lui` for the store and
the reload of D_80149B80 (retail: `lui at` for the store, `lui a0` for the reload). Both are scheduling / address
CSE residuals, not spelling. Not tried: `trace/force.sh` on the store web and the `as1t.sh` scheduler oracle.

### func_800A1BB4 (target 0x800A1BB4)
- r1 `b1` (15 variants, `t1.c`: the early `active == 0` return written three ways linked with the closing brace,
  and the final clear in five forms). Best 32 (all 15). Return shape alone does not change the count.
- r2 `b2` (216 variants, `t2.c`: declaration order, early-return forms, p-init with an early `p == 0` return,
  while/while(1) loop form, final clear). Best **28** (36 variants). Winning options: `p-init` form 2
  (`if (p == 0) { active = 0; return; }` after the head load) and loop form 1 (`while (1) { if (p == 0) break; ...}`).
- r3 `b3` (400 of 1728, `t3.c`: locked the r2 winners; varied data/next load order, `data->enabled` vs `!= 0`,
  `on != 0` vs `on`, `!p` vs `p == 0`). Best 28 (196 variants). No improvement.
Residual: retail duplicates `jr ra; nop` per exit (the early `active == 0` exit has its own return at
0x800A1BE8 and the final clear has its own at 0x800A1C64). In every variant IDO merges the early exit into the
final tail. Retail has 46 words; our best is 42 words. This is a return-merge (branch layout) residual.

### func_8008AD6C (target 0x8008AD6C)
- r1 `b1` (400 of 2592, `t1.c`: declaration order, word-load form, `x = w & 0xFF000000` forms, 0x05000000 and the
  0xFFFF0000 mask spellings, hi/lo statement order in both paths). Best 34 (all 400 are 34 or worse).
- r2 `b2` (400 of 3240, `t2.c`: adds type choices (s32 vs u32 for x, w, hi, lo) and assignment forms). Best 34
  (331 variants), no movement.
- r3 `b3` (81, `t3.c`: `(w >> 24) == 0x05`, else-if chain, inline hi/lo). Best 34 (12). No movement.
Residual: the aligned mnemonic diff is small (7 missing words) but 34 of 41 words differ. The colouring is off by
one: retail has the word in v0 and `x` in v1 (`and v1,v0,at`); ours has the word in v1 and `x` in v0. Declaration
order and the type forms did not change this, so the web order is set by something other than first appearance.
Not tried: `trace/force.sh` on the word web (`p2:w<N>=c2`), which is the next step.

## Integration notes
Nothing to integrate: no match, no group, no unit_overrides, no supersession.

## What generalises
- Standalone bscore and the Pi unit agree on "not equal" for all three best drafts (`0/1 equal`).
- Round-level count and option frequency (`vsum.py`) quickly shows which choice point moves the score. The
  wheel and 8AD6C choice points tie almost everywhere, so further gains need the colouring or scheduling oracles,
  not more spellings.
- For a near-miss, the decisive axes were code-shape axes (where the early-exit joins the tail, `while(1)` vs
  `while`), not the spelling of types or literals.
