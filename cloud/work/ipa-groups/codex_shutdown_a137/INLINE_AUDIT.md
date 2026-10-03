# A137 original native inline-operation audit

This audit records original protected instruction semantics only. It is not a score mask, raw opcode stream or a claim of original source syntax. No inline annotation was used in the frozen packets.

The original func_800C8918 contains no call to audio_effect_process@800960D4, object_type1_create@800C87F4 or object_type7_create@800C878C. Instead it contains the full real operations of those wrappers:

| Original caller range | Genuine complete operations |
|---|---|
| C8944..C8974 | Capture actual D_8011025C value, receive D_80152770 token(NULL,1), release captured allocation through audio_reverb_update(address,0), jam token(NULL,0). Global is cleared afterward. |
| C8990..C89C0 | Same full release operations for actual D_80110260; clear this global afterward. |
| C8AC0..C8AF4 | Same full release operations for actual D_80110244. |
| C8B00..C8B34 | Same full release operations for actual D_80110248. |
| C8B40..C8B74 | Same full release operations for actual D_80110270. |
| C89CC..C8A18 | Receive D_80142728 token(NULL,1), actual func_80091B00(), retain message, write byte2 type1, jam token(NULL,0), then post retained message to D_801427A8(NULL blocking flag0). |
| C8A1C..C8A68 | Same full message operations with actual byte2 type7. |

Every release range contains exactly the wrapper's three real call targets in the same order: osRecvMesg, audio_reverb_update, osJamMesg. Queue addresses and actual literal arguments agree with the accepted A25 source. The original release's address arrives in A1 and tag0 in A2 because true audio_reverb_update(address,tag) has its proven internal IPA convention; no unused formal is introduced. Each message range contains its helper's four real call targets in the same order: osRecvMesg, func_80091B00, osJamMesg, osJamMesg, with full queue/message/type stores and actual arguments matching its26-word body.

Prologue/epilogue operations of each standalone helper are absent inside the caller, as expected when operations are expanded into a caller. Original copied argument/message local homes differ by enclosing placement, so frame-size sums are explicitly not the basis of this audit. The preserved internal-entry control already naturally expands both one-call message helpers, while retaining five-call audio_effect_process out of line. A bounded explicit source inline-contract control would therefore use the unchanged complete true audio_effect_process body, not a fabricated wrapper or frame reservation. Any resulting caller/remaining context still requires full strict scoring and original unmasked proof before being called a match.
