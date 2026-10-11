# w17g - static side: Rush-specific boot `func_*` code + data-bound library items

Read-only except this lane dir and builder scratch `~/rush2049/scratch/frontier/w17g`. Nothing locked, promoted,
converted, spliced into src/, or committed. No edits to src/, asm/, tools/, include/, locks, rom_owned_data.json,
symbol_addrs or the live conveyor DB (but see "Disclosures": two content-addressed blobs were added to the live blob
store by a scratch-DB run). Static numbers only; never merged with game coverage.

## Headline: the Rush boot-tail code was compiled with IDO 7.1, not 5.3

The 176 unmatched static `func_*` rows (66,104 B, segments 0x11050-0x26f60) are almost all in the D10 boot-tail
ledger (`cloud/work/boot_tail/STATUS.csv`) as "COMPLETE-NONMATCH" (155) with a best source and a 5.3 diff count.
The common residual is one idiom: a narrow (`u8`/`u16`) parameter is homed (`sw a0,X(sp)`) and narrowed IN PLACE
(`andi a0,a0,0xFFFF`). IDO 5.3 always narrows into a temporary (`andi t6,a0,0xffff ... move a0,t6`).
Word scan of `asm/us/boot_tail/boot_tail_8000f3a4.s` (439 functions): the in-place form occurs in 84 unmatched
functions and in exactly one locked one (func_800149BC, an int parameter cast at the call). IDO 7.1
(decompals ido-static-recomp v1.2 release `ido-7.1-recomp-linux.tar.gz`, sha256
0d411696e178fcca34c31c3bf02011b928d7fd9c1fa7f8bf45070e0781b58e15) produces it from the natural source:

```
func_80015348 (void f(unsigned short id){ func_8001661C(id); }), -g0 -O2 -mips2:
  IDO 5.3: sw a0,24(sp); andi t6,a0,0xffff; jal; move a0,t6      (true=165)
  IDO 7.1: sw a0,24(sp); jal; andi a0,a0,0xffff                    (= ROM)
```

Proof that switching the boot-tail TUs to 7.1 is byte-neutral for everything already matched:
- `locked71.sh` -> `locked71.log`: every static lock (435 entries) re-scored under IDO 7.1 with its own source and
  flagset against its inventory target: 397 exact. All 38 that differ are libultra (segments 0x1050-0x10d60:
  scheduler, vi, pfs/cont, crc, ...). Every locked function in segments 0x11050-0x26ab0 is exact under 7.1.
- `tu71.sh` -> `tu71.log`: the 18 current boot-tail ROM TUs (lib_11050 ... lib_26ab0, as on master 66fe4d4f),
  built exactly like the Makefile ROM-TU rule (asm-processor + default -O2 CFLAGS) with 5.3 and with 7.1:
  `IDENTICAL` (.text words + relocations + .data/.rodata/.bss) for all 18. Controls lib_10d60, lib_105f0, lib_f700
  (libultra): `DIFFERENT`. So the split is clean: libultra = 5.3, Rush/MusyX-style boot tail = 7.1.
- `bt71.sh` -> `bt71.log`: the 155 D10 nonmatch sources re-scored under 7.1 at -O2: **58 exact unchanged**, plus
  a long tail at true 5-40 whose residual is relocation naming only (see "Near misses").

## Results

58 functions, 9,912 B, production-ready in `sources/` (all in converted segments, all LOCK true=0 strict=0
reloc_blind=0 objdump-dr identical against the reloc-aware inventory target), and all 58 spliced together into
their 12 TUs and built with 7.1 = byte-identical to master's 5.3 build of the same TUs (`tusplice.log`):

```
lib_113e0 IDENTICAL (152)  lib_11640 IDENTICAL (4276) lib_15a10 IDENTICAL (578)  lib_17dc0 IDENTICAL (2598)
lib_1a660 IDENTICAL (2626) lib_1cf90 IDENTICAL (1876) lib_1ece0 IDENTICAL (405)  lib_1f340 IDENTICAL (154)
lib_207b0 IDENTICAL (661)  lib_21210 IDENTICAL (1081) lib_22300 IDENTICAL (3382) lib_26ab0 IDENTICAL (297)
```
(each line: "<tu>: IDENTICAL (N lines: .text words+relocs, data sections)"; spliced TU text kept in `spliced/`).

