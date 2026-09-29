# F7 validation — 2026-09-29

Group scoring now reports `Members` and `Context` separately. Only member
results determine the exit status; context differences, excess instructions,
and missing context targets remain visible as diagnostics.

`compile_group` uses `TemporaryDirectory` and cleans intermediate files on
success and failure. It resolves the requested output path before invoking
the compiler in the scratch directory, so relative output paths also survive
cleanup.

## Acceptance

On watchman2 with IDO, in an isolated snapshot of the repository:

```sh
python -m pytest tests/conveyor/test_cloud_score.py -q
```

**163 passed**, including all locked single functions, both groups' members,
both group CLI commands, member/context exit-status combinations, and cleanup
after compiler and process-launch failures.

The real `score.py group src/blob/groups/entity_flag_check` command exited 0:

```text
Members:
entity_flag_check:
  MATCH

Context (informational; excluded from exit status):
func_800988D8:
  87/95 words differ (9 extra words (nonzero beyond target length))
```

Instruction-level diagnostics are omitted from the excerpt above.

On the Pi, the Conveyor suite excluding `node_required` tests finished with
**350 passed, 131 skipped, 5 deselected, 1 pre-existing failure** in
`test_closure.py::test_populate_keeps_suffix_row_conflicted_against_discovered_extent`.
The compiler-dependent cases skipped on the Pi passed on watchman2.
