# E114: complete runtime-B object-update semantic source

Research only. **COMPLETE-SEMANTIC-SOURCE / NONMATCH**, not an accepted replacement,
original-TU reconstruction, private-context match, or ROM-coverage claim.

Base: `6b2e9e506fe3d2267a710e41c85af5364ccd00c7`.
Native interval: B `[8038E114,8038F560)`, 5,196 bytes / 1,299 words.
Image SHA-256: `b55fc2d1b22eb1ebdf01286a69a181b496da7b45ff7aec888ff74b7db748e7cd`.
The target is authenticated against the base-commit asset in memory; no extracted
image, ROM bytes, raw assembly, or compiled object is part of this packet.

## Source milestone

`update.c` reconstructs the entire body previously mapped by the first E114
parent packet. It uses real fields and structured control flow, with shared
in-function movement, impact, and object-update joins. It contains no fabricated
caller, register-pressure fields, padding arguments, clobber masks, stub body,
allocator tuning, or source-shape sweep. Original variable names are unknown.
The source follows authenticated N64 battle-object behavior; earlier ancestry
research found no whole-function arcade donor for this N64-specific system.

Complete regions:

- Capture next record before callbacks, including destruction/release paths.
- Kind-3 zero-timer attached-player states: world offset and rotation, validity
  checks, primary/secondary scene objects, unsigned alpha minimum, texture
  selection, hold scaling, launch sound/ammunition/velocity/lifetime.
- Common lifetime and timer update, expiration and release.
- All nine motion kinds plus unsigned out-of-range default: gravity/damping,
  velocity integration, steering/forward motion, accelerating kind 6, and the
  kind-7 branch that requires no geometric radius value.
- Attached-effect position/flags; private DA78 collision with the appropriate
  actual prior-position input; world collision; kind-3 rest/bounce; kind-2
  bounce or explosion; kind-4/6 explosion; direct player damage and quad effect.
- Full quad corners with original floating-point operation order and resource
  loading/allocation/failed-create release. The A78BC result is an opaque
  quad-record pointer, not a scene index.
- Kind-7 private D498 and scene scale update, full scene-index word addressing,
  exact binary32 tick constant, and three diagonal matrix stores.
- Prior-position retention, all object creation/transform cases, flag-phase
  priority 0x10 then 0x20 then 0x40, and list advance.

The lazy native f22 scratch value is represented by `radius`, deliberately
uninitialized until the relevant kind sets it. Kind 7 bypasses every arithmetic
use. The source does not read a made-up formal or initialize this value merely
to mimic a stack spill. Disjoint-storage and helper-boundary assumptions are
explicit in `contracts.json`.

## Genuine context remains separate

E114 saves only ra and uses unsaved s0-s8/f20-f30. FCE0 is its genuine ordinary
caller and preserves these registers. The ordinary-ABI entry compiled here is
solely a semantic test interface, not evidence that E114 was originally a kept
or exported compiler root. Private helper argument order is readable source
notation, not recovered original declaration order.

Minimum known closure is FCE0 3,056 + F938 928 + E114 5,196 + D200/D328/E088
560 + DA78 868 + D498 768 = **11,376 native function bytes**. FCE0, F938 and the
three small children have separate semantic source packets. DA78/D498 still
need full authentic source. DDDC is an ordinary boundary, not another private
helper to add for register pressure. Original TU/export visibility and all
whole-context matching/integration gates remain open.

The shared E398 return-type inconsistency identified by the child packet is not
changed here. No protected source, header, target, lock, tool, compiler flag
policy, production rule, or accepted byte changed.

## Verification

From a current checkout with pinned IDO and MIPS GNU binutils:

    python3 cloud/work/runtime_b_e114_update_20261006/verify.py --check

For a source-only overlay, pass `--reference-root /path/to/repository`.
Use a private TMPDIR if desired. Temporary objects are automatically removed;
only the requested JSON receipt is retained. The canonical scorer adds its
mandatory R4300 multiplication backend workaround; this packet does not bypass
that route. Bare source recipe: `-g0 -O3 -mips2 -G 0 -non_shared`.

The receipt binds only packet-owned implementation/verifier/layout files,
authenticated native words/data, full compiled function and allocated sections,
complete relocations, independent GNU link, and bounded behavior. It does not
pin integration-sensitive source/manifests/scorer/locks, assert current lock
state, or hash its test file. Production context is consulted at the recorded
base with `git show`.

The complete ordinary-ABI compiled function is 5,252 bytes and is **NONMATCH**.
No instruction equality claim follows from a semantic link at a research address.
Every emitted allocated section is inspected; GNU resolves all referenced
externals and candidate-local data, and unresolved relocations fail closed.
The IDO layout test checks 42 source-view sizes/offsets. BEffect is only the
28-byte consumed prefix, not a claim about its real allocation size.

`verification.json` records 1,511 paired cases, 1,292/1,299 native words,
1,300/1,313 candidate words, branch outcomes, and nine compiled wrong-source
mutations plus three fail-closed controls. Each pair compares full mapped
nonstack state and ordered helper calls with state snapshots. Cases exercise all
raw flag bytes, every motion kind/default, collision combinations, hold/launch,
expiration, effect/object/resource presence, signed owners in bounded range,
unsigned alpha boundaries, full scene indices in bounded range, callback
mutations, and saved-next destruction. O32 hooks poison caller-save registers;
private hooks additionally poison conservative supersets of observed unsaved
state as a stronger bounded preservation stress test, not a recovered clobber mask. Vector
transform models use ordered binary32 operations from base-commit accepted
A61B0/E820 sources; other external effects remain explicit bounded models.

Seven native words are duplicated, statically unreachable compiler landing
operations; their offsets are listed in the receipt and independent review.
All remaining native instructions are exercised. Candidate unreachable words
are reported without inventing coverage. Wrong-source mutations and null
allocation/unknown-opcode controls fail closed. This is a bounded behavioral
proof, not original-game execution, sanitizer coverage, or a full-ROM test.

## Domains and exclusions

Records are initialized, aligned, nonaliased, acyclic 104-byte objects with valid
owners; player/vehicle strides are 952/2056. Live primary/secondary scene objects
are nonnull where dereferenced; object/quad allocations succeed. Models, player
texture mapping, texture banks and full scene indices are in bounds. Model
shapes, matrices, scene table and texture fixtures do not prove real asset
availability. Scene color calls narrow handles to signed16; kind-7 scene lookup
uses the full 32-bit index. A78BC may return null and that result is handled;
the preceding pool allocation has no null guard, matching native behavior.

Floating-point proof covers finite normal values and zero, normal/default
rounding arithmetic, and conversions within signed32 range. FCSR exceptions,
signaling NaNs, payload behavior, subnormal arithmetic, overflow, invalid
indices/ownership, arbitrary aliases, concurrency, real collision/gameplay,
private-helper bodies, full original context and all compression/ROM gates are
outside the proof. Hooks do not establish arbitrary real service mutation
permissions; the independent review probes additional bounded callback effects.

Focused tests are portable with absent IDO or linker and skip the replay cleanly.
The coordinator owns aggregate with/without-IDO suites and publication only
after this packet freezes and independent review concludes.
