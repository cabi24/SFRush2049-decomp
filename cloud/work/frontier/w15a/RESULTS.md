# Wave 15: lane w15a (camera_update)

Assignment: camera_update (2,876 B), 17 residual words, starting from `w12f/camera_update/best.c` in the w12f
group. Flags `-g0 -O3 -mips2 -G 0 -non_shared`. Builder scratch `watchman2:~/rush2049/scratch/frontier/w15a`
(fresh copy of `base`; synced src/blob, include, tools/cloud, asm/us/blob and `blob_matched.lock.json`; trace
toolkit installed with `--reuse wtk`).

**No match yet. camera_update went from 17 to 5 words.** The 8-word and 4-word register families are closed. The
5 words left are the two do-loop compare sites. Nothing is claimable, so there is nothing to splice.

| Function | Bytes | State | Flags | Exact scorer output |
|---|---:|---|---|---|
| `camera_update` | 2,876 | near-miss, **5 words** (was 17); frame 248/248, length 719/719 | -O3 | `FAIL camera_update: 5 of 719 words differ` |
| `camera_free_look` | 436 | code identical, provisional (its caller is unmatched) | -O3 | `EQUAL camera_free_look: 109 words (internal, c_group.c)` |
| `camera_track_spline` | 860 | code identical, provisional (its caller is unmatched) | -O3 | `EQUAL camera_track_spline: 215 words (internal, c_group.c)` |
| `camera_look_at_point` | 796 | not worked (unchanged) | -O3 | `FAIL camera_look_at_point: 38 of 199 words differ` |

Unit confirmation, run from the repo root on the Pi:
```
python3 -m tools.conveyor.pipeline.blob_unit --tag w15a score camera_process_input func_800C15FC camera_update \
  camera_free_look camera_look_at_point camera_track_spline camera_aspect_ratio camera_fov_control \
  camera_build_view_matrix --with cloud/work/frontier/w15a/groups/camera_aspect_ratio/group.c \
  --internal func_800C15FC --neighbours
  EQUAL camera_process_input: 255 words (kept, c_group.c)
  EQUAL func_800C15FC: 2 words (internal, c_group.c)
  FAIL camera_update: 5 of 719 words differ
  EQUAL camera_free_look: 109 words (internal, c_group.c)
  FAIL camera_look_at_point: 38 of 199 words differ
  EQUAL camera_track_spline: 215 words (internal, c_group.c)
  EQUAL camera_aspect_ratio: 50 words (internal, c_group.c)
  EQUAL camera_fov_control: 60 words (internal, c_group.c)
  EQUAL camera_build_view_matrix: 153 words (internal, c_group.c)
  locked bodies that differ in this unit: 0
blob_unit score: 7/9 equal; object build/blob_unit/w15a/unit.o (3.6s)
```

## What closed the 12 words (traced, then one lever each)

