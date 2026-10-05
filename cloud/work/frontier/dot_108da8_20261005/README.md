# func_80108DA8: Hidden-based HUD callback

Status: **EXACT-BODY; source-contract review pending**. The 408-byte native body
matches ordinary IDO `-g0 -O3 -mips2 -G 0 -non_shared`. No source was promoted,
no lock changed, and no new accepted byte coverage is claimed. This packet owns
only `[0x80108DA8, 0x80108F40)` on baseline
`cc4d5fdd0bbc42dbf6be49f00d9454af8cb9c4f5`.

## What changed

The old `heads_B13/func_80108DA8_directbool.c` describes the two visibility paths
as fragmented boolean logic and a partial menu-node structure. This packet uses
the authentic `Hidden(blt, hide)` helper from arcade `game/hud.c` and the named
Blit fields, preserving the observed N64 layout. The body is an N64-specific
per-player HUD indicator: deactivate and hide invalid slots, hide unavailable
players, select the split-screen position and texture, publish dimensions.

The early exclusion path returns the **post-update signed Hide byte**, while an
active hidden path returns `1`. Callbacks may change the Hide byte or player
count, so preserving the re-reads is part of the source contract. The position
table is naturally `s32 [][4][2]`, indexed by player count minus one and slot.
`Input_ApplyPadConfig` is the accepted UpdateBlit adapter; `func_800EF5B0` is the
accepted **RenameBlit** (124 native bytes), not SelectBlit.

The fresh ordinary-count Hidden variant and the archived direct-boolean source
each have 88 differing words out of 102. The former emits a 404-byte function:
it folds the car-count address into the load and shifts the temporary register
cycle. Using the car-count declaration already present in accepted EC914
reproduces the native 408-byte extent and every word.

## Volatile qualifier: evidence and remaining review

`candidate.c` inherits `extern volatile s16 D_801543CA` from current accepted
`src/blob/func_800EC914.c`, source SHA-256
`65b68d97388fd50b72410d280c83b41f3e6b196711147105280bff4aace153f7`.
Its lock records `image_gate`, 2026-10-05. The generated shared globals header
records the signed 16-bit width but omits the qualifier.

The qualifier is supported independently of this candidate's compiler match:

- The archived whole-image reference census contains 22 signed halfword loads,
  four halfword stores and 17 explicit address formations for this count across
  13 functions. `audit_contract.py` revalidates the sites against the current
  protected targets and records complete function hashes.
- In `func_800EB90C`, a count load at `0x800EB9B8` is followed by a second count
  load at `0x800EB9FC`, in the taken backedge delay slot to `0x800EB9BC`. The
  continuing loop cycle has no calls or stores. The nearby object-byte store at
  `0x800EB9F0` belongs to the opposite, exiting branch-likely path. Thus repeated
  count observation is native behavior outside this callback and is not
  introduced merely to improve this callback's score.
- All four surveyed writers are ordinary native setup/initialization paths:
  `func_800EC190` at `0x800EC1B0`, `props_render` at `0x800F6CC8` and
  `0x800F72C0`, and `init_state_continue` at `0x800FAFF0`. They write six, one,
  an accumulated count, or a configured total respectively.

No asynchronous/device writer or original N64 qualifier declaration has been
established. The static linear census can miss indirect or DMA writers. The
native repeated-read behavior and accepted declaration are concrete
corroboration, but the independent checker must decide whether they satisfy the
source-contract admission requirement. This packet deliberately keeps machine
identity separate from that decision and remains under `cloud/work`.

## Donor and layout provenance

Authentic donor revision:
`historicalsource/rushtherock@845329d7b36f5a384c5625ed9a0aef584ab46139`.
Local files were independently checked against the Git objects:

- `game/hud.c`, Hidden at lines 1271–1279:
  `9b4b0507db6d83eb25ec2066bd7fe9eb55ca1b04bf2aa69bcdc7bc79be7bc7a0`
- `LIB/blit.h`, Blit field meanings and signed Hide:
  `91516a3935024254702dbb1b66d3f87258cfbe3ca878f22ddc5e9cd10e4a1c6a`
- `LIB/blit.c`, RenameBlit/UpdateBlit contracts:
  `34652da79c592dfd0c77e93ba717bd0d3e52e9ae869c01d69b01d650da93a080`

N64 target loads/stores independently establish X/Y at 14/16, Width/Height at
20/22, Alpha at 24, signed Hide at 26, AnimFunc at 40, and AnimID at 44. The
952-byte object stride and signed mode byte at 239 are native layout, not local
padding. This is a used prefix view; unused trailing fields are not invented.

## Verification

`verification.json` binds exact source, harness, compiler and target hashes.

- Canonical strict scorer: 102/102 equal, no unresolved/unverified relocations,
  no errors or extra words.
- Full ELF function symbol: exactly 408 bytes. Independent GNU relocation and
  body comparison agree. The compiler emits eight preceding bytes for the
  naturally inlined helper; those are excluded from the claim. No own data or
  float literals occur in this body; no alignment bytes are credited.
- 8,416 bounded host/reference/native differential cases, 16,832 native runs.
  All 102 target instructions are reached. Cases cover all branch combinations,
  signed Hide extremes, 0–4 player counts, object modes, callback mutations of
  Hide/count/dimensions, ordered call snapshots and O32 caller-clobber stress.
  Stack and callee-save preservation pass. Host UBSan passes.
- Deliberately incorrect constant early return and object-mode predicate are
  rejected by semantic controls.
- Current accepted EC914, RenameBlit, Input_InitPadHandlers and
  Input_ApplyPadConfig sources recompile strictly unchanged.

The native oracle is a purpose-limited, fail-closed integer MIPS-II interpreter.
Callees are modeled at their audited O32 interfaces; actual callee internals,
asynchronous threads, image composition, compression and full-ROM execution
are not part of this proof.

## Reproduce

With the repository's IDO and GNU MIPS tools available:

```sh
python3 cloud/work/frontier/dot_108da8_20261005/audit_contract.py \
  --output /tmp/108da8-contract.json
python3 cloud/work/frontier/dot_108da8_20261005/verify.py \
  --output /tmp/108da8-verification.json
```

No target/scorer/shared-context/compiler/keep changes, raw native dumps, ROM
bytes, credentials, unrelated data, or fabricated helper bodies are included.
