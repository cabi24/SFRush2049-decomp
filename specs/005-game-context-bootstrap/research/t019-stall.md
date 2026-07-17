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
