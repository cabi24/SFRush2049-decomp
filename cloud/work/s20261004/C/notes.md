# s20261004 / lane C — notes

Builder: watchman2, private dir ~/rush2049/scratch/s20261004_C (copies of
tools/cloud/score.py, asm/us/blob, include; IDO = toolkit cache
d4c39cbc85750cd02d3494f318c65e770bbf7d4b10aae137bcf5b5f149137c5b/ido).
Scorer: `python3 tools/cloud/score.py fn|group ...` (true relocation-resolved
word compare). Extents = asm/us/blob section lengths (scanner/closure).
No true-zero match was obtained for any of the four targets.

## func_800BA61C (0x800BA61C, 424 B / 106 words) — best 17/106, residual = stack frame only
Path-point search: nearest point (s16 x,_,z records at D_801407F4, count
u16 D_801407F0) to object D_80151CE8[arg0] (0x50 stride; pos +0x0C/+0x14,
dir +0x18/+0x20, s32 radius +0x24), with side-of-heading sign change test.
Flags -g0 -O2 -mips2 -G 0 -non_shared. Key levers found: index the global
array directly (no local pointer -> lets IDO hoist pos loads, frees $f20),
natural dot-product order, `for (i=-1, j=0; ...; j++, i++)`, dist computed
before the side test.
- `ba61c/best_func_800BA61C.c`: 17/106 — every instruction/register is
  identical; only frame size (48 vs 56) and all stack-local offsets differ.
- `ba61c/match_with_unused_locals.c`: scores MATCH (true 0) but only by
  adding two never-used locals (`s32 probeA` above prevDist, `s16 probeB`
  above prevSide). That is a padding hack, so NOT claimed as a match. It
  proves the remaining residual is purely the 8-byte local-area layout
  (target: something 4 B above prevDist@48, s16 slot above prevSide@20).
  Vec3f-sized d/dir (360 order variants) reach the right frame but not the
  offsets (best 8/106). Next: find a real source reason for two extra
  stack-reserving locals (e.g. variables only used in compiled-out debug code).

## func_800979A0 (0x800979A0, 292 B / 73 words) — best 24/73, blocked (IPA empty-leaf)
Sequence start: audio_frame_sync(arg0+10,...)->D_80151AD4, display_list_alloc,
slot_value_get-style `func_80096288(slot,0,0); D_80156D38[slot].p0C`,
func_8001536C(...), D_8011EAA0 = func_800156E8(...), D_80151A6C = arg0.
Target keeps the slot in $a3 across `jal func_80096288` => -O3 IPA group with
the empty leaf. Same documented blocker as C62/B120 (`80096288` O3 blocker):
- plain leaf body: IPA inlines it (51/73);
- locked O2 leaf body (src/blob/func_80096288.c): still inlined (64/73);
- codex_sound_channel `if(0){switch}` leaf: call kept but IPA switches the
  leaf to a stack calling convention, value in a1 not a3 (24/73). The locked
  codex_sound_channel group itself reproduces the same failure for its
  slot_value_get/sound_update_channel context (verified).
`g979a0/` holds the group (currently the O2-body leaf variant).

## func_800A3640 (0x800A3640, 204 B / 51 words) — not attempted beyond analysis
Uses $s2..$s7 without saving them: IPA callee of the 3200-byte
track_render_process (Controller-Pak manager, 3 call sites), which
saves s0..s8 for it. A true match needs that caller in the same uld group
(see cloud/work/bigfish/track_render_process.md); a stand-in caller would
be invented context. Skipped.

## func_800A4CB8 (0x800A4CB8, 408 B / 102 words) — best 87/102, unresolved
Audio list/pool init: two 16-byte list headers (D_80146160, D_80146138),
D_801460F4 = arg0, 24-byte*count pool D_80144C48 and 32-byte*3*count pool
D_80146100 via audio_dma_sync/dma_request/memset, func_80091FBC insert loop,
D_801460C8[0..4] = 0, D_8011028C = 0, D_80110284 = 1.
Target shows IDO inlining artefacts (list base pointers in $v0/$s1, `li s0,1`
then `&D_801460C8[s0]`, `&D_8011028C` in $t4, loop index not strength-
reduced, arg0 reloaded from its home slot). -O2 single-file: 87-91/102;
-O3 uld single-file groups with non-static/static helpers: helpers are not
inlined (calls remain). Needs the real IPA unit (callers/helpers); left.
`a4cb8/v1-v3.c`, `ga4cb8/` hold the attempts.
