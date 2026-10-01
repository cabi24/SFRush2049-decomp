# Audio frame real getter closure (Lane A5)

Frozen 2026-10-01. **func_800B08FC strict MATCH: 99 words / 396 bytes.**
Canonical Rocky score.py group --claims on delivered files exits 0, plain MATCH,
without unverified-relocation allowance. Exact flags: `-g0 -O3 -mips2 -G 0 -non_shared`.
No splice, lock/layout edit, commit, or ROM gate performed by this worker.

## Actual source and mechanism

Original audio_frame_update group had a four-word B08FC residual and two synthetic
caller wrappers despite two actual sites each in its real audio_frame_update caller.
Copied into this NEW group and removed both wrappers; the existing group and all
locked sources were not edited. Real caller + B08FC + locked B0A88 total 361 words.
Removing fake callers preserves the original residual; it does not itself solve it.

The exact four differences were the model pointer carrier t9 versus retail v0,
with all other99 words/instruction counts agreeing. Global-source reference scan
found real func_8008B3A0: its ten retail words implement the same model-pointer
lookup at D_8012E708 + signed-handle*68 and return in v0. Copied its exact matched
body from src/blob/func_8008B3A0.c as unclaimed context, retained its original s16
parameter and s32 pointer-valued return, and replaced the flattened lookup with
that actual getter call. Umerge inlines it into B08FC, giving the target v0 carrier.
It initially added duplicate signed16 argument conversion (16/99); changing the
existing hd local from s32 to its actual handle-field type s16 removes that redundant
conversion and matches all99 words. This is a real getter, not an invented helper,
stand-in, ABI stub, or synthetic pressure function. One bounded local-type control
was enough after identifying it; controls.json records the strict result.

## Semantic and contract audit

B08FC's full99-word target retains its mode2/mode6/HudRec+7CA early exit clearing
the owned slot callback. Otherwise it sets real entity_anim_texture callback,
slot/index, resolves resource index D_80111299[slot*13+HudRec.byte8]*4+idx+0xE2,
and calls save_slot_valid with real flags15, previous slot16+idx handle, zero,
slot, -1,1. It stores the returned signed16 handle into own slot18+idx, looks up
that handle's real model pointer, writes D_80123C1C float at model+0x28, and calls
real model_data_load(handle,1,15). The added C getter emits no new runtime call
in claimed B08FC: every relocated instruction is identical to retail. Its handle
conversion semantics are unchanged for every signed16 field value.

The real caller still has exactly two B08FC and two B0A88 sites with slot and
literal indices0/1. Its prefix through+0x1BC is exact, including those actual
calls and parameter setup. No arbitrary caller argument duplication was added.
CarS stride0x3B8, SndSlot stride0x18 with handle+6/callback+0x14, HudRec stride0x808
with byte8 and short7CA, and SndCtl stride0x40 with vals+0x1C agree with full asm.
Its unmatched tail has folded master-slot offsets, redundant reloads of the float
constant in its four-iteration no-call loop, and changed counter registers; the
same records/fields/float values are written. It is not a context match claim.

Unclaimed context: audio_frame_update29/150 (same historical residual),
func_800B0A88 MATCH112 words, func_8008B3A0 MATCH10 words. Both latter functions
were already locked before this packet and are not new coverage. Their source
bodies and ABI semantics are preserved. No unverified claims remain.

## D11BC initial diagnosis (not a group submission)

Requested func_800D11BC is ABI, no IPA edges, direct closure35 words. Its three
ABI callees are110/137/249 words. Canonical current score reproduces B4's7/35:
only initialization-store scheduling differs; frame/global register lanes and
floating multiply tail already agree with current VR4300 errata handling.
B4 previously exhausted store/debug variants. No justified IPA allocation lever
was found, so no speculative 531-word helper group or more blind probes were built.

```bash
python3 tools/cloud/score.py group cloud/work/ipa-groups/codex_audio_frame_a5 --claims
python3 cloud/work/tools/ipakit/deps.py closure func_800D11BC --mode direct --json
```

All compiler objects/caches remain in private Rocky build/, outside repository artifacts.
