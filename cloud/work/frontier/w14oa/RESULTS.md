# Wave 14, lane w14oa: entity_lod_select (MATCH), entity_process_main (interrupted)

Written by the coordinator on 2026-10-10 from the lane's files; the lane's session ended before it reported.

| Function | Bytes | State | Flags | Scorer output |
|---|---:|---|---|---|
| entity_lod_select | 716 | **MATCH** standalone; spliced 2026-10-10 | -g0 -O3 -mips2 -G 0 -non_shared | `score.py fn` (builder, 2026-10-10): `entity_lod_select: MATCH / own .rodata verified at 0x80123A70..0x80123A8C`; `splice_singles.py`: image_ok, locked 940 |
| entity_process_main | 1424 | 2 of 356 words (unchanged from w11a); trials `epm/k0-k6` (t_dup/t_ld) best strict 18 standalone | -O3 | `epm/k6/score.txt`: `strict 18 ... v0010.c` (standalone vbatch; the unit baseline is 2 words) |

entity_lod_select: `cloud/matches/entity_lod_select.c` = `els/S5.c`; `els/f11-f12/score.txt`: `strict 0 ...
aligned-missing 2` (the 2 are the verified jump-table relocation). Progression w14n 3 words -> `els/h*` (and-temp
colour) -> `els/S*` 0. Shaping disclosed in the file header: a second pointer local `head = stk[depth]` whose code-free
web fixes the and-temp colours; a compiled-out read `if (cmd[0] & 0x00FF0000) {}`; frame-equivalent named temporaries.
