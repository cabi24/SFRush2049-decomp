# w27e — game units (IDO 5.3), wave 27

Status: done. Nothing is installable, and the pak write cluster is still 5/7 EQUAL. Both open members are now
reduced to one traced cause each:
- func_800A1DD4 has every colour right. It is one ugen temp pop short, and a listing oracle gives 0 rows.
- func_800A1BB4's found-path shape is solved. Its remaining cause is v0 (see below).

For the resource-loader sub-units, func_80097164 is down to the func_80096CA8 calling convention (7 rows, frame
exact). func_80096CA8 now gets retail's `first` in t0, and the remaining cause is located.

Nothing committed, spliced or pushed. No permission denials. Builder scratch watchman2:~/rush2049/scratch/frontier/w27e
(toolkit reused from wtk) was deleted at the end. The wave27 fast-forwards (w27s renames, w27r tools) were synced.
cluster.c uses none of the renamed libultra symbols.

| Function | Bytes | Before -> after | Scorer line (whole-program unit, `--neighbours`, 0 locked bodies differ) |
|---|---:|---|---|
| func_800A1C6C, track_data_decompress, track_collision_setup, MaxPathZeroControls, slot_state_lookup | | EQUAL (unchanged) | `EQUAL func_800A1C6C: 90 words (internal, c_cluster.c)` … `EQUAL slot_state_lookup: 47 words (kept, c_cluster.c)` |
| func_800A1DD4 | 192 | 27 words (w26d) -> 21 words / 28 rows, **colour-exact**, ring-only; listing oracle 0 rows | `FAIL func_800A1DD4: 21 of 48 words differ` |
| func_800A1BB4 | 184 | 9 rows (cluster.c, unchanged); alt B1 found path solved, 18 rows | `FAIL func_800A1BB4: 29 of 46 words differ; compiled body is 42 words, target 46` |
| func_80097164 | 344 | 33 rows (old group draft) -> **7 rows**, frame 112/112; the rest is CA8's convention | `FAIL func_80097164: 10 of 86 words differ \| words 7 ops 4 norm 4 \| frame 112/112` |
| func_80096CA8 | 1204 | 285/301 -> 130 rows (c2) / 265 rows with retail's t0 `first` (c1) | `FAIL func_80096CA8: 289 of 301 words differ; compiled body is 298 words, target 301 \| words 130 ops 63` |

Behaviour checks (w27r tools):
- draft_check func_800A1DD4 (f1, `--params handle=a0,address=a1,bytes=a2,mark=t3`, saves):
  `VERDICT: EQUIVALENT over 9 calls / 1 scripts (saves 9)`.
- draft_check func_800A1BB4 (B1, `--params index=a0`, saves): `VERDICT: EQUIVALENT over 3 calls / 1 scripts (saves 3)`.
- regmap_verify func_80096CA8 `'s8=slot:int,t0=first:int'`: `VERDICT: CONSISTENT`. That is 1627 calls, all from
  func_80097164's site 0x800972A4 (t0 = 1); fp_call_wrapper's site never runs.
- regmap_verify func_80097164 `'s8=slot:int'`: `VERDICT: CONSISTENT`. Sites: audio_frame_sync 1449 calls,
  suspension_setup 19, brake_force_apply 159.
- draft_check func_80097164 / func_80096CA8 (race): see "Late results" at the end.

## Target 1: pak write cluster (`pak_write_cluster/cluster.c`, now with the f1 func_800A1DD4)

