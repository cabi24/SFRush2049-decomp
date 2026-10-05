# w5d tools: tracing uopt before register colouring

These scripts trace what IDO 5.3 `uopt` decides before it colours registers: which expressions become
CSE/PRE candidates and which occurrences are deleted or inserted, which values are hoisted out of loops,
and which constants or addresses get a register live range instead of being rematerialised at each use.
They also bring the w3a colouring trace along, so a web number can be read as an expression.

The builder is watchman2. The uopt build is in `~/rush2049/scratch/frontier/w5d/uopt/uopt`, and snapshots
are in `~/rush2049/scratch/frontier/w5d/st_<LABEL>/`. The whole-program unit runs with `blob_unit --tag w5d`.

## Build and fidelity

```bash
cloud/work/frontier/w5d/tools/build_uopt.sh     # ~20 s; instrument_w5d.py + gcc on watchman2
cloud/work/frontier/w5d/tools/run_fidelity.sh [LABEL]
```

`instrument_w5d.py` first applies the vendored workbench `globalcolor` profile unchanged, then adds the w5d
hooks. Every hook is a no-op unless a `W5D_*` variable is set. `ecvt.patch.py` gives the recompiled libc real
`ecvt`/`fcvt`, because uopt's listing writer needs them; this matters only while a listing is being written.

Fidelity was checked on 2026-10-05 on two whole-unit snapshots: `st_fid`, with the audio_mixer_main
candidate staged, and `st_nsv_best`, with the net_state_validate candidate. The stock toolkit uopt, the w5d
uopt with tracing off, and the w5d uopt with *everything on* (`-l`, `W5D_LEVEL=3`, `W5D_OUT`, `CDX_LOG`, all
procedures) wrote byte-identical `opt`:

```
f6597ed9bf6c360848a39d5d2c8e70e01001ba99  opt.stock / opt.off / opt.on   (st_fid)
a516afce19eb2918e70d4a13b3323f6130ce9b23  opt.stock / opt.off / opt.on   (st_nsv_best)
```

The source is the recompiled `uopt.c` with sha256 `627eff8f…`, the same file w3a used.

## What it traces

**uopt's own debug listing.** Stock IDO uopt already contains dump routines (`printtab`, `printitab`,
`printcm`, `printscm`, `printregs`, `printhoist`, `printprecm`). They are selected by a debug level, set with
the option `-zdbug:N`, and written to the `-l FILE` listing. The level is an equality test, so one run gives
one level. `w5d_dbg()` routes every read of the level. With `W5D_LEVEL=N W5D_PROC=name`, only that procedure
is dumped. This keeps a run at about 1–6 s and the listing small.

| level | dump |
|---|---|
| 1 | expression hash table after local optimisation |
| 2 | bit table + loop info |
| **3** | bit table + **per-block PRE vectors** (antlocs avlocs alters absalters antin antout iv cand ppin ppout insert delete subdelete subinsert) + `@ iscolored 1/2` (register candidates), indmults, mvarbits |
| 4 | store code motion |
| 21 | hoist info (findinduct) |
| 25 | before insertion/deletion: avin/avout/pavin/pavout per block |

**`W5D_OUT` lines (`[W5D] …`):**
- `bit at=cm|gc` gives every expression bit (ichain) with its full text, rendered recursively. Variables are
  `varM(block+offset)` (`r` = register candidate), memory ops include their offset (`ilod.J@44(…)`),
  addresses are `ldaS(dense-symbol-number+offset)`, and constants are given by value. `cm` is after code
  motion. `gc` is at globalcolor entry and includes the bits that register-allocation preparation gave to
  constants and addresses. **The bit number is the web number in the colouring trace** (`sym=`).
- `regcand` covers each constant or address operand that makelivranges tests for a live range (constinreg /
  ldainreg / converted constants): the block, a context code and decision 1 (live range) or 0
  (rematerialised).
- `ordinal` is the procedure's globalcolor ordinal, used as `CDX_PROC` for `CDX_FORCE`.

**Oracle `W5D_NOPRE=bits`** (diagnostic only, like `CDX_FORCE`). After code motion it clears the given bits
from the procedure's *delete* vector (`W5D_NOPRE_VEC`: d delete, s subdelete, i insert, u subinsert, h
hoistedexp). It answers "would the code match if PRE had not removed these occurrences?". Clearing
insertions is not supported, because codemotion has already materialised them (uopt segfaults).

## Scripts

| script | use |
|---|---|
| `pretrace.sh NAME CAND.c LABEL [PROC]` | unit score (tag w5d; `EXTRA=` for `--internal/--keep`), snapshot `st_LABEL`, traced run, copies `list`/`w5d`/`cdx` to `runs/LABEL/` and writes `report.txt` |
| `prereport.py DIR PROC [--bits …] [--all]` | per-expression summary: occurrences, DELETE/INSERT blocks, candidate sets, web/colour; constant/address live-range table |
| `precmp.py RUN_A RUN_B PROC` | compares two runs by expression text (bit numbers differ between builds) |
| `vrun.sh NAME FILE.c…` | pretrace several variants and print the report rows matching `GREP` |
| `nopre.sh LABEL NAME PROC BITS` | PRE oracle (above) → ugen → as1 → aligned diff against retail |
| `force.sh LABEL NAME ORDINAL SPEC` | w3a's colouring oracle (`CDX_FORCE`) on a w5d snapshot |
| `cmparse.py LISTING PROC` | parser library for the level-3 listing |
| `redundancy_scan.py L3 L25 W5D [lock]` | whole-unit scan for fully-available occurrences that PRE kept |

Typical loop:
```bash
EXTRA="--internal X --keep NAME" tools/pretrace.sh NAME cand.c lab      # read runs/lab/report.txt
tools/nopre.sh lab NAME NAME <bit>                                      # is this CSE the whole residual?
tools/precmp.py runs/labA runs/labB NAME                                # what a source change did before colouring
```

## Layout notes (IDO 5.3 uopt, from the recompiled source)

- Bit table: `MEM_U32(0x1001cc30)` holds 8-byte entries, and the count is at `0x1001cb38`. An ichain has
  kind at +0 (1 islda, 2 isconst, 3 isvar, 4 isop, 5 isilda, 6 issvar, 7 dumped, 8 isrconst), dtype at +1,
  bit at +2 and hash/chain at +4/+6. An isop has the op at +16 and operands at +20/+24; the ilod offset is at
  +28 and the istr offset at +38. An isvar has offset +16, block +20, mtype +22, register flag +25. An islda
  has offset +24, block +28, mtype +30. A const has its value at +16.
- Basic-block nodes: list head `0x1001c8f8`, next at +12, number at +8. Vectors are {nblocks, ptr} with 128
  bits per block: antlocs 0x104, alters 0x10c, avlocs 0x114, absalters 0x11c, delete 0x124, ppin 0x12c,
  iv 0x134, cand 0x13c, subdelete 0x144, subinsert 0x14c, antin 0x154, antout 0x15c, insert 0x164,
  ppout 0x16c, hoistedexp 0xfc.
- Procedure name: `0x1001c4d0`, length at `0x1001c8d0`. Debug level: `0x1001eaf8`. `const_in_reg` flag:
  `0x1001eb1c`.
- The dtype letters assume the Ucode enum order `ACFGHIJKLMNPQRSWXZ`; J, L, R and A were confirmed.
  `str.?` means a value outside that range.
