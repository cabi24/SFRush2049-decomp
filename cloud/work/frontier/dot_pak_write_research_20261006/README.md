# Pak write/retry routine: captured inputs and error branches

Research candidate, explicitly NONMATCH; no accepted coverage or ROM claim.
Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Target: `track_data_decompress`, **652 bytes / 163 words**.
Prior source: `cloud/work/game_C96/group_bounds_assignment.c`.

Canonical positional differences improve from **156/163 to 101/163**. The
original local C96 body is 616 bytes; the refined ELF procedure is **648 bytes**,
still four bytes short of the target. Its frame is now the target's 112 bytes.
The canonical scorer also reports **one extra nonzero word** beyond the target
comparison range, which is retained as a warning, not suppressed or treated as
an acceptance pass. There are no unresolved/unverified references or relocation
errors. The mismatch and boundary limitations remain substantial.

The refinement follows observed native dataflow:

- Capture the file's slot number and allocation handle before writeback, instead
  of keeping the file pointer live and rereading its slot during retries.
- Form the channel pointer only after the dirty-run scan finds work; retain the
  separate slot pointer for clearing its modified byte.
- Read `handle->file` directly in the initial phase rather than retaining a
  redundant local alias.
- Use the genuine queue-unlock operation already reconstructed in PR #210.
- Preserve the two separate error-3 assignments for the flag and state cases,
  rather than merging their conditions into one OR expression.

The underlying read widths, callback inputs, retry decision, write arguments and
range-marking calls remain grounded in the complete native body. No padding,
invented argument, stand-in caller, volatile change or forced register is added.
Helper boundaries remain hypotheses. Required genuine context includes the Pak
flush refinements from PRs #210/#213; none is an additional claim here.

## Reproduce

With IDO 5.3 available through `IDO_DIR`, from the repository root:

```sh
python3 tools/cloud/score.py group cloud/work/frontier/dot_pak_write_research_20261006
```

Exact flags: `-g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul`.
The canonical pipeline includes `-Olimit 5000` and `as1 -r4300_mul`.
No broad tests, independent acceptance replay, image/ROM integration or CI
watching was performed. The independent checker owns acceptance and merging.
