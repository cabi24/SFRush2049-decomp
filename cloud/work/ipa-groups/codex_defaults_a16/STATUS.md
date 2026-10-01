# A16: genuine seven-output initializer

Frozen source SHA256: `ae7416de4d2f22033066caf03c6fd4cd14c27fbab44e4058b1ba099b95ed5182`.

Claim: `func_800B6748`, 16 retail words (64 bytes), strict MATCH, zero differing words, zero extra words, no unresolved or unverified relocations. Canonical `score.py group --claims` exited 0. The authoritative shared lock contained no entry for this function at freeze. Flags: `-g0 -O3 -mips2 -G 0 -non_shared`.

The helper consumes seven actual 32-bit pointers: RGBA bytes; float amount; two signed 16-bit coordinates; three signed bytes. Its actual stores set RGB to 230, alpha to 255, amount to -1.0f, coordinates to zero, and the three byte selectors to -1. All seven formals are consumed. There are no fabricated guards, extra formals, stand-ins, pressure-only locals, or synthetic caller wrappers.

The module contains the three actual direct callers, kept externally visible: `menu_input_process` (320 retail words), `world_trigger_check` (54), and `world_velocity_integrate` (178). Total retail closure: 568 words. The caller inputs and global pointer widths were repaired against actual assembly. The menu caller has a real payload pointer and signed-halfword length; the recording caller has a real byte input. Byte command-stream writes, big-endian coordinate/float serialization, changed-state comparisons, queue waits/releases and capacity gate are preserved. The menu scratch buffer's 256-byte extent is inferred from its retail frame, rather than asserted as proven original source. External callees remain genuine declarations; their hidden historical ABI is not claimed reproduced by the contexts.

Initializer call sites: menu `0x800B6CF8`, trigger `0x800EDD4C`, recording `0x800ED8A0`. Menu passes outputs at `0x80153F60`, `0x80153F80`, `0x80153FD0`, `0x80154180`, `0x80154184`, `0x80154194`, `0x8015419C`; both world callers pass `0x80149B48`, `0x80114748`, `0x80149D92`, `0x80149D9E`, `0x80149DA0`, `0x80149B60`, `0x80149B70`.

The actual recording switch table at `0x80124588` was inspected in the authoritative inflated image: cases 0..6 lead to `0x800ED910`, `0x800ED934`, `0x800ED958`, `0x800ED97C`, `0x800ED9CC`, `0x800EDA18`, `0x800EDA44`. These cases perform selector update; mode-byte2 update; mode-byte update; two signed-halfword updates with -32768 sentinel; float-bit decoding; four channel bytes; and variable payload forwarding respectively. The default consumes only the opcode. No replacement behavior was invented for unresolved m2c switch output.

All callers are explicitly unclaimed NONMATCH. Final caller strict differences/emitted words: menu 320/280; trigger 53/47; recording 165/161. Recording has two informational unverified section-relative jump-table relocations at offsets 0xf0 and 0xf8. No context byte match or clobber-contract certification is asserted, and no `allow_unverified` is requested for the claimed helper.

Reproduction on an IDO builder: `python3 tools/cloud/score.py group cloud/work/ipa-groups/codex_defaults_a16 --claims`. The immediate faithful baseline and the final unsigned float-bit serialization cleanup both matched; no broad mutation sweep was performed. Final sanitized counts are in `scores.json`. Raw canonical output remains private and in ignored `build/codex-A16/score_claims.txt`, because it contains target instruction hex; no ROM/object byte stream is included in this packet.
