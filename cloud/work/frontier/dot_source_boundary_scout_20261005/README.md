# Source-boundary scout: three bounded hypotheses

**Date:** 2026-10-05. **Result:** research only; no matches, claims, promotions or
accepted bytes. The useful result is a reproducible causal limit and several
rejected routes, not a newly identified original source boundary.

Initial reconnaissance used `52904b37`; the first compiler pass used `5c98cecc`.
The final complete replay uses master
`f88dc3cb3807b8be3246b719a9b1e008210b477c`; the selected source inputs did not
change across these heads, and all five investigated caller targets remain
unlocked. The newer BSS implementation, whole-unit machinery,
integration paths and ongoing matching work are outside this packet.

`verification.json` records input/compiler/object hashes, complete-function
comparisons, ELF extents, frames and native metadata. `verify.py` performs eight
fixed compiler experiments with the unmodified stock scorer and pinned IDO.
It emits no native words, assembly, ROM content or objects into the packet.

## 1. Hidden generic allocator: rejected before compilation

`func_8008E26C` has unusual native dataflow: a 40-byte frame, captured arguments,
an independently live index-times-68 value, a never-reloaded output-index home,
and a narrowed return saved separately. Those observations do **not** identify
an original helper or establish a generic-size/pointer-return interface.

A scan of the 1,216 manifest-verified targets and detailed comparison of five
allocators found no independent second consumer of the proposed conjunction.
The conservative direct-address candidate filter finds the high-water object
`D_801569A8` only in E26C. This is not proof that indirect accesses cannot exist.

- Closest sibling `func_800A78BC` uses an 88-byte first-inactive pool with count,
  high-water and capacity checks. Its apparent dead stores are actual outgoing
  arguments to seven-argument `func_8008C074`, not an output-index local.
- `func_80091B00` and `func_80090284` are no-frame pointer-return allocators.
- `func_800B3704` likewise writes real outgoing call arguments.

No invented helper was compiled. Existing
[`w2f` notes](../w2f/func_8008E26C/NOTES.md) already record roughly 1,000 variants.
The shared node-layout surface was 43 unmatched functions / 64,144 bytes at the
initial snapshot; that is not projected gain. E26C alone made only
`save_slot_valid` (176 bytes) directly ready in that graph snapshot.

**Reopen only for:** a second independent consumer or original declaration that
establishes the proposed helper's substantive interface, not another spelling.

## 2. Genuine scene accessors: composition fails the predicted improvement

Both recursive flag walkers independently narrow their flag-read index to s16
while retaining a full-width index for writes and child/sibling lookup. Four
adjacent accepted accessors implement those exact interfaces and have no direct
native J/JAL callers: `func_8008AE64`, `AE48`, `AE2C`, `AE10`. That makes inlining
plausible, but unused API entries or macros remain alternatives.

The audit found no accessor-bearing version in 57 preserved walker-body files.
The full historical builder-side sweep is unavailable, so this is not a claim
that nobody ever tried it. Sources: [prior best](../agentB/model_data_load/best.c),
[prior results](../agentB/RESULTS.md), and the four `src/blob/func_8008AE*.c` files.

Prediction: genuine accessor calls with their unchanged definitions visible
might recover the missing source webs. Locals, control flow, interfaces, flags
and keep list stay fixed across the paired comparisons.

| Fixed condition | model_data_load, native 372 B / frame 48 | model_transform_setup, native 404 B / frame 48 |
|---|---|---|
| Direct-access baseline | 3/93 words differ; ELF 372 B; frame 48; no excess | 88/101; ELF 408 B; frame 48; 1 nonzero excess word |
| Genuine flag getter/setter | 91/93; ELF 384 B; frame 48; 3 excess | 92/101; ELF 416 B; frame 48; 3 excess |
| All four genuine accessors | 91/93; ELF 412 B; frame 56; 10 excess | 97/101; ELF 448 B; frame 56; 11 excess |

All four unchanged accepted accessor bodies remain strict matches. All treatment
accessor calls inline; missing LTO is not the explanation. References resolve
with no unverified relocations or errors. The new compositions are worse.

A separate **gated canonical-table control** used research copies with one
68-byte table, preserving the native base, offsets, widths and index conversions.
The child getter, sibling getter and setter remain exact, but the signed-index
flag getter differs in **2/10 words**, at the exact 40-byte extent. The gate failed;
**no caller was rerun against that changed context**. Accepted sources and
protected symbol/context files were never edited. No layout or spelling sweep.

At the snapshot, matching both walkers would make six callers totalling 7,012
bytes graph-ready, besides their own 776 bytes. This conditional dependency
count is not a result or a forecast. No matching improvement was found.

**Reopen only for:** evidence identifying the original shared table/accessor
representation while preserving every accepted accessor body, or a measured
compiler explanation for the residual that predicts a specific authentic change.

## 3. Cleanup inline contract: real expansion, missing original boundaries

Three independent native consumers share 24-byte-separated allocation homes:

