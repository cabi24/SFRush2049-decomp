# Current game multiply-errata resweep — 2026-10-01

The authoritative current layout contains 92 functions with COP1 single/double multiply instructions. Twelve are already image-locked. Of the remaining80, 79 sources were obtained from current read-only candidate evidence or fresh m2c generation; one seed generation failed. Selection used immutable retail image words and current extents, rather than the old handoff count of90.

All79 were independently compiled on Rocky with exact flags `-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul` through the strict `score.py fn` path. **No MATCH**:56 compiled strict nonmatches and23 compile/score failures. Approximate m2c seeds with undefined stack variables or mistaken prototypes are failures, not zero results. No sources were spliced, no locks changed and no coverage claimed by this sweep.

The JSON table records all92 selections, status, source/context/target hashes where available, exact flags, return codes and strict verdicts. Full compiler diagnostics and generated seeds remain ignored/private in build/codex-r4300-sweep and Rocky D scratch. The closest six fresh42-word leads (six or nine differing words) were delegated to B7 for assembly-grounded repair. No blind line-reflow batch followed this sweep.

Static GU evidence is separate: static Makefile compilation already supplies the same errata flag globally, while historical isolated object verification omitted it. Explicit flag verification closes three static GU object matches; these require their own lock/shared-TU/ROM acceptance and are not game sweep matches.
