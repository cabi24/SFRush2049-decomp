# engine_torque_calc unit (engine_torque_calc + transmission_ratio_get + engine_sound_update)

Score (blob_unit, tag w4c, `--internal engine_torque_calc`, best.c): engine_torque_calc 215/224,
transmission_ratio_get 19/452, engine_sound_update 273/357 differing words. Provisional: none of the three
can be claimed until all three are exact (engine_torque_calc takes node in s1 by IPA and clobbers
s2-s4/f20 unsaved).

Real semantics (names are historical): this is the level-object instantiation system.
- transmission_ratio_get(Place *rec, s16 parent, f32 *pos, void *src, s8 index, s32 place, s32 other):
  finds rec->name in the 122-entry metadata table D_80117530 (48-byte records, strncmp = func_800950AC),
  applies mode filters (D_801174B4 bit 3, D_80156994, D_8014A110 == 2, team "_BW"/"_FW" vs D_80152570),
  allocates a 112-byte node from pool D_80143FC8 (func_8008E3C0), fills it, places it (rec matrix/pos, or
  pos + identity D_8011418C), registers it with the entity table (func_8008E26C), optional shift/distance,
  and if D_801174B4 bit 22 is set runs engine_torque_calc on it. Returns 1 when the name matched, else 0.
- engine_torque_calc(node): per-node setup by category: state pointers from per-category arenas
  D_80150E98/D_80118DDC (record sizes D_80117510), kind 4 gets a 64-byte matrix record from D_80150F38,
  category 5 claims a 136-byte slot, texture id into the entity table, optional animation (func_80090284)
  and callbacks.
- engine_sound_update(): walks the scene list D_801392D0/D_801392D4 (36-byte scenes, 68-byte resources),
  instantiates each enabled resource via transmission_ratio_get, allocates arenas (audio_dma_sync = heap
  alloc), resolves key references D_801497C0, resets slots in mode 6, and runs engine_torque_calc on every
  node of the pool list.
Layouts recovered (offsets are retail evidence): Node (next 0, u8 flags 4, key 8, s32 handle 12 read as s16
at 14, s16 metadata 16, matrix[9] 20, pos[3] 56, xform 68, s16 80, f32 84, s16 texture 88, s16 90, s8 92,
s32 96, s8 index 101, f32 104, state 108); Metadata48 (name, file, callback, animation, s16 kind 16,
u16 flags 18, s16 20, s8 category 22, s8 element 23, f32 value 24); Place (name[16], matrix 16, pos 52,
flags 64, key 76); Pool (b0, count, size, mem, head 16, free 20 — same as camera_lerp_position);
Matrix64 (record, matrix 4, position 40, velocity 52).

Closers found: transmission_ratio_get's flag OR-in on the entity table is an inlined s16 getter
(`static u32 ent_flags(s16 id)`), `res = parent` in an else-if, the owner pointer as a local, a 40-byte
locals pad (frame 160), node stores ordered f92, f80/texture, index, f90, f96, f104. engine_torque_calc's
zeroing loop reuses `index` as its counter (gives slti + pointer reduction); engine_sound_update's
"found" search is a goto into the processing block.
Open residuals: (1) transmission_ratio_get spill slots at sp+60/56 vs retail 64/60 (18 words) — not moved by
unused locals; (2) engine_torque_calc colours node s0 and matrix s1 (traced proc 344: equal priority 2.0,
web order decides; forcing the swap with force.sh removes ~70 rows) and its frame is 64-72 vs 96; retail
shows several a-register webs (a0 = -1, a1 = texture, v0 = id, v1 = value, a0 = &D_801392D0) typical of
inlined helpers; the caller-less stub func_800AB7D0 just before engine_torque_calc is one deleted helper
(texture-setter candidates in o9/; pass `--internal func_800AB7D0`). (3) engine_sound_update: retail keeps
-4097 and 0x01000000 in s8/s7 and re-reads D_801392D0 with lui/lw; mine makes an address web for
&D_801392D0 instead; frame 168 vs 160.
Tools: tools/eu.sh (scores the three in the unit; EXTRA= for more flags), tools/etrace.sh (traced uopt).
