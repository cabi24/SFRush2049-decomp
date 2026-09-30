# object_render (0x80087A08)

| item | value |
|---|---|
| words | **2512** (asm/us/blob/blob_80086a50.s, `.text.object_render`) |
| spliced? | No. Only extern prototypes in `src/blob/groups/*/group.c`; no definition anywhere in src/. |
| ABI or IPA | **ABI.** Frame 632, saves ra/s0/s1, reads only a0-a3 and stack args (11 args; stack args at 16..40(sp) from callers). Callers (in `Input_ProcessGameplayPad` at 0x800A0DA8/0x800A0E88 and `audio_doppler_calc` region at 0x800B68F0..) set a0/a2/a3 and stack only, no t-reg setup. |
| callees | 4 distinct, 9 jal: `func_80087804` x4 (55w, **already spliced** src/blob), `func_800878E0` x1 (74w, unmatched, ABI: reads a0), `func_80086A50` x1 (blob entry), `__ashldi3` x3 (libgcc 64-bit shift, static code) |
| globals | ~32 distinct `lui` targets; central one is the display-list cursor `0x80149438` (re-loaded before every command: `lw a0,0(a1); addiu t,a0,8; sw t,0(a1)`), cache words `0x8012E680..E68C` (last texture/mode, incl. a **64-bit** compare, hence `__ashldi3`), `0x8014A248`, `0x8012E608` |
| shape | Textured-rectangle/sprite drawer: picks a texture size mode (`a3` 0..3), a format (`a2` 0..2), then emits RDP words (SETTIMG 0xFD.., SETTILE 0xF5.., LOADSYNC 0xE6, LOADTILE 0xF4/F2, SETTILESIZE 0xF2, TILESYNC 0xE7, 0xE3001001 mode word). **88% of words are in repeated 12-word shape windows**; 6% branches; 0 FP. The same ~100-word command sequence appears in 20+ variants (format x size x "s0 != 0" flag). Not libultra macros (no gbi.h macro shape); it is hand-rolled `PUSH(w0,w1)` through a global pointer. |

## First pass
- m2c seed (`seeds/object_render.c`): compiles after one sed (`*(void *)0xADDR` -> `*(s32 *)`), **2468 words vs 2512 (-1.8%)** at -O2. Frame and structure are m2c-shaped so alignment is poor: opcode-shape LCS 31.6%, opcode+reg 4.1% (-O2), **-O3: opcode+reg 9.5%, exact 7.1%** (vs 2.9% at -O2). -O3 is the better starting flag; -O1 is 37% bigger (3432w), so it is not -O1.
- Hand chunk (`seeds/object_render_chunk.c`): one 97-word variant (`arg2==2`, `s0!=0`, load-tile + set-tile-size) written with a `PUSH` macro, in natural style. Best window in the target: **opcode-shape 75/97 (77%)**, regs off (out-of-context, expected). The target's own sequence shows `sw a0,484(sp); lw t7,484(sp)` (a Gfx cursor spilled to a stack slot), which means the original had a named local pointer `Gfx *g = gDlp;` in some blocks.

## Feasibility: MEDIUM-LOW (medium if it is done as a macro tower)
For: closed unit (2 tiny callees, one already done), no IPA, 88% repetition, the 64-bit cache compare and the arg types are visible in the asm. One correct PUSH macro + one variant block generates most of the function.
Against: 2500 words of uopt allocation in one frame (632 bytes, ~25 spill slots) means any wrong branch polarity or a name-vs-temp choice in the first block shifts everything after it; the variants differ in small ways (`| 0x200`, `<< 0xE`, `/ 8` clamps to 1) that are easy to get wrong 20 times. The `0x7FF` clamp and the `(w*2)/8`, `(x+0x7FF)/y` divide chain in the `s0==0` variant need exact expression trees.

## Recommended approach
1. Score at -O3 alone (`--flags "-g0 -O3 -mips2 -G 0 -non_shared"`), also try -O2; -O3 came closer on registers.
2. Infer the real prototype first: `(void *tex, u8/u16 a1?, u8 fmt, u16 size_mode, s64?/s32 flag, ...)`. Note `lw s0,672(sp)` (arg5) is tested `bnez` and later `>>31` (64-bit `(s64)flag` used in the cache compare). Check the caller words at 16..40(sp) to fix arg widths.
3. Do the top 150 words (mode dispatch, 64-bit cache compare, `sp274` = size mode) by hand against the disasm; use `align.py` on prefixes (truncate the C after each block, compare the first N words).
4. Then one variant block at a time, macroed, comparing windows.
5. Finish `func_800878E0` (74w, tiny) first; it also has 7 other callers.

## Effort
3-6 focused days for a full match; 1 day to get the first 25% matching (which is a useful go/no-go test). Payoff 2512 words (~1.6% of game code words, the single largest ABI function).
