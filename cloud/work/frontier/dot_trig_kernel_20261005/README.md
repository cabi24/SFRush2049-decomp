# Isolated native asin/acos unit: complete matching proof

**Strict MATCH, 524 candidate bytes, zero accepted-byte or ROM-coverage gain.**
This packet promotes the *evidence quality* of an existing provisional source:
it does not claim a newly discovered instruction match.

- `func_8009C3F8`: `[0x8009C3F8, 0x8009C5BC)`, 452 bytes / 113 words
- `camera_update_c`: `[0x8009C5BC, 0x8009C5E0)`, 36 bytes / 9 words
- `select_screen_update`: `[0x800BFD68, 0x800BFD8C)`, 36 bytes / 9 words

Base: `e24b47d89a0c8ffade1e4c75ad76b9d390a1c232`. Current accepted locks,
tracked claims and open PRs 110–119 were checked before reserving the three
ranges. No accepted entry or claimed submission for them was found. This check
cannot establish the state of unpublished work elsewhere.

Registered source and explicit three-member claims are in
[`ipa-groups/dot_trig_kernel_20261005`](../../ipa-groups/dot_trig_kernel_20261005).
The independent checker owns final admission, integration and merging.

## Provenance and what is new

The actual source ancestor is the pinned arcade
[`LIB/asincos.c`](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/LIB/asincos.c)
from historicalsource/rushtherock commit `845329d7b36f5a384c5625ed9a0aef584ab46139`.
The inspected file has SHA-256
`20556da59a3fc300e919ef96f95c68c3a6be3e8c5940a1718366f34aa0afdbc2`.
It supplies the Cody–Waite rational structure, the substantive
`farcsine(float x, int flag)` interface, its two genuine wrappers, and the final
sign/mode table structure. The donor's original error reporting, double-literal
expressions and `ldexpf` call are not claimed to be the N64 source text.

The complete N64-adapted kernel body is copied **byte-for-byte** from
[`w6a/dev/part_asin.c`](../w6a/dev/part_asin.c), hash
`2d4d0ef5fb33ed56a984fff220cc1a50daf0f04827a28dcfc689e3a44ef9afa0`.
The archive already reported a provisional instruction match. Its large
particle unit had two stand-in roots and missing callers, and consequently
claimed nothing. Those historical limits remain correct for that packet.

The new result is that the kernel and its two actual wrappers form a complete,
isolated ordinary O3 source unit: **all three remain exact without any particle
source, stand-in, artificial root or forced pressure value**. A separate real
camera/parent context also preserves the complete three-body result. This packet
adds source-bound reproducible complete-ELF and GNU proofs, owned-data checks,
private-ABI execution, host C89/UBSan differential tests, causal controls and
automatic changed-submission registration. The old source and old receipts are
unchanged. No claim is made that the original N64 translation-unit boundary has
been recovered.

## Actual interface and behavior

The two exported wrappers take their float in ordinary O32 `f12`, move it to
`f16`, set mode 0 or 1 in `a0`, and call the real kernel. The kernel consumes
**f16 and a0**, returns a float in f0, and is not an ordinary externally declared
O32 `(float, int)` function. No extra float parameter is invented to force this
allocation. The genuine O3 call relationship reproduces it naturally.

A manifest-backed direct-call census finds five kernel callers: the two wrappers,
`particle_system`, `func_800BFD8C` and `camera_follow_path`. No direct JAL callers
of the wrappers are found; that does not exclude indirect use. The kernel's
first float read is from f16; it defines f12 before consuming f12. The replay
checks all O32 callee-save GPRs/FPRs, stack/global/frame pointers and return
address. A deliberately changed initial read from f12 fails the oracle test.

For modes 0 and 1, the routine takes the input magnitude, handles a tiny-input
threshold and `|x| >= 1`, reduces the range above 0.5, evaluates the complete
rational polynomial in binary32, and applies the sign/mode tables. The N64
clamps out-of-domain finite values through its magnitude branch, unlike the
arcade's diagnostic-return path. Signed zeros, negative inputs, both modes,
subnormals and values on either side of all thresholds are covered.

The source's 48 coefficient bytes at `0x80123ABC–0x80123AEC` are verified against
protected owned data. The four-element external table at
`0x8011F010–0x8011F020` is independently checked against its meaningful donor
values, 0, pi/4, pi/2 and pi/4. Data bytes receive no code credit.

## Fixed compiler controls

All builds use the existing stock O3 group pipeline and R4300 multiply-erratum
assembler setting. No protected recipe, target, lock, header or scorer changed.

