# func_80012D18: branch-merge and frame-home audit

Research only, based on master `55e179413`. **No matching submission, no new
coverage, no promotion, no production source or lock edits.** The w23a baseline
is retained. Generated experimental C, objects, native listings and diagnostic
compiler binaries stay in ignored scratch directories.

## What this changes

1. The full protected native extent is **3,148 bytes / 787 words**, ending at
   `0x80013964`. Stock IDO 7.1 emits a **3,124-byte / 781-word** baseline body,
   plus twelve separate zero alignment bytes. The canonical and independent GNU
   comparisons both report **674/787 differing words**. The historical
   “seven missing rows” is not seven differing instructions, nor the true body
   deficit: the candidate is **six body words short**.
2. A genuine post-branch carrier does not necessarily replace its upstream
   expression web. `position_merge`, `excess_merge`, and `both_merge` give the
   named locals multiple reaching definitions and a real use after the merge.
   The variable webs survive, **alongside** the conversion and excess-expression
   webs. Four expression homes remain; the frame is 128 bytes. Merely asking
   for “a surviving named variable” is therefore not a sufficient next lever.
3. The inferred missing-local identities are not established by frame arithmetic.
   Caching the actual loop-start and loop-length fields provides a counterexample:
   it has **two expression homes**, the native **120-byte frame**, and the native
   command/state/address/length/padded homes, while unsigned position and excess
   remain expression webs. It is much worse code (**743/787**, fourteen body
   words short), so it is not a candidate. The observation refutes uniqueness,
   not the possibility that the original locals were position/excess.
4. There is no demonstrated source improvement. The lowest positional score
   here is `position_rebase` at 631/787, but it has a 128-byte frame and is seven
   body words short. Its changed layout makes that scalar score unsuitable as
   a claim that the native structure is closer.

## Bounded experiments

All use `lib_11640`'s verified stock IDO 7.1, flags
`-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul`, and project include paths.
`experiments.py` derives exactly twelve sources from committed ancestor seeds;
there is no randomized source search, oracle-patched candidate, invented helper,
padding, new volatile assumption or cross-target edit.

| Source | Full differing words | Body bytes | Frame | cfe area | Expression homes |
|---|---:|---:|---:|---:|---:|
| unchanged w23a baseline | 674/787 | 3124 | 120 | 68 | 4 |
| w23a named-position probe | 677/787 | 3124 | 128 | 76 | 4 |
| historical w26b declaration order | 666/787 | 3124 | 120 | 72 | 4 |
| position carried through address merge | 695/787 | 3112 | 128 | 76 | 4 |
| excess carried through length merge | 679/787 | 3112 | 128 | 76 | 4 |
| both carriers | 706/787 | 3080 | 128 | 76 | 4 |
| real loop-field carriers | 743/787 | 3092 | 120 | 76 | 2 |
| subtract remaining range | 677/787 | 3124 | 128 | 76 | 4 |
| add position before subtracting end | 651/787 | 3120 | 128 | 76 | 5 |
| explicit outer guard / plain-range label | 666/787 | 3132 | 128 | 76 | 4 |
| rebase position in whole-loop arm | 631/787 | 3120 | 128 | 76 | 4 |
| explicit nested excess branch | 677/787 | 3124 | 128 | 76 | 4 |

The diagnostic compiler is built from decompals' v1.2 generated 7.1 uopt/ugen C,
whose hashes match the existing trace71 pins, using the repository's existing
instrumentation patches. **All twelve traced whole objects are byte-identical
to stock objects with CDX, interference, spill and caller-cost tracing enabled.**
No force switches are used. The trace receipt stores decisions and geometry,
not native instruction words or raw assembly.

