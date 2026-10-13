# func_80011910: stock code equality, storage integration still blocked

**Research-only draft, empty claims.** `candidate.c` now emits all **208 native
bytes / 52 words** with unmodified IDO 7.1 and the correct `lib_11640` compiler
selection. The C-only actual-TU diagnostic also emits exact code and preserves
all **71 preexisting C bodies**. No new cartridge coverage is claimed.

The remaining integration gate is real: the source defines the 64-byte task at
`D_800382F8`. The current `context_check.py` refuses it:

> object has data/rodata (.bss:64) outside an owned slot of func_80011910

No owned-data registry, linker, protected assembly, production source, lock,
ROM byte or raw native listing is changed. Nothing is promoted or spliced.

## Source result

The source lever is ordinary pointer lifetime, not an allocator patch:

- Initialize `AudioTask *task = &D_800382F8` **before** the nonzero-count guard.
- Access the fields and submit callback argument through that local pointer.
- Define the actual task object in the compiler unit instead of declaring only
  an opaque external object.

All three are present in the candidate. The pointer is used, no arbitrary
observable work is added, and no padding/unused locals/false prototypes occur.
The same layout as the old complete semantic reconstruction is retained.
Native `func_800119E0` supplies a command pointer and unsigned-short count;
`func_80011104` allocates command buffers in 2,576-byte units; the neighboring
waiter `func_800118C0` observes and clears the same completion flag. No arcade
ancestor is asserted.

## Traced cause, then source proof

`baseline.c` is the previous BT02 complete-NONMATCH reconstruction. Its complete
function is 196 bytes, followed by twelve zero section-padding bytes; native is
208 bytes. Canonical and independent GNU comparisons both give 47/52 differing
words. Equal padded section lengths are not equal function extents.

A stock-fidelity 7.1 trace shows ten cfe direct task-field stores becoming uopt
indirect stores through one aggregate-address web. Web 23 has total saving 10,
best cost 2.1 and gets a2; unsigned-short count then gets a3. Native keeps count
in a2. The source local pointer makes uopt retain the direct field stores
instead of creating this aggregate address register.

Separately, as1 needs the actual object definition to combine the field-pair
address materializations. This causal matrix isolates both requirements:

| Source / diagnostic | Differing words | Full body | Task storage |
|---|---:|---:|---|
| old extern baseline, stock | 47/52 | 196 B | none |
| define task only, stock | 47/52 | 196 B | 64 B BSS |
| real local pointer, task remains extern, stock | 43/52 | 228 B | none |
| local pointer plus task definition, stock | **0/52** | **208 B** | **64 B BSS, unowned** |
| force only aggregate web uncolored, extern task | 43/52 | 228 B | none |
| same diagnostic oracle, defined task | 0/52 | 208 B | 64 B BSS, unowned |

The oracle is solely causal evidence. The submitted exact source is compiled
with the stock compiler. Aligned-union and volatile-object controls fail to
solve the baseline. `probes.py` generates a bounded six-cell source matrix.
`trace.py` checks stock/off/on identity for `.B`, `.O`, `.G` and the entire `.o`
for both causal baselines; compact results are in `trace_summary.json`. Raw
compiler logs, intermediates and generated assembly stay in ignored scratch.

## Verification

`verification.json` is protocol-v2 input-bound: this packet's sources, verifier,
trace driver/results, test, parsed target rows and selected symbol bindings.
Shared tools, lock files, build files and assembly text formatting are not
pinned. Replay uses current checkout sources, not a private commit.

- Stock IDO 7.1 selected by `compilers.tu_cc('lib_11640', ...)`, exact approved
  flags, complete STT_FUNC extent, frame 24 B, zero trailing words, no relocation
  waiver. MIPS2/o32 ELF, original relocations and extern bindings are checked.
- Independent GNU assembly of the symbolic native target reproduces its protected
  words. GNU linkage of the stock candidate at the retail function address gives
  all 52 equal words. The **diagnostic** linker places task BSS at 0x800382F8 with
  SUBALIGN(4), the declared C type's alignment. This is an explicit hypothetical
  placement, not proof of the actual ROM-TU BSS layout. IDO's input BSS section
  alignment is 16; its 64-byte definition remains unowned.
- 192 fixtures execute every native and candidate instruction against an
  independent logical oracle. They cover zero and maximum counts, high incoming
  argument bits, four opaque pointer values, every task field and untouched
  canaries, exact cache and submit arguments/order, callback mutation, caller-save
  clobbers, preserved registers, SP/RA restoration and permitted stack writes.
  All ten configured descriptor fields are written before cache/submit; the pending flag is
  written after submit, including when callbacks modify it.
- IDO compile-time assertions check all sixteen field offsets, descriptor size
  64 and 32-bit pointers. A wrong cache multiplier fails behavior; unknown
  instructions and a wrong RA stack-store offset fail closed.
- The real context gate's BSS refusal is required. A separate temporary C-only
  actual-TU substitution diagnoses code generation: the new function and all
  71 existing C bodies remain strict-exact. This neither runs assembly
  passthroughs nor bypasses the refused integration gate. Production TU is untouched.

Behavior fixtures model synchronous callback contracts. They do not execute the
cache implementation, submit hook, RSP hardware, concurrent accesses or gameplay.
No ROM is available, so no cartridge hash or full production build is claimed.

## Reproduce

```sh
python3 cloud/work/frontier/dot_static_11910_20261013/verify.py --deep
python3 -O cloud/work/frontier/dot_static_11910_20261013/verify.py --deep
REQUIRE_TOOLCHAIN=1 python3 -m pytest tests/cloud/test_static_11910_research.py -q -o addopts=''
```

The one default test checks the receipt and stock source in well under five
seconds; the deeper GNU, ABI and TU checks are explicit. To reproduce the trace:

```sh
python3 cloud/work/frontier/dot_static_11910_20261013/trace.py \
  --recompiler /path/to/official/ido-static-recomp-v1.2 \
  --out build/static11910-trace
```

Alternatively pass `--trace-ido` with the repository's trace71 + w24c PRE
instrumentation directory. Every replay independently verifies fidelity.

## Next integration step

The coordinator needs a genuine `lib_11640` BSS block/companion and named-object
placement proof, not a naked 64-byte slot. 0x800382F8 is not 16-byte aligned, and
current ROM BSS slots own complete aligned sections. Adjacent real globals
already have consumers: D_800382F0/D_800382F4 precede the descriptor, while
D_80038338/D_8003833C immediately follow it. They cannot be silently treated as
padding. Preserve their definitions/addresses and locked callers, establish the
owned-offset contract, rerun context/TU/full-ROM gates, and only then make a
nonempty matching claim. This packet deliberately stops short of inventing that
broader storage contract.
