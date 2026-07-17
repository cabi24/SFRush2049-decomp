# T019 stall report: SC-001 unreachable via `include/game_types.h` alone

**Status**: `include/game_types.h` populated per T018 (bounded by the T011
blocker list plus data-model.md's committed content plan); T019's iteration
loop run to convergence. SC-001 (≥8/10 cluster seeds compiling, including
≥4 of the 7 60+-instruction members) is **not reachable** through
`game_types.h` content alone. Two independent, evidence-backed root causes
found below — neither is a header-content problem, so no further header
iteration will close the gap. Per the task's own fallback instruction,
stopping here rather than forcing it.

## Result after full T018/T019 iteration

`python3 -m tools.conveyor.pipeline.autodecomp clusters --population
extracted --targets @tools/conveyor/clusters/game_loop.txt`:

- **Before** (empty `game_types.h`, T011 baseline):
  `compiled=0 blocked=9 decompiler_failure=1`, 90 distinct blocker symbols
  (t011-blockers.md).
- **After** (populated `game_types.h`, ~75 extern function prototypes + the
  9-entry symbol table's types + `Track_Data` + `GState` + a fully-typed
  `SoundClearRecord` for `sound_control`'s own parameter):
  `compiled=0 blocked=9 decompiler_failure=1`, **12** distinct blocker
  symbols remaining (`build/m2c_histogram.md`, scoped run).

Every "?-typed forward declaration" blocker (the ~75 `? funcname(...)`
compile errors from missing prototypes) is gone. `sound_control` itself now
has zero *prototype*-class errors (its own struct-typed parameter fixed all
13 of its member-access errors) — the one target where a header-only fix
was structurally possible went from ~13 errors to 0 self-contained errors,
but it and every other cluster member still carry at least one error from
root cause 1 below.

`compiled` stayed at 0/10 because **every remaining blocker, without
exception, traces to one of the two root causes below** — confirmed by
inspecting each of the 12 remaining blocker source lines in the current
`build/m2c_histogram.md`: all are either a bare `*(void *)0xADDR`/
`(void *)0xADDR->unkN` raw-address expression, or a call whose *argument*
expression contains one.

## Root cause 1: `disasm.py`'s symbolizer does not track register-formed
pointers (`lui`+`addiu`), only `lui`+`imm(reg)`

`tools/conveyor/pipeline/disasm.py:normalize_objdump` (`_LOW_RE` at
disasm.py:42, matched against `pending_lui` at disasm.py:106) only
recognizes a global address when the **very next relevant instruction**
using that register has the `imm(reg)` operand shape (a direct `lw`/`sw`/
`lb`/`sb`/etc. with the register as a memory-access base). It does not
special-case `addiu $r,$r,imm` — the three-operand *register-forms-a-
pointer* idiom the compiler used whenever a global is dereferenced through
a loop, an offset chain, or reused across multiple accesses. `_LOW_RE`'s
trailing-`(reg)` anchor never matches `addiu`'s operand text (`$r,$r,imm`,
no parens), so when that happens the `pending_lui` entry set by the `lui`
is silently never consumed *and never invalidated* — it just sits there
until overwritten by a later `lui` to the same register.

This is not a hypothetical: it is why **4 of the 9 committed game-symbol-
table entries never actually appear symbolized** in the derived asm for the
cluster, despite being correctly declared in `GAME_SYMBOLS`
(disasm.py:18-28) and correctly typed in `game_types.h`:

| Symbol | Where it should appear | Confirmed instead |
|---|---|---|
| `frame_counter` (0x80142AFC) | `game_loop.s` | `lui $t7,0x8014` / `addiu $t7,$t7,11004` (build/m2c_asm/game_loop.s:37-39) — raw `0x80142AFC` in the seed, `%hi(frame_counter)` never emitted |
| `msgq_ptr` (0x801497C8) | `attract_or_transition.s`, `Input_ProcessGameplayPad.s` | same `lui`+`addiu` shape (e.g. `Input_ProcessGameplayPad.s:15-17`); *does* symbolize correctly in `game_loop.s`/`game_mode_handler.s`, where the access is a direct `lui`+`lw` — proving the gap is the `addiu` idiom specifically, not the symbol table |
| `input_rec0`/`input_rec1` (0x8014A118/0x8014A164) | `process_inputs.s` | `lui $v1,0x8015` / `addiu $v1,$v1,-24296` (process_inputs.s:11,39) |
| `player_array` (0x80152818) | `countdown.s` | same shape wherever the cluster reaches it |

`grep -o '%hi([a-zA-Z_0-9]*)' build/m2c_asm/*.s` across all 10 cluster
targets confirms only `gstate`, `game_state_flags`, `state_word_a`,
`state_word_b` ever actually resolve (all four reached via a bare
`lui`+`lw`/`sb` pair with no intervening `addiu`).

Two further, more severe cases of the same root idiom exist for addresses
**not even in the 9-entry table** (so no header fix, even a perfect one,
could reach them without also growing `GAME_SYMBOLS`, which is
`disasm.py`/T007 territory, out of this task's stated scope):
`0x80114658` (`func_800a7480`'s struct base, playgame_state_change),
`0x8017A4E4`/`0x8017A4E0` (countdown/countdown_handler), `0x80140BF0`-family
(Input_ProcessGameplayPad), and `game_loop`'s own
`(void *)0x8002E8E8->unk27C` — traced instruction-by-instruction in
`build/m2c_asm/game_loop.s:56-60` (`lui t6,0x8003` / `addiu
t6,t6,-5912` forms `0x8002E8E8`, then `lw t7,636(t6)` reaches
`0x8002EB64`, a real, recorded field for `game_loop`
(research/cluster-data-refs.md) that the symbolizer never sees).

**Why this can't be worked around in `game_types.h`**: `extern` and
`typedef` declarations bind to *symbol names*. `*(void *)0x80142AFC` in the
generated seed is a numeric literal, not an identifier — there is no name
for a header declaration to attach a type to. The fix is mechanical and
lives entirely in `normalize_objdump`'s `pending_lui` state machine (track
`addiu $r,$r,imm` as updating/forming the pointer value for `$r`, and only
drop `pending_lui[$r]` on a *conflicting* write), not in this file.

This is the same failure class the A2 review's watch-item warned about
(mismatched `%hi`/`%lo` pairing) but a different manifestation: I found **no
case of a wrong symbol being substituted** (the `gstate`/`game_state_flags`
pair the review flagged is correctly paired everywhere it's used in this
cluster — verified by grepping every `%hi(gstate)`/`%hi(game_state_flags)`
occurrence in `game_loop.s` against its paired `%lo`). What actually happens
is *silent non-symbolization*: `pending_lui` for the register is simply
abandoned when an intervening `addiu` reassigns it, so the low-access either
matches nothing (common case, safe but incomplete — what's documented
above) or, in principle, could match a stale prior `lui` and compute a
*wrong* address that happens to collide with an unrelated symbol (I checked
for this specifically across the cluster and found no occurrence, but the
mechanism exists and is worth a test case if `disasm.py` is revisited).

## Root cause 2: SC-005's regression test cannot pass for any non-empty
`game_types.h`, by construction

`tests/conveyor/test_autodecomp_population.py::
test_empty_game_types_keeps_known_good_static_seed_byte_identical` asserts
the *exact source text* of a known-good static seed (`osViGetFramebuffer`)
is byte-identical before/after `game_types.h` is wired into the context
chain. `autodecomp.m2c_seed` (autodecomp.py:129-131) concatenates the
**entire** preprocessed context — `types.h` + all of `PR/*.h` +
`m2c_types.h` + `game_types.h` — into every seed's prelude verbatim (minus
only the target's own prototype line); there is no dead-code stripping by
symbol relevance. Confirmed directly:

```
$ printf '/* trivial */\n' > include/game_types.h   # comment only
$ pytest ...test_empty_game_types_keeps...           # PASSES (cpp -P strips comments -> no token diff)
$ printf '/* trivial */\nextern int probe;\n' > include/game_types.h
$ pytest ...test_empty_game_types_keeps...            # FAILS — one harmless extern is enough
```

So the test passes if and only if `game_types.h` contributes zero
preprocessed tokens — i.e., iff it stays empty (comments-only). T018's task
description ("Populate `include/game_types.h`") and this test's continued
existence as a gate are mutually exclusive as currently implemented. This
isn't a content problem I can curate around; it's structural to how
`_context()`/`m2c_seed()` build the prelude.

**What SC-005/FR-003 actually cares about** — that static-population
seeding doesn't *break* — does hold. I compiled `osViGetFramebuffer`'s seed
both ways (empty vs. populated `game_types.h`, real `mips-linux-gnu-gcc -c
-mips2 -mabi=32 -G0 -O2 -std=gnu89`) and `objdump -d`'d both: the emitted
instructions for `osViGetFramebuffer` itself are **byte-for-byte
identical** (only the unused, added declarations differ, which emit no
code). So the functional guarantee FR-003 asks for is intact; the specific
literal-text assertion the test encodes is a stricter, incidental property
that inherently cannot survive T018 landing anything.

I did not modify the test — that decision (relax it to a compiled-output
comparison, or scope it to "wiring didn't change" and accept it's now
superseded) belongs to whoever owns the SC-005 contract, not to a
header-content task.

## What this means for T014

T014's SC-001 acceptance run is blocked on the same two items, not on
`game_types.h` content: even a hypothetically perfect header cannot raise
`compiled` above what root cause 1 allows, since the remaining errors are
address literals with no attachable symbol. Unblocking SC-001 needs, in
priority order:

1. Fix `disasm.py`'s `pending_lui` tracking to follow `addiu $r,$r,imm`
   (root cause 1) — this alone would let the *already-written*
   `frame_counter`/`msgq_ptr`/`input_rec0`/`input_rec1`/`player_array`
   declarations in `game_types.h` take effect with no further header
   changes, likely fixing `process_inputs`, `game_loop`, and
   `game_mode_handler` outright and shrinking the rest.
2. Extend `GAME_SYMBOLS` with the handful of additional addresses named
   above (`0x80114658`, `0x8017A4E0`/`0x8017A4E4`, `0x80140BF0`,
   `0x8002EB64`, `0x8014A108`/`0x8014A110`) with their own evidence, then
   type them in `game_types.h`.
3. Resolve or accept the SC-005 test's incompatibility with a non-empty
   header (root cause 2) — needed regardless of (1)/(2), since *any*
   `game_types.h` content trips it today.

`game_types.h` as currently populated is ready to benefit from (1)/(2)
without further edits — every symbol-table type it declares is already
correct, just not yet reachable by the derived asm.

# 2026-07-17 T019 residue stop (header-only constraint)

The requested residue pass began from `compiled=0 blocked=9
decompiler_failure=1` and made four header/probe iterations, running
`test_game_types_keeps_known_good_static_function_body_byte_identical` after
each change. Extending `InputRecord` through the surveyed byte at `unk4D` did
not change the count. Typing the record returned by `func_800b3704` from the
`sound_control` seed made `sound_control` compile, producing the final scoped
probe `compiled=1 blocked=8 decompiler_failure=1` (the decompiler failure is
the accepted `RaceStateMachine_Update` jump table).

The residue then hit errors outside the four authorized mechanical classes:

- `game_mode_handler`: `*(void *)0x80035471 = 1` (`invalid use of void
  expression` on a literal dereference, not a call resolving to an extern
  prototype).
- `game_loop`: `game_loop_tick.unk-1718` (`m2c` emitted a subtraction-like,
  syntactically invalid member suffix rather than an `unkN` byte offset).
- `process_inputs`: `temp_v0->unk80156978` where `temp_v0` is the local scalar
  expression `temp_a2 * 4`; it is not a named scalar extern that can be
  retyped in `game_types.h`.
- `Input_ProcessGameplayPad`: `((? (*)(void *)) temp_v1)(...)`, plus literal
  `void *` dereferences such as `(*(void *)0x801406B8)->queue`.
- `playgame_state_change`, `countdown`, and `countdown_handler` likewise retain
  literal-address `void` operations (for example `*(void *)0x80156994 != 0`,
  `(void *)0x8014A250->unk7C6`, and `*(void *)0x801613B0 = var_v1`).

These expressions have no header-visible identifier to attach a struct or
scalar declaration to. Fixing them would require seed/disassembly/pipeline
rewriting, which this pass explicitly forbids. Work therefore stopped at the
iteration limit rather than introducing declarations that do not describe
the evidence.

# 2026-07-17 second-gate stop (complete survey table)

The second §5 amendment was implemented and the scoped probe was re-derived
from a cleared cluster cache. Per-consumer LUI rebinding removed the observed
`game_loop_tick.unk-1718` failure, complete survey symbol coverage removed the
direct literal-global failures, and the unknown function-pointer cast cleanup
landed. The best scoped result was:

`compiled=4 blocked=5 decompiler_failure=1 no_disasm=0 extent_conflict=0`

Compiled: `sound_control`, `game_loop`, `game_mode_handler`, and
`countdown_handler`. `RaceStateMachine_Update` remains the accepted m2c
jump-table failure. The five remaining compile failures contain classes not
authorized by the amendment:

- `Input_ProcessGameplayPad`: unsurveyed numeric bases `0x80120000` and
  `0x80138670`, plus the unrelated `func_8008a148` call expression.
- `attract_or_transition`: inferred `temp_t6->unk9CC0` member access.
- `countdown`: malformed inferred chains involving `active_player_count`,
  `gameplay_mode`, and `player_array` despite correctly matched `%hi/%lo`
  pairs in the derived assembly.
- `playgame_state_change`: unrelated `func_800a3424((*D_8014A160)->unk8, ...)`
  pointee inference.
- `process_inputs`: inferred local-member chains `unk04` through `unk14`.

Because the probe remained below 8/10 with unsurveyed-address and inferred
local/call error classes, the contract stop rule fired. No full histogram or
watchman scoring run was attempted for this second-gate pass.

# 2026-07-17 third-gate stop (four of five closed; two structural blockers)

Starting from the second-gate's `compiled=4 blocked=5`, this pass closed
**three** of the five remaining functions purely through `game_types.h` +
`GAME_SYMBOLS` evidence, bringing the scoped probe to:

`compiled=7 blocked=2 decompiler_failure=1 no_disasm=0 extent_conflict=0`

Compiled (new this pass): `process_inputs`, `attract_or_transition`,
`playgame_state_change`. Root causes and fixes, each confirmed by a clean
`mips-linux-gnu-gcc -fsyntax-only` pass on the regenerated seed:

- **`process_inputs`**: the "inferred local-member chains `unk04`..`unk14`"
  from the second gate were four register-formed array bases the survey
  missed — `lui $t2..t5,0x8015/0x8014` + `addiu ...,27000/27032/14848/26968`
  (build/m2c_asm/process_inputs.s .L800C99A0-.L800C99BC) forming
  `0x80156978`/`0x80156998`/`0x80143A00`/`0x80156958`, each indexed by
  `sll $reg,$a2,2` or `,3` (player index). Added via the contract's third
  amendment (direct derived-asm citation) as `s32[4]` / a two-float-struct
  `[4]` array in `GAME_SYMBOLS` + `game_types.h`.
- **`attract_or_transition`**: `temp_t6->unk9CC0` traced to a segment table
  at `0x80156BE0` (.L800EDDD4-.L800EDDD8, clean `lui+addiu`), indexed by
  `D_8015F72D*128`, with fields at `+0x58` (a second-level pointee, itself
  written through the same OSMesgQueue-shaped 8-byte-stride idiom `msgq_ptr`
  already uses elsewhere in this function) and `+0x7C` (passed straight to
  `osVirtualToPhysical`). Added `D_80156BE0` + two padded struct types.
- **`playgame_state_change`**: `(*D_8014A160)->unk8` was the double-pointer
  pointee the second gate named — `D_8014A160` retyped `T**`. The
  `var_s0 = **var_s0` linked-list walk off `D_8012E6E0` needed a
  self-referential single-pointer node type instead of `s32`. Both addresses
  were already in `GAME_SYMBOLS`/the survey; only their `game_types.h` types
  changed.

`countdown` and `Input_ProcessGameplayPad` remain blocked, and — unlike
every function closed so far in this feature — their remaining errors are
**not reachable from `game_types.h`/`GAME_SYMBOLS` content**, evidenced
per function below. `countdown` did yield one real fix (GameCar's
previously-blanket `pad000[0x380]` needed two named fields, `+0xE8` a
read-modify-write flags word and `+0x35B` a byte passed to `cpak_read`,
both confirmed at build/m2c_asm/countdown.s:1217/1219/1225 — `lw/sw
232($v0)`, `lb 859($v0)`), which cleared the "`GameCar` has no member
`unkE8`" errors but left the function still blocked.

## Input_ProcessGameplayPad: two occurrences of a disasm.py gap the
## amendments don't cover (`lui` + `addu $reg,$reg,$var` + fixed immediate
## applied at the *final* load, not at pointer formation)

Both `0x80120000` and a second, independent reach of `0x80138670` follow the
same instruction shape:

```
lui   $r,hi          <- forms ONLY the upper half; no addiu
...
addu  $r,$r,$idx      <- mixes in a runtime index; $r no longer "formed"
...
lw/lh $r,IMM($r)      <- the fixed low-half offset is applied HERE
```

`build/m2c_asm/Input_ProcessGameplayPad.s`:
- `.L800A0A74`/`.L800A0A7C`/`.L800A0A88` (`lui $s3,0x8012` /
  `addu $s3,$s3,$t7` / `lhu $s3,-4852($s3)`) — effective base
  `0x80120000-0x12F4=0x8011ED0C`.
- `.L800A0C1C`/`.L800A0C40`/`.L800A0C44` (`lui $t7,0x8014` /
  `addu $t7,$t7,$t6` / `lw $t7,-31120($t7)`) — effective base
  `0x80140000-0x7990=0x80138670`, i.e. the *same* table `D_80138670` already
  added this pass via its other, cleanly-`addiu`-formed occurrence
  (.L800A0CAC/.L800A0CB0) — proving the table itself is now correctly typed
  and the remaining failure is purely this second access path.

`tools/conveyor/pipeline/disasm.py:normalize_objdump`'s `pending_lui`
tracking invalidates a register's entry on *any* write other than `lui` or
a numeric-third-operand `addiu` (`_written_gpr`, disasm.py:124-138) — `addu`
is not special-cased, so `pending_lui.pop(register)` fires between the `lui`
and the load, and the load's own `_LOW_RE` match never finds a `pending_lui`
entry to consult `GAME_SYMBOLS` against. This is a distinct manifestation
from the two shapes the contract's three amendments already cover
((a) `lui`+`imm(reg)`, (b) `lui`+`addiu` pointer formation): here the low
half is a **third, later, independent instruction** separated from the
register-forming pair by an intervening variable-index `addu`. No
`GAME_SYMBOLS` entry or `game_types.h` type can reach this — the unresolved
expression is a numeric literal (`0x80120000`/`0x80140000` plus a negative
immediate) with no identifier for either file to attach to. Closing it needs
a fourth disasm.py shape: track `addu $r,$r,$idx` as "still traces to the
lineage, index unknown" and continue matching subsequent `imm($r)` loads
against the lineage's base value — out of this task's authorized scope
(GAME_SYMBOLS content only, not `normalize_objdump` logic).

## countdown: `active_player_count` reads m2c-corrupted by an unrelated
## expression merge, independent of disasm.py

Unlike `Input_ProcessGameplayPad`, every one of the six `active_player_count`
reads in `build/m2c_asm/countdown.s` (grepped exhaustively: lines 451/457,
487/491, 601/627, 798/800, 920/922, 1073/1075) is a clean, self-contained
`lui %hi(active_player_count)` + `lh %lo(active_player_count)($reg)` pair —
no intervening write, no `addu`, no alternate low-half consumer. There is no
raw (unsymbolized) `lui ...,0x8014` left anywhere in the derived asm for this
function (checked directly), so this is not a missed-symbolization case at
all: disasm.py already emits the theoretically-ideal derived assembly.

`mips_to_c/m2c/translate.py`'s `record_struct_access`/`has_nonzero_access`
(translate.py:416-422) is a **per-function, per-expression** flag: once *any*
use of a given value expression is recorded with a nonzero struct offset
anywhere in the function, m2c prints *every* occurrence of that
(deduplicated/interned) expression with explicit `.unk0`/`->unk0` syntax —
even offset-0 reads that would otherwise print as the bare identifier. Given
six independently-clean reads of the same global, the only way this fires is
m2c's own SSA value-numbering/copy-propagation deciding, somewhere else in
this 2,672-byte, multi-loop function, that a *different* local expression
(most likely tied to the `player_array[active_player_count]` /
`(active_player_count * 0x808) + &D_8014A250` pointer-arithmetic sites,
where `active_player_count` is the scale index for a non-power-of-2 stride)
is value-equal to an `active_player_count` read and merges their print
representations. This is internal to `mips_to_c`'s expression analysis, not
`disasm.py`'s address symbolization — there is no address, symbol, or type
declaration in `game_types.h`/`GAME_SYMBOLS` that changes which expressions
m2c decides to unify. (`gameplay_mode` and `player_array`'s remaining
`countdown` blockers are further errors past this one in the same function —
`(temp_v1_2*4)+0x80150000)->unk2698` is an unsurveyed table needing its own
third-amendment entry, and `(*temp_v0_2)->unk28` is a further pointee chain —
both are individually tractable, but moot while the `active_player_count`
corruption blocks the function regardless.)

## Disposition

7/10 falls short of the SC-001 gate (≥8/10, ≥4/7 for 60+-instruction
members — this pass reached 3 new members plus the prior 4, so 4/7 on that
sub-count remains satisfied even though the overall 8/10 does not). Per the
task's fallback instruction, stopping here: both remaining blockers are
evidenced, structural, and outside `game_types.h`/`GAME_SYMBOLS` (one is a
`disasm.py` address-tracking gap needing a fourth register-lineage shape,
the other is an `mips_to_c` expression-merging behavior). Closing either
requires code changes beyond this task's authorized files. No full histogram
or watchman scoring run was attempted for this third-gate pass.
