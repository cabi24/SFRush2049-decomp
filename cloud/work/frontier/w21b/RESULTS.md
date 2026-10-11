# w21b - static boot tail under IDO 7.1: w20b leftovers (wave 21)

Read-only except this lane dir and builder scratch `~/rush2049/scratch/frontier/w21b`. Nothing locked, promoted,
spliced into src/, or committed. Static numbers only. Toolkit 4586d230 (7.1), flags `-g0 -O2 -mips2 -G 0 -non_shared`,
targets = reloc-aware inventory objects (w17k fetch_targets.py, fetched this session).

## Headline

**3 functions, 1,676 B, production-ready** in `sources/` (whole-TU IDENTICAL, context rows ok) + **1 function, 460 B,
code-identical with a function-local static** (`ownbss/`, linked lib_22300 .text == baserom). Three of the four are
MusyX functions whose control flow comes from the GameCube MusyX decomp (doldecomp/ttyd `synthdata.c`,
`synthvoice.c`; shape only).

| function | B | TU | state | scorer line (check.py = lock add verify_entry) |
|---|---:|---|---|---|
| func_8001EF8C | 432 | lib_1f5b0 | MATCH | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (124 rows)` |
| func_8001671C | 636 | lib_16320 | MATCH with reloc_addrs entries | current target `LOCK: true=20 strict=20 reloc_blind=0 objdump-dr DIFFERS(6 rows) (186 rows)`; re-split target `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (186 rows)` |
| func_80016998 | 608 | lib_16320 | MATCH with reloc_addrs entries | current `LOCK: true=20 strict=20 reloc_blind=0 objdump-dr DIFFERS(6 rows) (169 rows)`; re-split `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (169 rows)` |
| func_80022874 | 460 | lib_22300 | code identical, owned .bss needed | `LOCK: true=0 strict=0 reloc_blind=6 objdump-dr DIFFERS(30 rows) (135 rows)` (rows = `.bss+N` vs splat labels); linked == ROM |
| func_8001A270 | 872 | lib_1a660 | near miss | `best_true0.c` true=0 strict=418 (frame 104 vs 96); `best_frame.c` true=225 strict=225 (D10 4745) |
| func_8001E0E0 | 864 | lib_1ece0 | not closed | `best.c` true=2215 (D10 2800): FP colouring only |
| func_80019F48 | 808 | lib_1a660 | not closed | `best.c` true=2235 strict=2453 (w20b 2235/2481) |

Whole-TU proof (`prove.log`, builder, `tools/prove.sh`):
```
== TU edit alone
lib_16320: IDENTICAL (1701 lines: .text words+relocs, data sections)
== all sources spliced into the edited TUs, vs original TUs with re-split GLOBAL_ASM
lib_16320: IDENTICAL (1701 lines: .text words+relocs, data sections)
lib_1f5b0: IDENTICAL (1152 lines: .text words+relocs, data sections)
== ownbss func_80022874
linked orig == linked spliced: IDENTICAL
linked spliced == baserom 0x22300..+13552: IDENTICAL
```
Re-run against the working tree's uncommitted lib_16320.c (the ownbss trio being promoted): both TUs still IDENTICAL
(1705 lines), appended to prove.log. Context rows: 3/3 `ok` (`ctx71_O2.jsonl`, `"compiler": "ido7.1"`).

## Levers

- **func_8001EF8C** (MusyX voiceSetPriority; w20b 310): the sorted insert's last test reads the just-stored link back,
  `if (D_80050648[channel].next != 65535) D_80050648[current].previous = channel;`. uopt forwards the u16 store, which is
  retail's `andi t8,v1,0xffff` before the compare; the loop's v0/v1 roles (current in v1, compare copy in v0) follow.
  `(u16)i > prio` from MusyX and declaration orders changed nothing. Typed with the TU's VoiceState (a later block
  prototypes `func_8001EF8C(VoiceState *, u8)`) and the func_8001EE9C record types; the call casts like func_8001F6EC.
- **func_8001671C / func_80016998** (MusyX dataInsertMacro / dataRemoveMacro; w20b 6420 / not attempted): MusyX's
  statement shape with `long` locals (main/pos/base/i), the sub table through the TU's packed-fields/word-union pattern
  (as func_80015720), and the search compare written `entry->id == id` / `entry->id != id` (operand order: 30->20).
  The remaining 20 is relocation naming: IDO's LFTR loop ends are `&D_8003DA28[512]` (splat: D_8003E228),
  `&D_8003E228[1]` (D_8003E230) and `&D_8003E228[-1]` (D_8003E220) -> `reloc_addrs_w21b.txt` (8 entries from
  `reloc_split derive`; `reloc_split check --file`: `all 8 reloc_addrs entries encode their ROM words`;
  `tools/resplit_target.py` -> `gate_target OK` for both). Same mechanism as the existing func_80017108 entry.
- **func_80022874** (MusyX mcmdStartSample-like; w20b 1710): the start call is two calls in an if/else on flag 0x200
  (`if (!(flags24 & 0x200)) f(index, &rec, 1); else f(index, &rec, 0);`) with the voice index computed into a local
  just before (retail's hoisted `move a2,zero`, two jals, &rec materialised after the lookup call, state in its home
  slot). w20b's function-local static record kept (`static SampleInfo_80022874 sample_80056208;`).
- **func_8001A270** (MusyX synthStartSound with StartKeymap inlined; D10 4745): `packed &= 0xFFFF00FF; packed |= prio << 8`
  as two statements puts packed in s0; the keymap packed built as `&= 255; |= prio << 8; |= id << 16`; an `int` local
  for the keymap entry id (`(kid = keymap[index].id) != 0xFFFF`) removes the `move t3,a3` copy -> true=0, but the frame
  is 8 B larger. `tools/mdlocals.py` (ECOFF .mdebug reader on a `-g3` build) shows every declared local takes a slot in
  declaration order; retail's offsets fit exactly six (4-byte X at -4, u16 id at -6, pan/index at -12/-13, priority -20,
  note -24). Every 6-variable assignment tried (36 role combinations, reuse of priority/note/pan/kid for the call result,
  priorityOffset reuse) gets the frame right but leaves 225+ of colouring.
- **func_8001E0E0** (sample volume/pan conversion): `scale = D_8002D910` at the top and the span fractions written inline
  (expression webs) give the integer code and $f2/$f14 right (2215); residual is FP colour order: retail colours scale
  last ($f20, callee-saved) and gain $f0; ours colours scale first. `noscale` (direct D_8002D910 uses) gets the two
  constants' registers ($f16/$f18) right but hoists the address instead. 120 declaration orders: no effect.
- **func_80019F48** (MusyX StartLayer): count declared first puts count/result/previous at retail's relative slots
  (-2/-8/-12; strict 2481 -> 2453); retail's locals area is 4 B larger again (spill at -60 vs -56) and the s-register
  assignment differs (i in s4, channel/set s5/s6, D_8004BEB8 s7, 416 in fp). Not pursued (time).

## Integration (coordinator; order matters)

### A. Three matches (sources/, ctx71_O2.jsonl)
1. **reloc_addrs** (before locking 1671C/16998): append the 8 entries of `cloud/work/frontier/w21b/reloc_addrs_w21b.txt`
   to `reloc_addrs.us.txt`; `python3 -m tools.conveyor.pipeline.reloc_split check` (expect all entries encode their
   ROM words); re-split (`make extract`; the two slots' operands become `%hi(D_8003DA28 + 0x800)`,
   `%lo(D_8003E228 + 0x8)`, `%hi(D_8003E228 - 0x8)` ...); ROM gate; refresh the inventory targets of func_8001671C and
   func_80016998 (as for func_8001144C in 7c49d8ff / w19b's relocname six). Emulated here: `tools/resplit_target.py`
   (targets) and `tools/resplit_asm.py` (TU proof).
2. **TU edit** (layout-neutral, proven alone): `python3 cloud/work/frontier/w21b/tools/tu_edits.py src/rom` from the repo
   root rewrites lib_16320.c in place: the packed `ResourceRange` typedef moves from the func_80017108 block to just
   before the func_8001671C slot (its first user). Must precede promote_batch (the sources' typedef line is then deduped).
3. **Locks** (7.1 toolkit; 1671C/16998 after step 1's target refresh):
```
TK71=4586d230749de1bc8a64add52ae53db0ffde18a1eee76c8a73f3791fd98e2557
for f in func_8001EF8C func_8001671C func_80016998; do
  python3 -m tools.conveyor.pipeline.lock add cloud/work/frontier/w21b/sources/$f.c:$f \
    --flags "-g0 -O2 -mips2 -G 0 -non_shared" --toolkit $TK71; done
