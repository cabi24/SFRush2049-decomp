# w36x4 — wave 36 second pass: visuals.c blast trio + CreateCar (game code, MAT3 donor)

Builder scratch: watchman2:~/rush2049/scratch/frontier/w36x4 (copied from base, master e68aea6e1).
Starting points (read-only): cloud/work/frontier/w36l/ (groups/w36l_blast/blast.c, music_seq_load/best.c).

## Status

| function | GC identity | bytes | state | best file |
|---|---|---|---|---|
| func_80090308 | StartNewBlast@visuals.c (w36l confirmed) | 1128 | **MATCH** (group w36x4_blast: score.py group exit 0 + blob_unit EQUAL) | groups/w36x4_blast/blast.c |
| save_write_data | StartBlast@visuals.c (w36l confirmed) | 1200 | **MATCH** (same group, claim; unit EQUAL) | groups/w36x4_blast/blast.c |
| entity_physics_update | AnimateBlast@visuals.c | 712 | **MATCH** (same group, claim; no longer provisional) | groups/w36x4_blast/blast.c |
| music_seq_load | CreateCar@visuals.c (w36l confirmed) | 1428 | **MATCH** standalone (score.py fn) + unit EQUAL (needs w36l's helper-single replacements, see below) | cloud/matches/music_seq_load.c |

## Permission denials
none

## Identities proven
- gTexList (MAT3 visuals.c/skids.c) = D_80161368 on N64, `unsigned short [10]`: CreateCar's sheen texture is
  D_80161368[9] (MAT3 `gTexList[11]`), MATCH of music_seq_load (cloud/matches/music_seq_load.c); StartShadow's
  gTexList[6] is D_80161368[6] (music_control, w36l). w36y's lead for gTexList at 0x801427C0 (inside gObjList) is wrong.
- (confirmed by MATCH, already in w36n) camera_collision_check = EmitGameSound@audio.c: inlined into StartBlast.
- StartNewBlast = func_80090308, AnimateBlast = entity_physics_update, StartBlast = save_write_data,
  CreateCar = music_seq_load: all now proven by MATCH (w36l had them by pattern).

## Notes (running)

### func_80090308 = StartNewBlast: MATCH in the w36l_blast group (2026-10-21, blast/m1)
Mechanism of w36l's "seed reload before the 4th Random" (traced: pretrace/ugt/as1t + an edited-listing as1 run):
1. uopt PRE deletes the seed loads of rand2..rand4 (ilod.J@0(lda seed), antloc 8,10,12,14 | DELETE 10,12,14) unless a
   store through a pointer VARIABLE (b->field) precedes the load in its block: istr via varM(b) kills the seed in uopt
   even though uopt later copy-propagates b into the IV (the store still codegens s0-relative, as in retail).
2. A load uopt leaves is still removed by as1 when the stored register is intact (as1 substitutes it; `.noalias $17,$16`
   lets it look past the s0 stores). Retail keeps rand4's load because ugen REUSES the stored temp (t9) before it:
   `clr.c[3] = b->alpha;` (a byte load through b right after `alpha = 128`) allocates one extra ring temp in block 14,
   so rand4's own load lands in t6 and the stored t9 is no longer intact -> `sw t9,0(s1); lw t6,0(s1)` exactly.
   (A volatile-read oracle for rand4 first proved the reload alone gives norm 0.)
3. Same kill for rand2/rand3 (b-> store before each: the uvs chain/scaleInc and yawInc) makes rand1's return reload a
   block-local temp instead of the coloured v1 web -> rand1/rand2 temp names match (19 -> 9 words).
4. s2/s3 swap = colouring tie (w45 &mat IV+4 vs w58 &gObjList[158+i], both save 8.2, lower web wins): CopyUV through b
   (`math_utility(D_8011418C, b->mat.uvs)`) takes one use off the IV+4 web -> gObjList pointer gets s2 (MATCH).
`g.sh blast/m1` (gscore group = score.compile_group + score.compare, builder):
```
func_80090308: MATCH | ALIGNED rows differ 0 (norm 0) want 282 got 282 frame -272/-272
entity_physics_update: MATCH | ALIGNED rows differ 1 (norm 1) want 178 got 179 frame -112/-112
save_write_data: 169/300 words differ | ALIGNED rows differ 97 (norm 42) want 300 got 288 frame -352/-344
func_80090770: MATCH
```

### (SUPERSEDED by the three-claim group below) first deliverable: group w36x4_blast with two claims
members func_80090308, entity_physics_update, save_write_data; context func_80090770 (byte-identical copy of
src/blob/func_80090770.c); keep entity_physics_update, save_write_data, func_80090770; claims func_80090308,
entity_physics_update. func_80090308 is internal (not kept); save_write_data must stay a member (unclaimed) so the
unit has both real callers of the internal StartNewBlast. New group (w36l_blast was research only, never installed).
`score.py group cand/w36x4_blast --claims` (builder, w36x4 scratch):
```
func_80090308:
  MATCH
    own .data verified at 0x8011B550 (4 bytes; .data is laid out per translation unit)
    own .rodata verified at 0x801239A8..0x801239C8
entity_physics_update:
  MATCH
    own .rodata verified at 0x801239C8..0x801239CC
(context save_write_data 168/300 words differ, func_80090770 MATCH)
exit=0
```
(without --claims: exit=1 only because the unclaimed member save_write_data differs.)
Pi unit: `blob_unit --tag w36x4 score func_80090308 entity_physics_update save_write_data func_80090770
--with cloud/work/frontier/w36x4/groups/w36x4_blast/blast.c --internal func_80090308 --neighbours`:
```
  EQUAL func_80090308: 282 words (internal, c_blast.c)
  EQUAL entity_physics_update: 178 words (kept, c_blast.c); stub tail: 2 words of deleted-procedure stubs follow (NOT as in the image)
  FAIL save_write_data: 160 of 300 words differ; compiled body is 292 words, target 300
  EQUAL func_80090770: 11 words (kept, s_func_80090770.c)
  locked bodies that differ in this unit: 0
```
The stub tail is a unit-layout note only: in the group object the deleted statics' stubs all precede
func_80090308 and entity_physics_update is followed by a zero word (blob_group's extra-words check counts nonzero
words only).

### DELIVERABLE (current): group w36x4_blast, all three claimed (cloud/work/frontier/w36x4/groups/w36x4_blast/)
files func_80090770.c, camera_collision_check.c (byte-identical copies of the locked singles; `cmp` clean), blast.c.
members func_80090308, entity_physics_update, save_write_data; context func_80090770 (ZOID_SetObjectDef),
camera_collision_check (= EmitGameSound, locked kept single; umerge inlines it into StartBlast); keep
entity_physics_update, save_write_data, func_80090770, camera_collision_check; claims = the three members.
func_80090308 stays internal (not kept). New group name (w36l_blast was research only).
`score.py group cand/w36x4_blast` and `... --claims` (builder) both:
```
func_80090308:
  MATCH
    own .data verified at 0x8011B550 (4 bytes; .data is laid out per translation unit)
    own .rodata verified at 0x801239A8..0x801239C8
entity_physics_update:
  MATCH
    own .rodata verified at 0x801239C8..0x801239CC
save_write_data:
  MATCH
    own .data verified at 0x8011B554 (4 bytes; .data is laid out per translation unit)
    own .rodata verified at 0x80123C00..0x80123C08
Context (informational; excluded from exit status):
func_80090770:
  MATCH
camera_collision_check:
  MISMATCH (9 extra words (nonzero beyond target length))
exit=0
```
(The context note is the deleted statics' 2-word stubs, which always follow the context function placed before
func_80090308 whatever the file order; no member has extra words.)
Pi unit: `blob_unit --tag w36x4 score func_80090308 entity_physics_update save_write_data func_80090770
camera_collision_check --with cloud/work/frontier/w36x4/groups/w36x4_blast/blast.c --internal func_80090308 --neighbours`:
```
  EQUAL func_80090308: 282 words (internal, c_blast.c)
  EQUAL entity_physics_update: 178 words (kept, c_blast.c); stub tail: 2 words of deleted-procedure stubs follow (NOT as in the image)
  EQUAL save_write_data: 300 words (kept, c_blast.c)
  EQUAL func_80090770: 11 words (kept, s_func_80090770.c)
  EQUAL camera_collision_check: 33 words (kept, s_camera_collision_check.c); stub tail: 2 words of deleted-procedure stubs follow (as in the image)
  locked bodies that differ in this unit: 0
blob_unit score: 5/5 equal
```
Overrides: none expected (func_80090308 internal by derivation: defined by the group, not kept).

### save_write_data = StartBlast: how it closed (169/300 norm 42 -> MATCH)
1. EmitGameSound in MAT3 form (int return, -1 guards; u8 priority) -> norm 36; then the real one: the locked single
   camera_collision_check IS EmitGameSound (0x800AEFE0: `if (!D_8010FFC0) return -1; if (index==-1) return -1;
   return camera_target_track(...)`). As context it fixes the `lh t1,6(v1); li at,-1` order (as1 line ties: the
   inlined body's lines come from its own file).
2. The hoisted MBOX args (retail `lui a0,0x8014; li a2,-1; lui a3,0x5; move a1,t0; lw v0,data` before
   `if (linkToCar)`, 952 by shifts): w26c's f308/swd_best.c had them; bisecting from it showed the cause is the
   inlined StartNewBlastRing's EARLY RETURN: MAT3 `if (!v) return;` in the ring blocks it, `if (v) {...}` gives it
   (norm 42 -> 2 in one edit). The main function's own `if (!v) return;` is fine.
3. Retail's `lh v0,0(t9); sh v0,8(v1)` (v0, not a ring temp) = an s16 local: `slot = *(s16 *)data; v->slot = slot;`.
4. Frame 344 -> 352: two unused ints after clr in the ring's locals (exact residual, disclosed in the header).
   Measured alternatives that fit the frame but break something else: an s16 `dlidx` local in MBOX_NewObject (MAT3
   mbox.c) gives StartBlast 352 but StartNewBlast 280; one-level ZOID_RemoveObject changes both by 8 per site.

### music_seq_load = CreateCar: MATCH (cloud/matches/music_seq_load.c)
w36l's residual (34 words, temp names only from the ZOID_SetTexture line) traced with ugt.sh: ugen allocates the
texture argument `&gTexDefPtrs[t >> 10].texDefs[t & 1023]` base-side first (sra/mul8/lw get t8/t9/t1, and/mul36
t2/t3) but retail's names are exactly the index-side-first order on the same free list (and t8, mul36 t9, sra t1,
mul8 t2, lw t3; as1 then schedules the sra chain first). A ugen probe (scratchpad ut/t1.c, six forms) showed ugen
evaluates the index side first when t is an ARRAY element (`garr[11]`) and the base side first when t is a scalar
(`g`, a parameter, or a local copy). The drafts used the scalar stand-in D_8016137A; the N64 gTexList is D_80161368
(10 entries) and the sheen texture is D_80161368[9]: one edit, 34 -> MATCH.
`score.py fn cand/music_seq_load.c music_seq_load --flags "-g0 -O3 -mips2 -G 0 -non_shared"` (builder):
```
music_seq_load:
  MATCH
exit=0
```
Unit: `blob_unit --tag w36x4 score music_seq_load func_8008E06C func_80092BC8 func_80092BF4 --with
cloud/matches/music_seq_load.c --neighbours` -> 4/4 EQUAL, locked bodies that differ 0. With the deliverable's body
alone (helpers as prototypes, cc/v3/only.c) + w36l/singles/func_8008E06C.c, func_80092BC8.c, func_80092BF4.c ->
`EQUAL music_seq_load: 357 words (kept, c_only.c)`, 4/4, 0 locked bodies differ. With the CURRENT locked m2c
singles instead: `FAIL music_seq_load: 256 of 357 words differ; compiled body is 338 words` -> INTEGRATION NOTE: land
w36l's three helper-single replacements (already required by w36l's tire group) before or with this single.
Frame residual u0..u3 kept from w36l (retail 192; without them 176, 20 words differ). Tried for it: an `int rim` /
`void *tex` local in the rim code, the fillers at the top of the list: no.

