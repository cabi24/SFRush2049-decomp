# Research: Game-Code Context Bootstrap

All primary evidence is in `research/` (codex ground-truth runs, 2026-07-15):
`cluster-data-refs.md` (per-function global/struct/call survey of the 10
cluster functions), `arcade-structs.md` (verbatim arcade type harvest with
provenance), `m2c-dryrun.md` (25-target derivation + m2c probe). Decisions
below cite them.

## D1 — Extent repair before anything else

**Decision**: Re-derive every extracted target's extent by control-flow scan
over `build/game_code.bin`: track the furthest forward branch target seen;
the function ends at the first `jr $ra` + delay slot at/after that point
(`jr $reg≠ra` — jump tables — does not terminate; a hard cap of 16 KB and the
image end bound the scan). Repaired extents update `n64_target.insn_count`,
re-carve words, re-assemble the target object; the changed `target_o_sha`
flows through 003's supersession (stale `matrix_entry` purge) unchanged.
Targets whose start address falls *inside* another target's repaired extent
are classified `extent_conflict` (they are mid-function labels, not
functions) and excluded from seeding/scoring but reported in the histogram.

**Rationale**: The dry run falsified the stored extents: 11/25 sampled
targets had branch targets past the recorded end or a missing delay slot;
`countdown` (recorded 2 instructions) is actually `0x800FBF88..0x800FC9F7`
(~2.7 KB, confirmed by the data-ref survey's independent disassembly). The
root cause is `load_work_inventory()` gap-capping: extents were inferred
from gaps between `work/**/info.txt` entries, and a mislabeled boundary
entry truncates its predecessor. Scoring a candidate against a truncated
target object is scoring against a lie — SC-002's evidence would be
meaningless and a false score-0 on a 2-instruction prefix is plausible.
Repair must therefore precede scoring (spec edge cases; user's explicit
plan input).

**Alternatives considered**: (a) Trust `info.txt` sizes and only fix the
cluster by hand — rejected: the histogram (FR-006) would misclassify
hundreds of truncated targets as "compiled" on prefix seeds (the dry run
showed truncated slices still emit plausible C). (b) Full recursive-descent
disassembler — rejected: the furthest-branch/`jr $ra` heuristic is the
standard splat/decomp approach, is deterministic, and the existing 752
prologue starts give strong cross-validation; a full CFG adds cost without
changing outcomes. (c) Repair extents only in the seeding path, leaving
objects untouched — rejected: violates evidence honesty; 003 built
supersession precisely so target-object corrections propagate.

## D2 — Disassembly derivation: objdump + three normalizations

**Decision**: New `pipeline/disasm.py`: carve `[address, address+4·insn_count)`
from `game_code.bin`, disassemble with
`mips-linux-gnu-objdump -D -b binary -m mips:4300 -EB --adjust-vma=0x80086A50`,
then normalize exactly as the dry run validated: (1) strip printed
address/opcode columns, (2) `$`-prefix register names, (3) emit `.L<addr>:`
labels and rewrite branch operands (including `bc1f/bc1t[l]`) to them;
rewrite `jal`/`j` targets to `target_id` names via the `n64_target` address
map, else `func_<addr>`. Output `glabel <target_id>` files cached at
`build/m2c_asm/<target_id>.s`, regenerated when extent or bin hash changes.

**Rationale**: The dry run proved this exact format parses in m2c with zero
tracebacks across 25 targets; every failure was an extent artifact (D1), not
a decoder problem. objdump is already a hard dependency of `targets.py` and
the reloc gate.

**Alternatives considered**: splat re-run over the blob (heavyweight, wants
a linker context the blob doesn't have); capstone (new dependency, violates
stdlib-plus-binutils convention); reusing `build/game_code_disasm.txt` from
`simple_mips_disasm.py` (format not m2c-compatible, no label/symbol
rewriting, stale relative to extent repair).

## D3 — Numeric globals score correctly; symbolization is for typing only

**Decision**: Seeds may keep raw numeric global accesses
(`*(u8*)0x801146EC`-style). Additionally, `disasm.py` symbolizes
`lui/%lo`-pair data references into `%hi/%lo(name)` for names in a small
committed game-symbol table (the confirmed globals from the data-ref survey:
`gstate`@0x801146EC (byte), frame counter@0x80142AFC, the two input-record
bases 0x8014A118/0x8014A164, state words 0x801174B4/B8, player-array base
0x80152818), so m2c can apply typed context to exactly the cluster's
load-bearing state.

**Rationale**: Extracted target objects are raw-word assemblies with
absolute addresses baked into the instruction stream, so a candidate that
compiles the same absolute constants produces identical `lui/addiu` words —
numeric addressing is score-correct against this population (unlike static
reloc-aware targets). Symbolization therefore buys *typing and readability*,
not score, and can stay deliberately small (FR-004's bounded scope). The
dry run confirmed no-context seeds emit compilable numeric derefs.

**Alternatives considered**: full symbolization from `symbol_addrs.us.txt`
(3,406 entries — pulls in the known #3 misattribution problem and risks
name-collision churn; the score doesn't need it); no symbolization at all
(seeds compile but m2c can't type the gstate dispatch or input records —
the actual point of the context bootstrap).

## D4 — Game-type context as a new provenance-stamped header

**Decision**: New `include/game_types.h`, appended to
`autodecomp._context()`'s header chain (after `m2c_types.h`). Contents,
bounded by the cluster's needs (FR-004), each with `/* rushtherock:
file:line */` provenance from the harvest: `GState` enum
(game/game.h:104-117) with the N64 note that `gstate` is a *byte*
(confirmed R/W byte at 0x801146EC); the two 0x18-byte-shaped input records
observed at 0x8014A118/0x8014A164 modeled on the harvest's switch/pot
globals and `fastintyp`/`mainintyp` shapes; a deliberately partial
`CAR_DATA`-derived player/car struct only to the fields the cluster touches
(the survey's 0x80152818 offsets {0x380, 0x3A3} prove the N64 layout
diverged from arcade — declare what is known, pad the rest); the
`Track_Data` timing struct (checkpoint.h:101-109, platform-neutral); extern
declarations for the confirmed globals. Arcade-hardware fields (coin/cabinet
switches, ZOID `objnum`, linked-cabinet `gnState`) are annotated
N64-inapplicable and excluded.

**Rationale**: Keeps arcade-derived game types separate from the LLM-grown
`m2c_types.h` (different provenance disciplines); the context chain is
already preprocessed by `_context()` with `cpp -P -nostdinc -I include
-I include/PR`, which resolves the dry run's raw-parse failures (codex fed
m2c the unpreprocessed header directly and with the wrong include path —
the shipped wrapper already does this correctly).

**Alternatives considered**: growing `m2c_types.h` in place (mixes verbatim
arcade provenance with inferred types; harder to audit FR-003); a full
arcade-header import (violates FR-004 and drags in hundreds of
3dfx/mathbox types the harvest explicitly flags as inapplicable).

## D5 — Population widening, not a parallel path

**Decision**: Parametrize the existing autodecomp entry points with
`--population {static,extracted}` (default `static`, preserving current
behavior) and a `--targets id1,id2,…` selector; the game-loop cluster ships
as `tools/conveyor/clusters/game_loop.txt` (the 10 confirmed target_ids).
Asm sourcing becomes a two-branch resolver: static → `_asm_index()` glabels
(unchanged); extracted → `disasm.py` cache. Flagset for extracted targets
defaults to the -O2 confirmed flagset with the -O1 one as the recorded
alternate (game code ancestry suggests -O2; FR discovery is recorded, not
tuned). `submit_one`/farm harvest, `function_status`, and reporting are
reused untouched — extracted rows already exist in the same tables.

**Rationale**: The static path has three hardcoded `population='static'`
predicates and one asm-source assumption; everything downstream (bundles,
permuter_search, ingest, reports) is population-agnostic already. Smallest
honest change; SC-005's no-regression gate protects the default path.

**Alternatives considered**: a separate `autodecomp_game.py` (duplicates
the seed/submit/status lifecycle and splits future fixes); widening the
default to both populations at once (silently changes existing farm
automation cadence — opt-in is safer for a walking skeleton).

## D6 — Histogram: harden `cmd_clusters` into the FR-006 instrument

**Decision**: Rework `cmd_clusters` to (a) accept `--population extracted
--limit 0` (whole population), (b) classify every target into exactly one
bucket — `compiled`, `blocked` (with named symbols), `decompiler_failure`
(m2c nonzero/timeout, stderr preserved), `no_disasm` (derivation failed),
`extent_conflict` (from D1) — and (c) write two durable artifacts:
`build/m2c_histogram.md` (ranked blockers with counts, function names,
arcade hints — human report) and `build/m2c_histogram.json` (per-target
bucket map, deterministically ordered — the SC-003 diffable instrument).
The local compile probe stays `mips-linux-gnu-gcc -fsyntax-only -std=gnu89`
after `cpp -P -nostdinc` (already the shipped mechanism), so the whole
histogram runs on the Pi with no builder (FR-008). The current wrapper's
stderr-swallowing (`tools/m2c.py` prints only "Error running mips_to_c:")
is bypassed by invoking `tools/mips_to_c/m2c.py` directly, as autodecomp
already does; diagnostics are preserved per target (FR-009, dry-run
recommendation).

**Rationale**: `cmd_clusters` already implements tokenized blocker
extraction with arcade hinting — the spec's instrument is that report with
population coverage, bucket exclusivity, and durability guarantees added.
The syntax-only gcc probe is a proxy for IDO compilability; that proxy is
already how static blockers are priced, and the walking-skeleton cluster
gets true IDO compiles at score time anyway.

**Alternatives considered**: compiling every seed with IDO on the builder
for the histogram (885 × IDO round trips, farm-dependent, violates FR-008);
building a new report module (duplicates working tokenization/hinting).

## D7 — Promotion firewall

**Decision**: `lock add` and every `promote` entry point look up the
target's population and refuse `extracted` with an explicit error naming
this feature's out-of-scope boundary. A unit test locks the guard.

**Rationale**: FR-010/SC-006. Today nothing *would* promote an extracted
target (no derived layout region covers blob RAM), but that is an accident
of data, not a guarantee; a one-line guard makes the boundary structural.

**Alternatives considered**: relying on the layout map's natural absence of
blob regions — rejected: silent, and future layout changes could open the
path unnoticed.

## Resolved unknowns from Technical Context

No NEEDS CLARIFICATION items remained after the codex runs: derivation
format (D2), context parse path (D4), extent truth (D1), scoring semantics
for raw-word targets (D3), and local-compile feasibility (D6) were all
settled empirically.
