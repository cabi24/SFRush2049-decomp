# Input_ProcessGameplayPad (0x800A04C4) - bigfish scout

## Facts
- 680 words (blob_8009dd18.s), frame 256, saves ra, s0-s8, f20/f22/f24. Not spliced, not in cloud/matches.
- The name is misleading: it is a per-frame display-list/render pass. Body starts with raw RDP/RSP word emission (`0xD9F9FFFF`, `0xDB020018`, `0xDC08060A` ...) into the DL pointer `*(u32**)0x8014xxxx`, then switches on a mode word (`lh 0x80141560 (=1560(a2)`, cases 0/1/2), and calls `func_8008705C` x6, `func_8008A148` x4, `func_800878E0` x4, `object_render`, `func_8008A3E4/38C/644/46C`, `func_80087110`, `func_8009F058` (the big renderer, 1300+ words).
- ABI: a0 read (`sw a0,256(sp)`), no other entry-register reads. Callers: `game_loop`, `audio_update_b`.
- Callees are IPA-suspect: `func_8008705C` is one of the three "tail groups" from Round 2 (matched only with stand-in callers, the dead `move s0,v0` still unexplained), `func_8008A38C/A644` are near-miss/IPA members. If they take non-ABI args, the call setup here must be reproduced with the group, which is unfinished.
- Globals: 33 distinct (D_8011ED08.., D_8013FEF4, D_80140618, D_801613AC..., pad_config, msgq_ptr, D_80150B70...). 51 mul/div counted by grep includes gbi shifts; 6 fp ops. 
- Shape: irregular macro-heavy DL code with three big mode arms. Not repetitive enough for a macro trick, but gSP/gDP macros exist in the project (check include/) if the DL words are recognizable (`0xDB02..` = G_MOVEWORD, `0xDC08..` = G_MOVEMEM, `0xD9F9FFFF` = G_GEOMETRYMODE).
- No first-pass compile: m2c cannot run on the blob (asm/us/blob is `.word` sections, m2c_output/ has nothing for it), and a hand skeleton of a 680-word DL function was not judged worth it for a scout.

## Feasibility: LOW-MEDIUM
Big, many globals, callee ABI uncertainty (three IPA-tail callees), and unknown gbi typing. 
Effort: 2-3 days; needs a decoded skeleton first (disassemble with tdis, map the three mode arms) and closure info for func_8008705C/8008A148/800878E0 first.
