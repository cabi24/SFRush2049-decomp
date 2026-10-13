# GameCube debug build as a second reference

## What it is

`rush_d.elf` (repo root) is the debug executable from the fan-made GameCube disc "San Francisco Rush 2049
Legacy Edition" (game ID `2049LE`). It was built with Metrowerks CodeWarrior (`MW EABI PPC C-Compiler`)
from `D:\rush_ngc\Rush_NGC\` (121 `GameSource\src` files and 28 `NGC` platform files). It still has its full
ELF symbol table (12,667 symbols), CodeWarrior DWARF v1 debug info (`.debug`, `.line`) and its relocation
sections. Provenance: Astra's upload, commit f45e3940, notes in [`writeup.md`](../../../writeup.md) at the
repo root (extraction offset, SHA-1, CSV schemas, DWARF pitfalls). The root files `rush_d.elf`,
`rush_d_*.csv` and `writeup.md` are inputs: never move or edit them (the owner decides their future).

It is a **divergent later sibling** of the N64 game, not its source: same-named functions differ a lot in
size, the platform layer is GameCube SDK, and it shares code with the PS2/PC/Dreamcast ports (`blit_pc.c`,
`audiosfxps2.c`, `paksysdc.c`). 562 of the 865 N64 game-code strings also occur in it. Use it **after** the
arcade source (`reference/repos/rushtherock/`), mainly for the N64-only code with no arcade ancestor:
results, high-score and records screens, save/pak, the menu block, countdown, attract.

## Files here (generated; do not edit)

| File | Content |
|---|---|
| `types.h` | All 2,431 struct, 178 union and 130 enum entries, de-duplicated to 336 structs, 13 unions and 120 enums, grouped by the first source file defining them. Every member has `/* offset size */`. DWARF v1 records no typedef names, so anonymous aggregates are `__anon_<debug offset>`, each with a `used by:` line naming the globals and members of that type. |
| `functions.json` | 1,844 functions with debug info: file, address, size, line range, C signature, parameters and locals with type and location (`{"kind": "reg", "reg": "r31"}` / `{"kind": "stack", "base": "r1", "offset": 8}`). |
| `globals.json` | 3,289 fixed-address variables (globals, file statics, function statics) with type, size and section. |
| `callgraph.json` | All 3,181 linked functions (including SDK/MSL/Bink without debug info): callees, callers, function-pointer references. Static functions with duplicate names are keyed `name@file`. |
| `strings_xref.json` | String → functions that reference it directly, and functions that reach it through a pointer table in `.data`/`.rodata` (the table object is named; one level). |
| `correlation.md` / `.json` | N64 game and overlay function → top 3 GC candidates with source file, score and evidence, plus the held-out evaluation. |

Bulk output that is not committed: `reference/gc/disasm/<source_file>/<function>.s` (symbolised disassembly
of every function, ~24 MB) and `reference/gc/features.json`. Both are in `.gitignore`.

## How a matching lane uses it

1. **Arcade source first** where a counterpart exists. The GC build is the fallback for N64-only code.
2. **Correlation.** Find your N64 function in `correlation.md` (sorted by address within `game`, `ovl_a`,
   `ovl_b`). Use **conf** pairs as working hypotheses; treat others as "look in this GC file". Read the
   evidence: a shared specific string (`str "CNTDWN%d"`) or a call-graph anchor is strong; one shared
   float or a size ratio is weak. The N64 symbol names come from speculative arcade attributions in
   `symbol_addrs.us.txt` and are often wrong (N64 `AdjustSpeed` at 0x800A2680 is a controller-pak
   routine; `audio_doppler_full` pairs with GC `ShowCountdown`); trust the evidence, not the label.
3. **Layout.** Look up the GC function in `functions.json` (signature, params, locals) and its struct types
   in `types.h`. Offsets are CodeWarrior/PowerPC; N64 IDO layouts often agree for 4-byte fields but check
   alignment, `bool`/enum sizes (enum sizes are in the `size` comment) and fields the N64 lacks.
4. **Disassembly** (on the Pi, which has capstone):
   `python3 -m tools.gcref.disasm --only _PrintScores` prints one function; the full dump goes to
   `reference/gc/disasm/`. Calls, `@ha`/`@l` pairs and `@sda21` small-data accesses are symbolised from the
   ELF's own relocations (exact, no register tracking); string literals are inlined as comments and
   `lfs`/`lfd` literals shown as numbers.
5. Write the N64 C against the N64 assembly as always; the GC code tells you intent, names and field
   meanings, not instruction order.

## Caveats

- Divergent code: the GC version was extended (e.g. battle mode: `drawBattleStandings`) and re-platformed. A
  confident pair means "same source function, probably edited", not "same code".
- CodeWarrior conventions: small data through `r13` (`.sdata`/`.sbss`, `_SDA_BASE_` = 0x802AC300) and `r2`
  (`.sdata2`/`.sbss2`, `_SDA2_BASE_` = 0x802ADAC0); float literals are compiler symbols `@N` in `.sdata2`;
  int→float conversion uses the 0x4330000000000000 double trick; function-local statics are named
  `_x$localstaticN$fn`; enums are 4 bytes.
- Gekko paired-single instructions (`psq_l`/`psq_st`, `ps_*`) appear in prologues and math code; capstone
  must use `CS_MODE_PS` or it decodes them as VSX garbage.
- Locals of nested blocks are flattened (CodeWarrior emits no lexical blocks); parameters shown in `r0`
  were dead or moved by the optimiser.
- `.rela.debug` is never applied (writeup.md); `.rela.text`/`.rela.data` are, and agree with the linked code.
- Correlation precision is limited (below). Nothing here is ROM coverage or matching evidence.

## Correlation method and precision

Anchors: shared strings (weighted by how many functions use them on each side and by how informative the
string is; pointer-table strings capped per candidate), call-graph anchors propagated from seed pairs
(libc/libm `sprintf strlen strchr memcpy memset memchr sinf cosf modf` and inflate `huft_build
inflate_block/dynamic/fixed/stored`) and from every confident pair found (iterated to a fixpoint), rare
shared 32-bit immediates and float literals, switch-table sizes, N64 neighbours already paired into the
same GC file, and size similarity as a tie-breaker. Confident = score >= 3, 1.5x the runner-up, mutual best.

The same-named N64 game symbols (`AdjustSpeed`, `AdjustSteer`, `no_catchup`, `CheckCPs`, `InitDrones` ...)
are not seeds: some sit at placeholder addresses that are not function starts, and `AdjustSpeed` is a pak
routine. Precision is therefore measured on string-anchored pairs (string evidence alone >= 3 and twice the
runner-up), two-fold: each half is held out with its string channel masked and must be recovered by the
other channels. Current numbers are in the header of `correlation.md`.

## Regenerate

```bash
python3 -m tools.gcref.dwarf1                 # types.h, functions.json, globals.json (stdlib only, <1 s)
nice python3 -m tools.gcref.disasm            # needs capstone (Pi): reference/gc/, callgraph, strings_xref (~5 s)
nice python3 -m tools.gcref.correlate         # needs build/game_code.bin and build/ovl/{a,b}/image.bin (~5 s)
python3 -m pytest tests/cloud/test_gcref.py   # skips without rush_d.elf / capstone / the N64 build
```

`correlate` reads `asm/us/blob`, `asm/us/ovl_{a,b}`, the census and the locks from this checkout and the
build images from this checkout or, in a worktree without `build/`, from the main checkout (`--n64-root`).
