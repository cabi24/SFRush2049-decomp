# w24a - static boot tail: the 0x26F60 segment (4 fns, 5,232 B) - wave 24

Read-only except this lane dir and builder scratch `~/rush2049/scratch/frontier/w24a`. Nothing locked, promoted,
converted, spliced into src/, or committed. IDO 7.1 toolkit 4586d230 (`-g0 -O2 -mips2 -G 0 -non_shared`, plain
flags, no `-Wab,-r4300_mul` needed). Static numbers only. Proofs against master `3bec9ce3`, `prove.log`
(builder `sh lane/tools/prove.sh`).

## Headline

**The whole 0x26F60 segment, 4 functions / 5,232 B**, all `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical`
against the reloc-aware inventory targets, TU-context identical, and all four spliced into the converted TU build
`lib_26f60: IDENTICAL` to the all-passthrough TU; linked at 0x80026360 the spliced `.text` is byte-identical to
baserom 0x26F60..0x283D0 (sha1 e23d0754...). With them, lib_26f60 has no GLOBAL_ASM slot left.

| function | B | identity | scorer line (check.py = lock add verify_entry; TU = ctx preamble + real TU prefix) |
|---|---:|---|---|
| func_80026360 | 504 | libgsm Gsm_Short_Term_Synthesis_Filter head (Decoding_of_the_coded_LARs inline) | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (126 rows)`; `TU: ... identical (126 rows)` |
| func_80026558 | 232 | libgsm Gsm_Decoder (modified) | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (61 rows)`; TU identical |
| func_80026640 | 656 | libgsm gsm_decode (no magic check, sub-frame loop) | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (164 rows)`; TU identical |
| func_800268D0 | 3840 | ring-buffer bit reader + run-length frame header + decode | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (962 rows)`; TU identical |

## Identification

The segment is a fixed-point **GSM 06.10 full-rate decoder from Jutta Degener / Carsten Bormann's libgsm**, cut down
for lib_26ab0's streamed voice player (func_80025F74 calls func_800268D0 once per 160-sample frame). Recognised from
the Decoding_of_the_coded_Log_Area_Ratios constants (MIC -32/-16/-8/-4, INVA 13107/19223/17476/31454/29708, B folded
into the `+0x4000` rounding constant) and the gsm_decode bit layout. The 400-byte decoder state is the head of
lib_26ab0's StreamState (`opaque_decoder[400]`): LARpp[2][8] at 0x168, j at 0x188 as libgsm; 0x18A/0x18C are this
variant's repeat/silence run counters. Callees func_800277D0 (RPE decoding, no state), func_800278C4 (long-term
synthesis into S->dp0) and func_80027B44 (rest of the short-term filter) lie past the boot-tail code end (in the
`data` bin from ROM 0x283D0): extern only; their classification is a separate job (STATUS.csv BT08 "blocked" note).

## Levers

- func_80026360: one expression per coefficient (`LARpp_j[i] = ((((LARcr[i] + MIC) << 10) - (B << 1)) * INVA + 16384
  >> 15) << 1`); libgsm's `temp1` steps give a different schedule (true=5610).
- func_80026640: header bits as one `|` expression per LARc on c[0..4]; sub-frame bytes as `c[j * 7 + n]` (uopt
  strength-reduces to retail's `move v0,a1` pointer; `*c++` true=8150, `c += 7` true=1680).
- func_80026558: libgsm's loop header verbatim (increment order = retail's).
- func_800268D0: (1) the 1-bit header flag is read into `j` (the sub-frame loop variable, reused): a separate `flag`
  local is coloured a0 and pushes the constant 1 to a1 (true=45, 8 rows); 120 declaration orders of a separate
  flag never fix it. (2) 7.1 gives every named scalar local a 4-byte home in declaration order even when it lives
  in a register: retail's 0xF8 frame = six scalars (j, left, word, pos, S, ring - all used; `left` at 0x58 and
  `pos` at 0x50 when spilled around bzero/func_80026558). With four scalars the frame is 8 B short (strict=379);
  `S = &stream->gsm` and `ring = stream->ring` are copy-propagated and cost no code. (An unused `int k;`, a `mask`
  temp or a `ret` variable also reach 0; the used pointer pair is the deliverable.) The GETBITS macro is on one line
  because context_check.py refuses a multi-line `#define` ("unterminated preamble statement").

## Conversion (what "needs conversion" needed)