| Function | Native bytes | Native frame | Allocation-address homes |
|---|---:|---:|---|
| `func_800C885C` | 188 | 64 | 56, 32 |
| `wheel_params_set` | 232 | 64 | 56, 32 |
| `func_800C8918` | 628 | 208 | 192, 168; then 80, 56, 32 |

Each home is witnessed by the native `a1` reload immediately before the real
`audio_reverb_update` call. The intervening shutdown operations include two real
message constructors. This is **1,048 bytes of direct target surface**, not an
unlock estimate: only C885C was callee-ready; wheel still needed
`func_800958B8`, shutdown `func_80091B00`.

The older [shutdown pragma probe](../../ipa-groups/codex_shutdown_inline_a137/STATUS.md)
left all five wrapper calls intact. Another
[A172 caller](../../ipa-groups/codex_pak_rename_inline_a172/STATUS.md) had already
used `__inline` unsuccessfully, so the keyword itself is not a discovery.

This experiment adds **only `__inline`** to the existing genuine
`audio_effect_process` definition in two preserved wrapper-call packets. Its
body, caller operations, interfaces, flags and keep decisions are unchanged.

| Packet | Before | After single keyword |
|---|---|---|
| wheel | 57/58 differ; ELF 144 B; frame 40; 2 wrapper calls | 4/58 differ; ELF **232 B**; frame **48**; no wrapper calls or excess |
| shutdown | 154/157 differ; ELF 396 B; frame 64; 5 wrapper calls | 86/157 differ; ELF 632 B; frame **88**; no wrapper calls; 1 nonzero excess word |

Wheel's homes become **40/32**, an 8-byte spacing instead of native **56/32**.
One genuine inline scope therefore explains an 8-byte contribution in this
paired experiment; the missing 16 bytes remain unexplained. Its earlier direct
expansion was already 4/58 words off, so this is causal evidence, **not a new best
match**. Shutdown remains a broad nonmatch.

Ten baseline-matching accepted context bodies remain exact in each experiment.
The internalized free wrapper was **already nonmatching** and becomes a deleted
stub; the shutdown message helpers and allocator were already nonmatching too.
Those failures are fully reported, never counted as accepted context. There are
no unresolved/unverified relocations or relocation errors in any recorded body.

The accepted deflate-driver reconstruction also reproduces 24-byte spacing, but
[its header](../../../../src/blob/groups/codex_heap_release_a25/deflate_mem.c)
explicitly identifies its three adapter names/chain as unestablished. Accepted
reconstructed source is **not proof of original source boundaries**. Stock
`ZFREE`/`TRY_FREE` are macros; stock `zcfree` has two formals. Similarly, assigning
lock/unlock bodies to adjacent empty native stubs `95CF4/95CFC` remains an
unproven reconstruction. Neither chain was imported or propagated here.

**Strongest focused lead:** recover authentic free/lock/unlock declarations or
independent substantive helper evidence that explains the 24-byte spacing across
multiple callers. Then predict both callers' exact homes and complete bodies.
Do not infer extra scopes solely to manufacture the missing bytes. No adapter
depth, padding, keeper or flag search is warranted by the current evidence.

## Reproduce and limits

From the repository root, with the project-pinned IDO/binutils environment:

```sh
python3 cloud/work/frontier/dot_source_boundary_scout_20261005/verify.py
python3 cloud/work/frontier/dot_source_boundary_scout_20261005/verify.py --compiler
python3 -m pytest tests/conveyor/test_dot_source_boundary_scout.py -q -o addopts=''
```

The first command checks source binding and native metadata only. The second
runs **eight fixed group builds** and compares the complete metadata receipt.
`--output PATH` optionally saves metadata. `--record` deliberately bypasses the
old receipt comparison for a reviewed new snapshot; ordinary verification never
rewrites this packet. Tests do not silently require an installed compiler.

Every native read verifies the current protected manifest. Historical region-file
hashes are retained as provenance; an unrelated acceptance-comment change does
not invalidate selected native-content fingerprints. Scorer/tool hashes likewise
record the measured snapshot rather than freezing unrelated infrastructure work.
Fresh compiler replay, not an unchanged tool hash, establishes reproducibility
under later infrastructure. Changes to the actual selected source inputs still
fail closed and require review/replay.

Final local verification on the stated base:
- Nine packet regression tests pass, including a rejected source-hash mutation
  and the guard against treating unrelated manifest annotations as target changes.
- All eight compiler builds reproduce the saved complete receipt in a fresh
  temporary directory, including all accessor/cleanup controls and context bodies.
- Selected packet, scorer, own-data, group-own-data and whole-unit test suites:
  **755 passed, zero failures or skips** (21.62 s). This is not the full repository
  suite or GitHub CI. The check did not attempt to repair unrelated upstream CI.
- `git diff --check` passes. Only this research packet and its focused test file
  are added; no production or protected file is changed.

These are bounded native/source/compiler checks, not a whole-program shadow,
linked-image, compressed-stream or ROM proof, and not gameplay validation.
No source splice, production context/flags/locks/tool changes or claimed gains.
The broader search also rejected a GBI-bitfield lead: two independent parsers
support the already-tested raw-word opcode idiom, and the one SDK branch-command
correspondence does not supply a shared explanation for their residuals.