| Source condition | Kernel differing words / ELF bytes | Wrapper differences |
|---|---:|---:|
| Earlier A21 three-body archive | 92 / 440 | 6 and 7; each 32 bytes |
| Isolated archived w6a body, float-first | **0 / 452** | **0 and 0; each 36 bytes** |
| Same source and calls, flag-first | 0 / 452 | 2 and 2 |
| Same source, replace `/ 2.0f` with `* 0.5f` | 88 / 440 | 0 and 0 |

The float-first interface has direct donor support and predicts both wrapper
schedules. The division and multiplication forms are arithmetically equivalent
within the tested domain, but IDO's constant sharing differs. The earlier w6a
notes already identified that mechanism; this pass does not rebrand it as a new
hypothesis. Every fixed control has an independent complete-body GNU comparison.
The receipt separates canonical scorer counts from full-extent GNU differences.
Diagnostic data mismatches preserve sites and error categories, not ROM words.

No spelling search, local permutation, padding, dead read, unused formal,
volatile shaping, keeper, inline assembly or stand-in is added.

## Complete object and execution proof

- Exact ELF STT_FUNC extents total 524 bytes. GNU readelf independently confirms
  all offsets and sizes. Four trailing zero bytes are outside every function.
- All **32 relocations** resolve through GNU ld: 24 owned-coefficient HI16/LO16
  records, six external-table records and two real kernel calls. All complete
  function words and the entire owned coefficient area agree without masks.
- GNU links the unchanged genuine object with the kernel and coefficients at
  their real addresses. Its two wrapper symbols are contiguous in that object,
  while native placements are noncontiguous. Their absolute calls resolve to
  the kernel and each complete wrapper stream is exact; the streams are also
  executed at their native addresses. This is not a claim of recovered whole-TU
  placement or final-image linking.
- **12,444 cases**, comparing an independent scalar oracle, unchanged host C89
  under UBSan, protected native instructions and GNU-linked instructions. Both
  direct private-kernel entry and the corresponding actual wrapper execute for
  each case, giving **49,776 MIPS executions**. No dependency hooks are needed.
- All **111 CFG-reachable kernel instructions** and all 18 wrapper instructions
  execute. Kernel offsets `+0x2C` and `+0x50` are duplicated loads bypassed by
  delay-slot copies; they remain part of the full byte comparison. Dynamic
  instruction coverage equals the independently traversed conservative CFG.
- All native/GNU read/write traces agree. Tables and literals remain read-only;
  the wrapper's sole stack write is its actual saved return address. Stack
  canaries and saved GPRs/FPRs are preserved.
- Five compiled wrong-contract source mutants are rejected: range threshold,
  polynomial coefficient, negative asin sign, reduced-mode index and negative
  acos table. Unknown instructions, redirected stack writes, wrong input
  register, wrong ELF extents and actual object-code/literal mutations are
  rejected by focused tests.

## Genuine wider context and limits

The verifier reuses the complete existing
[`dot_camera_path_contracts`](../dot_camera_path_contracts/README.md) source in a
temporary O3 group, preserving its real `stunt_combo_display`, `func_800D348C`,
`camera_follow_path` and `func_8008B2E4` bodies and existing root list. Only the
kernel declaration and its single real camera call are adapted to the
source-supported float-first interface. The genuine wrappers are added as
exported roots. There are no stand-ins.

The trig triplet remains strict MATCH, exact full ELF extent and independent GNU
equality in this seven-body context. The camera body is still 250/371 differing
words at 1,328 bytes. D348C, the parent and the RNG also remain nonmatches; the
receipt retains their complete ELF extents, excess and unresolved own-data
failures. They are neither claimed nor executed by the semantic harness. This
establishes compatibility with that real source context, not complete caller
correctness, acceptance of its historical types, or whole-game closure.

Defined-host proof covers finite binary32 inputs and modes 0/1. Signaling NaNs,
FCSR exception flags, hardware timing and arbitrary mode values are excluded.
The two other direct callers are audit-only. No full-game shadow, source-built
image, compression, gameplay or ROM SHA-1 gate is claimed. No full-suite or
remote CI pass is implied.

## Reproduction

```sh
python3 tools/cloud/score.py group cloud/work/ipa-groups/dot_trig_kernel_20261005 --claims
python3 cloud/work/frontier/dot_trig_kernel_20261005/verify.py
REQUIRE_TOOLCHAIN=1 python3 -m pytest tests/cloud/test_dot_trig_kernel_packet.py -q -o addopts=''
```

The final selected packet/scorer/integrity/submission/guard regression passes
**739 tests** without failures or skips, including 15 focused packet tests.
Both standard scorer sanity examples and all **402 static lock guards** pass.

The verifier checks current protected manifests and requires explicit `--write`
to refresh its receipt. GNU binutils, the pinned IDO tools and a host C compiler
with UBSan are required. Only UTF-8 source, metadata, tests and research are
submitted; temporary objects, native streams and disassembly are not published.
