# Image-B resource-selection helpers: strict group match

## Source-first deliverable

The source group passes the current selective installer body path in disposable
staging. Full production image build and ROM gates remain unrun. Base is fresh master `4f59607f7b15e252cfcbab67b9eccb6d9e6afba5`.

- New substantive helpers: `func_80390B10` and `func_80390D38`, 552 bytes each,
  **1,104 newly strict-matched candidate bytes**.
- Two separately accounted deleted-procedure stubs: `func_80390B00` and
  `func_80390B08`, eight bytes each. All four exact emitted bodies are in both
  `members` and `claims`; total 1,120 code bytes.
- Complete genuine parent `func_80390F60` is **NONMATCH context: 5/247 words**,
  988-byte whole extent, no extra words. It is the only kept function.
- Zero prior preserved members, zero owned bytes claimed, zero accepted-image
  or ROM-verified bytes. Existing integrated B matches and pending B:80391650
  are excluded. No locks or installed sources change. Keep the PR draft;
  installation and merging belong to the independent checker.

## Trace and authentic boundaries

These are fresh reconstructed bodies, rather than another attempt at the frozen
925D0/92894/92FE4 residuals. Both helpers manage a timed object selection: wait for
an active object's release, reset other entries of their resource kind, or select
an eligible wrapped slot and expose its new model/texture state. The real outer
caller first invokes D38 then B10, updates player alpha/countdowns, and performs
the third selection mode. No whole-function arcade donor is established.

The first natural source inlined B10 incorrectly; D38 was 131/138 differing
words and the parent remained broad. Workbench and retained stock UGEN were read
before source changes. The adjacent caller-less B00/B08 stubs plausibly own the
alpha and third-selection blocks. Giving them those real bodies causes stock
umerge to inline both and leaves both substantive helpers internal, reproducing
the native caller-owned s0/s1/f20 preservation. **Their original identity is an
inference**, not a claim to have recovered original names or the whole TU.

The remaining substantive-helper mismatch was an early address materialization:
ordinary direct-scalar delta-time syntax uses an assembler temporary base,
whereas native code materializes the full address into its ordinary temporary
ring. A target-width pointer integer round-trip restores that observed form.
Under the unmodified stock compiler both full helpers then match. The causal
rollback returns each to 131/138; no compiler force or modified scorer supplies
acceptance. `unsigned long` is 32-bit on the target and pointer-width in the LP64
host fixture, so the test does not silently truncate host pointers.

The parent pointer traversal restores live player-count reloads; its guard gives
the pointer the native lifetime. Spelling its phase-a zero as `0.f` separates
that constant web from the alpha loop. The remaining five offsets are +0x34
(table home sp60 versus sp56), +0x5c/+0x60 (two address instructions reversed),
and +0x384/+0x390 (byte-offset spill/reload sp44 versus sp48). The actual 88-byte
frame already agrees; no unused filler or capacity change was added. Naming or
reusing phase carriers, changing helper declaration order, and naming the byte
offset did not close that combination. Stop ordinary layout guessing pending a
grounded local/inline-scope trace. The parent receives no matching credit.

The direct native census is B10 from F60+0x18, D38 from F60+0x10, and neither
stub has a direct caller. The game calls F60 only from render_viewport_init+0x68,
without incoming argument setup. RNG uses the established f12/f0 float boundary;
90770 is the accepted signed-halfword/unsigned-halfword setter; the accepted
model_transform_setup is the real three-argument scene helper. The overlapping
scene word and +14 signed index are explicitly target-big-endian union views.

## Strict score and complete proof

Run the canonical scorer wrapper without `--claims` or `--allow-unverified`:

```
python3 cloud/work/frontier/dot_runtime_b_resource_callbacks_20261012/verify.py --score
```

Exact member output:

```
Members:
func_80390B00:
  MATCH
func_80390B08:
  MATCH
func_80390B10:
  MATCH
func_80390D38:
  MATCH
```

The informational complete parent prints `5/247 words differ`; authenticated
own-data binding removes all unverified/unknown references and errors.

- Whole object: five accounted STT_FUNC extents, 2,108 meaningful text bytes,
  complete .text 2,112 with four checked zero alignment bytes. No hidden
  procedure, dropped suffix or unsupported allocated storage is accepted.
