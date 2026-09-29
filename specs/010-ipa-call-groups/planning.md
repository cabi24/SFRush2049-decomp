# 010 — Matching IDO -O3 interprocedural call groups (planning draft)

Status: **planning draft, 2026-09-29**. Written as input to a spec-kit package
(spec → plan → research → contracts → tasks → HANDOFF). It is not a spec, so
nothing below is normative yet. Background evidence is in
[COMPILER_SETTINGS.md § Interprocedural register allocation](../../docs/COMPILER_SETTINGS.md).

## 1. Problem

The pipeline compiles each extracted function **alone at -O2** and scores it
against its own target object. That covers self-contained functions (121/912
spliced). It cannot match code built with IDO **-O3 interprocedural register
allocation (IPA)**. There:

- a callee receives parameters in non-ABI registers (`$t0`-`$t3`, `$s*`, `$f16`)
  and reads them without writing them;
- a caller keeps values live in caller-save registers across a `jal` to a callee
  it knows leaves them alone;
- a callee's own code changes with its callers. A single-caller static function
  was inlined, and with two callers the callee took its parameters in `$s` registers.

Measured scope: **99 IPA callees + 144 callers = 243 of 912 gate-passed
functions, about 39% of their instructions** (an upper bound; 12 callers
already matched at -O2 because their call sites happened to use ABI registers).
This group includes most `partial_decomp` seeds (the "Read from unset register"
errors), the `saved_reg_s*` histogram blockers, and the unsolved
`move $t0,$zero` wrapper family.

## 2. What is already established (2026-09-28/29)

| Fact | Evidence |
|---|---|
| Toolkit IDO supports whole-program -O3 | `cc -O3 -c a.c b.c` runs cfe → ujoin → **uld** → usplit → umerge → uopt → ugen → as1 and emits one `u.out.o` |
| Register-passed parameters require the callee to be `static` | the same callee as a global kept O32 stack arguments even in whole-program mode |
| Callee codegen depends on its callers | the same static body compiled with one caller vs two callers produced different code |
| Per-file -O3 with external callees misbehaves | a caller whose callees were extern had its calls dropped (the m2c spill-local seed returned an uninitialized slot) |
| Most IPA groups are address-local | median caller span 0x1D28 bytes, consistent with per-file static functions |
| Some are program-wide | `state_utility` (125 callers), `object_bytes_sum_global` (77), `object_manager_update` (107) span the whole image |

## 3. Goal and success criteria (proposed)

- **SC-1 (feasibility):** one real IPA call group compiles **byte-identical for every
  member**, proven by the image gate and the full-ROM SHA-1.
- **SC-2 (pipeline):** a group job type scores all members of a group together
  on the nodes, with per-member scores and deterministic group definitions.
- **SC-3 (yield):** a measured number of new functions spliced from IPA groups,
  reported separately from single-function matches.

## 4. Unknowns and the spikes that resolve them

Run the spikes in order. Each has a pass/fail and a stop rule. Spikes are cheap
(hand-driven on rocky) and gate the spec.

**S1: Reproduce one real group byte-exact.** Candidate:
`resource_slot_clear` (18 insns, IPA callee) with its callers `draw_message` (43)
and `resource_slots_clear_multiple` (12), 73 insns total. Next candidates:
`suspension_setup`+`suspension_params_init` (75) and
`main_menu_input`+`main_menu_render` (115). Write C for all members, put them
in ROM order in one TU with the callee `static`, compile at `-O3`, and compare
each function's slice.
- Pass: all members byte-identical. Fail: document what differs. The likely
  suspects are the missing transitive callees (their register-clobber summaries)
  and function order.

**S2: How much of the neighbourhood must be real?** With S1 passing, replace
transitive callees outside the group with declarations or stubs and see what
breaks. This decides the group boundary: exact bodies for members, stubs that
preserve register summaries, or extern declarations.

**S3: Program-wide mechanism.** For the image-spanning callees, decide between
(a) a unity build (one TU `#include`-ing many files, so statics are
program-wide) and (b) `uld`-level symbol hiding. Test the `cc`/`uld` options for
hiding or localizing symbols. Look for layout evidence of file boundaries
(alignment padding, rodata or string-pool grouping). This decides whether program-wide groups are
reachable at all or stay out of scope.

**S4: Extracting and splicing members from a multi-function object.** Static
functions carry no symbols in `u.out.o`. Establish how to map object ranges to
members (source order = output order in the tests so far; confirm with
S1). Decide whether `blob_splice` takes per-member slices of one group object.

