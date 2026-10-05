# Boot-tail promotion, wave 4 (2026-10-05)

Wave 4 covers the 20 boot-tail bodies that earlier waves left refused because no
true score 0 existed. Nineteen were "relocation symbol split" refusals.
`func_80013964` also depended on relocking the accepted `func_8001144C`. All 20
are now promoted.

## Result

| | Static functions | Static bytes |
|---|---:|---:|
| `make progress` before | 379 / 668 | 69,680 / 160,720 (43.35%) |
| `make progress` after | 399 / 668 | 73,292 / 160,720 (45.60%) |
| Promoted in wave 4 | **20** | **3,612** |
| Still refused | 0 | 0 |

Game-code coverage is reported separately and was not touched by this wave. It
moved from 796 to 799 functions during the session because of the integrator's
concurrent splices.

## The problem

splat gives every address reached by a `%hi/%lo` pair its own `D_XXXXXXXX`
symbol. Sometimes the original code reached that address as `base+offset`: an
array element, a struct field, or the end pointer of an array loop. The
reloc-aware target then relocates against the split name. IDO's object for the
byte-identical C relocates against `base`, with the offset in the instruction
field. For example, `func_80018E6C` loops `for (i = 0; i < 8; i++)` over
`D_80043EB8[8]`, so IDO forms the end pointer as `D_80043EB8+0x7FC0`, while the
target named it `D_8004BE78`. The linked words are identical. The relocation
records (symbol, in-place addend) are not, so the true score was 5 to 40.

## Choosing the fix

We considered two options:

1. **Name the instruction, not the address (chosen).** splat's
   `reloc_addrs.us.txt` overrides the relocation symbol and addend of one
   instruction by ROM offset. Only the listed `%hi/%lo` instructions change.
   The split symbols remain defined, and every other reference to them is
   untouched.
2. Make the scorer resolve symbol+addend to an absolute address. This was rejected
   for these reasons:
   - It changes the toolkit scorer on every pool node.
   - It needs an address for every candidate-side name.
   - It would let a body that names the wrong symbol at the right address score
     0. The full-ROM gate catches that only later.
   - It weakens the true-score rule globally, while the defect is local
     disassembly naming.

Removing the interior labels, or giving the bases sizes in `symbol_addrs`, would
also be wrong. Many split names are real, separately used objects that happen to
sit at the end of the array before them. For example, `D_8004BE78` is declared in
`include/sequence_context.h`. `D_80058680`, `D_8004F800` and `D_800586A0` are
similar. Only the instruction knows which object the code meant.

The scorer is unchanged and still compares symbol+addend, so the controls still fail:

| Control (pool, true score) | True | Reloc-blind |
|---|---:|---:|
| `func_80018E6C` with the wrong addend (`i < 7`) | 5 | 0 |
| `func_80018E6C` with the wrong global (`D_80043EB0`) | 20 | 0 |
| `func_8001144C` with the wrong addend (`D_800382D8[2]`) | 5 | 0 |
| `func_8001144C` with the wrong global (`D_800382E0[1]`) | 10 | 0 |
| Old accepted `func_8001144C` (split name `D_800382DC`) against the new target | 10 | — |

Sources are in `wave4_controls/` and pool output is in `wave4_controls/pool_scores.tsv`.

## Tooling (`tools/conveyor/pipeline/reloc_split.py`, tests in `tests/conveyor/test_reloc_split.py`)

- `derive FN --candidate IDO.o` pairs target and candidate relocations by offset
  and type. It proposes an entry only under all of these conditions:
  - the symbols differ;
  - the target field is zero, meaning a plain split name;
  - `address(base) + addend == address(split name)`;
  - the candidate's own `%hi/%lo` field encodes exactly that addend.

  It refuses a wrong global or a wrong addend. Against the pre-wave-4 targets, all
  four controls above are refused with a reason.
- `check` verifies every `reloc_addrs.us.txt` entry against the ROM word it names.
  For `%lo` this checks the low half. For `%hi` it checks that the word is a `lui`
  and checks the carried high half. It also refuses unresolved symbols and
  duplicate offsets. The checked-in file passes: `all 58 reloc_addrs entries
  encode their ROM words`.
