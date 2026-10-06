# Runtime A storage/slot prompt reconstruction

`func_8038A400`, `[0x8038A400,0x8038A62C)`, 556 bytes / 139 words.
**NONMATCH: 116/139 words differ**, no unresolved or unverified relocations,
no extra words. Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.

This is a complete natural C reconstruction, improved from the first
121/139-word, three-extra-word draft. It scans the four 16-byte status records,
checks established per-slot validity/capacity helpers, and creates, controls or
releases an existing prompt before requesting a state transition. The inferred
storage/slot interpretation and historical helper names are not original-name
claims. No exact arcade donor was identified.

The signed-byte return types of `func_800A1A3C`, `func_800A35F8`, and
`func_80094FC4` come from their locked bodies. The capacity helper returns
unsigned32. Two-argument player state/mode setters use the shared documented
prototypes. The MultiBlit layout follows the locked `sound_control` source;
the status structure only models the observed first byte and 16-byte stride.

The main remaining source issue is control-flow formation: the first scan's
break/exit folding differs, causing positional shifts before later temporary
allocation differences. Both O3 and O2 give the same best result. Natural
pointer/index/while/do scan spellings, lexical-scope/early-exit variants,
declaration ordering and meaningful integer-width variants were tried without
closing the residual. There are no dummy helpers, fabricated ABI formals,
unused locals, volatile fields or private compiler flags.

Reproduce with documented IDO from repository root:

```
python3 tools/cloud/score.py fn cloud/work/frontier/dot_runtime_a_storage_prompt_20261006/candidate.c func_8038A400 --targets asm/us/ovl_a --flags "-g0 -O3 -mips2 -G 0 -non_shared"
```

Matching compile/score only. This research is not a strict match, standalone
`cloud/matches` entry, accepted coverage, or ROM result. Full behavior and
integration verification are left to the independent checker.