The steps follow the brief's order: force, trace, then lever. All forms are in `v/`, scored with
`tools/s3n.sh FILE.c`, which splices the body into `base2.c` (w12f's) and unit-scores it.

1. **Oracle re-confirmed.** `force.sh b0 camera_update "p1:w277=c4,p1:w242=c5,p1:w286=c3,p1:w344=c27"` on w12f's
   best leaves 5 rows (the compares). All p1 caller-saved costs are 0, so the only colour levers are forbids
   or copy relations.
2. **na is the parameter `flag`** (`v/p1/naflag.c`). The do-nothing `flag` (s16, only tested at entry) is
   reused for the new texture index: `flag = cam->s58 + idx; if (flag != cam->s50) {...; cam->s50 = flag;}`.
   - The flag web's formal-parameter copy relation (workbench L57) makes a1 its only `available0` colour
     (`0x08000000`). na therefore gets a1 as in retail.
   - No frame change: na had shared a 4-byte word with m.
   - Then m takes v1 and tb takes a0. Forcing m=a0 moves tb to v1 (13 rows). So retail has a v1 holder that
     interferes with both m and tb.
3. **The v1 holder is `lk`, through a disclosed compiled-out read** (`v/p3/lk3.c`):
   `lk = sc->link; if (lk) {}` right after `m = (&D_801427C0)[flag];`.
   - The load emits no code because its only use is compiled out.
   - uopt merges the chain into lk's single web (all du-chains of a variable are one web, L9). That web is v1
     in the top region (`lw v1,24(s8)`, as in retail).
   - With save 2.0 it is coloured before m and tb, and it interferes with both in m's block. So m→a0 and tb→a2.
     Result: **9 words.**
   - Placement matters. Defining it right after tb (`lk2.c`) spreads the web over more blocks, drops its save
     to 1.33 below the setter webs, and it loses v1 (30 rows).
   - A copy (`lk = sc`, `lk5.c`) is copy-propagated and does nothing.
   - The same read inside the setter's else arm also works (`v/pb/q3.c`, 5 words in the final base). A bare
     `if (sc->link) {}` (`q1.c`) gives only 13.
4. **The f12 holder is a disclosed compiled-out read of dur after the division** (`v/p4/h3x.c`):
   `if (sc->keys[idx].dur != 0.0f) {}` right after `fr = tt / f;`. Result: tt = f14, **5 words**, frame unchanged.
   - Diagnosis: a new f32 local held across the division (`g3.c`) also gives f14, but costs the frame
     (248→256). So the holder only has to be live across the division block, where it interferes with f (f0)
     and fr (f2).
   - The same read before *and* after (`h1.c`, with `ctl->f10`) also gives 5. A read after only, of `ctl->f10`,
     gives 9. A read of `f` gives 9 (`q4.c`). A bare `if (sc->keys[idx].dur) {}` gives 5 (`q5.c`).

## Residual: 5 words, the do-loop compares (stop rule reached, about 3 h of tracing)

At both `idx + 1` / `sc->count` sites, retail's ugen evaluates count first (t6, then idx+1 in t8). Ours
evaluates idx+1 first. This is ugen evaluation order of the compare, not colouring. Traced with `pretrace.sh`
and ucode dumps (`udn.py` on `opt.u`/`merged`):

- uopt rewrites site 1 `equ(idx+1, count)` to `equ(idx, count + -1)` (expression bit 218). It swaps site 2
  `equ(count, idx+1)` to `equ(add(idx,1), ilod count)` (bit 226). ugen then always works on the idx side first.
- The canonical order is **not** creation order:
  - count's ilod (bit 216) is created before add(idx,1) (bit 222), yet add(idx,1) goes left.
  - Compiled-out or dead-def reads of `sc->count` before the loop (`v/p8/n*.c`, `v/p5/*`) change nothing.
  - Declaration order (`v/p9`) changes nothing.
  - Seven spellings of each site (`v/p7`, `v/p8`, `v/pd`: `count - 1 == idx`, `idx == count + -1`, `1 + idx`,
    `(s32)`/`(u16)` count, `1U`) are canonicalised to the same output or create the idx+1 CSE (488+ words).
  - Changing idx's type to s16 (`v/pc`) changes nothing.
- The observed rule is a kind ranking: var < const < add < ilod (`equ(var,0)`, `equ(0,ilod)`, `equ(4,ilod)`,
  `equ(add,ilod)`, `equ(var,add)`).
- **Lead:** writing site 1 as `ctl->idx + 1 == sc->count` (`v/p8/a.c`) *does* flip the order, so count is in t6
  as in retail. But it adds a reload of ctl->idx (720 words). So retail's idx side at the compares is probably
  an ilod-ranked operand that is still in v0, i.e. a PRE web of `ctl->idx`, not the variable idx. Rewriting the
  whole do-loop on `ctl->idx` (`v/pa/x1.c`) breaks much more (510 words, frame 256), because no single web
  forms.

**Best next hypothesis:** find a do-loop form in which `ctl->idx` stays an expression web in v0 across the
loop (loaded at the loop top and after each `ctl->idx` store) without the extra reload. Compare `pretrace.sh` of
`v/p8/a.c` with best.c for why the site-1 ilod is not available (which store kills `ilod.J@12(ctl)`). Then retry
with site 2 also written on `ctl->idx`.

## Files

- `best.c`: the body. Its header lists every shaping device, including the two new compiled-out reads.
- `groups/camera_aspect_ratio/group.c` + `group.json`: the w12f work base with this camera_update body.
  - Same name and the same five members as the locked group (`camera_aspect_ratio`, `camera_fov_control`,
    `camera_build_view_matrix`, `func_800C15FC`, `camera_process_input`), all byte-identical to w12f's group.c.
  - **`"claims": []`**. **Do not splice it.**
  - This supersedes `w12f/groups/camera_aspect_ratio/` as the work base.
- `base2.c`: copy of w12f's cluster without the body. `retail.s`: retail disassembly.
- `tools/s3n.sh`, `fnset.py`, `fnget.py`: splice-and-score helpers, adapted from w13c.
- `v/p1`–`v/pd`: every probe named above.
- Pi traces: `build/trace/w15a/` (`*.cdx` colour records, `pre_h3x.txt` PRE listing, `ugt_lk3.txt` ugen
  listing, `u/opt.txt` and `u/merged.txt` ucode dumps).

## Integration notes

Nothing to integrate this wave: no overrides, and no group is superseded in `src/blob`. When the compares close:
- add camera_update to `members` and `claims`, and add camera_free_look and camera_track_spline (EQUAL,
  internal) to `claims`;
- switch `D_80123E8C` to `0.0425f` and check the own .rodata at 0x80123E8C;
- keep the existing `prefer_definition` func_800C15FC → the group file. No other override is needed.
- The body uses two compiled-out reads (`if (lk) {}` after a dead `lk = sc->link;`, and
  `if (sc->keys[idx].dur != 0.0f) {}`). Both are disclosed in the header. It also reuses the parameter `flag`
  as a local, which is plain C. It has no implicit int, no unused locals and no pad arrays.

## Permission denials and incidents

- No permission denials.
- The shared rules file gives the install order as `install.sh --reuse OTHER SCRATCH`. The script reads its
  first argument as the scratch directory, so it built a fresh toolkit (2 gcc jobs) into `watchman2:~/--reuse`.
  I removed that directory (my own artefact) and reinstalled with the correct order,
  `install.sh ~/rush2049/scratch/frontier/w15a --reuse ~/rush2049/scratch/frontier/wtk`.

## What generalises

1. **An "invisible holder" can be a dead definition kept alive by a compiled-out read, and it can be grafted
   onto an existing variable.**
   - `x = <load>; if (x) {}` emits no instructions.
   - But the chain is merged into x's single web. If x is coloured R elsewhere, that colour is now forbidden
     to every web live in the block where the dead chain sits.
   - Pick x as an existing variable that retail colours with the missing register (here lk = v1). Keep the
     def and the read in one block, so the merged web's save stays above the webs it must beat.
   - This is the lever to try after `force.sh` proves a colour-only family that needs a register no retail
     instruction uses.
2. **The FP equivalent needs no variable.** A compiled-out read of an expression just after the block that
   needs the forbid (`if (k->dur != 0.0f) {}`) closed an f12/f14 tie at zero frame cost. A new f32 local does
   the same but grows the frame.
3. **A reused parameter takes its argument register through the copy relation.** A local that retail keeps in
   a1 (or a0) where nothing forbids the lower registers is often the parameter reused (`flag` here).
4. **Compare operand order is a canonical kind ranking in uopt, not creation order or spelling.** Only changing
   an operand's kind (variable vs ilod) moved it here.
