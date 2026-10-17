# High-score source-boundary research

Research only. `claims` is empty. No accepted credit, installation, whole-unit or image acceptance is claimed.

The group replaces the old oversized ellipse buffer and duplicate font wrappers with the independently matching native 128-byte ellipse, one genuine `HAL_SetFont` implementation, real semaphore helpers, and the actual font/reset/resource call graph. It retains the genuine high-score root and all three emitted callees. The source keeps N64 menu indices, coordinates and platform-specific branches.

## Native results

| Native routine | Current result |
| --- | --- |
| highscore_animText, 445 words | 18 differing words; frame560 versus584; four strict ownership refusals |
| DrawName, 260 words | Strict equality in the genuine partial caller group; no new credit |
| DrawGhost, 601 words | 10 differing words; frame568 versus576; twelve strict ownership refusals |
| DrawUnlocked, 629 words | Strict equality, including five owned format literals; native frame264; no new credit |
| object_bytes23_sum, 18 words | Inherited four-word difference |
| object_bytes_sum_global, 21 words | Four-word donor-based partial from the font-height lane |
| world_velocity_integrate, 178 words | Inherited158 differences, three extra words, two own-table refusals |

Every other listed native member reproduces strictly. All31 native bodies are explicit in `members`, with no hidden `context` list. The compiler also emits four eight-byte helper/deleted-procedure extents, inventoried in `fidelity.json`; they receive no native claims.

## What the evidence establishes

- The root's18 code differences are exactly16 stack-address/frame fields and the interchange of two constant loads. All other words, including register fields, are checked directly.
- DrawGhost's10 code differences are exactly its two frame fields and eight stack-address fields. The original donor orders its unsigned128-byte and16-byte buffers before signed-short `gFreePages`. That short is unused in the N64 flow. Moving it before the buffers recovers the old code-equal result but lacks native source justification; the verifier rejects that older shaping choice. Removing it from the recorded order leaves every emitted code, data, relocation and symbol section identical; only `.mdebug` metadata changes.
- DrawUnlocked restores the original `int x`, unsigned64-byte buffer, and nested `if (gStuffUnlocked[p])` guard. The int removes48 opcode differences; the nested guard closes the remaining64 register-field differences. Its five `%s %s` format literals are owned, and DrawName owns its original `DEL`, `END`, and `SPC` statics. Both routines strictly reproduce without external literal-address aliases. The N64 eight-entry transmission scan is retained: importing the GC four-entry, `i * 2` loop fails native verification.
- Native ellipse, Center/Right, justification, font, slot and reset helpers are retained. The actual recorded-text replay caller replaces the old credits stand-in at the reset boundary. Its own failure is fully disclosed.
- The five-entry root table, eight-entry Ghost table, both time literals, the score literal and initialized justification data independently match native content. This window-by-window proof does not establish a single layout for the combined section. The normal scorer's layout refusals remain mandatory and unchanged. Window extents are fixed by the actual objects and tables; zero-byte trimming cannot discard the second justification field in the wrong-initializer negative.
- Moving real HAL/text bodies to their original source-unit boundaries leaves the measured caller results unchanged. Placing the semaphore definitions after their declarations/calls closes retained HAL_SetFont scheduling without an added local, wrapper or line directive.

## Provenance and limits

MAT3 `highscorescr.c` provides the original root's sole `int p`, DrawCircuitCode's five locals including unsigned64-byte storage, DrawScores' `i`, `numWid` and unsigned16-byte storage, DrawGhost's six recorded locals, DrawUnlocked's `int x, y, i, p`, unsigned64-byte buffer and `int stuffCount`, and DrawName's unsigned two-byte local followed by `int x, y, i`. The full animText/DrawGhost source was recorded100% in a complete Matching MAT3 unit, source SHA256 `0d41d84b53240ecbd9b4d289b43cfc6b68187bb197904af3111539a289ef1377`; no new MAT3 build is asserted here. Its unreachable status branches and reconstruction-only first-use choices do not prove N64 operations or extra storage.

The original HAL font wrapper has an integer `old` local around genuine semaphore calls. The original text helpers and native128-byte ellipse were checked independently. N64 contracts and native content are verified separately; MAT3 equality is not N64 acceptance.

Inherited resource context still contains compiled-out inline barriers in `slot_state_setup`, `func_80096288` and `object_byte9_set`, plus existing do-block scheduling forms. These are disclosed context, not newly authenticated source. The text replay retains its unresolved native IPA dependency. No unrelated context is silently promoted.

## Replay

From a repository checkout with the existing IDO and authenticated native artifacts available:

    python path/to/this/verify.py
    python -O path/to/this/verify.py
    python path/to/this/test_guards.py

The two verifier outputs must agree byte-for-byte. It pins every source and target, rebuilds the entire group, checks all31 receipts and the whole object, validates the narrow residual/content proofs, rejects eleven current-source mutations, and proves the unused-local removal control preserves every emitted code/data/relocation/symbol section. Three standard-library guard tests cover scope, source and target pinning. Stock, staged-stock and traced-uopt whole-object fingerprints agree; tools, objects, traces, native bytes and assembly dumps are not part of this packet.

The causal negatives restore the old short-x type and early-continue form separately, and reject the GC-only transmission scan. Owned-format and owned-static mutations must fail the normal strict scorer. Wrong-literal receipt descriptions omit compared native byte values.
