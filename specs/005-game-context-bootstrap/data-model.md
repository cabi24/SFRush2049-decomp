# Data Model: Game-Code Context Bootstrap

No schema migration. The feature reuses existing tables and adds two derived,
regenerable artifacts plus one committed list and one committed header.

## `n64_target` (existing table — value-level changes only)

| Column | Use in this feature |
|---|---|
| `target_id` | unchanged primary key; cluster list references these |
| `address` | scan start for extent repair |
| `population` | selector: this feature operates on `'extracted'` |
| `insn_count` | **rewritten by extent repair** to the scanned extent |
| `target_asm_sha` | recomputed identity hash of the re-carved words |
| `target_o_sha` | recomputed on re-carve → triggers 003 supersession |
| `tier` | stays `raw_word` for extracted targets (no reloc asm exists) |
| `gate_reason` | new values: `extent_repaired` (extent changed, object re-carved), `extent_conflict:<container_id>` (start lies inside another target's repaired extent — excluded from seeding/scoring) |

### Extent lifecycle (state transitions)

```
info.txt extent (status quo)
   │  extent scan (D1): furthest-forward-branch / jr $ra + delay slot
   ├─ scan agrees (±0)            → unchanged (no supersession)
   ├─ scan disagrees              → insn_count updated, words re-carved,
   │                                object re-assembled, target_o_sha changes,
   │                                matrix_entry purged (003 supersession),
   │                                gate_reason = extent_repaired
   └─ start inside another extent → gate_reason = extent_conflict:<id>,
                                    object retained but target excluded from
                                    seed/score; histogram bucket = extent_conflict
```

Determinism rule: the scan is a pure function of (`game_code.bin` bytes,
`address`); two runs with no input change produce identical extents (SC-003).

## `function_status` (existing — reused unchanged)

Extracted targets already have rows. `seed_kind='m2c'` and
`status: unmatched → seeded/in_search → matched` transitions are identical to
the static path. No new columns.

## `matrix_entry` (existing — reused unchanged)

Scores from cluster seed searches land exactly as static permuter evidence
does, carrying `target_o_sha` attribution. Supersession from extent repair
purges any pre-repair evidence in the same transaction (003 contract).

## Cluster list (new committed file)

`tools/conveyor/clusters/game_loop.txt` — one `target_id` per line, `#`
comments allowed. Initial contents (10 confirmed rows):

```
game_loop                 # 0x800FD464 dispatch
game_mode_handler         # 0x800C9AE0
attract_or_transition     # 0x800EDDC0
process_inputs            # 0x800C997C
sound_control             # 0x800B37E8
playgame_state_change     # 0x800CA3B4
RaceStateMachine_Update   # 0x800DB81C
countdown                 # 0x800FBF88 (extent-repaired, ~668 insns)
countdown_handler         # 0x800FBC30 (extent-repaired, ~124 insns)
Input_ProcessGameplayPad  # 0x800A04C4
```

Validation: every line must resolve to an `n64_target` row; unresolvable
lines abort the run with the offending name.

## Game symbol table (new, small, committed — inside `disasm.py` or a
sibling data file)

| Name | Address | Type basis (research/cluster-data-refs.md) |
|---|---|---|
| `gstate` | 0x801146EC | byte R/W around state dispatch — confirmed |
| `frame_counter` | 0x80142AFC | word, init/read/increment per loop — confirmed |
| `game_state_flags` | 0x801146E8 | word R/W adjacent to gstate |
| `state_word_a` | 0x801174B4 | word R/W (secondary state, 110 refs) |
| `state_word_b` | 0x801174B8 | word R/W |
| `input_rec0` | 0x8014A118 | base with offsets {0x01,0x04,0x08,0x0C,0x10,0x14,0x4D} |
| `input_rec1` | 0x8014A164 | same shape — two-player input records |
| `player_array` | 0x80152818 | indexed base, offsets {0x380,0x3A3} — array confirmed, stride unknown |
| `msgq_ptr` | 0x801497C8 | pointer deref {0x00,0x04,0x0C} |

Rule: only names in this table are symbolized in derived asm (D3); everything
else stays numeric. Names are provisional (`gstate` is the only
arcade-aligned certainty) and rename freely until a target is matched.

## `include/game_types.h` (new committed header)

Each definition carries `/* rushtherock:<file>:<lines> */` provenance and,
where the N64 diverges, a `/* N64: */` note. Contents (bounded by FR-004):

- `GState` enum — game/game.h:104-117; N64 note: stored as `u8`.
- Input record struct(s) shaped to the observed 0x18-byte layout, informed
  by globals.h switch/pot fields and modeldat.h `fastintyp`/`mainintyp`.
- Partial player/car struct: declared fields only where the cluster reads
  them (offsets 0x380/0x3A3 region), explicit `u8 pad[...]` elsewhere;
  arcade CAR_DATA (modeldat.h:403-467) cited as shape reference, not layout
  truth.
- `Track_Data` — checkpoint.h:101-109 verbatim (platform-neutral).
- `extern` declarations matching the game symbol table.

Excluded by policy (arcade-hardware): coin/cabinet switch globals, ZOID
`objnum`, `Visual`, `gnState`/`gnData` link-cabinet records, force-wheel
fields.

## Histogram artifacts (new, regenerable, in `build/`)

`build/m2c_histogram.json` — the SC-003 instrument:

```json
{
  "run": {"timestamp": "...", "population": "extracted",
          "game_code_sha": "...", "context_sha": "...", "targets": 885},
  "buckets": {"compiled": N, "blocked": N, "decompiler_failure": N,
               "no_disasm": N, "extent_conflict": N},
  "targets": {"<target_id>": {"bucket": "blocked",
                               "blockers": ["<sym>", "..."],
                               "detail": "<one-line diagnostic>"}, ...},
  "blockers": [{"symbol": "<sym>", "count": N,
                 "functions": ["..."], "arcade_hint": "<file:line|null>"}]
}
```

Invariants: every extracted target with a target object appears exactly once
in `targets`; bucket counts sum to the target count; `targets` and
`blockers` are deterministically ordered. `build/m2c_histogram.md` is the
human ranking rendered from the same data (successor of
`build/m2c_clusters.md`).

## Promotion firewall (existing modules, new check)

`lock add` / `promote run|batch`: before accepting a function, resolve its
`n64_target.population`; if `extracted`, fail with
`extracted-population targets are evidence-only (feature 005, FR-010)`.
