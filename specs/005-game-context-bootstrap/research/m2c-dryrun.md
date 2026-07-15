# `m2c` dry run on extracted targets

## Result

All 25 requested byte ranges could be carved and disassembled from
`build/game_code.bin`. With normalized GAS syntax and **no context**, 14 of 25
produced C and 11 stopped with a controlled decompilation-failure diagnostic.
There were no Python tracebacks, assertion failures, or timeouts.

The dominant failures are not instruction-decoder failures: the database's
sampled `insn_count` span ends before a referenced branch label (8 cases) or
ends on a branch without its delay slot (2 cases). One additional case reports
an absent label in flow-graph parsing. Thus the histogram cannot safely treat
the stored count as a complete function extent without validating branch
targets and delay slots.

## Per-function results

`Disasm` means exactly `insn_count` big-endian instructions were recovered.
Identifiers in the final column are symbols emitted into C for which this run
provided no declaration/definition.

| Function | Address | Instructions | Disasm | `m2c` | Result / unresolved identifiers |
|---|---:|---:|---|---|---|
| `sound_stop` | `0x800B358C` | 1 | OK | fail | Last instruction is missing a delay slot (`beqz` to `0x800B3624`). |
| `countdown` | `0x800FBF88` | 2 | OK | OK | C emitted; no `D_`/`func_` identifiers seen. |
| `track_segment_load` | `0x800A1648` | 6 | OK | fail | Missing branch target `0x800A1688`. |
| `props_render` | `0x800F6AB8` | 8 | OK | OK | C emitted; no `D_`/`func_` identifiers seen. |
| `game_mode_post` | `0x800FBC38` | 10 | OK | fail | Missing branch target `0x800FBC88`. |
| `resource_slots_clear_multiple` | `0x800C937C` | 12 | OK | OK | `func_800c9334` |
| `Input_InitPadHandlers` | `0x80094E8C` | 15 | OK | OK | No `D_`/`func_` identifiers seen. |
| `hiscore_check_handler` | `0x800DBFC0` | 16 | OK | fail | Missing branch target `0x800DC054`. |
| `music_control` | `0x800B1B48` | 18 | OK | OK | No `D_`/`func_` identifiers seen. |
| `menu_saveload` | `0x800D5000` | 20 | OK | fail | Missing branch target `0x800D4F44` (before the sampled start). |
| `menu_dialog_close` | `0x800CD6E0` | 22 | OK | OK | `func_800a2504`, `func_800b466c` |
| `slot_state_lookup` | `0x800A2504` | 24 | OK | fail | Missing branch target `0x800A25B0`. |
| `fp_call_wrapper` | `0x800AC6F4` | 26 | OK | OK | `func_80096ca8` |
| `audio_effect_process` | `0x800960D4` | 30 | OK | OK | `func_80007270`, `func_800075e0`, `func_80095fd8` |
| `pool_linked_list_init` | `0x800B04D0` | 32 | OK | OK | No `D_`/`func_` identifiers seen. |
| `render_display_update` | `0x800FAD58` | 34 | OK | OK | `func_800e95dc` |
| `random_float` | `0x800FD9F8` | 38 | OK | fail | Last instruction is missing a delay slot (`beqzl` to `0x800FDAB4`). |
| `sync_maxpath_to_checkpoint` | `0x800A133C` | 43 | OK | OK | `func_800a11e4` |
| `state_camera_handler` | `0x800FC038` | 45 | OK | fail | Flow parser says label `0x800FC994` is absent. |
| `camera_smooth_lerp` | `0x800AF51C` | 49 | OK | OK | `func_800a7d6c` |
| `render_flag_check` | `0x8008E0C8` | 53 | OK | OK | No `D_`/`func_` identifiers seen. |
| `high_scores_display` | `0x800D5C90` | 55 | OK | fail | Missing branch target `0x800D5D74`. |
| `track_spline_interp` | `0x800A3654` | 58 | OK | OK | `func_80007270`, `func_800075e0`, `func_80091fbc`, `func_8009211c`, `func_80095fd8` |
| `hud_speed_display` | `0x800C84FC` | 61 | OK | fail | Missing branch target `0x800C8710`. |
| `sfx_stop` | `0x800B2658` | 64 | OK | fail | Flow parser says label `0x800B27D8` is absent. |

