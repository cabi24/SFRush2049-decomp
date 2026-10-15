# Evidence, source experiments and N64 constraints

All N64 references use `c226123281724be9459ffa5d8ea41850bede3187`. The preceding
smoke, collision, path and weapons investigations inspected original GC ELF
symbols/DWARF and native bodies. The common donor ELF SHA-256 is
`d4a06e4516ab9a75e6de780beee10ea07b8358016e4964a8baf9e8eaa282908c`.
This packaging pass validates their actual files and receipts; it does not
rebuild GC or claim to have independently repeated the entire donor analysis.
Original artifact hashes, body hashes, scoped native paths, inherited fixture
results and verified upstream source links are in [packet.json](packet.json).
No GC source tree, ELF, raw target listing or ROM payload is published.

## Priority and next experiments

1. **CameraCollision / entity_iterate (224 B).** Smallest bounded new experiment.
   Native six-argument world collision is `(lookat, ipos, surfuvs, .5f, 1, 6)`;
   after a hit it normalizes `lookat-ipos`, advances `ipos` by one quarter and
   always returns zero. The new source adds the donor's named surface-pointer
   local to the original seed. GC locals are pointer + `float[3][3]` + `float[3]`.
   Native frame is 88, temp at sp+36, matrix at sp+48. Compare seed/candidate once
   each in the real unit, retaining the locked normalization group. Diagnose
   entry copies/homes and FP temporary order before another source lever.

2. **StartWeapon / C910 (276 B), through Astra.** Natural `v=&car.visuals[21]`,
   embedded `visuals[22]`, authentic two-argument callback and CarParts naming
   replace the old opaque declarations. Keep the full-word creation-result store,
   narrowed visual handle, unsigned model-byte load and post-call model reread.
   Historical differences are thirteen words at +0x60..+0xA0 in outgoing setup;
   the earlier ugen kept `t0`, while as1 reused sign-extension carrier `t6`.
   Compare nested-record/type information separately from the old two pointer
   round trips. Native creation returns a sign-extended s16 index, while the
   accepted `save_slot_valid` declaration says void. The candidate's seven-word
   declaration is an explicit incompatible research ABI, requiring reconciliation
   with real calls before integration. Do not change the shared prototype here.

3. **SyncCheckpointsToPaths / audio_mixer_main (732 B).** GC recovers a separate
   static `FindPathDistances(void)` boundary and
   `unsigned int GetNextCheckpoint(unsigned int cp)`. The four primary variants
   cross signed/unsigned successor type with definition above/below its direct
   caller; the distance helper itself always follows the synchronizer. Private
   helper names isolate the locked `func_800B930C`. The inherited w6b baseline,
   distance-boundary-only control, plus-two semantic negative and kept-helper ABI
   negative complete eight configurations. The kept-helper negative intentionally
   reuses unsigned_above's exact C with different `keep` metadata. The new source
   removes the inherited `pad[2]`; baseline-only padding remains diagnostic.
   All route types and the full `audio_priority_find` body are retained verbatim.
   Its historical 432 B provisional EQUAL depends on this caller; it is outside
   the fourteen-target total and earns no new credit before parent acceptance.

4. **AnimateBlast / entity_physics_update (712 B), then StartNewBlast / 90308
   (1,128 B) and StartBlast / save_write_data (1,200 B).** Begin with the supplied
   natural effects source and original StartBlast baseline in both the group and
   whole unit. The natural AnimateBlast historical residual is 21/178, a slot
   expression versus inlined setter colour-priority issue. Empty gotos are not a
   proposed fix. StartNewBlast's device-based w26c residual is 202/282, frame
   200 versus native 272; the packaged natural source is different. Its earlier
   header's natural aligned-row count is not that strict score.

   StartBlast's historical 149/300, frame 248 versus 352, uses a hypothesized
   inlined `blast_init(s16 slot)`. New source removes only its unsupported unused
   `mat[12]` and adds a disclosure. GC's StartBlast `mat` is a four-byte **pointer**
   to a 48-byte transform, not a local matrix array. The old array still left
   104 frame bytes unexplained. Retain signed initializer homes and word colour
   copy; trace outer pointer/node spills, sound-69 setup and genuine inline areas.
   GC StartNewBlast locals are only visual pointer, index and colour. The inherited
   natural effects draft's unused `vehicle` is still unproved, explicitly baseline
   material. Validate the initializer's orphan-stub hypothesis before installation.

5. **CalcResPathBounds / B9740 (408 B).** Current useful source is w14r 83/102.
   Discarded w14k “77” omitted a required vertex assignment and is invalid. GC
   has s16 extrema, `validPt`, indexed alternate ranges, no early break and ternary
   stores on every axis. A typed/indexed port is the next experiment, not supplied
   as completed C. If three values still cross loops versus native's sole saved
   s0, diagnose alias/PRE and induction forms before declaration trials.

6. **PlayMoveSound / camera_look_at_point (796 B).** Work within the existing
   target-path group and preserve all eight locked members. GC locals are dPtr,
   one float ratio, int maxVel and int curVel. Retain integer truncation before
   abs, zero-max fallback, division then `/2.0f + .5f`, original clamp topology,
   N64 sound IDs 1/18/97 and actual emitter signatures. Current draft uses extra
   temporaries and a disclosed empty-if; deleting it is not a proven fix. The
   already matched animator is the real moving-sound caller.

7. **ReadPathData / audio_effect_apply (696 B).** The current w17h context is the
   baseline, with no reliable fresh score. First change only the header-skewed
   route types and three no-write conversion-loop nests to the donor topology.
   Preserve native endian handling, allocation ownership, four drone headers,
   restart order timing→sync→alt→bounds→timing, and success sync→alt→bounds→timing.
   Compile with the real `physics_response` caller/callee unit. Its 920 provisional
   bytes depend on the loader's IPA convention; standalone scoring is insufficient.
   Keep locked `func_800B930C` verbatim above that body initially; w17h measured
   after-definition 2-word and separate-file 16-word regressions.

