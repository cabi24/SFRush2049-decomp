# AnimateWingFlame: strict natural-source N64 match

Deliverable: [`group/`](group/), a complete replacement candidate for
`codex_visual_tire_module_20261002`. Game target `entity_anim_texture` at
`0x80091874` matches all **159 words / 636 bytes**. All three prior members remain
strict matches: `matrix_scale_apply` (100 B), `anim_state_update` (1840 B), and
`buffer_swap` (1676 B). Only the wing animator is new credit.

This packet does not install or splice anything. Image/compression/full-ROM
verification belongs to the independent coordinator. No cartridge coverage is
claimed here. The PR is deliberately a draft at the owner's request.

## Source and evidence

The accepted MAT3 ancestor is `visual.c:AnimateWingFlame` (488 B), task-11
checkpoint-60, accepted TU SHA256
`c90126e3f37dd595bab483ae66b6bebc2933c592dfbf59e423552a2b660d5449`.
Its complete body and accepted visibility helpers established the helper boundary,
appearance test ordering, and real matrix/scale operations. MAT3 is evidence of
source structure, not proof of N64 allocation or layout.

The new source retains N64 car/object layouts (952/68 bytes), side records (24 B),
field offsets, signed slot/model indices, scales `8.0f`, `-8.0f`, and `2.0f`, and
private visibility-helper conventions from the genuine complete group.

The N64 matrix getter is the already-locked `func_8008B3A0` (40 B), corresponding
to MAT3 `ZOID_FindMatrix(signed short)`. Its ordinary typed body returns the
object's matrix pointer. It is inlineable context here, not a replacement for
its independently locked O2 body. That existing O2 source was freshly rescored
MATCH. The context's standalone O3 body differs by two scheduling words; it is
not claimed. No call to a stand-in is involved.

All seven animator locals have genuine reads/writes. There are no spare locals,
padding arrays, dead/empty guards, fabricated helpers, forced registers, target
patches, or extra parameters. Six other C inputs are byte-identical copies of
the existing group. The original group name and all old members are retained.

## Causal progression

| Complete-group source | Strict differences | Aligned opcode differences | Frame |
|---|---:|---:|---:|
| Fresh canonical baseline | 148 / 159; 154-word body | 36 | 64 vs 80 |
| MAT3 shape plus N64 early slot/boolean side | 127 / 159; 155-word body | 5 | 72 vs 80 |
| Genuine inlineable FindMatrix boundary | 61 / 159; 159-word body | 0 | 80 / 80 |
| Typed side-record access | 12 / 159 | 0 | 80 / 80 |
| Separate side index and appearance hulk | 4 / 159 | 0 | 80 / 80 |
| Used `inv` declaration before `pose` | **0 / 159** | **0** | **80 / 80** |

FindMatrix creates the actual 8-byte inline area and the four native spill
reload/store instructions around the subsequent CopyUV call. The typed side
record makes IDO retain the complete 24-byte side offset instead of the
word-indexed reconstruction's intermediate multiply-by-three carrier.

The allocator trace explains why blindly reusing MAT3's `hulk` local is wrong on
N64: its combined web receives `v1` because `v0` is forbidden by the later
visibility-expression web. Keeping the genuine side index distinct restores
native `v0` for the side and `v1` for the early slot, fixing eight words. The
remaining four words are just homes of the used matrix pointer and visibility
flag; declaring `inv` before `pose` gives the native homes 72 and 68. No local
was added solely to enlarge the frame.

## Reproduce

Use an installed IDO 5.3 toolkit and MIPS binutils as documented in
`CloudHandoff.md`. Set `IDO_DIR` if the toolkit is outside `tools/cloud/ido`.

```sh
python3 tools/cloud/score.py fn src/blob/sound_handles_clear.c sound_handles_clear
python3 tools/cloud/score.py group src/blob/groups/resource_slot_clear
python3 tools/cloud/score.py group cloud/work/frontier/mat3_wingflame_20261017/group
python3 cloud/work/frontier/mat3_wingflame_20261017/verify.py
python3 -m pytest -q tests/cloud/test_mat3_wingflame_20261017.py
```

The first two controls and all four candidate members passed freshly on base
`58425072b49e95f0fbe5c4c239988fb4885f7c5c`. The single packet test passed. It checks
input/target hashes, all four strict members, and a fresh wrong-XOR-mask control.
Changing only `0x80000000U` to `0x40000000U` produces exactly **1 / 159** differing
words, with no extras, unresolved symbols, or unverified relocations; all three
old members remain exact.

`verification.json` pins packet source, its verification scripts, and native
target words. It intentionally does not pin shared tools, locks, or assembly
text. `score.txt` is the complete member/context summary from `score.py group`
without `--claims`; instruction-detail rows are omitted to avoid publishing raw
assembly. Informational context differences are inherited except the disclosed
O3 matrix getter; they do not enter the claimed group members.

Optional diagnostic reproduction with the already-installed instrumented IDO:

```sh
python3 cloud/work/frontier/mat3_wingflame_20261017/trace_fidelity.py \
  --traced-uopt /path/to/instrumented/uopt
```

The instrumented pass enables allocator/PRE/spill tracing. Both complete objects
are identical to each other and to the independently generated stock scoring
object; hashes and source binding are recorded in `trace_fidelity.json`. Raw
objects, listings, traces, and negative-control scratch stay under ignored
`build/`, outside this packet.

## Verification guard hardening

The initial packet used Python assertions for its proof guards. Those assertions
were effective in the recorded normal replay but disappear under Python `-O`.
The verifier and stock/traced identity check now raise explicit errors. Three
focused tests include twenty reason-specific optimized refusal cases and a
relocated-packet replay. All four claimed ELF symbol extents are checked against
the protected targets, and the fresh full receipt must equal the sealed receipt.
The fresh complete object must also equal both source-bound trace hashes.

The C sources, recipe, compiler flags, strict comparisons and candidate credit
are unchanged. A fresh stock/traced run reproduces the original complete object
hash. Informational context remains fully recorded with its existing nonmatches;
this repair does not expand the four-function claim set or assert new closure,
canonical integration, runtime or image coverage.