| group | fns | bytes | compiler | promotable |
|---|---:|---:|---|---|
| empty hooks / forwarders (w17g, new) | 7 | 116 | 5.3 AND 7.1 | today (5.3), `dryrun.log` LOCK+TU PASS |
| D10 sources exact under 7.1 | 51 | 9,796 | 7.1 only | after the 7.1 tooling below |
| LOCK-exact under 7.1, TU-context blocked (`lockonly/`) | 9 | 2,532 | 7.1 | needs type harmonisation (below) |

Static would move 431/668 (77,844 B) -> 438/668 (77,960 B) with the 7 stubs, -> 489/668 (87,756 B) with all 58.

Per-function score lines (IDO 7.1, w16g `check.py` with TK=tk71): `score_table.md` (sizes, segment, exact
LOCK line, TU line). Representative, verbatim from `dryrun71.log`:
```
== func_80015348  LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (9 rows)   TU: true=0 strict=0 reloc_blind=0 objdump-dr identical (9 rows)
== func_80020200  LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (9 rows)   TU: true=0 strict=0 reloc_blind=0 objdump-dr identical (9 rows)
== func_8001CCDC  LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (243 rows) TU: true=0 strict=0 reloc_blind=0 objdump-dr identical (243 rows)
== func_80025F74  LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (213 rows) TU: true=0 strict=0 reloc_blind=0 objdump-dr identical (213 rows)
```
16 rows show "TU: COMPILE FAIL redeclaration of 'MacroCommand'/'Vector'": check.py appends the preamble without
promote_batch's statement dedup; the real splice (`splice71.py` = promote_batch.splice with `--prefix-dedup`)
dedups them and those TUs are proven IDENTICAL above.

The 7 w17g stubs (5.3, `dryrun.log`, `run_all.sh`): func_80014CF4, func_80014D14, func_8001E0D4 (`void f(void){}`,
8 B), func_80014CFC, func_80014D08 (`void f(void **, void **){}` / `(void **, void *)`, 12 B: IDO homes declared
params of an empty body), func_80014BD8 (`func_80010E80(state)`), func_80014BF8 (`func_80011074()`), 32 B each:
all `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical` and `TU: ... identical` in lib_11640/lib_1cf90.
Prototypes come from the locked callers (lib_16320, lib_207b0, lib_113e0) and call sites; headers say so.