Unit command (Pi, repo root), output verbatim:
```
python3 -m tools.conveyor.pipeline.blob_unit --tag w27e score func_800A1BB4 func_800A1C6C func_800A1DD4 \
  track_data_decompress track_collision_setup MaxPathZeroControls slot_state_lookup \
  --with cloud/work/frontier/w27e/pak_write_cluster/cluster.c --internal func_800A1BB4 --internal func_800A1C6C \
  --internal func_800A1DD4 --internal track_data_decompress --internal pfs_queue_init --internal pfs_lock \
  --internal pfs_unlock --neighbours
  FAIL func_800A1BB4: 29 of 46 words differ; compiled body is 42 words, target 46
  EQUAL func_800A1C6C: 90 words (internal, c_cluster.c)
  FAIL func_800A1DD4: 21 of 48 words differ
  EQUAL track_data_decompress: 163 words (internal, c_cluster.c)
  EQUAL track_collision_setup: 117 words (kept, c_cluster.c)
  EQUAL MaxPathZeroControls: 99 words (kept, c_cluster.c)
  EQUAL slot_state_lookup: 47 words (kept, c_cluster.c)
  locked bodies that differ in this unit: 0
blob_unit score: 5/7 equal
```
`score.py group` was not run, because the group is not complete.

### func_800A1DD4: colours solved; one ugen temp pop left (traced to the statement)

The f1 form (`alt/a1dd4_f1_colours.c`, now in cluster.c):
```
end = address + bytes;
first = (bytes = (u32)(address - *file->data)) >> 5;
last = (u32)(end - *file->data - 1) >> 5;
map = *file->data + file->data_size;
for (i = first; i <= last; i++) ...
```
- **The codeless web in a3 is `shr(bytes,5)`.** The loop pretest uses it, uopt colours it, and as1 coalesces its moves
  away. With it, every colour is retail's: sum v0, X a3, last t0, map t1, i t2, mark t3. track_data_decompress stays
  EQUAL. The rest is the temp ring: last's `srl` gets t9, retail's gets t6.
- **Listing oracle.** On f1's ugen listing (`asm.sh`), adding `srl $25,$6,5; move $7,$25` at line 155 (first's
  shift through a temp at its own statement) and dropping the line-158 `srl $7` gives **0 rows**. as1 coalesces the
  move chain into `srl t2,a2,5`. So retail evaluates first's shift at its own statement, through a temp.
- **Where the temp comes from.** ugen uses temp + move when the destination register is also the operand, as in
  `bytes >>= 5`. The t2 form (`alt/a1dd4_t2_16rows_ring.c`: `bytes = (address - base) >> 5; for (i = bytes; …)`)
  makes the ring exact. But it loses the X web, so every colour moves one register down (16 rows). It also gives
  mark t2, which breaks track_data_decompress by 1 row.
- **Retail needs both**: the line-155 pop and an X web in a3 that the pretest reads. uopt sinks X to its first use
  (line 157/158) unless `first` survives copy propagation.
- **Forms measured that did not combine the two** (~60 in total):
  - statement orders (12 permutations);
  - `do {} while (0);` at 4 points (after `first` it makes uopt unroll the loop: 97 words);
  - holders and a volatile read in the empty range-check body;
  - `last` in five spellings;
  - `first`/`i`/`last` signedness;
  - chained `i = first = …`;
  - an explicit pretest with a do-while (`y1`–`y4`);
  - reusing `bytes` for last's intermediate (`v1`–`v3`, `z1`–`z4`).
- **Next:** a form where `first` is a live variable assigned at line 155 with dest == operand, and `i = first` follows.
  For example, `first` itself reassigned, as in `first = address - base; first >>= 5;` but with the subtraction in a2.
  Or use uopt's copy-propagation trace (pretrace) to find what keeps `first`'s store.

### func_800A1BB4: the found-path jump-around is an inlined value return; v0 is the blocker

- **B1** (`alt/a1bb4_B1_helper_found_path.c`): the list walk is a non-static `--internal` helper `a1bb4_find(index)`
  that does `return p` from inside the loop and at the end. The wrapper is
  `if (!modified) return; if (a1bb4_find(index) == 0) modified = 0;`. It reproduces retail's
  `beqz t8,next; nop; b exit; nop` exactly. The exit test reads p in the retval's coalesced register.
  - The cost is that any inlined value return forbids v0 to webs live across it (`forbidden0=0x50000000`). The
    record address `&D_80144030[index]` (retail v0) gets v1, and p/next rotate: 18 rows.
  - B13 (a single `return p` at the end) shows the same forbid.
