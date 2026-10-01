# Control-settings real helper closure (Lane A4)

Frozen 2026-10-01. **Both claimed helpers strict MATCH: 67 words / 268 bytes.**
func_800DCD58 39 words; func_800DCCE0 28 words. Canonical Rocky score.py group
--claims on delivered files exits 0, plain MATCH, without an unverified-relocation
allowance. Exact flags: `-g0 -O3 -mips2 -G 0 -non_shared`.
No splice, lock/layout edit, commit, or ROM gate performed by this worker.

## Actual closure, no stand-ins

Direct closure is 705 words: the two helpers and their sole actual caller,
control_settings638 words. Full retail caller has six DCD58 calls and four DCCE0
calls. Delivered full C caller retains those ten real sites and its15-arm menu
switch. It is unclaimed context, not a synthetic two-call wrapper. No ABI stubs,
synthetic source/runtime callers, unsafe dead reads, or undefined unset-register
expressions remain. No m2c/tool/scorer source was modified.

Seed evidence came from near_miss_B6/func_800DCD58_best.c and
near_miss_B4/func_800DCCE0_group_lead.c; those readonly artifacts were not edited.
Standalone ABI seeds cannot reproduce the retail unsaved s0/s1 and24-byte frame.
The real caller admits exactly that IPA allocation: DCCE0 matches immediately.
Two logical row/slot parameters suffice; the standalone unused arg0 formal and
its code-free read are removed. IDO naturally places the used parameters in
retail a1/a2 in this real whole-program closure.

DCD58 then differs 8/39 only in later temporary-register webs: its initial table
address, cached flags, unsaved s0/s1, and frame already agree. Replacing the seed's
casted `byte == 0` assignment with ordinary `byte = (s8)!byte` closes all 8 words.
Logical zero comparison has identical runtime behavior for every signed byte.
Only the first bounded target expression control was needed; controls.json records
that match. Additional blind controls were not run after both exact results.

## Claimed helper semantic audit

DCD58 computes table row offset arg0*88 + arg1*44, reads flags at+32, calls real
audio_doppler when mask0xC00 is set, then toggles byte+1 to its logical inverse.
It reloads flags after that call, and mask2 invokes real entity_flags_apply
(37,0,1,0) followed by byte0=1. It leaves those branch behaviors and call arguments
identical to full39-word target. Its retail unsaved s0/s1 clobbers are expected:
actual control_settings stores live s1 to88(sp) before its calls and reloads it.

DCCE0 computes the same table using its two row/slot parameters, tests flag mask7,
calls real resource_type_select with the loaded flags if nonzero, and stores
byte0=1. Full 28-word target match includes delayed stack adjustment,24-byte frame,
unsaved s0, parameter-register allocation, and branch-likely load of ra.

## Real caller repair and precise limits

The old control_settings m2c seed was263 lines with scalar collapsed buffers and
one unset-t9 expression. The delivered body restores byte arrays from the actual
stack-base intervals: sp80/sp188/sp290 each0x108 bytes, sp3980x110, sp4A80x148,
sp5F00x128, sp7180x4C. These sizes represent the observed local stack regions, not
recovered original C type names. The real table is declared an external byte array;
its address uses D_80153FD8, not an invented numeric address.

The case3 unset-t9 load is actually countdown_object+0x1AC after a fresh pointer
load (0x800DD8C4/8C8). Repaired to the real pointer field. slot_state_setup's four
calls pass actual slot11. All four DCCE0 and six DCD58 calls pass actual row/slot.
DD0C0 receives actual row/slot. String buffers and state_utility retain real values,
and the half-width object-manager result uses unsigned shift as retail does.

The seed omitted non-ABI attract-handler inputs. Reconstructed all actual values
from caller asm: mode-handler row/slot/text (stack text buffers for cases4/15/6/9,
countdown fields1DC/1E0/1E4 for cases11/12/13); video-handler selected byte1,
row/slot/text, and both stack auxiliary values from countdown state arrays; demo
selected bytes1/2, row/slot, fields1D8/3B0 and both selected array elements.
Array values are loaded from the actual global array base + index*4, without the
seed's extra pointer indirection. The global0xC/0x10 loads use countdown_state,
not a four-byte-shifted address relative to countdown_object.

These logical external prototypes make the real caller compile and retain its
actual high-level behavior. The external attract/slot functions are not inside this
packet, so its caller uses conventional ABI argument placement for those calls;
that is a known compilation-context gap, not a claim that caller instructions or
original formal-parameter ordering are recovered. Only the two helper bodies are
splice candidates. Final control_settings is unclaimed630/638, emits581 words,
and its two switch-table .rodata relocations remain unverified and unclaimed.
No context byte-match or complete638-word IPA module claim is made.

```bash
python3 tools/cloud/score.py group cloud/work/ipa-groups/codex_control_settings_a4 --claims
python3 cloud/work/tools/ipakit/deps.py closure func_800DCD58 func_800DCCE0 --mode direct --json
```

All compiler caches/objects are in private Rocky build/, outside repository artifacts.