The emitted C uses inferred primitive/pointer types and generated function
symbols. No `D_XXXXXXXX` data symbols appeared in this sample because the raw
objdump input has numeric `lui`/offset operands rather than project relocation
names; global accesses consequently remain pointer casts/numeric addresses.
The `func_` list above is the immediate compile blocker visible by inspection.

## Disassembly format that worked

`m2c` accepted a minimal file containing:

```asm
glabel function_name
.L80000000:
    addiu  $sp,$sp,-24
.L80000004:
    sw     $ra,20($sp)
```

GNU objdump's instruction spelling is usable, but its presentation needs three
normalizations:

1. Strip the printed address and opcode word.
2. Prefix GPR names with `$` (GNU prints `sp`, while `m2c` expects `$sp`).
3. Emit `.L<address>:` before every instruction and rewrite branch operands to
   those labels. Rewrite numeric `j`/`jal` targets to symbols such as
   `func_80001234`.

Floating-point branch mnemonics (`bc1f`, `bc1fl`, `bc1t`, `bc1tl`) need the
same branch-label rewrite. Once included, they parse normally.

## Context-header findings

Passing `include/m2c_types.h` directly fails before assembly processing:

```text
Syntax error when parsing C context.
Directives not supported yet at line 12, column 1
#ifndef M2C_TYPES_H
```

Preprocessing it with `cpp -P -Iinclude` removes directives but still fails:

```text
Syntax error when parsing C context.
before: OSMesgQueue at line 23, column 5
OSMesgQueue *msgq;
```

Consequently the table reports the reproducible no-context run. The repository
wrapper `tools/m2c.py` preprocesses `include/types.h`, not
`include/m2c_types.h`, and suppresses the detailed underlying failure by
printing only `Error running mips_to_c:` when the child returns nonzero. For a
pipeline, a context header must first be self-contained from `m2c`'s parser's
point of view.

## Reproduction commands

Selection query (exactly the prompt's sampling rule):

```sh
python3 -c "import sqlite3,os;db=sqlite3.connect(os.path.expanduser('~/.conveyor/conveyor.db'));[print(r) for r in db.execute(\"SELECT target_id,address,insn_count FROM n64_target WHERE population='extracted' ORDER BY insn_count LIMIT 400\").fetchall()[::16]]"
```

Carve/disassemble one target, where `START`, `COUNT`, and `NAME` come from the
query:

```sh
END=$((START + COUNT * 4))
mips-linux-gnu-objdump -D -b binary -m mips:4300 -EB \
  --adjust-vma=0x80086A50 \
  --start-address="$START" --stop-address="$END" build/game_code.bin
```

The output was transformed mechanically according to the three rules above
into `/tmp/NAME.s`, then invoked as:

```sh
python3 tools/mips_to_c/m2c.py --target mips-ido-c /tmp/NAME.s
```

The two context probes were:

```sh
python3 tools/mips_to_c/m2c.py --target mips-ido-c \
  --context include/m2c_types.h /tmp/NAME.s

cpp -P -Iinclude include/m2c_types.h >/tmp/m2c_types.preprocessed.h
python3 tools/mips_to_c/m2c.py --target mips-ido-c \
  --context /tmp/m2c_types.preprocessed.h /tmp/NAME.s
```

## Pipeline implications

- Blob-to-assembly derivation is viable with installed GNU binutils.
- `insn_count * 4` is sufficient to carve bytes but not sufficient to assert a
  complete control-flow region. Validate every direct branch target and the
  presence of every branch/jump delay slot before calling `m2c`.
- A truncated slice can still emit plausible C (including the two-instruction
  `countdown` result), so `m2c OK` alone is not proof of a complete function.
- Preserve underlying `m2c` diagnostics; the wrapper currently discards useful
  stderr details.
- Preprocess and dependency-complete the context header, or deliberately run
  without context and accept numeric global accesses/inferred types.