8. **ShotToWorldCollision / input_deadzone_apply (3,580 B).** Reproduce w12a
   `best.c` with its original `hdr_ipcorder.h` and provisional cylinder callee
   context before editing. Six arguments mean old/new/uvs/radius/interp/ignore;
   return is a surface pointer. GC has separate `radiussq`, unsigned i, s16
   quad0/quad1/midh/midv, u16 slnum[35]/path/spath/snum and three vectors plus a
   3×3 temporary. Call roles are quadtree query, DecodeSlist, cylinder test,
   CopyUV, halfpipe/path interpolation and normalization. Preserve all real
   partners; locked `func_800AD4C8`'s stand-ins do not prove the caller context.

9. **CollideCarBodyAgainstSurface / camera_play_script (3,520 B).** Strict
   793/880 and bad own-rodata differ from the 27-row aligned diagnosis; fresh
   target/object comparison comes first. Native -.707 uses bits `BF34FDF4`.
   GC supplies u16 vertIndices[16], bodyPoints[8][3], surfPoints[16][2],
   surfTrans[3][3], four distinct i/j/k/l integers and an isSurfReady flag.
   Test signedness and array/pointer forms individually around the first body
   transform and endpoint loads. Inspect a1/t0 preference, ka/kb priority
   asymmetry, dy spill order and multiply operands. The earlier empty-block
   hypothesis cannot create the required priority asymmetry. No recovered GC
   source spelling for this body is available, and no new implementation is made.

10. **Weapon cleanup and animation, 8,116 B combined.** Build the real AA8C
    callback and both cleanup helpers together. `_RemoveGattMFlash` removes five
    guarded handles at stride 0x10C; `_RemoveCannMFlash` uses stride 0x148, then
    ripple handle +0x104 and signed-byte texture index +0x139. A95C consumes slot
    in low `s4`; AA14 consumes signed-low `a0` and does not save s0–s3. An ordinary
    standalone o32 helper score cannot establish their private ABI. AA8C consumes
    visual pointer and signed-low a1 operation, saves s0–s7/ra, dispatches nine
    held types, handles crash/op cleanup, first-person camera masks, five flash
    objects, gatling random adjacent pair and cannon ripple progression. Use GC
    as behavior/type evidence while reconstructing N64 control flow and literals.
    No cleanup or 7,812-byte callback C is fabricated in this packet.

## Type, layout and behavior constraints

- N64 car stride **0x3B8**, GC **0x3CC**. N64 visuals start +0xF8 and entry 21
  is +0x2F0; GC +0x110/+0x308. A visual is 0x18 with signed halfwords at
  +4/+6/+8, data +C, time +10, callback +14. Callback takes `(visual*, s16 op)`.
- N64 defense adds a float phase at **car+0x39C**, absent in GC. Defense is 0x14,
  battle 0x34. N64 shoot/fade/loaded/time/pitch/yaw follow at 3A0/3A1/3A4/3A8/
  3B0/3B4. GC battle is 0x30; transplanting it shifts those accesses incorrectly.
  `c910/weapons_types.h` is the earlier proposal with a comment-link adaptation,
  not a canonical header change. Its 32-bit pointer assertions are intentional.
- CarParts is N64 0x40 / weapon +0x18, GC 0x60 / +0x38. Models retain consumed
  stride 0x808, unsigned body byte +8 and crash byte +0x640. Do not infer complete
  structure equality from those fields. Held weapon type 8 is NONE; shot type 8
  is TRACER. The adjacent 10/7 resource words are separate namespaces.
- Path checkpoints are 0x0C header + ten 0x50 records; twenty s16 path crossings
  per record start +0x22. Flattened views add the 12-byte header skew, so +0x2E
  is the same field, not a platform difference. Retained idx[17] is a partial
  historical view. Route points are s16 triples (6 B), alt records 0x10 and drone
  headers 8 B. Integer radius is compared to squared distance without a sqrt.
  Test GC's **unsigned** checkpoint successor privately before changing a lock.
- Smoke particles share a corroborated 88-byte layout, list descriptor 24 B;
  [smoke_layout_guidance.h](smoke_layout_guidance.h) is reference only. Blast
  object 84 B, per-car aggregate 400 B, transform pointee 48 B are documented in
  [blast_layout_guidance.h](blast/blast_layout_guidance.h). A transform pointer
  does not account for a local matrix frame allocation.
- N64 blast resource/player gates are real: players<4 and expanded-memory or
  track≥6 gates differ from GC. N64 per-object timer uses +25 versus GC +17;
  extra cloud alpha 95 versus 56. Preserve N64 values. Timer literal bits are
  `3D088880` (`0.0333333015f`), not rounded `1/30`. The .005 literal belongs at
  801239B4 (`3BA3D70A`); historical own-data failure at 80123220 is a different
  payload and must be diagnosed separately from register colour.
- Path sound/object IDs differ between platforms; GC endian registration/swaps
  must not replace N64 native no-write conversion loops. GC volatile time
  declarations do not establish N64 qualification. FMA lowering, ISA length,
  scratch slots and source spellings alone are not gameplay differences.
- Current labels are misleading: blast routines are not save/physics, checkpoint
  routines are not audio, target-path `Camera` is not the camera subsystem,
  `entity_iterate` is camera collision, and `input_deadzone_apply` is world
  collision. Proposed GC names remain research. No global rename, canonical type
  replacement or protected source change is part of this PR.
