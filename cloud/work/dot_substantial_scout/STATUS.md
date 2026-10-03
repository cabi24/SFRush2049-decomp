# D02 substantial-function preflight: no ready execution packets

Owner: round-10 scout A. Branch: `research/round10-a-scout`.
Base: `d701b59592463d1e33bcee7011ed2a1e6c07b066`, 2026-10-02.
Status: **SCOUT ONLY, NO MATCH**. `claims=[]`. No C candidate changed, no accepted
coverage or cartridge claim. This bounded handoff audit found **zero of the requested
two ready packets**; it does not label blocked prerequisites as ready work.

## New evidence and corrected dispatch decision

- Independently regenerated protected full-body identities, entry frames, direct
  callers and every direct callee for all four D02 candidates. Combined native
  extents are 12,788 bytes; this is investigation scope, not matching credit.
- Corrects the old `bigfish/func_80100E58.md` statement that it has no direct caller:
  the current registered `func_8010221C` calls it. The old report's conclusion
  that isolated compilation is insufficient remains consistent with its native
  `slot_state_setup` and `func_80100D5C` dependencies.
- `func_80102F30` is not reconstructed merely because it has two C definitions.
  Both registered-head group seeds contain a TODO body returning zero. Their
  `seed-info.json` explicitly records m2c=false. No score of these placeholders
  can represent a complete 890-word baseline.
- Freshly replayed C974's generated seed at canonical O2: IDO rejects the
  pointer-to-float assignment `var_f12 = arg0` at line 879. Candidate size and
  strict differing-word count are **unavailable**, not 0 bytes or 659 measured
  mismatches. The old generated header saying “compiles” is not trusted.
- Crosschecked all current PR1–41 fetched trees. `pr_tree_inventory.json` records
  exact heads/trees and scoped filename evidence. A filename is only evidence
  of a source artifact, not completeness, correctness, acceptance or current
  ownership. A prototype/reference does not establish prior reconstruction.
  Multiline C definitions invalidate a one-line function-definition freshness
  filter: FC9F8 is a concrete counterexample.

## Audited shortlist

| Target | Full native extent | Frame / direct calls | Existing evidence | Decision |
|---|---:|---:|---|---|
| `func_800D91A0` | 3,860 / 965 words | 240 / 33 | `bigfish/func_800D91A0.md`; input auxiliary context | Blocked: full replay/pause closure, not an isolated near-match |
| `func_80102F30` | 3,560 / 890 | 640 / 51 | registered-head 102448 / 102980 groups: TODO placeholders | Blocked: no complete source, native indirect branch/table plus shared tail helpers |
| `func_80100E58` | 2,732 / 683 | 528 / 40 | `bigfish/func_80100E58.md` | Blocked: full menu/drawing body plus shared tail helpers |
| `func_8010C974` | 2,636 / 659 | 392 / 24 | registered-head seed, currently does not compile | Reconstruction prerequisite only, not ready for matching |

Historical subsystem names are hypotheses. D91A0 replay/pause purpose follows
its existing native audit. E58/F30 are menu/drawing-control descriptions rather
than assertions about original source names. C974 visibly manages object/model
resources, timers, transforms and cleanup; an arcade identity is not established.

### Prerequisite packet 1: C974 source/ABI recovery (not ready)

Identity/hash/callees are in `native_audit.json`. Entry uses a0 as a pointer and
a1 narrowed to signed16, saving them at entry-frame offsets392/396. Native zero
mode retains a0 and sets a1=1 before `entity_transform_apply`; the old seed's
`entity_transform_apply((void *)1, 0)` reverses that relationship. Do not fix the
pointer-to-float error by casting an object pointer into a synthetic float input.

Observed field contracts: entry object+12 selects a state; state+4 is a byte flag,
state+101 is signed8 index, state+108 selects auxiliary state. The native table
at D_80151528 uses 20-byte rows with four pointer slots and count at+16. These
are inferred views, not original C type identities. The full body includes
initialization, two-part animation and cleanup; recovery must preserve all three.

Concrete route: reconstruct the entry and all three phases with typed views;
verify the real `camera_trigger_check` and `math_utility` caller/callee register
contracts before compiling, since native caller values survive helper calls.
Audit the direct call to **0x8038D798** at native0x8010D2C4: it has no symbol in
current blob symbols and no repository body found. Establish its actual ABI,
stack-argument and clobber contract or explicitly retain it as a blocker. Do not
invent a helper or argument list from the generated seed.