- Native next boundaries are checked independently of source names. Deleted
  stubs are emitted in reversed order; each is relocated by its actual symbol
  span, not by an assumed order.
- Independent original-REL GNU reconstruction resolves all 48 text relocations
  (10 in each substantive helper, 28 in the parent), preserves all five whole
  extents and finds exactly the same five parent differences. Every claimed
  word is exact. Undefined symbols, residual relocations, unknown allocation
  and unexpected section-relative references fail closed.
- The parent's eight-byte alpha table at 0x803942B8 is verified against the
  authenticated current image, with its eight zero section-alignment bytes
  checked separately. This table belongs to NONMATCH context and is not claimed
  for installation. Missing and corrupted context data fail closed.
- Receipts bind the exact source/recipe/verifier/host/test, selected native
  words and extents, external symbol bindings, real incoming-call census and
  helper targets. All eight actual IDO pass binaries are fingerprinted, and the
  compiler family is recognized IDO 5.3. No static ROM TU is rebuilt. There are
  no shared scorer/pipeline/lock/Makefile/generated-assembly hash pins and no
  historical non-ancestor Git replay.
- Stock diagnostics leave the entire object byte-identical. The two helpers
  have M0/T16/P4; the parent has M48/T16/P4. Umerge records exactly the two
  inferred deleted-procedure bodies being inlined.
- Actual-source logical-host tests pass 600 selection cases and 5,120 alpha
  cases, including callback pointer mutation, alpha subtraction wrap, all
  countdown-table positions, inactive players and positive/expired timers.
  The same matrix passes UBSan/bounds with strict aliasing. Tests compare whole
  typed records and opaque fields. They do not execute retail code, real
  external helpers, or establish native layout/target-endian union behavior.

## Selective installation and private ABI

A read-only replay uses the actual `ovl_splice.packet_spec`,
`blob_group.group_bodies` and `ovl_splice.mismatch` functions with this same
stock object in disposable staging. Output is exactly the four claimed
bodies; all pass their full native extents and F60 is explicitly excluded.
Selective substitution into the authenticated image leaves every byte equal,
including the original native F60 caller. The production group path explicitly
allows unspliced context to remain nonmatching. No protected source, lock,
production artifact, image build or ROM operation is written/run by this proof.

This is not a stand-in-caller-only match: the real native F60 already owns the
same s0/s1/f20 saves and no-argument calls reproduced by the genuine context.
Its five remaining differences do not alter those boundaries. Thus the local
selective installer test exposes no caller-dependency blocker for the four
claimed slices, while F60 itself remains unclaimed. The independent checker's
normal complete image build and full-ROM gates are still required before
crediting accepted or ROM coverage.

Independent paired review approved the original frozen source and proof:
normal/-O deep replay, strict score, focused test/absent-IDO skip, UBSan/bounds,
48-relocation GNU gate and source/ABI audit. The added selective-installation
replay is separately rechecked before draft publication.

## Replay and limits

```
python3 cloud/work/frontier/dot_runtime_b_resource_callbacks_20261012/verify.py --deep
python3 -O cloud/work/frontier/dot_runtime_b_resource_callbacks_20261012/verify.py --deep
REQUIRE_TOOLCHAIN=1 python3 -m pytest tests/cloud/test_runtime_b_resource_callbacks.py -q -o addopts=''
```

The single focused test passes in 0.51 seconds; with IDO deliberately absent it
cleanly skips in 0.41 seconds after validating input bindings. Deep GNU/host and
stock-trace experiments remain optional and outside the cheap collected test.

Valid backed signed counts/selection indices, a reachable eligible wrap-loop
entry, phase-3 countdown 0..7, finite RNG/timers and representable conversions
remain caller contracts. The native loop has no termination guarantee when no
eligible entry exists. Byte subtraction underflow is intentionally preserved.
Host handle views use the host union representation explicitly, and do not
assert that arbitrary native big-endian aliases are reproduced by LP64 tests.
No asynchronous mutation, full gameplay, image splice/compression, hardware
execution, original source identity or cartridge hash proof is claimed.
