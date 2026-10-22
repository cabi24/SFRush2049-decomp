# GhostEnd and font-height near-miss replay, 2026-10-22

Base: `dd4cf01d3846ef60ca88708dac83af446da5a4f2`.

**No new matches and no progress credit.** These are source-only research
recipes, not installation packets. No locked source, lock, receipt, target,
SHA256SUMS, or scorer changed. No reference source or binary payload is included.

## Scope and exact baseline results

The initial static lane was already exhausted: `docs/static-asm-ceiling.md`
records the last C-reachable leaf, `func_80012D18`, as landed on 2026-10-21.
STATIC is 677/677 functions, of which 634 are C and 43 intentionally assembly;
96 alignment bytes are not a remaining function. The lane therefore moved to
three current, unclaimed game near misses recommended by the owner.

Stock IDO 5.3, `-g0 -O3 -mips2 -G 0 -non_shared`, canonical `score.py group`:

```text
ghost_race_setup:
  3/110 words differ
object_bytes23_sum:
  4/18 words differ
object_bytes_sum_global:
  4/21 words differ
```

Both invocations exit 1, as they should. None of these roots has extra nonzero
words, unresolved/unverified relocations, or scorer errors. GhostEnd is exactly
440 bytes/frame 64; the font roots are 72/84 bytes, both frame 24.

GhostEnd uses the existing `w39b/ghost/base.c` plus unchanged genuine files from
`src/blob/groups/codex_heap_release_a25`. The unrelated `initsystem.c` unit and
its `game_late_init` keep/member entry are omitted. All 20 retained pre-existing
members and all six informational contexts report `MATCH`.

The font pair uses `w38a/tb/d1.c` and `w38a/tb/c1.c`, the current real
`src/blob/groups/slot_sound`, and the unchanged accepted
`src/blob/func_800B3D0C.c`. All 13 pre-existing group members plus the leading
helper report `MATCH`. The obsolete `codex_sound_channel_extra` context is not
an adequate substitute: its font setup, internal resource stub, and slot getter
fail their own targets. This replay does not alter those old files.

Put root files first in the group. Appending GhostEnd after the heap family
lets an unrelated anonymous inlined-helper stub fall into its inferred suffix,
and the scorer correctly refuses the resulting extra word. Reordering files
avoids that packaging error without changing the root, compiler, or scorer.

## Bounded negative results

Twenty-two GhostEnd and eight font-height root-only variants were tried, with
no improvement. The replay retains a small representative subset and all eight
font probes; it does not launch a sweep. Distances below exclude no errors.

| GhostEnd source lever | Differing words / 110 | Extra nonzero words |
| --- | ---: | ---: |
| baseline | 3 | 0 |
| reuse `g` for the intermediate Ghost pointer | 23 | 0 |
| function-scope car pointer, computed before header load | 54 | 0 |
| unsigned loop variable | 5 | 0 |
| swap modelrun and ghost-slot clear stores | 5 | 0 |

The other experiments covered source-line coalescing, direct/void-pointer
spelling, unsigned car indexing, and condition-local alias lifetime. None
improved the baseline; empty comma/address expressions also changed nothing.
No failed probe is proposed as production source.

| Font-height source lever | Differing words / 18 | Extra nonzero words |
| --- | ---: | ---: |
| baseline | 4 | 0 |
| `u8locals` | 5 | 0 |
| `s32locals` | 5 | 0 |
| `s16locals` | 13 | 3 |
| `u8sum` | 4 | 0 |
| `empty_values` | 6 | 0 |
| `compound` | 8 | 1 |
| `metrics_lifetime` | 6 | 0 |
| `get_both_cond` | 6 | 0 |

The empty `if` probes are explicitly compiled-out shaping experiments, not
recovered semantics. They failed and are not recommended. They are retained in
the replay only to prevent someone repeating them as an unexplored idea.

## Pass ownership, not a match claim

GhostEnd retains the previously documented copy-propagation residual at
three sites, offsets 0x98, 0x9c and 0xac. The ghost copy and slot-clear exchange
order, and the header load uses the original value instead of the copied
argument. Stock ugen still emits the car-address calculation in the later
negative-flag block; as1 hoists it after its copy-propagation decision. A direct
array reference preserves the important alias region. A named car pointer
loses that region and regresses badly. This confirms the w36w2/w39b analysis,
rather than discovering a new match.

For the height helper, workbench diagnosis on locally relocated comparison
objects reports four register-only differences, zero opcode/gap differences,
and equal frame sizes. A fresh uopt trace in the current real slot context
locates the cause precisely:

- First-result web 5: saving 1.0, assigned t1.
- Font-header address web 15: saving 0.5, two occurrences, total saving 1.0.
- t2, t3, t4 and t5 each cost zero; the allocator chooses first-free t2.
- Native uses t4 for that address web. The other generated instructions agree.

Trace fidelity was checked: stock and unforced traced `.opt` outputs were
byte-identical, and stock versus traced final objects were byte-identical.
Using the diagnostic compiler's `CDX_PROC=15 CDX_FORCE=p1:w15=c11` makes the
height helper compare at 0/18. **This is an allocator oracle, not stock source
matching, and is not a claimed match.** It only confirms that this one coloring
decision accounts for the entire residual. No instrumented output or compiler
change is included in this packet.

Next useful work needs new evidence, not further root spelling sweeps:

1. GhostEnd: trace an evidence-backed source/context shape that computes the
   car address in the main block before the header load, while preserving its
   direct-index alias region. Moving it in an assembly listing is not a fix.
2. Font pair: explain why two lower-cost registers are unavailable natively,
   via authentic inlining/liveness context. Root local temporaries and empty
   value tests did not supply that explanation. Do not fabricate phantom values
   or modify the accepted getter/setup bodies merely to reserve registers.

## Reproduction

Set `IDO_DIR` to the stock 5.3 directory and put GNU MIPS binutils on PATH, as in
`CloudHandoff.md`. From repository root, use fresh output directories:

```sh
python3 cloud/work/frontier/dot_ghost_font_20261022/replay.py ghost \
  --out build/dot-ghost-font/ghost
python3 cloud/work/frontier/dot_ghost_font_20261022/replay.py font \
  --out build/dot-ghost-font/font
```

Each stages unchanged context and calls `tools/cloud/score.py group` directly;
expected exit status is 1. `claims` is empty and no `--claims` shortcut is used.
The script refuses to overwrite an existing output directory. `--stage-only`
stages without scoring. For a single negative probe, add e.g.
`--variant reuse_g_intermediate` (ghost) or `--variant u8locals` (font), using a
new output directory. Source replacements are checked for exact occurrences.

Validation: Python compilation, both complete baseline canonical runs, and the
12 retained probe replays. No source-install tests or whole-game verification
are claimed because nothing is being installed.
