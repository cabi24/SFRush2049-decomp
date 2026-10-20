# Init_MDrive: 700-byte stock IDO match

The deliverable is `group.c` plus `group.json`. The complete member, claim and
keep set is `func_800E543C` (game/blob, `0x800E543C`, 700 bytes / 175 words).
The genuine static `multiinit_n64` helper is inlined; there are no claimed
stand-ins or preserved locked members. The source also matches through the
ordinary single-function compiler route.

Fresh stock IDO 5.3 verification on master
`58425072b49e95f0fbe5c4c239988fb4885f7c5c`:

```text
$ python3 tools/cloud/score.py group cloud/work/frontier/dot_mat3_physics_20261017/mdrive
Members:
func_800E543C:
  MATCH
$ python3 tools/cloud/score.py group cloud/work/frontier/dot_mat3_physics_20261017/mdrive --claims
Members:
func_800E543C:
  MATCH
$ python3 tools/cloud/score.py fn cloud/work/frontier/dot_mat3_physics_20261017/mdrive/group.c func_800E543C --flags '-g0 -O3 -mips2 -G 0 -non_shared'
func_800E543C:
  MATCH
```

All comparisons have zero differing words, extra nonzero words, unresolved
relocations, unverified inputs and errors. The native frame is 72 bytes.
This is 700 individually strict bytes; no image splice or ROM build was run,
and no accepted image/ROM byte gain is claimed. Independent integration and
whole-image checks remain necessary.

## MAT3 source insight and disclosures

The accepted MAT3 `mdrive.c` reconstructions identify `Init_MDrive` at
`0x80029C28` and the genuine static helper `multiinit` at `0x80029DF0`.
The donor lookup reported canonical 100% for these functions, rather than a
fresh donor compile. Donor file SHA-256:
`5489273138939dd87a30852becfc9aac2f76320ffea18ab8694516884bcc1b9d`.

The useful source-level insight is the helper boundary: camera/resurrection
setup, clearing the four tire/wheel states and optional final setup belong
inside `multiinit`; the configuration-mode, speed threshold and drift reset
remain in `Init_MDrive` after the call. This keeps the index live through the
inlined helper and reproduces its native argument home. The forward loop
uses wheel, sideforce, traction order. The recovered function identities are
supported by the donor; exact historical N64 source spelling is inferred.

Two source choices required additional N64 experiments:

* A live `Config8 *config` carries the final configuration address. It is
  used by the mode check, not an unused local, and is not present in the
  donor's listed DWARF locals. Its allocation supplies the last eight bytes
  of the native frame. Replacing it with direct indexing changes ten words
  and reduces the frame to 64 bytes.
* Separate physical statement lines at the active/state stores and final
  gear-assignment arms preserve the stock scheduler's ordering. Joining
  either site changes two words without changing the frame or semantics.

`D_801525F0` retains the existing volatile signed-16 store-side declaration
from accepted N64 sources `func_800D60AC.c` and `players_frame_update.c`.
Structure gaps describe observed field offsets and strides. There are no
false return prototypes, unused locals, artificial padding arrays, synthetic
callers, new volatile accesses, inline assembly or object patching.

## Reproduction and controls

Point `IDO_DIR` at the stock IDO 5.3 toolkit and put the existing GNU MIPS
binutils on `PATH`, as for the repository scorer. Then run:

```sh
python3 cloud/work/frontier/dot_mat3_physics_20261017/mdrive/replay.py --check
python3 -m pytest -q tests/cloud/test_mat3_init_mdrive.py
```

`proof.json` follows packet protocol 3: it binds the C source, group recipe,
replay, host semantic test and README, the target's complete word hash and
extent, and referenced symbol addresses. It also pins the actual nine IDO
stage executables, scorer, own-data verifier and canonical lock. Replay reads
the current checkout, without historical git objects. A source, tool or lock
change requires a fresh reviewed receipt; absence of the compiler follows
the shared toolchain policy.

Replay makes two independent group compiles and one single-function compile.
Five controls are freshly compiled and must reject:

| Control | Differing / target words | Frame |
|---|---:|---:|
| 44.0f threshold changed to 45.0f | 1 / 175 | 72 B |
| Replace live config pointer with direct indexing | 10 / 175 | 64 B |
| Reverse wheel/tire clear order | 8 / 175 | 72 B |
| Join active/state assignment lines | 2 / 175 | 72 B |
| Join final gear assignment lines | 2 / 175 | 72 B |

The four focused tests validate the stored inputs and strict compile, execute
111 optimized-Python refusal checks, run the complete optimized replay with
all five compiled controls, and check the missing-compiler diagnostic. The
unchanged host layout/branch/order model runs 288 bounded cases under fatal
UBSan with an empty-stderr requirement. It is a regression check for the
stated semantics, not a separate native-MIPS behavioral-equivalence claim.
Toolchain absence uses the shared repository skip policy;
`REQUIRE_TOOLCHAIN=1` makes missing tools a failure.

## Verification hardening (protocol 3)

The source, recipe, five source controls and all prior comparison results are unchanged. The replay now pins the actual nine IDO-stage executables, scorer, own-data verifier and canonical lock; compiler lookups use the shared missing-tool policy. It records and enforces the complete emitted function inventory and 700-byte ELF extent for all three positives and all five controls, with frame scans bounded to that extent. Explicit guards check complete claims, compiler flags, all comparison fields, control counts, native frame and zero image credit. These checks remain active under optimized Python. The unchanged 288-case host model now also runs inside the full replay with fatal UBSan and an empty-stderr requirement. The original source-shaping and volatile-global disclosures above remain in force.

## Compiler-input correction

The frontend driver invokes `cfe` followed by `ujoin` for the retained `cc -j` path. The receipt now includes the previously omitted `ujoin` executable hash, and the optimized guard test rejects a stale `ujoin` fingerprint. This input-seal correction changes no C, recipe, compiler flags, emitted object, target, comparison, layout or semantic result. Existing source and acceptance limits remain unchanged.