- **umerge inlines a one-statement wrapper into track_data_decompress** ("compiled body is 2 words": B2, B6, B7, B8,
  B11, B12). A wrapper with its own early `return;` is not inlined (B1, w25e base). So retail's func_800A1BB4 has an
  early return in its own body.
- **Found-path holders that changed nothing** (all 9 rows):
  - `{ next = p; break; }`, `{ p = p; break; }`, `{ data = 0; break; }`, `{ index = index; break; }`;
  - `do {} while (0);` or `if (1) {}` in the found block;
  - `goto done`;
  - `do {} while (0)` in the early-return block.
- **Next:** a found-path block that survives to ugen with a dead instruction that is not a v0 return value. For
  example, an inlined helper returning through a pointer argument, or a value return whose web dies before the
  record's live range (a helper that also does the final store). Measure the forbid with ctrace first.

## Target 2: resource-loader sub-units of component 104 (`res_loader/`)

The function is not audio. It is the resource loader. STATUS.md of `cloud/work/ipa-groups/audio_frame_sync` describes
it: func_80096CA8 fixes up the OBHD/TXHD/PLHD/PTHD/TXLD/IMAG chunks, and func_80097164 DMAs, LZSS-decodes or inflates
a slot, then calls func_80096CA8(slot, 1). It is N64-only, so there is no arcade ancestor. Callers in the units:
- fp_call_wrapper and brake_force_apply are w27d's; their bodies are used unchanged as context;
- audio_frame_sync and suspension_setup are unmatched; the old group drafts are used as context.

All of these are in `res_loader/unit_best.c`. It is the old group.c head plus `lzss_decompress(s32, void *)`,
`inflate_decompress(s32, void *, s32)` and `osInvalICache(void *, s32)` declarations (all three were implicit).

Command: `blob_unit --tag w27e score func_80096CA8 func_80097164 … --with res_loader/unit_best.c --internal func_80096CA8 --internal func_80097164 --neighbours`
(`locked bodies that differ in this unit: 0`).

Register mappings, proved from the call sites (ugen prototype order):
- **func_80096CA8 (slot, first) = (s8, t0).** fp_call_wrapper does `move s8,a0; jal; move t0,zero`, and
  func_80097164 does `jal; li t0,1` with slot already in s8. regmap_verify: CONSISTENT.
- **func_80097164 (slot) = s8.** audio_frame_sync and suspension_setup do `move s8,a3`; brake_force_apply does
  `lw s8,D_801569AC`. regmap_verify: CONSISTENT.

### func_80097164: 7 rows, all from the callee convention (`res_loader/func_80097164_best_piece.c`)

- **Levers:**
  - a `u8 type = r->type` local;
  - one `dst` variable selected per branch (retail s0 in all three paths);
  - `rom` before `size`;
  - a real `OSIoMesg mb` (the old draft had a 4-byte M2C_UNK).
- **Remaining 7 rows:** slot s5 vs s8, `sw s5,0(sp)` stack argument, `li s8,1` vs `li t0,1` (all func_80096CA8's
  convention in our unit), plus one `lw s3` scheduling row.
- **Frame 112/112** needs three unused locals (`u8 *dst2, *dst3; s32 kind;` declared, never used). That is the exact
  frame residual (96 without them; 3 slots plus 8-byte alignment = 112). They are DISCLOSED here and must be in the
  header of any deliverable.
- Alternative not proven: the caller-less locked stub func_8009715C sits between func_80096CA8 and func_80097164,
  which suggests an inlined helper. A non-static `u8 *func_8009715C(ResSlot *)` buffer-select helper (r8) gave 75
  words / 61 rows, so it is not that helper.

### func_80096CA8: retail's `first` in t0 reached; slot loses s8 (`res_loader/func_80096CA8_c1_piece.c`)

What retail's frame and registers show (traced from the disassembly):
- **Frame:** `ParseCtx pc` is the first declared local (80..111). The other homes are flags 64, a slot copy 68,
  and r 52.
- **No pointer locals for the tables:** objs/texs/pals are coloured expressions `&D_801161F4[slot]` etc. (s3/s0/s4),
  so the draft writes them in global form.
