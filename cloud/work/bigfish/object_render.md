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

## Phase 1 attempt (bigfish hail-mary, 2026-09-30) -- verdict: NO-GO for a full match

Files: `object_render/base.c` (hand prefix, ~155 words + tail stubs), `object_render/pre.py` (prefix compare),
`object_render/tdis.py` (disasm of a retail function), `object_render/g878/` (IPA group for func_800878E0),
`object_render/f878e0_ipa_best.c`, `object_render/seed.c` (sed-fixed m2c seed).

### func_800878E0 (74 words): 68/74 words match (6 differ), NOT a strict match
- Semantics: `if ((D_8012E608 & m) != m) { D_8012E608 |= m; if (m&0x4000) {emit 0xFCFFFFFF/0xFFFDF6FB; if (D_8014A248<=0||>=4) {emit 0xE3000A01,0; } D_8014A248=-1;} if (m&1) emit(0xE2001E01,1); if (m&0x10) {emit(0xE2001D00,4); func_80086A50(D_8014A248);} if (m&0x20) func_80086A50(D_8014A248); }`.
  Sibling of func_8008705C (the "clear" version); Gfx pushes as `Gfx *g = D_80149438++;`.
- Standalone (`score.py fn`, -O2 or -O3): 50/74 differ. Structure identical, only registers: target keeps the mask in
  `t0` and avoids a2/a3. **Same IPA finding as func_8008705C**: it needs the callee `func_80086A50` in the group
  (stand-in `stub.c` clobbering a0-a3,t6-t9 but not t0). Using `g878/` (extscore.py, -O3 group, keep func_800878E0) gives 6/74 differ.
- Remaining 6 diffs: block 1 `ori` order (t7 then t6) and, in the m&1 and m&0x10 blocks, the target stores w1 (`sw t6,4(v0)`)
  BEFORE w0 while materialising the w0 constant first (lui/ori t9, li t6, sw t6,4, sw t9,0). Source order `w0=;w1=` gives
  the right constants but stores w0 first; `w1=;w0=` gives li first and t6/t9 swapped. Tried temps, `g[0].`, s32 w1, `g=D; D=g+1`,
  param types: no help. Not spliceable anyway (stand-in callee).

### object_render prefix (first 155 words)
- Decoded head (params: a0 tex u32, a1 fmt u16 (0..5), a2 mode u16 (0..3), a3 pitch u16, stack: h u16 @648 (written back),
  x1 @652, y0 @656, x0 @660, y1 @664, p9 @668, flag s32 @672):
  flag==0: `tex += y0*pitch*{4,2,1,/2}` by mode, `h = y1-y0+1`;
  flag!=0: `flag = (s32)((u64)y0<<32)+(s32)((u64)x1<<48)+(s32)((u64)x0<<16)+y1` (three `__ashldi3` calls, sums low words only, 64-bit
  temps spilled at 80..92(sp));
  then `sp274` from fmt (0/2: D_8012E608&0x8030 ? 1:0; 5: 4; 4/3: (D_8012E608&1)?3:2), `if (fmt==3) func_800878E0(0x20)`,
  `if (sp274 != D_8014A248) func_80086A50(sp274)`, cache test `tex==D_8012E684 && (s64)flag==D_8012E688` (64-bit
  compare; hit jumps to 0x272c = function tail), then `sp272 = func_80087804(x1-x0); func_80087804(y0-y1)`.
- Prefix compare (`pre.py base.c`): best **-O2: aligned-exact 32/155, opcode-shape 110/155, positional 4**; -O3: 27/155, shape 94; -O1: 26, shape 78.
- Why a prefix cannot be strict-matched in isolation: the retail frame is 632 with `s0`=flag, `s1`=pitch chosen by global use counts across all
  ~25 variants; a0/a1/a2 stay in their home slots (`lhu a3,642(sp)` reloads); s0/s1 are later reused as plain temporaries (`lhu s0,626(sp)`
  at 0x39c); spill slots at 0x24..0x5c and pointer spills like `sw a0,600(sp); lw t9,600(sp)` (a named Gfx* local in some blocks).
  Truncating the function changes every one of those decisions, so chunk-by-chunk strict scoring is impossible; only the complete function
  can be scored strictly, and each of ~20 variants (format x size x flag) must be exact simultaneously.
- Dispatch beyond the head is ~20+ variants of a ~100-word block with per-variant differences (`|0x200`, `<<14`, `(w*2+9)>>3&0x1ff`,
  `+0x7FF`, `/8` clamps, reused `s0/s1` temps) and interleaved reads of stack-spilled copies (`36(sp)`, `52(sp)`, `56(sp)`, `60(sp)`, `64(sp)`).

### Verdict
**NO-GO** for a strict match of object_render in this session: repeated shape holds (the emitter macro reproduces the shape), but
register/spill fidelity is whole-function dependent, the prefix measure stalls at 32/155 exact even with the right structure, and the
whole function must be written (2400 more words) before any strict feedback exists. Next steps if resumed: write the entire function
with the macro (all variants) at -O2 (better than -O3 here), then tune locals/named pointers using near.py; and first finish
func_800878E0 (6 words) inside an IPA group containing a real or stand-in func_80086A50.
