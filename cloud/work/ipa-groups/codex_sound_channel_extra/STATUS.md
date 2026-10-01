# Sound-channel extra real callers (Lane A3)

Frozen 2026-10-01. **Three new strict claimed MATCH functions, 94 words / 376 bytes**:
object_byte9_set 16 words, func_800F68A4 33 words, camera_shake_update 45 words.
Canonical Rocky score.py group --claims on delivered files exits 0; no unresolved or
unverified relocation allowances. Exact flags: `-g0 -O3 -mips2 -G 0 -non_shared`.
No splice, lock/layout edit, commit, or ROM gate performed by this worker.

## Real source and mechanisms

This new group copies audited real sound_update_channel + real empty func_80096288
from codex_sound_channel; that accepted directory was not modified. Four accepted
callers and slot_value_get are unclaimed real context. All five initially selected
additional callers were reconstructed from full retail assembly, not synthetic
wrappers. The two unmatched sums remain context only. No stand-ins exist here.

- object_byte9_set: retain signed-byte input across sound_update_channel, return
  previous signed byte at bank+9, store new byte. The ordinary real body matches
  on first compile; no dead switch was added to this target.
- func_800F68A4: copy each 12-byte entry's id at offset 4 to output, then append zero;
  reload bank entry count on every iteration as retail does. Initial compiler-derived
  offset/output-pointer webs were reversed (6/33); an explicit `offset += 12`
  induction variable makes all words exact. Count and offset stay within 255 and
  3060 because the real count field is u8; no introduced overflow or dead reads.
- camera_shake_update: preserve u16 input across sound call; if bank byte9 is nonzero
  return byte6, otherwise return byte7 for id32/id>=256 or missing negative mapping,
  zero for id10, and selected entry byte8-byte6+1 otherwise. Full asm gives byte
  offsets 8/6, stride12; using Entry.pad[3]/pad[1] replaces equivalent pointer casts
  and closes five temporary-register differences. Entry is the already audited
  pointer+id+seven-pad-byte layout, total12, id offset4.

D_80149B60 is declared s8 here because the sum-global target uses lb. Writes from the
accepted mode setter retain identical low-byte semantics and all four accepted
context callers still score MATCH. No claimed function otherwise changes its role.

## Forty bounded source controls, all compile

controls1.json (11), controls2.json (16), controls3.json (13) record strict scores.
They test real getter inlining, return/local integer types, typed entry fields,
explicit byte-offset/output-pointer induction, and sum grouping. Three new targets
match; two stay unclaimed: object_bytes23_sum 4/18 (global base t2 versus retail t4),
object_bytes_sum_global 11/21 (same base plus load/result webs). Named partial-result
forms reduce the latter to9 but were not needed in the frozen source.

Other unlocked direct sound callers: slot_state_setup 58 words (known full-module
return-copy/color blocker), state_utility99, object_manager_update133,
audio_doppler_calc281, explosion_effect365. All fourteen direct caller names were
checked against blob_matched.lock.json; only the four prior accepted callers were
already locked. This packet does not claim any old context match as new coverage.

## Semantic context and register contract audit

Sound runtime body retains the prior full122-word assembly audit: entry id offset4,
bank-count loops, forced-only range/global resets, no-op callee, and final mode-derived
state assignments. Its compiled instructions are unclaimed 122/122 +3 nonzero extra;
slot_value_get is unclaimed13/15 +1 extra. Empty func_80096288 compiles jr ra;nop
versus retail empty branch+jr sequence (3/4 different), semantically no-op. Its
inherited unreachable if(0) switch prevents umerge inline; no runtime loads/calls or
synthetic side effects are introduced by it.

Independent raw emitted-function audit bounded by next compiled symbol shows
sound_update_channel defines only at,v0,v1,a0-a3,t6-t9,sp,ra; the real empty callee
has no register definitions. Neither writes t1 or t4, which the new callers and
unclaimed sums keep live across the sound call. Thus the real callees preserve the
retail caller clobber contract; the claimed caller matches are not paired with a
callee that destroys their carried values.

```bash
python3 tools/cloud/score.py group cloud/work/ipa-groups/codex_sound_channel_extra --claims
python3 cloud/work/tools/ipakit/deps.py edges sound_update_channel --json
```

Compiler objects/caches remain in private Rocky build/, outside repository artifacts.