**S5: Seeds for IPA members.** m2c cannot express register parameters. Options:
treat entry-unset `$t*`/`$a*` reads as extra trailing parameters; enable the
experimental patch 0003 (keep values across calls) for group members only;
arcade-source ancestry where it exists. Measure how many members yield
compiling seeds.

## 5. Architecture sketch (to firm up after S1-S4)

1. **Group discovery (deterministic, Pi).** Scan the image for `jal` edges. Mark
   IPA callees (entry reads of unset registers, confirmed by caller-side register
   setup) and IPA callers. Build groups as connected components restricted to
   address windows (a file-sized locality heuristic), and emit
   `build/ipa_groups.json` with member order = ROM order. Program-wide callees
   are flagged and excluded until S3 says otherwise.
2. **Group seeds.** A group TU = member seeds in ROM order, with the IPA callees
   declared `static`, each with m2c's best seed (plus S5 treatment).
3. **`group_score` job type (nodes).** Compile the TU at `-O3` and score each member
   slice against its target object (true score + the strict stack-visible score).
   Content-addressed like `compile_score`, so re-scoring is cached.
4. **Group permuter.** The permuter mutates one member at a time while scoring
   the whole group (the objective = sum of member scores). This is either a
   decomp-permuter extension or a wrapper that rewrites one function in the TU.
5. **Group splice.** `blob_splice` accepts a group object and splices every
   member atomically behind the image gate. The lock records the group id, flags
   and member sources.
6. **Flywheel hygiene (quick win, independent).** Exclude known IPA members from
   -O2 triage searches. They cannot match standalone, and they currently use
   node time.

## 6. Phasing and decision gates

| Phase | Work | Gate |
|---|---|---|
| 0 | Flywheel exclusion of IPA members (small, standalone). **Done 2026-09-29**, see below | node time goes to matchable targets |
| 1 | Spikes S1-S2 by hand on rocky. **Done: S1 pass; S2 favourable** ([s1](research/s1-poc.md), [s2-s5](research/s2-s5-spikes.md)) | **S1 pass**, or stop and write up |
| 2 | S3-S5 research. **Done: S3 = whole-program `uld -kp`; S4 = per-slice relocation passes the image gate; S5 = needs an m2c register-parameter map** | decisions recorded in research.md |
| 3 | Spec-kit package (spec/plan/contracts/tasks/HANDOFF) | review |
| 4 | Implement discovery + group_score + group splice | SC-1 through the pipeline |
| 5 | Group permuter + flywheel integration | SC-3 measured |

## 6a. Phase 0 outcome (2026-09-29)

`tools/conveyor/pipeline/ipa.py scan` writes `build/ipa_members.json` from each
gate-passed function's derived assembly using forward dataflow:
- **callee**: a non-O32 register live on entry;
- **preserver**: a caller-save register used after a `jal` with no write since.
  That includes an argument register passed through to the next call.
  `jal` delay slots count as running before the call, and float arguments in
  `$f12`/`$f14` legitimately leave `$a0`/`$a1` unset;
- **caller**: a function that writes a callee's non-ABI parameter register and calls it.

Result: **365 of 912 members** (132 callees, 174 preservers, 178 callers).
Checks: all 99 m2c "unset `$t`" callees are found, and **0 of the 121 already-spliced
-O2 matches** are flagged. `farm.flywheel_cycle` drops members from triage
and full-search promotion; the compile-only sweep still covers them. No
IPA member was in the queue when this landed. Rerun `ipa scan` after any
extent or derivation change.

The membership (365) is larger than the first estimate (243), because the
preserver pattern also catches callers that keep argument registers across
calls, not only callers of register-parameter callees.

## 7. Risks

- **Sensitivity cascade:** if member codegen depends on exact transitive callee
  bodies, groups grow until they are not tractable. S2 measures this.
- **Program-wide IPA** may need the whole program's source to reproduce, so those
  groups stay unreachable until most of the game is decompiled. The plan
  treats them as out of scope unless S3 finds a cheaper mechanism.
- **Layout:** the whole-program output order and alignment must match the ROM.
  The tests so far emit functions in source order.
- **Scoring cost:** a group compile costs more than a single function, and the permuter
  runs a group compile per iteration. The nodes can absorb it (Rocky is 20 cores).

## 8. Immediate next steps

1. Phase 0 flywheel exclusion (small change, keeps nodes productive).
2. Spike S1 on `resource_slot_clear` + callers.
3. Record spike outcomes in `specs/010-ipa-call-groups/research/`.
