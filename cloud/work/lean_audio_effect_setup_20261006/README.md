# audio_effect_setup: 184-byte group match candidate

Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Target: `audio_effect_setup`, `[0x80095800, 0x800958B8)`, **184 bytes / 46 words**.

Observed canonical group output:

```text
Members:
audio_effect_setup:
  MATCH
```

The full 46-word comparison reports zero differences, zero nonzero excess words,
no unresolved symbols, no unverified relocations and no errors. This is an
observed local match candidate only. It is not standalone, integrated game/ROM
coverage, an independently verified result, or a whole-group match.

## Source / required context

The starting point is the genuine typed B132 packet at
`cloud/work/ipa-groups/codex_effect_typed_b132/group.c` in the base above, whose
cleanup had six differing words. The candidate changes only the cleanup:

- Use a consumed local `Effect *effect = input` view of its real incoming pointer.
- Test the actual owner field before binding the local owner pointer.

The second change reproduces the native load/guard/owner-lifetime split. Without
the consumed effect view, the owner and effect receive exchanged saved registers;
together the ordinary C spelling closes all six words. This compile-affecting
spelling is disclosed rather than asserted to be the original source. There are
no extra formals, unused locals, artificial arrays, volatile accesses or fake callers.

The complete real `func_800988D8` and `func_80098FB8` callers, their actual
`entity_flag_check` / `audio_pitch_adjust` helpers, and the real insertion/removal
routines are included unchanged from B132. The new complete engine caller from
PR #184 was useful during exploration but is not required: the declared B132
context alone prints the MATCH above. No body has been replaced with a stand-in.

`group.json` is authoritative: the cleanup is the sole member/claim, the two
callers and their helpers are context, and the original root/export choices are
preserved. Context scores are informational and must not be promoted wholesale.
In particular, `audio_pitch_adjust` is inlined/deleted in this compiled group and
its separately accepted standalone body must remain with its existing owner.
The real callers remain nonmatching. No already-accepted context bytes are
counted as new work.

## Reproduce

Use the project's IDO 5.3 toolchain, from the repository root:

```sh
python3 tools/cloud/score.py group cloud/work/lean_audio_effect_setup_20261006
```

Exact group flags: `-g0 -O3 -mips2 -G 0 -non_shared`; the unchanged canonical
backend adds mandatory `-r4300_mul` for the assembler. No scorer or target edits.
The raw native pointers/field layouts follow O32 and the existing typed B132
contracts. Behavior outside valid linked lists, including corrupted links or
unexpected callback aliasing, has not been independently tested.

This lean publication contains only the full real C group, its declared recipe,
and these notes. No test harness, receipt, independent verification, full tests,
CI wait, production replacement, image/ROM integration or merge was performed.
The independent checker owns acceptance and merging.
