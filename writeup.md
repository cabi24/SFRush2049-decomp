# Rush 2049 (GameCube) symbol recovery: handoff notes

Short version: the debug build `rush_d.elf` on the fan-made GameCube disc `Rush2049.iso` still has its full ELF symbol table and its CodeWarrior DWARF v1 debug info. Both have been extracted and decoded into the CSVs below. The ELF's code and data are byte-identical to the disc's `rush_d.dol`, so every address applies to the DOL unchanged.

All paths are relative to `C:\rush2049-mat3\`.

## Files

| File | What it is |
|---|---|
| [Rush2049.iso](Rush2049.iso) | GameCube disc image. Game ID `2049LE`, title "San Francisco Rush 2049 Legacy Edition + MAT3". Fan-made; Rush 2049 had no official GameCube release. 7,770 files. |
| [rush_d.elf](rush_d.elf) | Extracted from the ISO (disc offset `0x55056800`, 5,611,448 bytes, SHA-1 `81181994c6231ce20886f571525d740e10f2796f`). PowerPC, ELF32 big-endian, linked executable, entry `0x80003154`. |
| [rush_d_symbols.csv](rush_d_symbols.csv) | **Main table.** All 12,667 ELF symbols, each with its source file where known. |
| [rush_d_functions.csv](rush_d_functions.csv) | 2,196 functions from the debug info, with source line ranges. 352 of them were stripped by the linker. |
| [rush_d_variables.csv](rush_d_variables.csv) | 3,917 variables with fixed addresses (global or file-static) from the debug info. 628 of them were stripped. |
| [rush_d_compile_units.csv](rush_d_compile_units.csv) | The 149 source files that have debug info, with code address ranges. |
| [DATA_GDF_contents.csv](DATA_GDF_contents.csv) | File list of the **PC** release's `DATA.GDF` archive (8,207 entries). Unrelated to the ELF; kept for reference. |
| [rush.exe](rush.exe), [DATA.GDF](DATA.GDF) | The PC (Midway Arcade Treasures 3) build. No ELF anywhere, and no embedded symbols (details below). |
| [tools/](tools/) | The Python scripts that produced all of the above (standard library only). |

## Build facts

- **Compiler:** every compile unit lists its producer as `MW EABI PPC C-Compiler`, i.e. Metrowerks CodeWarrior for GameCube. Expect CodeWarrior conventions: function-local statics are named like `_half$localstatic3$sqrtf`, small data is addressed through `r13` (`.sdata`/`.sbss`) and `r2` (`.sdata2`/`.sbss2`), and the code uses Nintendo's GameCube SDK.
- **Original source tree:** `D:\rush_ngc\Rush_NGC\`
  - `GameSource\src\`: 121 game files (`cars.c`, `drones.c`, `hud.c`, `weapons.c`, `carscr.c` …)
  - `NGC\`: 23 GameCube platform files (`ngcGraph.c`, `ngcPad.c`, `ngcMemcard.c` …), plus `NGC\FONT` (3), `NGC\BINK` (1) and `NGC\MATMEM` (1)
- The code shares a base with the other ports: file names such as `audiosfxps2.c`, `blit_pc.c` and `paksysdc.c` come from the PS2, PC and Dreamcast versions.
- **Memory map** (from the ELF section headers):

| Section | Address | Size |
|---|---|---|
| .init | 0x80003100 | 0x24D0 |
| .text | 0x800055E0 | 0x1030A0 |
| .ctors / .dtors | 0x80108680 / 0x801086A0 | 0x4 / 0x8 |
| .rodata | 0x801086C0 | 0x10B80 |
| .data | 0x80119240 | 0x3F418 |
| .bss | 0x80158680 | 0x14BC64 |
| .sdata / .sbss | 0x802A4300 / 0x802A4860 | 0x544 / 0x124C |
| .sdata2 / .sbss2 | 0x802A5AC0 / 0x802A9960 | 0x3E98 / 0x4 |

- **Match with the DOL:** all 8 segments of the disc's `rush_d.dol` (`.init`, `.text`, `.ctors`, `.dtors`, `.rodata`, `.data`, `.sdata`, `.sdata2`) are identical to the ELF at the same addresses. Checked with `tools/dolcheck.py`. The disc's boot program (`main.dol`) is a different file, the movie/menu launcher.

## CSV schemas and how to read them

### rush_d_symbols.csv

Columns: `index, name, address, size, type, bind, section, source_file, source_path, line, source_via`

- `type` is FUNC (3,181), OBJECT (7,431), SECTION (1,535, unnamed; safe to ignore), FILE (337) or NOTYPE (183).
- `line` is the function's first source line, from the debug line table. Filled in for the 1,844 functions that have debug info.
- `source_via` says how `source_file` was determined. **Check it before trusting the source file.**
  - `dwarf` (1,844 FUNC, 2,040 OBJECT): exact address match to a debug-info entry. `source_path` is the full original path. Most reliable.
  - `symtab_file` (363 FUNC, 5,292 OBJECT): a LOCAL symbol listed after a FILE symbol in the ELF symbol table. Only a bare file name is known. Reliable.
  - `address_neighbors` (164 FUNC, 1 OBJECT): **inferred.** The nearest attributed symbols on both sides, in the same section, come from the same file. The linker lays out each object file contiguously, so this is very likely right, but it is not proven.
  - empty (810 FUNC, 98 OBJECT): unknown. These are almost all global symbols from SDK and support libraries built without debug info: `OS*`, `GX*`, `DVD*`, `AI*`/`AX*`/`DSP*`, `CARD*`, `EXI*`, `VI*`, `PAD*`, `TRK*` (the debugger stub), the MSL C runtime (`__flush_all`, `__kernel_sin` …) and Bink (`RAD*`, `YUV_*`). Their name prefixes identify the library.
- Overall, 2,371 of 3,181 functions and 7,333 of 7,431 objects have a source file.

### rush_d_functions.csv

Columns: `name, address, end, size, scope, stripped, source_file, line, end_line, source_path, debug_offset`

- `scope`: `global` or `static`. Stripped entries show `unknown` because CodeWarrior erased their tag.
- `stripped=1`: the function was compiled but removed by the linker, so it has no address and doesn't exist in the binary. It still tells you the function existed in that source file.
- `line`/`end_line`: the smallest and largest line numbers in the function's address range.
- `debug_offset`: the entry's byte offset inside `.debug`, for anyone who wants to decode it further (parameters, local variables, types).

### rush_d_variables.csv

Columns: `name, address, scope, stripped, source_file, source_path, debug_offset`

Only variables with a fixed address (globals and file or function statics). Stack and register locals are left out.

### rush_d_compile_units.csv

Columns: `cu_index, source_file, source_path, low_pc, high_pc, code_bytes, functions, functions_stripped, variables, language, producer, debug_offset`

`language` is empty because CodeWarrior doesn't record it. Six files have 0 code bytes (`cars.c`, `os.c`, `sched.c`, `timingbar.c`, `waveinfo.c`, `defaultFont.cpp`); they hold data only, or all of their code was stripped. The largest files by code are `carscr.c`, `visuals.c`, `playerscr.c`, `hud.c`, `recordscr.c` and `trackscr.c`.

## Format notes and pitfalls (useful if you re-parse anything)

- **CodeWarrior's DWARF v1 differs from standard DWARF.** When the linker strips a function or variable, it sets the entry's tag to 0 and extends the entry's length to cover its whole subtree; after the parent's attributes the bytes are zero. Treat an attribute ID of `0x0000` as the end of that entry. The stripped entry's name and other attributes are still readable.
- **Don't apply `.rela.debug`.** The `.debug` and `.line` sections are already linked. The leftover relocations disagree with the stored values in 52 places, all lexical-block `high_pc` values, and the stored values are the linker's final ones.
- **Line table** (`.line`, one block per compile unit, found through `AT_stmt_list`): `u32 length, u32 base_address`, then 10-byte rows of `u32 line, u16 column, u32 address_delta`. All 149 compile units have one.
- **Not yet decoded:** type information. The debug info holds 2,431 structs, 26,640 struct members, 178 unions, 130 enums, 2,757 function parameters and 5,591 local variables. That is enough to rebuild struct layouts and function signatures, and it's the obvious next step. Ghidra's DWARF importer may not handle DWARF v1 cleanly; a custom pass over the `debug_offset` entries listed in the CSVs is the safe route.

## Other ELF files on the same disc (not extracted)

All are PowerPC ELF executables in the disc's root directory with symbol tables intact. `_d` = debug build with `.debug`/`.line`.

| File | Game | Symbols | Debug info |
|---|---|---|---|
| offroad_d.elf | Off-Road Thunder | 14,048 | 4.3 MB (largest) |
| hydro_d.elf | Hydro Thunder | 14,574 | 335 KB |
| rock.elf | Rush the Rock | 8,881 | 1.4 MB |
| ffe.elf / movie.elf | front-end / movie player | ~3,700 each | ~140 KB each |
| racedriv, stunrun, badlands, Offroad, offroad_pack .elf | Atari arcade games and Off-Road release builds | 5–6.5k | symbols only |

`tools/gcm.py` lists the disc's files with their offsets, and `tools/extract_syms.py <iso> <offset_hex> <size> <out.elf> <out.csv>` extracts any of them.

## PC build (for contrast)

- `rush.exe`: 32-bit Windows (PE) program, a **Debug** build. Its symbol table is empty, but it points to `C:\MAT3PC\Rush_PC\Debug\rush.pdb` (not available) and contains 84 source paths, which show it shares code with the PS2 and Xbox versions.
- `DATA.GDF`: a Midway archive. Raw-deflate data in 64 KB blocks, then a block table, then 8,207 records of 200 bytes (name[128], hash, size, extension, block start, offset in block, block end, timestamp), then a trailer giving the table sizes. Contents: textures, Xbox-format meshes, music and other audio, ROM data. No executables. See `tools/gdf.py`.
- Separate find in the installed PC version: the Hydro Thunder archive (`...\Midway Arcade Treasures 3\HydroData\DATA.GDF`) lists `gamedata\bank_waves\boat_07_thresher\windowsupdate.pif` and `...\boat_06_razorback\microsoft office.exe`. These look like 2004-era worm copies packed in by accident. **Do not extract and run them.**

## Regenerating

```
python tools/extract_syms.py Rush2049.iso 0x55056800 5611448 rush_d.elf rush_d_symbols.csv
python tools/dwarf1.py rush_d.elf .
python tools/dolcheck.py Rush2049.iso rush_d.elf
```

The second command overwrites `rush_d_symbols.csv` with the full version, including the source columns, and writes the other three `rush_d_*.csv` files.