- **The object loops' bounds use a second copy of slot** (`sw s8,68(sp)` early, `lw t6,68(sp)` -> s7 =
  `&D_801161F4[idx]`), while the stores use s3.
- **`first` is a split param web**: pieces in t0, home 116 = its o32 slot.

Results:
- c1 (global form, `idx = slot`, pc first) gets `first` in t0 with the 116 home, as retail does (frame 128/112).
- Slot's param web is split: save 0.89 against 9 s-registers already taken. The D_80140A04 address constant takes
  s8 (save 20). Retail rematerialises it per iteration (`lui t9; lb t9,2564(t9)`, branchy `? 1 : 0`).
- c2 (`D_80140A04 ? 1 : 0`) removes that web (130 rows), but then `first` takes the free s8.
- **Retail needs slot decided before `first`, or s8 blocked for `first` only.** Next: ctrace the `first`/slot
  priorities with the frame fixed (pc first, no tx/img named locals), and test whether `first`'s inner-loop uses are
  really the param (retail tests t0 three times per j iteration) or a copy.

## What generalises

- **as1 coalesces move chains** (`srl $7,$6,5; move $10,$7` -> `srl t2,a2,5`). So a coloured web can be codeless
  in the final code but still block a register (func_800A1DD4's X in a3). Check the ugen listing (`ugt.sh`) before
  calling a web "codeless".
- **A ugen temp pop with no code is `op tmp; move var,tmp`, emitted when the destination is also the operand**
  (`x >>= 5`, `last = f(last)`). as1 coalesces it. This is a third code-free pop mechanism, next to the as1 narrowing
  deletion and the label-broken CSE.
- **Any inlined value return forbids v0 to every web live across it** (ctrace `forbidden0` bit c1). A
  value-return helper inside a loop whose record pointer lives in v0 cannot be retail's shape.
- **umerge inlines a one-statement wrapper of an inlined helper.** A function with an early `return;` of its own
  is not inlined.
- **IPA split params keep their o32 home**: func_80096CA8's t0 `first` spills to 116(sp) = 112 + 4.
- **Unused named locals size the frame even when a used local does not**: r10 (kind used) gives 104, r11 (kind
  unused) gives 112. Measure the frame, do not count declarations.

## Integration

Nothing is installable. When func_800A1BB4 and func_800A1DD4 match, the group is:
- members: func_800A1BB4, func_800A1C6C, func_800A1DD4, track_data_decompress, track_collision_setup,
  MaxPathZeroControls, slot_state_lookup;
- keep: track_collision_setup, MaxPathZeroControls, slot_state_lookup;
- unit_overrides force_internal: func_800A1BB4, func_800A1C6C, func_800A1DD4, track_data_decompress;
- pfs_queue_init, pfs_lock and pfs_unlock as non-static `--internal` helpers (not members, not keep). If B1's form
  is adopted, a1bb4_find is one more such helper;
- no data-symbol changes, nothing superseded;
- run `score.py group` on the builder before claiming.

The resource-loader units need func_80096CA8 first. When it matches, func_80097164 should follow (its residual is
the convention), and fp_call_wrapper/brake_force_apply (w27d) unblock.

## Late results

- draft_check func_80097164 (r11 piece; `--params slot=s8 --callee 'void func_80096CA8(s32 slot, s32 first)=s8,t0'`,
  race): `VERDICT: EQUIVALENT over 79 calls / 1 scripts (race 79)`.
- draft_check func_80096CA8 (c1 piece; `--params slot=s8,first=t0`, race): `VERDICT: DIVERGES`. The first divergence
  is at frame 2, call #6: `word 0x80031AF4: retail 80218A20->00000001 | draft unchanged gGameThreadStack+0x1044`.
  The only differing word is retail's `sw t0,116(sp)`. That is the spill of `first` (= 1) to its o32 home in the
  caller's argument area, written by the split register-parameter web; the draft, compiled alone, keeps `first`
  elsewhere. Every other word written matches (retail 1647 vs draft 1646). This is a compiler-shaping difference in
  the caller's frame, not logic, but it is a DIVERGES verdict and is reported as such.
