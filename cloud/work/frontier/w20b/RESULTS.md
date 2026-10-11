# w20b - static boot tail under IDO 7.1: w19b near-miss list + w19a "not attempted" D10 list (wave 20)

Read-only except this lane dir and builder scratch `~/rush2049/scratch/frontier/w20b`. Nothing locked, promoted,
spliced into src/, or committed; no edits to src/, asm/, include/, tools/, locks, rom_owned_data.json. Static
numbers only. Toolkit 4586d230 (the 7.1 bundle `lock add --toolkit` uses), flags `-g0 -O2 -mips2 -G 0 -non_shared`,
targets = reloc-aware inventory objects (w17k fetch_targets.py, fetched this session).

## Headline

**8 functions, 2,184 B** production-ready in `sources/`, every one `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr
identical`, all spliced together (promote_batch `--prefix-dedup` replica) into their four TUs build IDENTICAL to the
current TUs (`prove.log`). One layout-neutral TU edit (lib_15a10, one line), proven IDENTICAL alone.
**Plus 3 functions, 392 B, code-identical with function-local statics** (`ownbss/`, true=0, the only objdump rows
that differ are `.bss+N` vs the splat labels): the spliced lib_16320 linked with its .bss at 0x80042670 equals the
retail ROM bytes 0x16320..0x17DC0 (`ownbss/linkproof.log`); needs an owned .bss storage block (tooling).

