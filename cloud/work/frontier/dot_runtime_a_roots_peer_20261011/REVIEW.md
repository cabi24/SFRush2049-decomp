# Independent availability-root review

Reviewed `d679ab326` plus `95dacc28e` and final verifier hardening at exact
commit `c38827123fb44701991088c5b00e0da8635a8d6b`.
**Approved as bounded NONMATCH research, with zero new matching or accepted bytes.**

Fresh normal and optimized-Python verification agrees with the frozen receipt
apart from elapsed time. Fresh stock compiler metadata agrees exactly with its
separate receipt. All five focused tests pass, including source/scope rejection,
missing member/context rejection, decoder guards, normal import isolation and
injected-failure search-path restoration. The reviewed worktree is clean and
`git diff --check` passes.

The complete 1,292-byte 94058 root remains 2/323 words different, solely its
120-byte versus native128-byte frame adjustment/restoration. All 84 root
relocations independently resolve through GNU. The five inherited helper
bodies remain strict matches without duplicate credit. Complete source and
call-sequence review confirms authentic 923B0/93B3C dependencies; those two
FP bodies and unchanged9338C remain nonmatching with their compiler extents,
extra words and positional owned-data failures explicitly retained.

The entire 48-byte compiled literal section equals a unique authenticated
non-code image interval. This content-ownership proof does not override the
canonical mismatching bodies' positional literal failures. No payload bytes
are published in the peer receipt.

The 1,024 native/GNU root boundary cases reach 307/323 instructions, 75 branch
outcomes and every one of 14 callback targets. Non-stack memory, ordered
arguments and preserved integer/FP registers agree. Child routines are
explicit boundary fixtures, not executed implementations. Stack temporaries
differ with the frame and are not hidden as an exact-body result. The decoder
and source guards use explicit exceptions, and optimized replay retains them.

Independent stock-stage inspection confirms the source root's merged memory
area contracts to M2; UGEN reports T16/P4/M2. Stock diagnostic output is whole-
object-identical to the canonical build. Saves occupy24..111 and the signed-byte
homes118/119 are exact, leaving native bytes120..127 unused by the body.
Original missing declaration or expression-home ownership is not established.
No unused variable, buffer enlargement, synthetic caller or speculative empty-
helper implementation was added by this review.

Two review findings were fixed before approval: restoring the complete incoming
Python search path around both public verification calls, including failure,
and enforcing all nine scored members plus the five inherited context members.
This prevents a future manifest omission from silently dropping a body check.
The native/ELF-reader module aliases were already isolated correctly.

Reproduce with stock IDO and GNU MIPS tools configured:

```
python3 cloud/work/frontier/dot_runtime_a_roots_20261011/verify.py
python3 -O cloud/work/frontier/dot_runtime_a_roots_20261011/verify.py
python3 cloud/work/frontier/dot_runtime_a_roots_20261011/compiler_evidence.py
REQUIRE_TOOLCHAIN=1 python3 -m pytest tests/conveyor/test_runtime_a_roots.py -q
```