- `lock verify` now treats a header beside the TU (`src/rom/rom_tu.h`) as an
  include directory, as the Makefile does with `-I src/rom`. ROM-TU locks can now
  be re-proved through the pool; before, they failed with "Cannot open
  rom_tu.h".

## Regeneration (no hand-edited asm)

- `reloc_addrs.us.txt` has 58 entries (29 `%hi/%lo` pairs) across the 20 functions
  in the table below. Each entry comes from `reloc_split derive` against the
  locked body's IDO object.
- `make extract` re-split and regenerated exactly the 19 GLOBAL_ASM slot files.
  `rush2049.us.ld`, `undefined_syms_auto.us.txt` and `undefined_funcs_auto.us.txt`
  are byte-identical. A prior idempotence check (re-split without entries)
  changed nothing.
- `make extract` does not rewrite the asm of promoted slots. Those files are
  frozen targets. `func_8001144C.s` was therefore produced by
  `splat split --disassemble-all` in a scratch copy, which writes promoted slots
  to `asm/us/matchings`. The copied file differs only on the two listed lines.
  No other promoted file changes under the entries.
- `refresh_split_targets.py` rebuilt the 20 targets through `targets.populate`, the
  `matrix extract` path. All 20 pass the reloc-aware round-trip gate.
  `func_80013964`'s target is unchanged.

| Function | Split name → base+addend | Entries |
|---|---|---:|
| `func_80010A40` | `D_8003822C` → `D_80038228+0x4` | 2 |
| `func_80017108` | `D_8003E228` → `D_8003DA28+0x800` | 2 |
| `func_800171C0` | `D_800426C8/E0/F8` → `D_800426B0+0x18/0x30/0x48` | 6 |
| `func_800175B4`, `func_80018E6C`, `func_800199F4` | `D_8004BE78` → `D_80043EB8+0x7FC0` (loop end) | 2 each |
| `func_80018B8C`, `func_80018EB4`, `func_80018F20`, `func_80018FEC`, `func_800193C8` | `D_80044E7E/7D/7A/78/7C` → `D_80043EB8+0xFC6/FC5/FC2/FC0/FC4` | 2 each |
| `func_8001C1D8` | `D_8004BEB8` → `D_8004BE98+0x20`; `D_8004F2B8` → `D_8004BEB8+0x3400`; `D_8004F440`/`D_8004F800` → `D_8004F300+0x140/0x500` | 8 |
| `func_8001E9B0` | `D_80050A60/70/80` → `D_80050A50+0x10/0x20/0x30` | 6 |
| `func_8001EE34` | `D_80050648` → `D_80050548+0x100` | 2 |
| `func_800218CC` | `D_800561E0` → `D_80056160+0x80`; `D_80056200` → `D_800561E0+0x20` | 4 |
| `func_800251A8`, `func_80025D84` | `D_80058680` → `D_80056230+0x2450` | 2 each |
| `func_80025C68` | the above plus `D_800586A0` → `D_80058698+0x8` | 4 |
| `func_80025DC0` | `D_800586A0` → `D_80058698+0x8` | 2 |
| `func_8001144C` | `D_800382DC` → `D_800382D8+0x4` | 2 |

## Proofs and promotion

All 19 original `cloud/matches/boot_tail` bodies scored **true 0** through the pool
against the regenerated targets. `relock_wave4_results.tsv` has the first 19
rows.

**`func_8001144C` / `func_80013964`.** Production adopted the audio packet's
two-cell table `unsigned short *D_800382D8[2]`, applied with
`audio_record_contracts/verify.py:adapted_cleanup`. The cleanup now frees
`D_800382D8[0]` and `[1]`. The standalone adapted copy scores true 0, and the
full-ROM gate passed. The production lock was re-pinned as `rom-sha1` and
re-verifies at score 0 through the pool. `func_80013964` then locked from the
packet source at true 0 and was promoted.

**Promotion runs** (`promote_batch.py`, full gate each: sync, make, `make test`,
`ROM matches!`, fresh objects):