| function | B | TU | state | LOCK (check.py = lock add verify_entry) | whole TU |
|---|---:|---|---|---|---|
| func_800177EC | 56 | lib_17dc0 | MATCH | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (16 rows)` | IDENTICAL (2598) |
| func_80017824 | 56 | lib_17dc0 | MATCH | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (16 rows)` | IDENTICAL |
| func_800154A4 | 208 | lib_15a10 | MATCH (+TU edit) | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (63 rows)` | IDENTICAL (578) |
| func_8001536C | 312 | lib_15a10 | MATCH (+TU edit, C) | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (91 rows)` | IDENTICAL |
| func_800230B0 | 224 | lib_22300 | MATCH (2 unused locals = frame) | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (56 rows)` | IDENTICAL (3382) |
| func_8001DC08 | 472 | lib_1cf90 | MATCH (C: holder) | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (130 rows)` | IDENTICAL (1876) |
| func_8001DA74 | 404 | lib_1cf90 | MATCH | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (116 rows)` | IDENTICAL |
| func_80022160 | 452 | lib_22300 | MATCH | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (117 rows)` | IDENTICAL |
| func_80016E68 | 120 | lib_16320 | code identical, owned .bss | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr DIFFERS(11 rows) (44 rows)` | linked == ROM |
| func_80016EE0 | 120 | lib_16320 | same (twin, unassigned, needed for the block) | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr DIFFERS(11 rows) (44 rows)` | linked == ROM |
| func_80016F80 | 152 | lib_16320 | same | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr DIFFERS(11 rows) (52 rows)` | linked == ROM |
| func_8001EF8C | 432 | lib_1f5b0 | near miss | `func_8001EF8C/best.c` true=310 (D10 1710) | |
| func_80022874 | 460 | lib_22300 | near miss, static-record lead | `func_80022874/best.c` true=1710 (D10 3055) | |
| func_8001671C | 636 | lib_16320 | not closed | `func_8001671C/best.c` true=6420 (D10 6700) | |
| func_80019F48 | 808 | lib_1a660 | not closed | D10 true=2235 (`func_80019F48/best.c` = D10) | |
| func_80016998, 8001E0E0, 8001A270 | 608/864/872 | | not attempted (time) | triage below | |

The `TU: COMPILE FAIL ... redeclaration` rows in `prove.log` are check.py appending the ctx preamble without
promote_batch's prefix dedup (same as w17g/w18c/w19a/w19b); the spliced TUs (`tools/splice.py`, prefix dedup) are the
proof:
```
== TU edits alone
lib_15a10: IDENTICAL (578 lines: .text words+relocs, data sections)
== all sources spliced (into the edited TUs)
lib_15a10: IDENTICAL (578 lines: .text words+relocs, data sections)
lib_17dc0: IDENTICAL (2598 lines: .text words+relocs, data sections)
lib_1cf90: IDENTICAL (1876 lines: .text words+relocs, data sections)
lib_22300: IDENTICAL (3382 lines: .text words+relocs, data sections)
```

## Levers (each in its file header)

- **func_800177EC / func_80017824** (w18c/w19b: true=120, "scheduling only", ~40 forms tried): LINE LAYOUT. w18c's
  h5.c wrote the body on the `{` line; with the call on its own line as1 gives retail's order. The function's entry
  code (u8 parameter narrowing) carries the `{` line; a statement sharing that line is scheduled with it.
- **func_800154A4 / func_8001536C** (50/60): each id-list address is a block-scoped running pointer
  `{ u8 *p = (u8 *)group; p += group->offsets[i]; p += 8; f((u16 *)p); }`. 7.1 keeps the base-first operand order
  (`addu a0,s0,tN`) only for `p += x`; every one-expression spelling canonicalises load-first (probes `probe/v/ord`:
  even a parameter base goes second in `(char *)p + p->off`); `p + 8` as the argument colours p (true=90); a
  function-level p goes to s1 (400). 1536C: a local cursor `u8 *group` assigned inside the guard, `group->id == id`,
  and (device C, disclosed) the chain-end test through a `u32` view (`*(u32 *)group != 0xFFFFFFFF`) while the advance
  reads the int field: one type lets uopt CSE the size load (retail reloads it; true=630).
- **func_800230B0** (frame 40 vs 32): (1) `offset = word[1] & 0xFF` before the start-pan store; (2) UNUSED LOCALS
  `u32 unused0, unused1;` declared FIRST = the exact frame residual (retail 40, without 32, 2 slots). Under 7.1, locals
  declared before the used ones take slots, declared after them they do not (w19b's probes declared them after). A
  used local in their place (`delta`) flips the target sum's operand order (true=10).
- **func_8001DC08** (140): `group = D_8004FDA0; lower = D_8002D908; if (lower) {} do {` on ONE line: line layout gives
  as1 retail's preheader schedule (true=20, only f20/f22 swapped) and the compiled-out holder `if (lower) {}` (device,
  disclosed) gives `lower` the colour tie (f20). Types: the TU's EmitGroup/LargeNode/SmallNode + StateNode.
- **func_8001DA74** (D10 4290): the locked twin func_8001D944's style (indexed globals, `long i`), the head test reads
  `D_8004FDA0[i].large` again (a head local swaps a1/a2), and LINE LAYOUT of the six node stores (2 on one line, 4 on
  the next) places the count increment's `sb` as retail (one per line: true=60).
- **func_80022160** (D10 1130): macro id as `(u16)(word[0] >> 16) << 16` (the u16 conversion is the extra ugen temp in
  retail's ring; without it every temp is one register early, true=320), D_8004BEB8 indices `x & 255`, decl order
  key/macro/child.
- **ownbss trio** (w18c/w19b best 225/245): the search key and result are FUNCTION-LOCAL STATICS (MusyX snd_data.c
  dataGet* shape: `static KEY key; static KEY *result; key.id = id; if ((result = bsearch(&key, ...)) != NULL)`;
  identification is a hypothesis). uopt treats the address-not-taken static `result` unlike an extern global:
  retail's store/re-read (E68/EE0) and v1-held result stored on both exits (F80) both follow. The three functions'
  statics are lib_16320's whole .bss, 0x30 B, laid out by IDO exactly as retail (0x80042670 key/+4 id, +8 result;
  +0x10/+0x18; +0x20/+0x2C). Statics carry the table address in their names (`key_80038610`, ...) because the ROM-TU
  asm-processor (`--convert-statics local`) rejects two function statics with one name ("symbol key defined twice").

## Integration (coordinator; order matters)

### A. Eight matches (sources/, ctx71_O2.jsonl)
1. **TU edit** (layout-neutral; `python3 cloud/work/frontier/w20b/tools/tu_edits.py src/rom` from the repo root
   rewrites lib_15a10 in place): func_80014E10's block `extern unsigned short D_80038390;` -> `extern short
   D_80038390;` (the group-stack depth is signed: retail lh + slti/sra; 14E10 only stores 0, `sh` either way).
   `prove.log`: `lib_15a10: IDENTICAL (578 lines)` with the edit alone. Must precede promote_batch (the two sources'
   `extern short D_80038390;` is dropped by the dedup only when the TU already says it).
2. **Locks** (7.1 toolkit):
```
TK71=4586d230749de1bc8a64add52ae53db0ffde18a1eee76c8a73f3791fd98e2557
for f in $(ls cloud/work/frontier/w20b/sources | sed 's/\.c$//'); do
  python3 -m tools.conveyor.pipeline.lock add cloud/work/frontier/w20b/sources/$f.c:$f \
    --flags "-g0 -O2 -mips2 -G 0 -non_shared" --toolkit $TK71; done
```
3. **Context rows**: `cloud/work/frontier/w20b/ctx71_O2.jsonl` (8 rows, all `ok`, `"compiler": "ido7.1"`, computed on
   the builder against master 456d1be0, after w20a's lib_17dc0 promotion; prove.log re-run there). Recompute: `python3 cloud/work/boot_tail_promotion/context_check.py
   --compiler ido7.1 OUT.jsonl fn=cloud/work/frontier/w20b/sources/fn.c ...`
4. **promote_batch** (dry run first; `--prefix-dedup` required):
```
SEGS="0x15a10 0x17dc0 0x1cf90 0x22300"
ONLY=$(ls cloud/work/frontier/w20b/sources | sed 's/\.c$//' | paste -sd,)
DEDUP=rom/lib_15a10,rom/lib_17dc0,rom/lib_1cf90,rom/lib_22300
python3 cloud/work/boot_tail_promotion/promote_batch.py cloud/work/frontier/w20b/ctx71_O2.jsonl $SEGS --only $ONLY --prefix-dedup $DEDUP --dry-run
python3 cloud/work/boot_tail_promotion/promote_batch.py cloud/work/frontier/w20b/ctx71_O2.jsonl $SEGS --only $ONLY --prefix-dedup $DEDUP
```
5. No unit_overrides (static), no data-symbol changes, no owned data (no source emits .data/.rodata/.bss; spliced TUs'
   data sections identical). Nothing superseded (none was locked). Header patch: none (func_8001DA74/8001DC08 include
   `boot_tail_sample_contract.h` as the TU already does).

### B. ownbss trio (ownbss/, lib_16320) - needs tooling (w20c / coordinator)
- `lock add ownbss/<fn>.c:<fn> --toolkit $TK71` should pass (verify_entry = scoring.score: true=0). context_check
  refuses them (`object has data/rodata (.bss:16) outside an owned slot`), so the splice is manual
  (`tools/ownbss_splice.py` = prefix-dedup replica that takes the preamble from the source and drops basic typedefs).
- The TU's .bss after the splice is exactly 0x30 B (align 16) and must land at **0x80042670** (16-aligned, size 16x):
  a storage block in `existing_tu` mode for `src/rom/lib_16320.c` (IDO 7.1, `-g0 -O2 -mips2 -G 0 -non_shared`),
  members = the three, no data slot. owned_existing_storage.py only knows two reviewed profiles (and a data slot), so
  this needs a reviewed profile/tool change; not attempted here (tools/ is off-limits).
- D_80042670..D_8004269C are absolute symbols in `undefined_syms_auto.us.txt`; only these three functions reference
  them (grep of asm/ and src/), so they can stay or become block aliases.
- Proof replayable on the builder: `sh lane/tools/prove.sh` (last section) -> `ownbss/linkproof.log`:
  linked spliced .text sha1 b777c6b2... == linked original == baserom.us.z64[0x16320, +0x1AA0).
- Neighbours: func_80016C20 / func_80016CF0 (statics at 0x80042630..0x8004266B) and func_80017040 (0x800426A0) look like
  the same dataGet* family; if they are matched later with statics, the block must grow to cover them (re-derive).

## Near misses / not closed (stop rule or time)

- **func_8001EF8C** true=310 (from D10 1710): `& 255` index (copy-then-narrow), `state->channel2E == channel`,
  `if ((link->next = D_80050548[channel]) != 255)`, for-loop with `if (channel < current) break;`, declaration order
  current/previous/link/index (index slot 39). Residual: in the sorted insert loop retail keeps `current` in v1 and an
  int copy for the compare in v0 (ours swapped) and the 0xFFFF constant in a0 (ours v0); `u32`/`int current` is far
  worse (2300+). Next: trace which web owns v0 (copy vs variable) - likely the loop rotation's copy web numbering.
- **func_80022874** true=1710 (D10 3055): the sample record D_80056208 (+0 info, +4 address, +8 offset = label
  D_80056210, +12 length = D_80056214) is a FUNCTION-LOCAL STATIC: the whole switch body then matches modulo .bss
  offsets (retail addresses each field `lui at; sw ..(at)` and never CSEs the base with the call argument; as an
  extern struct or separate externs it does). Remaining: &record materialised after the first call (retail) vs in the
  switch head, and func_800146B4's third argument (retail hoists `move a2,zero` and has two calls). Packaging would
  also need lib_22300's .bss: func_80023E9C (unmatched, no D10) uses D_80056200 / D_80056228 around it.
- **func_8001671C** true=6420: needs the TU's aligned union view for the entry copy (retail lw/sw), D_8003DA20 kept in a
  register across the insert, and the target names two compiler-made addresses by labels (scan end = D_8003E228 =
  &D_8003DA28[512]; copy end = D_8003E230 = &D_8003E228[1] + position): label views or reloc_addrs entries needed even
  when the code is right. A `total` local made it worse (10755).
- **func_80019F48**: D10 2235; frame 176 vs 168, previous/result live in memory (stack-homed), i in s4; declaration
  order has no effect (24 orders identical). Not pursued further.
- Not attempted (triage only, `tools/triage.sh` on the D10 sources):
  `func_80016998 verdict=structure-mismatch aligned_total=52 words=111 opcodes=75`,
  `func_8001E0E0 verdict=structure-mismatch aligned_total=109 words=202 opcodes=184`,
  `func_8001A270 verdict=structure-mismatch aligned_total=119 words=175 opcodes=132`.

## What generalises

1. **IDO 7.1 as1 scheduling depends on source line layout.** Three closures here were line-only or line-plus:
   177EC/17824 (call off the `{` line), 8001DC08 (preheader assignments + `do {` on one line), 8001DA74 (store
   grouping). For a "scheduling only" 7.1 residual try line layouts before C variants (cheap: vbatch of 5-10).
2. **Function-local statics** explain store/re-read and register-held-global patterns that extern globals cannot
   reproduce (E68/EE0/F80, lead for 22874). Check whether the D_ symbols are referenced by one function only; IDO 7.1
   lays out statics per function in declaration order, 16-aligned TU .bss. asm-processor needs unique static names.
3. **`p += x` keeps operand order** where every one-expression form is canonicalised load-first (block-scoped running
   pointer, no frame cost).
4. **7.1 frame: locals declared first take slots even if unused**; declared after the used ones they do not.
5. A `(u16)` conversion inside an expression can be the missing ugen ring temp (temps "one register early").

## Files

`sources/` (8), `ctx71_O2.jsonl`, `prove.log`; `ownbss/` (3 + `linkproof.log`); `func_*/best.c` (near misses);
`tools/`: `prove.sh` (ctx rows + LOCK/TU + TU edit alone + all spliced + ownbss linked proof), `tu_edits.py`,
`ownbss_splice.py`, `linkproof.sh`, `S.sh` (7.1 `cc -S` listing), `asm71.sh` (reassemble an edited listing; NOTE the
.s path does not reproduce the binasm path exactly), `t.sh`/`vb.sh` (Pi: push + score one / a directory), and the
w18c/w19a helpers (`check.py`, `sc.sh`, `ct.sh`, `dif.sh`, `rdiff.sh`, `splice.py`, `tusplice.sh`, `obj.sh`,
`diag.sh`, `triage.sh`, `d10.sh`). Variants under `func_*/v/` and `probe/v/` (git-ignored). Builder scratch:
`~/rush2049/scratch/frontier/w20b` (base copy + rsync of src/blob, src/rom, include, tools/{cloud,conveyor,
asm-processor,workbench.py}, third_party/n64-decomp-workbench, asm/us/{blob,boot_tail,nonmatchings/rom}, locks,
boot_tail_promotion, boot_tail D10 sources, symbol files, baserom.us.z64 for the linked proof; `tgt/` = inventory
targets; `tools/ido-static-recomp/build71/out` -> the 4586d230 toolkit's ido/).

## Disclosures

- Devices: 8001536C u32-view end test (C); 8001DC08 `if (lower) {}` holder; 800230B0 two unused locals (frame residual,
  arithmetic above); line layouts (177EC/17824, DC08, DA74) are formatting, disclosed in headers. Casts/type views:
  1536C `u8 *` cursor with casts, 22160 private packed view of D_8004BEB8, 230B0 private VoiceState view, ownbss
  comparator casts. No pad arrays, no false prototypes, no implicit int.
- func_80016EE0 is not on my list: taken because the static block is only contiguous with it (CLAIMS.md: "other
  remaining boot tail func_*" open). CLAIMS.md not edited.
- Permission denial (not retried): `cd func_800230B0; rm -f v2/*` was refused by the rm safety check; the old variant
  files were left and later moved under `func_800230B0/v/` (mv, no deletion). No other denials.
- Builder: check.py/dif.sh/rdiff.sh use mktemp under the builder's /tmp (as w18c/w19a/w19b). baserom.us.z64 copied into
  my builder scratch for the linked proof. Nothing outside `~/rush2049/scratch/frontier/w20b` otherwise.
- Pi: scratchpad holds fetched target objects (read-only DB/blob-store reads).
