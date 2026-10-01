# Stunt clear real closure (Lane A6)

Frozen 2026-10-01. **No new match claimed. func_800C3578 remains 2/37 words.**
Canonical Rocky score.py group returns exit 1; claims are empty. Exact flags:
`-g0 -O3 -mips2 -G 0 -non_shared`. No splice, lock/layout edit, commit, or ROM gate.

The only strict differences are its pointer spill/reload at +0x60/+0x70:
retail uses36(sp), candidate32(sp). Same40-byte frame, same instructions and
registers elsewhere. Retains real event11/index call and real memset on the124-byte
player record, then ORs flag0x15000000. No input, memory-field, callback, or event
semantics were changed to claim a match.

## Real context and concrete controls

Direct target+callee closure is334 words (37 + actual func_800C1B60 297). With
only that actual call site, umerge specializes/inlines the event11 call and the
caller no longer matches (32/37 plus233 extra words). Restoring the full existing
real scoring callers prevents this: eight real functions,1701 words in total.
This NEW directory copies camera_scene_manager's real source and removes all
three historical synthetic wrappers. No synthetic pressure functions or source
helpers were introduced. The old group/accepted sources were not modified.

Twenty-four bounded controls tried typed scalar/aggregate pointer carriers,
reusing the original pointer, lexical lifetime blocks, memset's actual return,
byte/word layout declarations, and address casts. Empty-layout declarations move
the hidden pointer spill to the desired36(sp), but inflate the frame40→48 and
shift argument home slots; they leave four differences and are not delivered.
Typed carriers change register allocation (10–15 differences); pointer laundering
breaks global CSE (15–33). Plain first-field pointer and separate lifetime blocks
remain2. One unsigned-parameter control failed a prototype consistency check;
its result is explicitly recorded as a compile error, not a score improvement.
The frozen source is the clean real-context baseline, without speculative padding.
controls1/2/3.json record all results. No further blind spill-slot permutations ran.

## Limits

All context scores retain the existing actual group results: C1B60 9/297 with four
unverified switch-table relocations; C2004 6/130; C220C 6/131;
camera_scene_manager546/614. These are context only; no allowance or unverified
claim is requested. Already locked C2430/C26C4/C2944 still score MATCH in this
stand-in-free group and are not new coverage. That supports preserving their
real source/context while honestly leaving the clear helper unclaimed.

```bash
python3 tools/cloud/score.py group cloud/work/ipa-groups/codex_stunt_clear_a6
python3 cloud/work/tools/ipakit/deps.py closure func_800C3578 --mode direct --json
```

All compiler caches/objects remain in private Rocky build/, outside repository artifacts.
