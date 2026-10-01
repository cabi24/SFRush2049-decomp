# Lane 1 worker B2 — refreshed one-word packet (2026-10-01)

One strict match delivered: `cloud/matches/save_load_data.c`, 109 words / 436 bytes.
Exact flags line `/* flags: -g0 -O2 -mips2 -G 0 -non_shared */`.
Copied exact deliverable back to Rocky and rescored: bare **MATCH**.
No integration, lock changes, commits, or ROM gates performed.

All four were absent from game lock at start. Used refreshed build/diagnose
source origins: existing near-miss full TUs for the first three; save_load_data
best permuter source read via cloud_worklist._best_source and expanded shim.
Exported current target objects read-only from coordinator n64_target.target_o_sha
into the documented private Rocky loop targets. Each exact seed ran through
strict score and diagnose. Compute serial, never more than one compiler process.

| Target | Strict baseline | Final strict result |
|---|---:|---:|
| mode_byte2_set | 1/20 | 1/20, IPA group prerequisite |
| object_type_byte2_get | 1/10 | 1/10, IPA group prerequisite |
| object_type_byte3_get | 1/10 | 1/10, IPA group prerequisite |
| save_load_data | 1/109 | MATCH |

## save_load_data

Single difference at +0x58: target addiu t9,t9,0x3B20 versus candidate ori.
The source stores hard-coded function pointer address 0x80093B20 at car+0x274.
Read-only target registry identifies that address as **drone_ai_update**.
Changed only the address literal to `(s32)drone_ai_update`; one directed variant
produces strict MATCH. The existing full TU already declares drone_ai_update.
The unlinked workbench then reports two relocation-layout sites because the
synthetic target object encodes the address literal while source object uses a
real function relocation. This is expected and demonstrates why the strict
linked scorer, not diagnose's masked count, owns acceptance.
No unused-local/dead-read quirk introduced; symbolic callback expresses intent.

## mode_byte2_set and object_type_byte2_get/object_type_byte3_get

These diagnostic 1-word counts are also truly one strict word, but the word is
an IPA outgoing argument convention, not a register-allocation near miss:
- mode at +0x1C: target move t0,zero; ordinary single TU move a0,zero.
- getters at +0xC: target move t0,zero; seed nop because call has no argument.

Checked existing `cloud/work/ipa-groups/mode_byte_set/STATUS.md`: it already
establishes real sound_update_channel takes force in IPA t0, with matches for
mode_byte_set/mode_byte2_set only when stand-in callees provide that convention.
That group has empty claims and is explicitly never spliceable with stand-ins.
Cloud toolkit handoff section 5 documents the real empty-func_80096288 blocker.

Five bounded controls: -O3 on all three retains the exact same one-word result;
explicit correctly prototyped zero argument on each getter changes nop to move
a0,zero, still 1/10. This establishes the missing semantic argument separately
from the IPA register. More ordinary parameter/type/line mutations cannot be
accepted as a remedy for the known callee convention. Stopped after the evidence
rather than spending the allowed twenty on irrelevant variants. No new claims.

The tool gets the local residual shape right but does not identify its group
prerequisite from the single candidate. Cross-checking real callee ABI and the
cloud handoff prevented repeating the known stand-in-only result. These targets
should be attached to a real sound_update_channel closure workstream.

All sources and probe results remain private at Rocky
`~/agents/B/scratch/codex_B`, including `B2_probe.py` and `B2_scores.json`.
