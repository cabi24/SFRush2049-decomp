# C54: complete native collision-effect update, frozen nonmatch

Canonical `entity_collision_detect` at0x80090B68 is820 bytes/205 words, with actual Node* and signed-half update inputs. Target was absent from archived native report text/artifact filenames and current accepted source/lock; reservation map checked before selecting. No match or coverage claim.

Source describes actual external Car2056 (enabled signed byte1600), Player952(position8..16), Effect60(handle word0/overlapping signed-half index2, real scale fields4/20/36, vector40..48, alpha52,phase53,delay56) and Resource68(color60) records. Its only local storage is a proven four-byte packedRGBA value: retail writes four individual byte channels28..31 and reads their packed word. Unknown field intervals are external record views, never local buffers.

Actual `entity_spawn_callback(s16,s32,s32)` consumes three standard ABI inputs and preserves all used callee-saved registers. Accepted `entity_transform_apply(Node*,int)` is untouched. Native source retains both actual destroy calls, original index reloads after potentially aliased stores/calls, common early release, phase threshold16, fade start7 and unsigned alpha subtract24 when>=25. The overlapping Effect id/index is represented honestly by a union rather than converting the original signed read to an unsigned one.

Eight compilations all exit0, empty stderr, zero nonzero extras, zero unresolved/unverified/error references. Target object SHA-256 `0c4021e95b3412a3461448699d37ac7316826ed600e99563363f6735fcb52636` (see result files for authoritative full hash). Literal flags are recorded per source/result. Ordinary O2: `-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul`; ordinary O3 differs only that optimization setting.

| Actual source control | Strict weighted | Canonical differing/205 |
| --- | ---: | ---: |
| Initial complete source, release at tail | 4230 | 197 |
| Actual common release placed before main update | 3562 | 199 |
| Capture actual loaded phase in consumed scalar | 3572 | 198 |
| Four-channel struct with original packed word read | 3572 | 198 |
| Ordinary O3 with early release/phase capture | 1790 | 168 |
| Actual elapsed scalar expressed through array view | 1790 | 168 |
| Real packedu32 local with byte channel access | 2028 | 166 |
| Capture actual1.5f growth once | 1790 | 168 |

O3 naturally recovers target's no-saved-s0 shape; source size204 versus original205 words, but honest frame40 versus original48 remains. Workbench reports a mixed constant/structural/register residual (no lane rotation) and suggests dropping a local although its own frame delta says the candidate is already8 bytes smaller. That recommendation does not explain the frame. No invented padding, extra RGBA words, pressure operations, unused formal or line-reflow was introduced.

Best weighted source `entity_collision_detect.o3.c` SHA-256 `364949c39366f410b7b0f11d73c8f79764eb9131d75c999b57e55b89d40bea76`; counts/raw-best variant and all hashes are frozen in baseline/controls results. Objects/retail assembly remain only in ignored build/private Rocky scratch. No protected target/scorer/layout/lock or accepted neighbor changed.
