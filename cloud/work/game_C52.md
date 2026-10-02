# C52: complete native car/player initialization, frozen nonmatch

Reserved canonical `world_physics_tick` at `0x800EC2F8`, 1,564 bytes / 391 words. No match or coverage claim. Complete actual native rewrite describes external 2,056-byte Car, 952-byte Player, 76-byte Reference, 8-byte Config and 16-byte Bounds records. These are actual retail strides and field offsets; opaque intervals describe external storage, with no invented local array capacity or frame padding.

Arcade provenance: `reference/repos/rushtherock/game/mdrive.c:init_cars` has the same real clear/init/active-list/appearance/checkpoint/body-bound-radius sequence. The N64 implementation additionally preserves Car1816 through its clear, uses the six-car tail and actual local configuration arrays. This is an algorithm/naming lead, not an assertion that arcade source compiles to the N64 bytes.

Callee audit: func_800B27E4 consumes one Player pointer and clears21 actual24-byte entries. func_800EC270 consumes both Car* a0 and Player* a1; it clears Player788/792/824 and uses Car1996. Its second input was verified before source compilation rather than inferred from only the final a0 setup. The radius uses the real SDK-style `#pragma intrinsic (sqrtf)` after the initial compiler control showed an external call. The final candidates retain every original unsigned-byte/signed-half access, null guard and final global store. Five appearance tables have a real13-byte row stride proved by all five address calculations. Consumed body captures and independent placement/slot/checkpoint carriers are honest source controls.

All nine compilations exit0 with empty stderr. Target object SHA-256 `1a2394244559f731416392aa95c50902e9d798f7701a5113d87a3d75f0120489`. Exact literal flags are in every source and result; ordinary recipe `-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul`, with one O3 table control.

| Control | Strict weighted | Canonical differing/391 | Extra words |
| --- | ---: | ---: | ---: |
| Initial native source (missing intrinsic declaration, diagnostic only) | 18846 | 379 | 0 |
| Real sqrtf intrinsic declaration | 18611 | 379 | 0 |
| Actual captured body value | 13845 | 384 | 0 |
| True13-byte multidimensional appearance rows | 10010 | 380 | 0 |
| Same true rows, ordinary O3 | 10010 | 380 | 0 |
| Unsigned Car body byte | 10735 | 384 | 0 |
| Distinct consumed placement/slot/checkpoint carriers | 10575 | 384 | 0 |
| Actual row-pointer assignment order | 10590 | 384 | 0 |
| Actual active-count pointer used for both accesses | 11986 | 369 | 0 |

Every canonical comparison reports no unresolved/unverified references or errors. Native frame72 matches the retail frame72 without dummy locals. Workbench table diagnosis remains mixed structural/register/constant with no known lever; candidate384 versus aligned target392 words. The target section includes one alignment word beyond the canonical391-word function; no words or addresses were masked. Source globals still differ in load/store scheduling and captured table/body webs, so this packet is bounded and frozen rather than forcing artificial pressure or applying physical line-reflow.

Raw retail assembly and objects remain only in ignored `build/C52` and private Rocky scratch; the packet contains source, probes, hashes, sanitized diagnoses and count evidence. No target/scorer/layout/lock/shared accepted source was modified. The corrected complete source `world_physics_tick.distinct.c` has SHA-256 `2755bf4772d9506ce40ffbe67df48121d5e1764b8054f464a87abdcf96085bb6`.
