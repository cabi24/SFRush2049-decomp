# Image B DA78: complete position-hit helper reconstruction

**Research only. Complete natural C and bounded native behavior, not a native
private-ABI match or accepted-byte claim.**

Base: `6b2e9e506fe3d2267a710e41c85af5364ccd00c7`.
Native interval: `[0x8038DA78,0x8038DDDC)`, 868 bytes / 217 words.
SHA-256: `7c7128167920d9661e579931d669aa4821c940234e8d7ed09ee89235bc7b2dcf`.

## Recovered behavior and actual context

`position.c` reconstructs the complete player-hit selection helper. It rejects
record kind 3 and near-zero horizontal motion, scans eligible players, projects
them onto the original movement segment, checks inclusive vertical/horizontal
bounds, and clips the record's position to the nearest accepted projection.
Equal fractions replace the previous result, so the later player wins a tie.
It returns the complete player pointer, or null.

The native child receives its record through `s1` and previous-position pointer
through `s2`. It saves only `ra` in its 144-byte frame and writes unsaved
`s0/s3/s4/s5/s6/s7/s8` and `f20/f22/f24/f26/f28/f30`. The two explicit C inputs
are a semantic adapter; they do not establish the original formal order or an
ordinary standalone ABI. No fake ordinary admission, context stub, synthetic
keeper or invented clobber mask is used.

Both direct calls in the authenticated B text are from E114: `+0x9B0` passes
record+0x2C for kinds 4/6, while `+0xA08` passes the real E114 local position
snapshot for other non7 kinds. Both sites spill live registers and reload
constants around the child. FCE0 is the actual ordinary caller above E114.
The complete genuine source family and original translation-unit visibility
must still be established before attempting private-context matching.

Important source semantics:

- Player count is signed16 and reloaded each iteration. Owner, active, blocked,
  and kind fields are signed bytes; the vehicle state is signed16 and must be
  exactly -1. Record+0x10 serves as collision radius here.
- Delta remains the entry movement vector. The native horizontal dot expressions
  include the y-times-zero term, with their arithmetic association preserved.
- The accepted vertical band is `[-radius,3.5+radius]`; horizontal distance must
  not exceed `(radius+6.75)-0.5`. A negative radius is not silently clamped.
- Kind5 player hits recompute `previous - record.position`, transform by the
  player's basis, and set record flag bit0 when absolute angle is strictly
  below 1.35. This reverse vector sees position clipped by any earlier hit.
- Position is written after the kind5 calls. The earlier flag bits are preserved.

The repository's documented arcade checkout is absent at this base. No arcade
original or source-name identity is established for this N64-specific helper.
The reconstruction follows the authenticated native control flow and layouts.

## Verification

The source uses the exact requested bare O3 header and unchanged canonical
`score.compile_single`, including its automatic `-Wab,-r4300_mul` backend flag.
The one complete ordinary-ABI function is 996 bytes, plus 12 verified zero
alignment bytes and 16 bytes of owned rodata. Whole-object symbol, relocation,
allocated-section and linked-address evidence is recorded in `verification.json`.
All five referenced external bindings are checked; fabs/sqrt compile intrinsically.
No native instruction slice, patched object, executable neighbor, inline assembly,
volatile shaping, artificial pressure locals or declaration sweep is used.

The research-placement diagnostic differs in 215/217 native word positions,
with 32 nonzero candidate words beyond the native extent. This is an explicit
broad NONMATCH. It includes research-placement relocations and is not strict
native owned-data placement or evidence for register-allocation tuning.

The fail-closed interpreter compares return pointer, complete mapped nonstack
state and ordered helper-call arguments/state digests for 1,802 paired fixtures.
It poisons ordinary caller-save registers across helper calls and checks the
candidate's conventional preservation. Every reachable native instruction is
executed: 216/217 words. Native+0x20 is a duplicate load unreachable behind an
entry likely branch. Candidate coverage is 248/249 words; candidate+0x208 is an
annulled initial loop-peel slot for a fixed three-element vector. Exact exclusions
are asserted and explained in the receipt; full target extents remain authenticated.

Fixtures include nonpositive signed counts, signed field boundaries, rejected
players, negative radii, segment/height/distance boundaries, denominator threshold
neighbors, angle threshold neighbors, nearest/equal hits, real previous-position
aliasing, nonidentity bases and seeded geometry. Twelve independently compiled
wrong-source controls are rejected; unknown opcodes are refused. Additional
native contract tests assert expected return/position and strict-angle behavior.

`func_800A61B0` is modeled as its exact rounded row-dot product. Its full 148-byte
native identity and historical production source are checked. The 268-byte
`func_8008C768` identity/source is checked, but its atan-like approximation and
subordinate math routines are not executed. Host atan2 or supplied boundary
outputs provide the same bounded helper result to both executions. Adversarial
callback mutations probe later reads and snapshots; they intentionally expand
known helper effects and do not claim those writes occur in the native helpers.

## Reproduce and integrate

With the pinned IDO toolchain and MIPS GNU linker:

    python3 cloud/work/runtime_b_da78_position_20261006/verify.py --check
    python3 -m pytest tests/cloud/test_runtime_b_da78_position.py -q

A source-only overlay can pass `--reference-root /path/to/repository`; tests
use `RUSH_REFERENCE_ROOT`. Choose a unique `TMPDIR`; ordinary cleanup stays on.
Historical asset/helper/extent context is read using `git show BASE:path`.
Live selected native words come from `score.targets()[FN]`. Receipts bind only
the packet's own sources and proof fields; there are no mutable production,
manifest or scorer hashes and no live lock-state assertions. Tool provenance is
recorded separately and excluded from portable replay equality. The test file
is not hashed into its own receipt. Compiler-dependent replay skips cleanly if
IDO or the GNU linker is missing. `--source` is deliberately unsupported so a
receipt cannot claim to bind a different compiled input.

The coordinator must complete independent frozen-source review and the required
current-master full suite with and without IDO before publication. This packet
does not change the active matrix, accepted files, production source or gates.

## Limits

Tests admit aligned initialized live disjoint record/player/vehicle storage,
a previous pointer aliasing record+0x2C or separate storage, and a signed count
of at most four mapped players. This is a bounded proof domain, not a claim
that four is the actual global allocation bound. Zero and normal finite
binary32 inputs/intermediates/results use default rounding. Overflow, underflow,
denormals, NaN, infinity, exception/FCSR effects, invalid pointers, arbitrary
aliasing, concurrent writes and exact N64 angle approximation are unproved.

This supplies one complete missing body for genuine FCE0/E114 closure work.
It does not claim original source/TU visibility, private-ABI MATCH, accepted
coverage, full-game behavior, image composition, compression or ROM identity.
