# Codex delegation prompts — feature 005 research inputs

Run from repo root (`~/projects/rush2049-decomp`) on the Pi. Each prompt is
self-contained (Codex starts cold). Outputs land in
`specs/005-game-context-bootstrap/research/` where `/speckit.plan` will
consume them. Suggested invocation:

    codex exec --dangerously-bypass-approvals-and-sandbox "<prompt>"

## Prompt 1 — Cluster data-reference survey (feeds the struct definitions)

```
In ~/projects/rush2049-decomp: build/game_code.bin is N64 MIPS code loaded at
RAM 0x80086A50 (file offset 0 = 0x80086A50, big-endian). Disassemble these 10
functions (start addresses; each ends at the next function's prologue or jr
ra+delay slot): game_loop 0x800FD464, game_mode_handler 0x800C9AE0,
attract_or_transition 0x800EDDC0, process_inputs 0x800C997C, sound_control
0x800B37E8, playgame_state_change 0x800CA3B4, RaceStateMachine_Update
0x800DB81C, countdown 0x800FBF88, countdown_handler 0x800FBC30,
Input_ProcessGameplayPad 0x800A04C4. Use mips-linux-gnu-objdump on carved
.bin slices if available, else tools/simple_mips_disasm.py, else
build/game_code_disasm.txt (already-disassembled full dump; grep by address).
For EACH function produce: (a) every global address referenced via
lui/addiu|lw|sw %hi/%lo pairs, with access width (byte/half/word/float) and
read/write; (b) every base+offset access pattern that implies a struct
(same base register, multiple distinct offsets) — list base source (which
global or argument) and the offset set; (c) all call targets (jal) with
names where symbol_addrs.us.txt or the table in CLAUDE.md knows them.
Cross-check known globals: 0x801146EC is believed to be gstate,
0x801461D0 a main game struct, 0x80152818 a player/car array, 0x80142AFC a
frame counter — confirm or refute from the access patterns. Write the
result as a markdown table per function to
specs/005-game-context-bootstrap/research/cluster-data-refs.md. Do not
modify any other file.
```

## Prompt 2 — Arcade struct harvest (feeds provenance-stamped definitions)

```
In ~/projects/rush2049-decomp: reference/repos/rushtherock/ is the arcade
(Rush The Rock) source sharing ancestry with our N64 target. Harvest the
type definitions a minimal game-loop port needs. Find and extract verbatim,
with file:line provenance: (1) the GState enum / gstate variable declaration
(game/game.h or similar); (2) the per-player/car state struct(s) used by
game/game.c's game(), playgame(), preplay() — likely CAR_DATA / player
structs from game/cars.h, game/carstuff.h or similar; (3) the input/controls
record read by the game loop (switch/pot inputs); (4) the countdown/race
timer state; (5) any small structs referenced by game/game.c's state
machine cases (attract, TrackSel, CarSel, CountDown, hiscore). For each:
quote the definition, note the header it lives in, list which game.c
functions use it, and flag fields that are arcade-hardware-specific (3dfx,
mathbox, coin mech) and would not exist on N64. Output to
specs/005-game-context-bootstrap/research/arcade-structs.md. Read-only task:
create only that one file.
```

## Prompt 3 — m2c dry-run on a sample of extracted targets (pre-prices the histogram)

```
In ~/projects/rush2049-decomp: we want to know whether tools/mips_to_c/m2c.py
can consume disassembly derived from build/game_code.bin (N64 MIPS, loaded
at 0x80086A50, big-endian, file offset 0 = that address). Pick 25 functions
spanning sizes from tools' DB: run
  python3 -c "import sqlite3,os;db=sqlite3.connect(os.path.expanduser('~/.conveyor/conveyor.db'));[print(r) for r in db.execute(\"SELECT target_id,address,insn_count FROM n64_target WHERE population='extracted' ORDER BY insn_count LIMIT 400\").fetchall()[::16]]"
For each: carve address..address+insn_count*4 from game_code.bin, produce
GAS-style disassembly in the format m2c accepts (look at asm/us/*.s for the
expected shape: 'glabel name' header then one instruction per line; m2c's
parser is in tools/mips_to_c/, check its accepted directives), then run
tools/m2c.py or 'python3 tools/mips_to_c/m2c.py --target mips-ido-c' with
the context header include/m2c_types.h if that helps. Record per function:
disassembly-derivation OK/fail, m2c OK/crash (with the assertion text), and
if m2c emitted C, the undefined identifiers that would block compilation
(just eyeball the output for D_/func_ symbols and unknown types — do NOT
try to compile with IDO). Summarize failure modes and the disasm format
that worked into
specs/005-game-context-bootstrap/research/m2c-dryrun.md, plus the exact
commands used so they are reproducible. Create only that file; do not edit
m2c or pipeline code.
```

## What NOT to delegate

Implementation of the pipeline changes (autodecomp population gate, blob
asm derivation, histogram hardening) stays with the Opus implement pass per
the established Fable-specs/Opus-implements workflow — these prompts only
produce research inputs for `/speckit.plan`.