The baseline first mismatches are the command-pointer home (72 rather than 64)
and the `frames` register (a2 rather than a1). Baseline uopt makes position
conversion expression w348 v0 and excess expression w358 a1; frames w19 a2 is
colored afterward. In the merged-carrier variants, genuine position/excess
variable webs coexist with those upstream expressions. The field-carrier
counterexample removes the early field-load homes and lets later expression
homes reuse their slots. It still grows one extra temporary after spill-home
allocation; its final local area is 88, not proof of an original declaration list.

## Verification and boundaries

`verification.json` is protocol-v2 input-bound: owned source/proof files,
generated source hashes, ancestor seeds, semantic headers, parsed protected
native targets, selected symbols and the static caller edge. Shared scorer,
Conveyor, build files, locks and raw-assembly formatting are not pinned.

- Every object is compared canonically and independently linked by GNU from its
  original full ELF. Each has 24 original relocations, resolved named externals,
  no owned data, no uncertainty waivers, separately checked STT_FUNC extent and
  zero alignment suffix. Missing body words are counted as missing, never
  borrowed from alignment or a neighbor. All twelve remain NONMATCH.
- Full-body static ABI checks require balanced stack allocation/release, matching
  s0/ra save/restore slots, and the complete ordinary return/delay-slot tail.
  Existing accepted helper sources and contracts are unchanged. The incoming
  static caller is `func_800139D4+0xA4`; no expanded caller census is claimed.
- A small fail-closed interpreter runs the authentic native and all twelve
  GNU-linked **range-planning prefixes**, ending immediately before the flags /
  format dispatch. **1,280 fixtures / 16,640 executions** cover all six range
  leaves, finite low/high-bit unsigned conversions, arithmetic wrap boundaries,
  command/floor helper arguments, caller-save GPR clobbers, stack homes and FCSR
  restoration. External helpers are controlled hooks, not executed implementations.
- The unchanged planning statements are also extracted into a logical host
  wrapper. All twelve prefixes pass the same **1,280 cases each at O0, O2 and
  ASan/UBSan/bounds**, with sanitizer recovery disabled (LeakSanitizer alone
  disabled for this container). Native structure layout is not inferred from
  host pointer size.
- Fresh wrong-filter-margin C fails prefix behavior. An added data allocation
  fails the complete-object gate. Unknown instructions and unmapped memory fail
  closed. Normal and optimized-Python verification must agree.

**This is not full-function behavioral equivalence.** The format-specific sample
fetches, envelope work, volume arithmetic, final position update, helper internals,
nonfinite/out-of-range floating inputs, unsupported formats, arbitrary aliasing,
concurrency, hardware and gameplay are not proved. The static return check does
not turn partial execution into a full dynamic ABI proof. No standalone match
or cartridge identity is claimed.

## Reproduce

```sh
python3 cloud/work/frontier/dot_static_12d18_20261013/verify.py --deep
python3 -O cloud/work/frontier/dot_static_12d18_20261013/verify.py --deep
REQUIRE_TOOLCHAIN=1 python3 -m pytest tests/cloud/test_static_12d18_research.py -q -o addopts=''
```

The one default test checks inputs and freshly compiles the stock baseline;
expensive prefix replay and diagnostic traces are explicit deep work. To rebuild
and recheck the optional traced compiler locally:

```sh
sh cloud/work/frontier/dot_static_12d18_20261013/build_trace.sh
python3 cloud/work/frontier/dot_static_12d18_20261013/verify.py --deep \
  --trace build/static-12d18-tracer/ido/cc
```

Diagnostic executable hashes are provenance, not an imposed host compiler build.
Every trace replay must independently pass whole-object stock identity and
reproduce the saved decisions. Publication remains a draft research packet;
installation, cartridge gates and merging belong to the independent checker.

## Next justified step

Trace the **earliest common use before the range branch** and the basic-block
interference that keeps conversion/excess separate. A useful next experiment
must replace an upstream expression web, or prove why its home can be shared,
without deleting native reloads or changing the effective algorithm. The native
switch-join and mainRem/loopRem allocation differences remain downstream;
declaration permutations and post-merge carriers alone do not close them.