The segment derives cleanly (`layout.derive`: no refusal, 4 functions tiling 0x80026360..0x800277D0, no data, no
gaps); inventory targets already exist (n64_target rows, reloc-aware: fetch_targets OK). It needs only:
1. `layout convert 0x26f60` (splat.us.yaml `[0x26F60, asm]` -> `[0x26F60, c, rom/lib_26f60]`, `make extract`,
   all-passthrough src/rom/lib_26f60.c + asm/us/nonmatchings/rom/lib_26f60/*.s, asm/us/26F60.s removed);
2. the 7.1 pin `"0x26f60": "ido7.1"` in rom_compilers.json + `write_opt_overrides()` (`lib_26f60.o: CC := $(IDO71_CC)`);
3. docs/COMPILER_SETTINGS.md row update. No new TU split, no data slots, no inventory rows, no symbol changes.
`conversion.diff` (= `tools/convert.sh` on master 3bec9ce3; `git apply --check` passes) is exactly that. Note:
regenerating opt_overrides.mk also re-adds `lib_cc50.o: CFLAGS := $(subst -O2,-O1,...)` (dropped by hand in
7a90b14e; master's file is stale vs the generator). Harmless: lib_cc50 builds through storage_overrides.mk's explicit
`-O1` rule, which ignores $(CFLAGS) - but it is a generator/file drift the tooling lane may want to settle.
Proof of the conversion step: the passthrough TU built by 7.1 through asm-processor, linked at 0x80026360, equals
baserom 0x26F60+0x1470 (prove.log `orig:` line); the coordinator's full-ROM gate is still required.

## Integration (coordinator; static only, no unit_overrides, nothing superseded)

```
L=cloud/work/frontier/w24a; TK71=4586d230749de1bc8a64add52ae53db0ffde18a1eee76c8a73f3791fd98e2557
git apply $L/conversion.diff        # or: sh $L/tools/convert.sh
# full-ROM gate on the builder (make COMPILER=ido && make test), commit the conversion (promote_batch needs a clean tree)
for f in func_80026360 func_80026558 func_80026640 func_800268D0; do
  python3 -m tools.conveyor.pipeline.lock add $L/sources/$f.c:$f --flags "-g0 -O2 -mips2 -G 0 -non_shared" --toolkit $TK71; done
python3 cloud/work/boot_tail_promotion/promote_batch.py $L/ctx71_O2.jsonl 0x26f60 --only func_80026360,func_80026558,func_80026640,func_800268D0 --prefix-dedup rom/lib_26f60 --dry-run
# then without --dry-run
```
Context rows `$L/ctx71_O2.jsonl` (4 rows ok, ido7.1, computed against the converted TU; recompute with
`python3 cloud/work/boot_tail_promotion/context_check.py --compiler ido7.1 OUT.jsonl fn=$L/sources/fn.c ...`).
Coverage: the four are already in the static denominator (unconverted, unrefused segments count): +4 fns, +5,232 B.
No sequence_context.h views needed (private GsmState_26F60/GsmStream_26F60 typedefs, deduplicated by the preamble).

## What generalises

1. **7.1 homes every named scalar local** (4 B each, declaration order, even register-only ones): a frame N*4 B larger
   than the draft's means N missing named locals; look for natural used ones (state/base pointers that copy-propagate
   away) before unused ones. The spill offsets of the spilled locals pin their declaration positions.
2. **A reused loop variable** as an earlier scratch value is a colouring lever (j as the header flag here).
3. **Boot-tail third-party codecs**: check libgsm/MusyX/ultralib by constants first; this segment's whole identity fell
   out of the INVA/MIC table.
4. Converting a clean asm segment is mechanical (`layout convert` + 7.1 pin); the other `asm` code segments in
   splat.us.yaml should be checked with `layout.derive()` for refusals the same way.

## Files
`sources/` (4), `ctx71_O2.jsonl`, `prove.log`, `conversion.diff`, `tools/` (`convert.sh`, `prove.sh`, `t.sh`, `vb.sh`, and
w23c's `check.py`/`sc.sh`/`dif.sh`/`splice.py`/`tusplice.sh`/`asmcmp.py`), `tgt/` (inventory target objects, ignored),
variants in `func_800268D0/v/` (ignored).

## Disclosures
- No shaping devices, no unused locals, no pad arrays, no implicit int, no false prototypes (callee return types void
  where retail ignores v0; func_800268D0/func_80026640 return int 0 as retail `move v0,zero`).
- Reused local: `j` holds the header flag before the sub-frame loop (func_800268D0). Copy locals `S`/`ring` (used).
- Builder: only `~/rush2049/scratch/frontier/w24a` (+ mktemp dirs under its /tmp by check.py/dif.sh); scratch copies of
  tools/asm-processor, rom_owned_data.json, rom_compilers.json, baserom, the converted TU files, and a symlink
  `tools/ido-static-recomp/build71/out` -> the 4586d230 toolkit. Conversion produced in a session-scratchpad
  `git archive` copy of master (own throwaway git repo), never in the project tree.
- Permission denial (not retried): one `cd ...; rm -rf func_800268D0/v4; rm -f func_800268D0/v4/*` cleanup of a fresh
  variant dir was refused by the safety check; the variants were written without the removal.