Source headers: line 1 `/* flags: -g0 -O2 -mips2 -G 0 -non_shared */`, then compiler (IDO 7.1), D10 provenance and
semantics (the ledger's name hypothesis), 5.3 diff count, every w17g edit, shaping devices. Bodies are D10 verbatim
except these codegen-neutral TU-context edits (all re-proved by LOCK/TU and the whole-TU build):
- private struct views renamed `<Type>_<addr>` where the TU already defines a different view of the same object
  (MacroState in 11 lib_22300 sources, SpatialState in func_8001D660, StreamState in func_80025F74);
- where such a function calls a callee the TU already prototypes with the TU's type, the source copies the TU's
  typedef verbatim + keeps that prototype, and casts the argument `(MacroState *)state` / `(SpatialState *)state`
  (disclosed as a shaping device: type view only, no code) - func_80021E74, 8002279C, 8002300C, 80022514,
  800239A4, 80022A98, 8001D660;
- func_80016E68 declared `void *` (table lookup by id) in both w17g callers (D10 used two pointee types);
- lib_11640 four (func_80011C1C, 8001467C, 8001489C, 80014A0C) and func_8001CCDC use the shared headers
  (`boot_tail_audio_record.h` AudioState, `boot_tail_sample_contract.h` StateNode; same offsets, field renames in
  the header) instead of private copies; func_80018BEC drops an unused typedef/extern;
- func_80022A40 keeps its `#pragma pack(1)` view under its own name (the TU's MacroState is unpacked; with it,
  field60 compiles to `lw` instead of retail's `lwl/lwr` - caught by the TU proof).
No pad locals, no false prototypes, no implicit declarations (scanned), no compiled-out shaping blocks.
`volatile` globals appear where D10 had them (busy flags), disclosed per file.

### LOCK-exact but TU-blocked (`lockonly/`, 9 fns, 2,532 B; all LOCK true=0 strict=0 rb=0 identical under 7.1)
- func_800180A0, func_80018184, func_8001897C (lib_17dc0): their private `Context` view of D_8004BE80 conflicts
  with `sequence_context.h` (`SequenceContext *D_8004BE80`), and the fields they use (0x118/0x11C/0x120 as s32,
  0xF78 list head, 0xFC2 scale, per-track array at 0x568) are `unknown`/u32 there; needs header fields added
  (signedness matters: `half120 >> 1` is sra), not casts.
- func_8001D764 (lib_1cf90): `extern SpatialState *D_8004FD54` vs the TU's `extern LinkNode *D_8004FD54`.
- func_8001BDB8, func_8001C19C (lib_1a660), func_8001D0F0 (lib_1cf90): need `ChannelState`/`Vector` before the
  TU defines them (the TU's typedef sits after their slots in a later promoted block) - a definition-order problem
  in the TU, not a codegen one.
- func_80011F60, func_800123A8 (lib_11640): exact under 7.1 once the callee is named `osInvalDCache` (D10 used the
  address label func_800084E0); the TU later declares `extern void *D_8003833C, *D_80038338, *D_80038344`
  differently from their typed externs.

### Near misses (not worked; stop rule / breadth)
`bt71.log` lines with reloc_blind=0 and small true are relocation NAMING only (linked bytes identical): the target
uses splat's per-address labels where D10 wrote base+offset fields, e.g. func_80015720 (`D_8003C61C` vs
`D_8003C618+4`), func_80015C0C, func_800161A0, func_80019420 (`D_800443E0` vs `D_80043EB8+0x528`),
func_8001E7BC. Lever: reference the labelled symbol (all exist in symbol_addrs). Others at true 10-40 with rb 1-2
(func_80014E1C/E64/E90/EBC/EE8, 800114C0, 80018AEC ...) were not examined.

## What the integration needs (smallest tooling change; nothing changed here)

1. A conveyor toolkit bundle identical to d4c39cbc but with `ido/` = the IDO 7.1 recomp above (my scratch
   `tk71/` is exactly that, symlinked), registered with the coordinator; `lock add` gains `--toolkit SHA`
   (verify_entry puts `http.pinned_toolkit()` into the manifest today; the lock entry already records
   `toolkit_sha`).
2. Build: IDO 7.1 binaries in the tree (e.g. `tools/ido-static-recomp/build71/out`, from the release tarball or
   built from the recomp source with VERSION=7.1), and a per-TU compiler override written by
   `layout.write_opt_overrides` for segments whose locks carry the 7.1 toolkit:
   `$(BUILD_DIR)/src/rom/lib_11640.o: CC := $(IDO71_CC)` (same mechanism as the -O1 / -mips3 rules).
   Because every boot-tail TU is already byte-identical under 7.1 (`tu71.log`), the switch can land first, alone,
   behind the ROM SHA-1 gate.
3. `promote_batch.py` with `--prefix-dedup` for these TUs and `ctx/ctx71_O2.jsonl` (context rows computed with
   IDO 7.1: `ctx71.py` = context_check.py unchanged with CC=tk71). promote_batch's flagset check compares strings,
   so the flagset stays `-g0 -O2 -mips2 -G 0 -non_shared`; the compiler identity lives in the lock's toolkit_sha.

### Commands for the main checkout

Today (5.3, no tooling change; 116 B):
```
for f in func_80014CF4 func_80014D14 func_8001E0D4 func_80014CFC func_80014D08 func_80014BD8 func_80014BF8; do
  python3 -m tools.conveyor.pipeline.lock add cloud/work/frontier/w17g/sources/$f.c:$f --flags "-g0 -O2 -mips2 -G 0 -non_shared"; done
python3 cloud/work/boot_tail_promotion/promote_batch.py cloud/work/frontier/w17g/ctx/ctx_O2.jsonl 0x11640 0x1cf90 \
  --only func_80014CF4,func_80014D14,func_8001E0D4,func_80014CFC,func_80014D08,func_80014BD8,func_80014BF8
```
After the 7.1 tooling (51 fns, 9,796 B; no `layout convert` needed, all segments are converted):
```
L71="$(ls cloud/work/frontier/w17g/sources | sed 's/\.c$//' | grep -vx -e func_80014CF4 -e func_80014D14 -e func_8001E0D4 -e func_80014CFC -e func_80014D08 -e func_80014BD8 -e func_80014BF8)"
for f in $L71; do python3 -m tools.conveyor.pipeline.lock add cloud/work/frontier/w17g/sources/$f.c:$f \
  --flags "-g0 -O2 -mips2 -G 0 -non_shared" --toolkit <IDO71_TOOLKIT_SHA>; done
python3 cloud/work/boot_tail_promotion/promote_batch.py cloud/work/frontier/w17g/ctx/ctx71_O2.jsonl \
  0x113e0 0x11640 0x15a10 0x17dc0 0x1a660 0x1cf90 0x1ece0 0x1f340 0x207b0 0x21210 0x22300 0x26ab0 \
  --only $(echo $L71 | tr ' ' ,) \
  --prefix-dedup rom/lib_113e0,rom/lib_11640,rom/lib_15a10,rom/lib_17dc0,rom/lib_1a660,rom/lib_1cf90,rom/lib_1ece0,rom/lib_1f340,rom/lib_207b0,rom/lib_21210,rom/lib_22300,rom/lib_26ab0
```
(`ctx_O2.jsonl` = 5.3 rows for the 7 stubs; `ctx71_O2.jsonl` = 7.1 rows for all 60 incl. the two moved to
lockonly - `--only` excludes them.)

## (c) Data-bound items

### osAiSetNextBuffer (144 B, lib_ca70, libultra = IDO 5.3)
Registry entry (`data/owned_osAiSetNextBuffer.json`), append to `rom_owned_data.json` `rom_slots`:
```
{"owner": "osAiSetNextBuffer", "tu": "src/rom/lib_ca70.c", "source": "assets/us/data.bin",
 "container_section": ".data", "owner_section": ".data", "offset": "0x4bf0", "size": "0x10",
 "container_vram": "0x800277d0",
 "sha256": "374708fff7719dd5979ec875d56cd2286f6d3cf7ec317a3b25632aab28ec37bb",
 "passthrough_asm": "asm/us/nonmatchings/rom/lib_ca70/osAiSetNextBuffer.data.s",
 "composed_source": "build/us/assets/us/data.composed.bin"}
```
Companion `data/osAiSetNextBuffer.data.s` (asm-processor accepts only `.balign 4`; the section still comes out
size 0x10 align 16): `.section .data` / `.balign 4` / `.incbin "assets/us/data.bin", 0x4BF0, 0x10`.
Owner = the function name so `promote run`'s `strip_companion(text, func)` removes it.
Dry runs: `owned_data.rom_slots` on a temp repo with the entry: OK (no overlap; 16-aligned; bytes = 16 zeros);
`rewrite_linker` moves `lib_ca70.o(.data)` into the slot with `ASSERT(START == 0x8002c3c0)` and size 0x10.
Builder (ai/): lib_ca70 built three ways (current; passthrough + companion; w16g C body + ctx additions), each
linked with .text at 0x8000BE70, .data at 0x8002C3C0, symbol_addrs/hardware_regs: all three
`.text == ROM 0xca70..+0x1e0`; C object `.data` = 0x10 zero bytes, align 16.
Sequence: (1) registry entry + companion .s + insert `owned_data.companion_block(row)` right after the
osAiSetNextBuffer slot in src/rom/lib_ca70.c -> ROM gate; (2) `lock add cloud/work/frontier/w16g/sources/
osAiSetNextBuffer.c:osAiSetNextBuffer --flags "-g0 -O2 -mips2 -G 0 -non_shared"`, then promote with the
`w16g/ctx/osAiSetNextBuffer.h` additions (AI_*_REG defines, `__osAiDeviceBusy` prototype): `promote run` splices
the body only, and promote_batch refuses data-carrying rows and does not strip companions, so this one is a manual
splice of additions + body + companion removal behind the same gate. `__osAiNeedsAlign` stays defined by
undefined_syms_auto (only the passthrough referenced it).

### __fixunsdfdi / __fixunssfdi (160+156 B, lib_a5f0, `-O1 -mips3 -32`)
1. Reloc-aware targets: `targets_rename apply __fixunsdfdi __fixunssfdi` refuses today
   (`assemble_error: macro used $at after ".set noat"`): `targets.assemble_region`'s isolated prelude lacks
   `.set gp=64`, so GNU as (-mabi=32) expands the 64-bit `ld $t7,%lo(gInfinity)($t7)` as a macro. The ROM build's
   `tools/asm-processor/prelude.inc` has `.set gp=64`. One-line change: add `".set gp=64",` after `".set noat",`
   in `assemble_region`. `fixuns_rereg.py` applies it in memory: on a scratch DB copy
   `refresh __fixunsdfdi @0x80009A28 40 insns seg 0xa5f0 reloc_aware o=b9227be98966` /
   `refresh __fixunssfdi @0x80009AC8 39 insns ... o=b81b16683e14` (supersedes 360 matrix rows each).
   Against the new objects the w16g sources score `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr DIFFERS(2 rows)`
   (both); the 2 rows are the HI16/LO16 symbol `.rodata` vs `gInfinity`/`gNegInfinity` - exactly the owned-data
   case (against the old raw_word targets: true=10).
2. Owned .rodata (`data/owned___fixuns.json`): one slot, owner `__fixunsdfdi`, owner_section .rodata,
   offset 0x6030, size 0x10, sha256 fa9ad8e381ae4fc97686921369c821ca3444d5deebd56c82faf9df71bb39ba79 (two u64
   0x8000000000000000 at 0x8002D800/0x8002D808), companion `data/__fixunsdfdi.rodata.s`. rom_slots: OK; linker:
   `ASSERT(START == 0x8002d800)`, size 0x10. Builder (fx/): lib_a5f0 with BOTH C bodies at -O1 -mips3 -32:
   `.rodata` = `80000000 00000000 80000000 00000000`, size 0x10; linked .text at 0x800099F0 == ROM 0xa5f0..0xa800.
   One slot cannot be split per function (the registry keys sections per TU), and promoting one alone leaves 8 zero
   bytes at 0x8002D808, so both must land in one transaction (manual splice of both + companion removal; neither
   promote run nor promote_batch does a two-function companion strip).

### Stale inventory rows
`targets_rename audit` (scratch DB copy) lists exactly `__osGetCause @0x8000DBB0 layout name is osCauseGet` and
`__osContAddressCrc insn_count 23 != layout extent 52`. Dry run of the fix:
```
python3 -m tools.conveyor.pipeline.targets_rename apply osCauseGet __osContAddressCrc --dry-run
register  osCauseGet          @0x8000DBB0    3 insns seg 0xe4f0   reloc_aware o=54eac4d7b131  (new row)
refresh   __osContAddressCrc  @0x8000F9F0   52 insns seg 0x105f0  reloc_aware o=0cdbca3b4ac5  (was 23 insns; object unchanged)
retire    __osGetCause        @0x8000DBB0  renamed to osCauseGet ...
```

## Inventory (step 1)
`inv.py` (Pi, read-only) -> `inv.json`: conveyor `n64_target` population=static (666 rows) minus the
`target_id`s in `matched.lock.json` = 233 unmatched static targets, 82,032 B by layout size; 176 `func_*` =
66,104 B, all in converted segments except 4 in 0x26f60 (5,232 B). By segment (count/bytes): 0x11640 26/12,288,
0x22300 38/11,724, 0x1a660 14/7,976, 0x17dc0 17/6,140, 0x16320 17/5,576, 0x26f60 4/5,232 (asm), 0x1cf90 14/4,980,
0x21210 12/3,336, 0x1f5b0 3/2,144, 0x1ece0 3/1,448, 0x25bb0 4/1,268, 0x15a10 11/1,256, 0x207b0 6/788,
0x1f340 3/416, 0x11050 1/372, 0x113e0 2/320, 0x26ab0 1/840. D10 ledger: 155 nonmatch, 13 open, 8 not in it.
Columns per row in `inv.json`: size, segment/converted/flagset, jal callers/callees (static asm) and which are
locked, D10 status/diff/source, prior-lane mentions. Full func_ table with the IDO 7.1 re-score and w17g state:
`inv_table.md` (176 rows).

## Files
`sources/` (58, ready), `lockonly/` (9), `ctx/ctx_O2.jsonl` (5.3 rows, stubs), `ctx/ctx71_O2.jsonl` (7.1 rows),
`spliced/` (the 12 spliced TUs proven identical), `data/` (owned-data entries + companions), logs: `bt71.log`,
`locked71.log`, `tu71.log`, `tusplice.log`, `dryrun.log` (5.3), `dryrun71.log` (7.1), `score_table.md`,
`inv_table.md`. Scripts: `inv.py`, `package.py`, `run_all.sh` (5.3), `run71.sh`, `ctx71.py`, `bt71.sh`,
`locked71.sh`, `tu71.sh`, `splice71.py`, `tusplice.sh`, `harmonize.py` (mechanical type renames, used then
hand-finished), `fixuns_rereg.py`. Builder scratch: `~/rush2049/scratch/frontier/w17g` (`tk71/` = 7.1 toolkit view,
`dl/ido71/` = the downloaded release, `lane/` = this dir, `ai/`, `fx/`).

## Disclosures
- IDO 7.1 was downloaded on the builder into my scratch only (`dl/ido71`, release tarball sha256 above).
- `fixuns_rereg.py` was applied to a scratch copy of conveyor.db whose `blobs/` was a symlink to the live
  `~/.conveyor/blobs`, so two new content-addressed target objects (b9227be9..., b81b1668...) were added to the
  live blob store. No DB row, lock or existing blob changed; they are unreferenced until the re-registration runs.
- `layout derive` rewrote the derived `build/layout.us.json` on the Pi (no content change expected).
- No permission denials.
