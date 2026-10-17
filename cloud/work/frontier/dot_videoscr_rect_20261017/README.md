# videoscr calcMenuRect: genuine-caller research candidate

Baseline: `58425072b49e95f0fbe5c4c239988fb4885f7c5c`.

## Result and limits

`particle_render` is `calcMenuRect@videoscr.c`, target 0x800B8650, 448 bytes / 112 words. This package compiles with the stock IDO 5.3 O3 group pipeline and returns **accepted=true, 0/112**, no unresolved or unverified relocations, no errors or extra words. The scorer verifies its own literal range at 0x8012267C..0x8012268C. Stack frame and buffer anchor both agree.

This is a **local research match candidate**, not canonical integration, full-unit proof, or new ROM coverage. Claims remain empty. `particle_collision` (initScreen@videoscr.c, 0x800B8820) remains **5/253**, solely the frame/stack-home residual: frame 104 versus 136. Its float literals verify. `physics_sym`, the genuine locked root, remains exact. The inherited context definitions are now explicitly listed as members and measured: `slot_state_setup` 2/58, `object_create` 28/28, `func_800B669C` 5/5. They are nonmatching context, not new results. The existing `func_800B59F0` source has a mixed-section own-data verification refusal in this expanded group; the full receipt preserves that failure. Do not promote the entire package or describe the whole group as matched.

No synthetic callers, padding, unused locals, forced register assignments, or compiler modifications are used. Stock and instrumented-uopt complete objects are byte-identical; instrumentation is diagnostic only.

## Source basis

The genuine graph is physics_sym -> initScreen -> calcMenuRect. MAT3 videoscr DWARF records calcMenuRect locals in this order: menuSize, optionWidth, valueWidth, menuIdx, unsigned-byte str1[40]. It also records validOption(enum item), returning signed char, with one signed-char local named valid. Restoring that semantic helper and the four-word local ordering gives the target allocation and buffer position. The old 56-byte array was replaced by the authenticated 40-byte extent.

N64-specific unsigned text-width comparisons, font 11, array-only option validity and existing object layouts are retained. The later MAT3 validOption cable/state check is absent from the N64 target and is not transplanted. The color helper uses the N64 target's full-width object index; MAT3's signed-short object contract is not asserted for N64.

HAL_SetFont retains its real nested queue-lock bodies, return type and old-font local. The initializer uses a real four-byte color union and color-pointer helper, no-argument createBG ABI, and N64 float values verified by the target-owned-data check. The MAT3 video initialization body reports 100% individually; its containing TU is incomplete, and MAT3 calcMenuRect is partial. Those MAT3 statuses are provenance, not N64 verification.

## Reproduction

From the repository root, with stock IDO_DIR and MIPS tools configured:

    python3 PATH/TO/THIS/DIRECTORY/verify.py
    python3 tools/cloud/score.py group PATH/TO/THIS/DIRECTORY

This invokes the unchanged repository group compiler/scorer. Read `verification.json` for complete per-member acceptance and refusal states. No target objects, bytes, raw assembly, intermediate streams, trace dumps or toolchain binaries are included.

## Bounded negative evidence

- Original inherited direct-read helper, 56-byte buffer: 9/112.
- Genuine validOption boundary, 40-byte buffer, direct return without its recorded local: 5/112, frame 120 rather than 128.
- Restore the recorded signed-byte valid local while leaving the inherited rectangle local order: 1/112, frame 128 but buffer anchor 16 bytes high.
- Restore the four authentic rectangle locals before the 40-byte buffer and remove the redundant outer flag local: accepted 0/112.
- Restoring HAL_SetFont's old-font return preserves accepted 0/112.

The initializer improved from 25 opcode differences to zero after restoring the color aggregate, validOption and no-argument createBG. Its five frame/home words remain open. No arbitrary frame-size workaround is accepted.

The reproduction script also rejects a wrong-font mutation and a wrong-literal mutation. The literal mutation is rejected by ownership verification even when its instruction shape remains plausible. `test_results.json` records all members and all four negatives.
