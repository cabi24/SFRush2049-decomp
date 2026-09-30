# audio_pitch_adjust -> 18/18 MATCH (real function, extent 18 words)

## Current status (rescored 2026-09-30, master d0891f3)

**18/18 MATCH** (strict, `extscore.py --norm`: exact after alignment 18/18, structure 1.000; also via
`python3 cloud/work/tools/zbuild.py cloud/work/ipa-groups/audio_pitch_adjust --as1=-r4300_mul` since the target is registered). Extent 18 words. Caveat: matches with stand-in callers
`caller_a`/`caller_b`, so this file is not spliceable as is. Closure (approximate `closure.py`): the real callers
`entity_ai_pathfind`, `func_800988D8`, `func_80098FB8` and others are absent, replaced by the stand-ins.

`audio_pitch_adjust` (0x800959DC) is a real head: `addiu sp,-24`, IPA parameter in `$s0`
(a node pointer), no saves of `$s0/$s1`. Extent 18 words, one `jr ra`. INDEX said 752 insns
(`entity_ai_pathfind`), that is the caller, not this function.

Verify: `python3 cloud/work/tools/extscore.py cloud/work/ipa-groups/audio_pitch_adjust`
(`extscore.py` is the copy from `dynamic_difficulty`; targets here are registered, so `zbuild.py` works too).

- Body: `n->state = 3; func_8009211C(n->next, n); func_80091FBC(&D_80144C50, n, D_80144C50.next); n->next = &D_80144C50;`
- Getting IPA registers (`s0` param, `s1` unsaved): the function must NOT be in `keep` and needs
  two call sites (one call site gets inlined). `caller_a`/`caller_b` are stand-ins for the real
  callers (`entity_ai_pathfind` 0x800987E8 region and `func_80098FB8`); they are in `keep`.
- Callees `func_8009211C`, `func_80091FBC` are plain externs (ABI).

Blockers: none for the function. Splicing needs the real callers in the group (or the maintainers'
whole-module IPA), because the stand-ins are not the retail callers.

## Spliceability

Unspliceable as is: MATCH, but callers are stand-ins (`caller_a`/`caller_b`). The stand-ins only reproduce the IPA register/frame context; they are not
the retail callers, so this C cannot go through `blob_splice` until the real callers are in the group
(or the maintainers' whole-module IPA is used). Scores above were reproduced on 2026-09-30 with `extscore.py --norm`.