```
4. **Context rows**: `cloud/work/frontier/w21b/ctx71_O2.jsonl` (3 rows, all ok, ido7.1). Recompute:
   `python3 cloud/work/boot_tail_promotion/context_check.py --compiler ido7.1 OUT.jsonl fn=cloud/work/frontier/w21b/sources/fn.c ...`
5. **promote_batch** (dry run first; `--prefix-dedup` required):
```
CTX=cloud/work/frontier/w21b/ctx71_O2.jsonl
python3 cloud/work/boot_tail_promotion/promote_batch.py $CTX 0x16320 0x1f5b0 --only func_8001671C,func_80016998,func_8001EF8C --prefix-dedup rom/lib_16320,rom/lib_1f5b0 --dry-run
python3 cloud/work/boot_tail_promotion/promote_batch.py $CTX 0x16320 0x1f5b0 --only func_8001671C,func_80016998,func_8001EF8C --prefix-dedup rom/lib_16320,rom/lib_1f5b0
```
6. No unit_overrides (static), no owned data (no source emits .data/.rodata/.bss), nothing superseded. Header patch: none.
   Data symbols: none renamed (the reloc entries only rename the 8 instruction operands; D_8003E220/D_8003E228/D_8003E230
   stay as symbols for other references).

### B. func_80022874 (ownbss/) - needs an owned .bss storage block in lib_22300
- `lock add ownbss/func_80022874.c:func_80022874 --toolkit $TK71` should pass (true=0). context_check refuses it (.bss).
- The static is the 28-byte sample record at **0x80056208** (D_8005620C/D_80056210/D_80056214 are its fields). It is NOT
  at a 16-aligned TU .bss start: D_80056200 (8 B) and D_80056228 belong to func_80023E9C (w21c's target, unmatched), so
  the lib_22300 .bss block is 0x80056200.. and needs func_80023E9C's statics (or a tooling block that places this one at
  +8). Proof here: `tools/linkproof.sh lib_22300 0x80021700 0x80056208 0x8002D950` links the spliced TU with its .bss at
  0x80056208 (`SUBALIGN(4)`), .rodata at 0x8002D950 (jtbl_8002D950_main): linked .text sha1 72e105bf... == baserom.

## What generalises
1. **Store-to-load forwarding as a narrowing lever (7.1):** an `andi rX,v,0xffff` before a compare of a value that is
   already a u16 register is a read-back of a u16 store just made; test the stored field instead of the variable.
2. **MusyX shapes close boot-tail functions directly** (dataInsert/RemoveMacro 6420/unattempted -> 0 in 2-3 variants).
   Check `doldecomp/ttyd libs/musyx/src/musyx/runtime/{synthdata,synthvoice,synth}.c` before decompiling a voice/data
   function; Rush's MusyX is older (packed id|prio|key arguments, no hwSetPriority) but control flow matches.
3. **LFTR end pointers carry splat labels**: a 7.1 index loop over a global array compiled to a pointer loop ends at
   `&array[N]`, which splat names as the next symbol; true=20/reloc_blind=0 with only those addends differing means
   reloc_addrs entries, not a source change.
4. **Frame-slot oracle for 7.1:** build the candidate with `-g3 -O2` and read the ECOFF .mdebug locals
   (`tools/mdlocals.py`, `tools/md.sh SRC FN`): it prints every declared local's frame offset, so a retail frame residual
   can be mapped to "how many locals of what size in which order" before guessing (every declared local takes a slot,
   block-scoped ones too; `register` changes nothing).
5. **if/else with two calls** instead of a computed argument reproduces a hoisted constant argument + two jals.

## Files
`sources/` (3), `ctx71_O2.jsonl`, `prove.log`, `reloc_addrs_w21b.txt`, `ownbss/func_80022874.c`; near misses
`func_8001A270/{best_true0,best_frame}.c`, `func_8001E0E0/best.c`, `func_80019F48/best.c`; MusyX drafts
`func_8001671C/m1.c,m2.c`, `func_80016998/m1.c,m2.c`. `tools/`: `prove.sh` (full proof), `tu_edits.py`, `mdlocals.py` +
`md.sh` (new: mdebug slot oracle), `linkproof.sh`/`ownbss_splice.py` (w20b's, parametrised by TU/addresses),
`resplit_target.py`/`resplit_asm.py` (w19b's), and w20b's `t.sh`, `vb.sh`, `sc.sh`, `dif.sh`, `check.py`, `splice.py`,
`tusplice.sh`, `triage.sh`, `obj.sh`. Variants under `func_*/v/` (git-ignored).

## Disclosures
- No devices, no unused locals, no pad arrays, no false prototypes, no implicit int in any delivered source. Type views:
  RecordFields_8003E228/RecordStorage_8003E228 (new names, TU pattern), MacroState_80022874/SampleInfo_80022874 (private).
  func_8001EF8C's last test reads the stored field (semantically identical), disclosed in its header.
- TU edit (lib_16320 typedef hoist) is declarations only, proven IDENTICAL alone.
- Permission denial (not retried): `rm -f v/*` inside func_8001EF8C was refused by the rm safety check (relative glob
  after `cd`); later rounds used new subdirectories `v/rN/` instead. No other denials.
- Builder: only `~/rush2049/scratch/frontier/w21b` (plus mktemp under the builder's /tmp by check.py/dif.sh, as w18c-w20b);
  baserom.us.z64 copied there for the linked proof; `tools/ido-static-recomp/build71/out` there is a symlink to the
  4586d230 toolkit's ido/. prove.sh temporarily rewrites the scratch copy of asm/ (restored).
- Pi: scratchpad holds fetched target objects and re-split targets (read-only DB/blob-store reads); ttyd sources fetched
  with `gh api` into the scratchpad (read as reference only).
