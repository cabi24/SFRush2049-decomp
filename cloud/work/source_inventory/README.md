# Source-inventory prerequisite repair

Tooling research only: no candidate compilation, match, source/data coverage, or
ROM integration. Base `0bfebc7367ebc1ddb4d6105b2012ed07fb080faf`.

## Why this is new

The inherited PR1–41 scouting index omitted bodies that actually existed. A path
index is insufficient when packets use generic filenames such as `group.c`.
An unanchored signature regex can also consume the next definition as part of a
preceding `#define` expression. For example, the `MOTION_AT` macro precedes
`func_800FC9F8` in the existing `tiny_A69` packet. Freshness was incorrectly inferred
from absence in that index. No stale candidate was compiled in this investigation.

This utility indexes unique C blobs across explicitly supplied immutable Git
revisions, masks directives/comments/literals before scanning, anchors signatures,
handles nested callback arguments and balanced bodies, and retains source line,
blob identity, source SHA-256, path, and every queried revision for each lead.
Empty/comment-only stubs, TODO/M2C_ERROR bodies, and other nonempty bodies have
separate labels. **None of these labels claims completeness or semantic validity.**

## Verified practical corrections

The PR1–45 run read 5,081 unique C blobs and produced 10,751 path/blob/definition
records with 4,469 distinct names. These include library and historical/stub names,
not 4,469 game functions. Exact revisions and example records are in evidence.json.
The full working index stays local rather than duplicating thousands of sources.

- `display_list_traverse`: existing `tiny_A113` target plus four controls; not fresh.
- `func_800FC9F8`: existing `tiny_A69` body; not fresh.
- `func_800E3430`: existing `codex_physics_a85` group plus seven controls; not fresh.
- `physics_float_calc` and `init_state_continue`: their work/game definitions are
  empty stubs, not completed reconstructions. That alone does not make them ready:
  native data/ABI prerequisites still require separate inspection.

These corrections prevent repeating old reconstruction work. They do not provide
a new ready medium-function packet. The bounded game scout also encountered real
IPA calls/unsaved registers in remaining candidates, so it stops without claiming
an easy isolated target or republishing their no-go diagnoses.

## Reproduction

Use an existing checkout with the relevant heads fetched. Pin every revision to
the commits in evidence.json when reproducing that snapshot; refs can move.

    python3 cloud/work/source_inventory/inventory.py --ref <commit> --ref <commit>
    python3 -m pytest -q tests/conveyor/test_source_inventory.py

The utility never fetches, modifies refs, changes source, compiles targets, or
reads target instruction data. Its only repository operations are Git read calls.
Nine synthetic tests cover macro swallowing, continued macros, comments/literals,
callbacks, pointer returns, split signatures, truncation, stub classification,
inactive preprocessor branches, continued line comments, and a temporary Git repository proving deduplication/provenance.

## Deliberate limitations and required follow-through

This is a conservative C-source lead scanner, not a C parser or preprocessor.
Inactive conditional branches remain leads; macro-generated definitions, K&R
headers, attributes between a signature and body, multiple definitions on one line, unusual declarators and
non-UTF-8 source may be missed. The selected roots are cloud/, src/, and work/;
other directories and non-.c sources are outside scope. Names may be aliases or
historical guesses and are not native identities. A body can be a stub without a
TODO marker. A nonempty body can be partial, wrong, or a wrapper.

Consequently, absence is never proof of freshness, presence is never proof of
completeness, and source counts are never coverage. Before claiming a target,
inspect the actual source/status and current reservations, check address aliases
and native interval locks, and search relevant PR trees independently. Compiler
flags, body extents, differing words, unresolved relocations, gameplay semantics
and cartridge gates are outside this utility's evidence; no such gates ran here.