One testable hypothesis: the erroneous `var_f12` entry dependency belongs to the
camera-helper contract rather than C974's ABI. Trace native reaching definitions
at that call and compare the genuine callee before replacing it. Stop if its
actual closure or the external8038D798 contract cannot be established. Baseline:
compile failure, candidate size/diff unavailable. Edited context: none.

### Prerequisite packet 2: shared tail drawing context (not ready)

F30 and E58 share `slot_state_setup` and `func_80100D5C`; allocate them together
only after genuine contracts are proven, never to independent execution lanes.
For F30, native mode selection reaches an indexed indirect jump near0x80102FBC;
its seven-case table and full case destinations must be verified from authorized
repository evidence before generating a switch. Do not extrapolate table words.
E58 now has a proven direct caller `func_8010221C`, useful authentic context that
the older report missed. Inspect this real call chain before deciding external
roots/keep lists. The old reports already document nonstandard input registers
and the unexplained `slot_state_setup` result move. No new compiler explanation
was established here. Baseline: no complete source compile; candidate size/diff
unavailable. Next hypothesis: use the newly registered genuine caller to audit
live-out requirements; stop without a new contract explanation.

### D91A0 rejection

The previously documented closure includes D816C (2,796 bytes), D8154, D8078 and
`render_replay_ui`, with shared callers. D816C consumes a non-O32 t2 input;
other calls preserve caller-save values. Its old partial source/53.5% aligned
opcode metric is not a complete strictly relocated baseline. No new closure
hypothesis arose. Do not restart its old skeleton or claim its old score as new.

## Additional bounded fallback audit

These were rejected, not newly reconstructed:

- F769C: full `game_C38` statistics source and array/extern controls; ordinary leaf
  ABI alone does not make it fresh.
- FC9F8: full `tiny_A69` point-list motion source, 235/254 historical residual;
  prior report proves unusual positive-Y threshold. Source definition spans lines.
- D169C: full `game_C64` nearest-route source; uninitialized closest-index domain
  already documented. No invented initialization is a matching remedy.
- E114C: full `game_C25` velocity/helper group. Real context already tried.
- 100564: native call to `slot_state_setup`; not an independent fallback.
- 101D84: unsaved s-register use and the same tail-helper closure.
- E8F10: actual calls to B9338/E8D50; no complete ABI/freshness clearance established.
- 10D680: indexed indirect branch; table/contract prerequisite not cleared.

B/C independently found all D04 suggestions already reconstructed. B's native
purpose corrections: race_setup_1 is palette/display-list animation, assign_drones
is ranking/statistics work. Those descriptive names must not select arcade donors.

## Reproduction and limits

`python3 cloud/work/dot_substantial_scout/preflight.py --check cloud/work/dot_substantial_scout/native_audit.json`

`python3 -m pytest -q tests/conveyor/test_dot_substantial_scout.py`

Compile failure reproduction:
`python3 tools/cloud/score.py fn cloud/work/registered-heads/seeds/func_8010C974/seed.c func_8010C974`

Canonical compiler recipe for a future real baseline: pinned IDO5.3,
`-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul`; use genuine whole-program O3
only once actual closure is established. This audit does not endorse old fake
stand-ins. No semantic/sanitizer test is applicable to unchanged candidate C;
the new tests exercise the scout decoder and compare complete current metadata.
No raw native instruction/object archives, private ROM input, protected edits,
source splices or ROM gates. Direct-call scans do not find indirect callers,
prove full ABI, or establish lack of external users. Empty caller lists are not
proof of root ownership. No queued execution or automatic replenishment started.

## Independent D04 companion audit

[D04_PREFLIGHT.md](D04_PREFLIGHT.md) and [d04_replay.json](d04_replay.json) preserve
lane B's independently run source-hashed compiler replays and native-purpose
corrections. They add dispatch evidence, not new matching improvement. Source
artifacts are existing near_miss_B104 and near_miss_B62 packets at the pinned PR9
head in pr_tree_inventory.json. Table relocations remain explicitly unverified.

The user restriction recorded in master `dot_response` at
`0bfebc7367ebc1ddb4d6105b2012ed07fb080faf` takes precedence over the historical
handoff: func_800D1248 and its proposed helper-context investigation are offlimits
until the owner lifts the restriction. They are not an execution recommendation.
