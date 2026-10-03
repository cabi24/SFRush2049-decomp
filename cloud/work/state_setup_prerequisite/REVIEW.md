# Review and verification

Independent native/source review passed for the bounded FB378–FB5BC fragment:
fields/strides, selected slot versus ordinal lists, signed bounds, wrapping,
post-call reloads and final publication. The reviewer independently reran
strict C89 and ASan/UBSan checks and the focused pytest wrapper. The final
harness has eight scenarios plus layout assertions. It lives under tests,
not the recovered source corpus; all callbacks are test-only.

Final repository run on the submitted source/test contents:
**1,396 passed, 41 skipped, 9 node-required deselected** in 98.06 seconds.
Command: `python -m pytest tests/conveyor -q -o addopts= -m 'not node_required'`.
Used the existing pinned IDO, binutils and Python environment. Initial local
attempts lacked copied submodules and binutils shared-library configuration;
those setup problems were corrected. A packaging-only move of the harness
during one run invalidated its already-collected old path; the complete final
suite was rerun after the move and passed as stated above.

`make check-matched` passed: all 161 static lock records intact (not a coverage
count). Strict host C89 and ASan/UBSan passed. LeakSanitizer cannot run under
this container's ptrace environment, so leak checks were disabled for the
sanitizer run. No dynamic allocations occur in this harness.

No IDO score experiment, native differential execution, accepted game-source
mutation or private image/compression/ROM gate was performed. Independent
source review and host tests are not matching evidence. Evidence hashes bind
the fragment, layout, harness, wrapper and existing tracked native sources.