| Commit | What |
|---|---|
| `7c49d8ff` | `reloc_addrs` naming, `reloc_split` tool and tests, regenerated asm, 19 true-0 locks, `func_8001144C` relock. ROM gate passed on the regenerated asm and on the relocked TU. |
| `206a6235` | **Promote 3**: `func_80013964`, `func_80017108`, `func_800218CC` (408 B), from `context_wave4.jsonl`. |
| `f7362923` | 10 adapted bodies (`wave4_sources/`), spelling only against declarations already visible at their slots. All true 0 (`context_wave4_adapted.jsonl`). |
| `5fcc92a7` | **Promote 10** (1,744 B): lib_17dc0 ×7, lib_25bb0 ×2, lib_11640 `func_80010A40`. |
| `62d984bb` | Declaration-only TU changes (ROM gate passed) plus 6 adapted bodies at true 0 (`context_wave4_hoisted.jsonl`). |
| `162bd57a` | **Promote 6** (1,020 B): `func_80018B8C`, `func_80018EB4`, `func_8001E9B0`, `func_8001EE34`, `func_800251A8`, `func_80025C68`. |
| `9b7d51a0` | `VoiceState_80019C8C` field naming plus adapted `func_8001C1D8` at true 0 (`context_wave4_voice.jsonl`). |
| `3d8e760b` | **Promote 1**: `func_8001C1D8` (440 B). |

**Why 17 needed adaptation.** The first run refused 17 bodies for shared
declarations in their ROM TU, the same class as wave 2. The errors were
diagnosed by compiling each splice on watchman2:

- The body's own view conflicted with the TU's canonical one (`sequence_context.h`
  `D_80043EB8`/`D_80043EB0`/`D_8004BE80`, `StreamState_80025264`,
  `SequenceNode`, `D_800381F8`, `VoiceState_80019C8C`).
- The canonical declaration came after the slot.

Those 17 refusal rows had only log-tail detail and were discarded, as in wave
3. Every adapted copy keeps the code and changes only its spelling. Each was
re-proved at true 0 and context-checked.

The production declaration changes, each with no body or layout change and each
gated by the full ROM:

- `src/rom/lib_1f5b0.c`: the packed `SequenceNode` and `ChannelLink_8001EE9C`
  typedefs are hoisted above the first slot, with their text unchanged.
- `src/rom/lib_25bb0.c`: `StreamState_80025264` is hoisted. The `allocate` hook
  at +24 is named in `ServiceHooks_8002574C`; `release` stays at +28.
- `include/sequence_context.h`: `disabledFC5` and `valueFC6` are named inside the
  former `unknownFC5[0x1B]`.
- `src/rom/lib_1a660.c`: the fields `func_8001C1D8` initializes are named in
  `VoiceState_80019C8C`. Existing names, offsets and the 0x1A0 size are unchanged
  (host layout check). The voice-list layout proofs still hold.

## Checks

- `python3 -m tools.conveyor.pipeline.lock check`: all 402 locks intact at every
  commit, via the pre-commit hook.
- Target scope: the naming fix changed exactly 20 target objects. Of the existing
  locks, only `func_8001144C`'s target changed, and it was relocked.
- `lock verify` over **all 402 locks** through the pool after the last promotion,
  using the include fix: **402 / 402 score 0**. That is 399 `rom-sha1` ROM-TU
  bodies and 3 `score0` candidates (`wave4_lock_verify.jsonl`).
- Side effect, recorded for transparency: the first target refresh attempt ran
  `targets.populate` over the whole `work/**` inventory (246 static targets). This
  is the `matrix extract` code path. Inventory targets whose DB object was stale
  `raw_word` became `reloc_aware` from the checked-in asm, and their matrix
  evidence was superseded. A second run is idempotent: 224 `reloc_aware`, 22
  raw-word fallbacks. No source, asm or lock changed. Every lock on those
  targets is covered by the all-lock re-verification above.
- The promotion gate syncs the whole Pi working tree to the builder, including
  the integrator's uncommitted game-code files at the time. Every gate still
  ended in `ROM matches!`. No game-code paths were staged in any wave-4 commit.
- `pytest tests/conveyor tests/cloud -q -m "not node_required"` on a clean
  worktree of the final commit: see the final report. In the shared Pi tree,
  `test_frontier::test_real_detectors_flag_no_standalone_lock` and
  `test_name_lookup_caller_contract::test_native_callers_keep_the_real_fifth_word`
  fail only because of other agents' uncommitted game-code work
  (`blob_matched.lock.json`, `cloud/work/frontier/**`). Both pass at the wave-4
  commits.
