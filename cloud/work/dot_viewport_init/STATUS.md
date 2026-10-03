# Default viewport initialization: NONMATCH research

Fresh target `func_800A5A40`, native extent `0x800A5A40..0x800A5B3C`
(63 words / 252 bytes). Current master base:
`a12daa63ae67c8dab3404efb666089c884dadfd4`.

This is a new tested reconstruction, not an accepted match. `claims` is empty.
The canonical strict comparison has **15/63 different resolved words**; the
candidate ELF function is 248 bytes, followed by two zero alignment words.
The first 43 words match exactly. The residual consists of post-call global
address register allocation, scheduling and the shortened return tail. The
full differing offsets, source/object/target hashes and resolution results are
in `verification.json`. No masks, unresolved symbols, extra nonzero words or
unverified relocations were used. No ROM or coverage claim, lock, promotion,
splice, build-wiring or protected target change.

## Semantic findings

The historical generic symbol names are retained. The operation appears to
initialize the default viewport, rather than an arbitrary audio rate. This is
an inference from the dimensions, bounds, pointers and callee writes, not an
established original name. The arcade reference submodule is unavailable in
this checkout, so no direct arcade equivalent is asserted.

- Signed width/height globals at `8002AFC0/C4` feed five stack float arguments:
  zero, width, height, signed integer width/2, signed integer height/2.
  Division occurs before conversion and truncates toward zero, including odd
  negative values. Four register arguments are index zero, viewport pointer,
  bounds pointer, and the context pointer at `80154188`.
- `arb_rate_set` reads argument slots at entry sp+16 through sp+32. The caller
  and callee have ordinary ABI; there are no implicit s-register inputs.
- The two flags are set before the callback. Width and height are read again
  after the callback, narrowed to unsigned halves. Native lhu +2 accesses
  prove low-half reads on big-endian N64; the portable C unsigned-short casts
  generate those accesses naturally when captured into real locals.
- Bounds receive zero left/top and the reloaded low halves right/bottom.
  The first two binding fields receive the viewport/bounds pointers.
  Layout names are inferred views, not assertions about complete types:
  Bounds is 8 bytes; ViewBinding covers only its first two native pointer words.
  Viewport is deliberately incomplete because this function does not access it.

## Causal experiments

1. Direct final field assignments from integer globals: 19/63 differences;
   initial 43 words exact, but full-word reload ordering differs.
2. Capture real post-callback low-half values before the stores: 15/63.
3. Explicit union big-endian halfword view: identical 15/63. Rejected for the
   final source as unnecessary; ordinary integer globals/casts emit the same
   native halfword reads and avoid host endian assumptions.
4. Genuine bounds/binding pointer locals and int-versus-short post-call locals:
   unchanged 15/63.
5. O1 control: 50/63 with 7 nonzero excess words and an unpaired relocation;
   rejected. O2 retained. Workbench diagnose confirms the shorter tail and
   address-allocation/scheduling residual; raw target relocation warnings are
   not accepted as evidence because the target object uses resolved words.

No fake helper, padding, extra argument, volatile barrier, pointer laundering,
artificial side effect or altered scorer was introduced.

## Reproduction and tests

Use the pinned IDO 5.3 installation from `tools/cloud/setup.sh`; set IDO_DIR
as an absolute path if installed elsewhere.

```
python3 tools/cloud/score.py fn cloud/work/dot_viewport_init/candidate.c func_800A5A40
python3 cloud/work/dot_viewport_init/verify_semantics.py --output /tmp/viewport.json
python3 -m pytest tests/conveyor/test_viewport_init_research.py -q
cc -std=c89 -pedantic -Wall -Wextra -Werror -O1 -g \
  -fsanitize=address,undefined -fno-omit-frame-pointer -DHOST_MAIN \
  cloud/work/dot_viewport_init/host.c -o /tmp/viewport-host
ASAN_OPTIONS=detect_leaks=0 /tmp/viewport-host
```

The scorer correctly exits 1 for NONMATCH. Semantic tests pass:
- 20,722 cases, 41,444 complete target/candidate instruction executions,
  compared to the literal portable host C. Signed extrema, odd negatives,
  16-bit truncation, float rounding boundaries, randomized values and
  post-callback mutations are exercised.
- 100,000 host cases under ASan/UBSan; leak detection disabled because the
  execution environment uses ptrace. Three focused pytest regressions pass.
- Interpreter rejects unsupported opcodes, checks mapped accesses/alignment,
  executes delay slots, validates all nine callback arguments, poisons
  caller-saved registers, and checks stack/callee-saved restoration.
- Single and three-member group canonical controls pass, all 161 static locks
  intact. Full repository test result and independent peer audit follow below.

Limits: host/interpreter agreement establishes this bounded model, not real
N64 execution, exception flags or alternate FCSR rounding modes. Float
conversion assumes round-to-nearest. Callback effects are modeled, not the
complete callee implementation. No image splice, compressed ROM identity,
full-ROM hash or `make test` was run without a private ROM.

## Repository validation

Full `tests/conveyor -m 'not node_required'` run exited zero: 1,310 passed,
41 skipped, 9 deselected (1,351 selected). The three new tests are part of this
suite and will run in ordinary PR CI. Initial local collection lacked pinned
submodules; initialized them without changing gitlinks. A subsequent run lacked
MIPS objcopy in PATH; rerunning with the approved binutils PATH/library path
passed. All 23 protected target manifest files pass SHA-256 verification.

Independent lane D review passed as NONMATCH research only. The reviewer
freshly compiled with pinned IDO and independently GNU-linked all relocations:
15/63 complete resolved words differ, ELF function 248 bytes, linked section
256 bytes with two zero alignment words, native function 252 bytes. The
reviewer also reran all 20,722 three-way cases and 100,000 ASan/UBSan cases.
Source/ABI audit found no invented arguments, type erasure, padding or fake
operations. See `peer_review.json` for the bounded verdict and limitations.
