# Lane w14u: func_800F45F8 and sound_position_update

Status: **neither function matched.** Both are open. No cloud/matches, group dir or splice was produced.
Builder scratch: `watchman2:~/rush2049/scratch/frontier/w14u` (base copy, src/blob, include, tools/cloud, asm/us/blob
and blob_matched.lock.json synced from the Pi this session). Nothing committed, spliced or pushed.
Scoring helper: `sc.sh FILE.c NAME [lines]` (standalone score.py fn with full diff); batches via
`tools/vbatch/vbatch.sh w14u <dir> <name> --flags "-g0 -O3 -mips2 -G 0 -non_shared"`.

## Facts recovered (from retail asm, checked this session)

sound_position_update (0x800951E0, 111 words):
- `a1` is read as an integer: `mtc1 a1,$f4; cvt.s.w`. The second parameter is **s32**, not f32. The existing
  prototypes `void sound_position_update(f32 arg0, f32 arg1)` in src/blob (audio_distance_atten.c,
  Input_SetPadEnabledFlag.c, PhysicsObjectList_Update.c, Effects_UpdateEmitters.c, Input_SetAnalogBounds.c)
  look wrong for arg1. arg0 is a pointer (emitter with +8 = node pointer).
- Node fields: s32 +0x10 (type), s32 +0x14 (unk14), s8 +0x19 (flag), u8 +0x1A (count), f32 +0x20, f32 +0x24.
- Record from func_80091B00() (no args, returns pointer): u8 +2 kind, pointer +4 node; node->+0x1A incremented.
- Two tables: block with kind 5 uses counter D_80110250 and table D_80143CF0; kind 3 uses D_8011024C and
  D_80143AE8.
- Rodata literals, read from the image bytes: D_80123A64 = 0.2f (0x3E4CCCCD), D_80123A68 = 0.1f (0x3DCCCCCD),
  0.75f = 0x3F400000 inline, 14400.0f = 0x46610000 inline.
- Retail keeps `li t0,2` and `li t1,1` live across the function, and reads node/arg0 through a3/a2 after the call.
  Those reads survive func_80091B00 only because of the IPA preservation (a2, a3), so the standalone
  frame (32 vs 24) is not a valid signal. Judge by the unit score.

func_800F45F8 (0x800F45F8, 225 words, frame 80):
- Record stride 76 (D_8014A118 with tgt at +72, Tgt blk at +0x2C), active_player_count = s16 at 0x8014A108,
  count byte D_801543D4 (swapped when > 0), bit-packing loop writing bit 0 of each byte into the 9-byte records
  at (*blk)+0x6F4 (bytes +0x14), then net_session_update(arg1, ...), D_80154628 decrement,
  func_800F43B8 when (s8)rec[9] >= D_80154640, and a ranking block over D_80154450 (stride 76: +1, +2 s16,
  +0x4D, +0x99, +0xE5) before object_data_allocate on D_801543D4's record.
- arg1 is f32 (mtc1 a1,$f12). The arcade ancestor was not identified yet.

## Round log

sound_position_update (standalone score.py / vbatch unless noted):
- Draft v0 (hand): `strict 105 mnem-missing 26 aligned-missing 88` (standalone). Unit: 101 of 111 words differ.
- Round 1, spu/b1 (48 variants, 5 axes): best `strict 101 mnem-missing 34 aligned-missing 81` (v0001, v0017,
  v0033). (v0000 control score not recorded this session.)
- Round 2, spu/b2 (300 variants, 14 axes incl. empty-if condition, operand order, `f14` init, `(u8)` casts): best
  `strict 101 mnem-missing 34 aligned-missing 81` (v0053, v0081, v0165, v0247). No strict improvement.
- Structural draft v3 (goto layout matching the retail block order: check1 -> check2 -> call-1 block -> 310
  block): unit `blob_unit score sound_position_update --with spu/v3.c` gives "0/1 equal" (not equal). Standalone
  108 of 111 words differ.
- Round 3, spu/b3 (300 variants, 17 axes on v3): best `strict 102 mnem-missing 41 aligned-missing 90` (v0005).
  No strict improvement over round 1.
- Unit confirmation, v0053 (round 2 best): `FAIL sound_position_update: 101 of 111 words differ`,
  `locked bodies that differ in this unit: 0`, `blob_unit score: 0/1 equal`.
- Stop rule: rounds 2 and 3 gave no strict improvement over round 1's 101. The 3-round minimum is met, but
  the control v0000 was not re-run each round, so the rounds are not fully to the brief's standard. Not a success.
- Remaining residual: the constant registers (t0=2, t1=1 hoisted and kept live), the layout of the call-1 block
  (IDO emits it before the 310 block; retail after), and the unit diff `lw a3,8(a0)` / `move a2,a0`
  ordering. Next hypothesis: retail reads a2/a3 across the call, so the source keeps `arg0` and
  `node` both live through func_80091B00. Test that with a `diagnose` run on the unit build, which I did not run.

func_800F45F8:
- Draft v0 (hand, incomplete): `208/225 words differ` (standalone). Missing: the ranking tail, the stride-76
  pointer walk, and the correct argument passing into net_session_update. Only one scoring run was made, so no
  round count is claimed.
- Not started under the 3-round procedure.

## Next steps
1. sound_position_update: `tools.conveyor.pipeline.diagnose one sound_position_update --source spu/b2/v0053.c
   --flags "-g0 -O3 -mips2 -G 0 -non_shared"` (not run), then vgen rounds aimed at the constant-hoisting axis and
   the block order. Verify in the unit before any claim.
2. func_800F45F8: finish the draft (ranking block, stride-76 walk), then run the standard 3-round procedure.
3. Fix the sound_position_update prototype (arg1 s32) in integration; a locked file's signature should be
   changed only by its owner.