## What generalises
1. A store through a pointer VARIABLE (`b->field`, b = &array[...]) kills a global's PRE availability in uopt
   even though uopt later folds b into the strength-reduced IV (the store still codegens IV-relative). Use it when
   retail reloads a global (here the rand seed) that an index-form store would not kill.
2. A load uopt leaves in place is still removed by as1 if the stored register is intact; retail keeps it only when
   ugen has reused that temp first (one extra ring temp in the block, here `clr.c[3] = b->alpha`). Check both layers
   (pretrace antloc/DELETE, then the ugen listing) before writing variants; a volatile-read oracle proves the reload
   is the only structural residual.
3. An EARLY RETURN inside an inlined static helper (`if (!v) return;` in StartNewBlastRing) stops uopt carrying
   later call-argument constant webs over an if/else in the caller; the `if (v) {...}` form restores retail's
   argument setup before the branch (and the shift-expanded multiplies that follow from the missing free register).
4. ugen evaluates `&tbl[t >> 10].p[t & 1023]` index side first when t is an array element and base side first when
   t is a scalar/parameter/local: a scalar stand-in for an array element (D_8016137A for gTexList[9]) is enough to
   rename every later temp. Use the real array symbol with an index.
5. Locked kept singles that umerge inlines (camera_collision_check = EmitGameSound) go in the group as context
   copies: their own source lines decide as1 ties inside the caller.
6. When a prior draft has the right structure in one region (w26c's StartBlast had the argument hoisting), bisect
   from it toward the current form instead of guessing; it found item 3 in four batches.
