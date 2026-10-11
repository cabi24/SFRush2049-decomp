# Overlay A cleanup: strict 676-byte match

Population/source: `ovl_a`, `group/group.c` and `group/group.json`.
New claim: `func_8039D05C`, complete `[0x8039D05C, 0x8039D300)`, 169 words / 676 bytes.
Existing exact preservation members: `func_8039D494` (528 bytes) and
`func_8039ED20` (480 bytes), plus newly landed `func_8039E200` (444 bytes)
and `func_8039E3BC` (136 bytes), all unclaimed with zero new credit. Seven authentic
reconstructed functions remain context, including the real root `func_803A1EAC`;
their existing exact/nonmatching status is informational and earns no new credit. This packet is self-contained.

## Source and exact result

The body comes from PR #311's record-context source at
`d725fbe089ba81b36386b849f492035261af4883` (historical source provenance only).
The only noncomment code change is `if (player) {}` at the start of the panel
cleanup loop. It reads an initialized, nonvolatile local and has no observable
side effects or added undefined behavior. This is **disclosed source shaping**,
not a reconstruction claim about the original spelling. There are no fake
callers, injected helpers, padding variables, inline assembly, register bindings,
or target-word-dependent source transformations. Arcade ancestry is unknown.

Current project guidance explicitly recommends the compiled-out `if (x) {}`
priority lever in `WAVE_BRIEF.md`, Wave 12, and documents code-free holder reads
in Wave 15. The source header discloses the exact reason and placement.

Compiler: canonical stock IDO 5.3, `-g0 -O3 -mips2 -G 0 -non_shared`, ordinary
whole-group `uld`/`usplit`/`umerge` pipeline with the scorer's VR4300 multiply
workaround. `compilers.identify_dir` rejects unknown or diagnostic compilers.

```
python3 tools/cloud/score.py group cloud/work/frontier/dot_cleanup_colortrace_20261011/group --targets asm/us/ovl_a
Members:
func_8039D05C:
  MATCH
func_8039D494:
  MATCH
func_8039ED20:
  MATCH
func_8039E200:
  MATCH
func_8039E3BC:
  MATCH
```

Exit 0; 0/169 differences, no unresolved or unverified relocations, no own-data
failures, no excess words. Every claimed member passes without `--claims` or
`--allow-unverified`. The informational context is still nonmatching overall.

## The measured cause

A fresh, locally rebuilt diagnostic uopt identifies a priority inversion,
not a tie, frame error, or ugen temporary-ring shift. The baseline's 18 word
changes are entirely a four-register permutation; both frames are 24 bytes and
all 29 temporary-register occurrences are identical.

- Baseline player web 13: total 92, nocs 11, save 8.363636, assigned s7
- Slot/panel byte bounds: total 100, nocs 7, save 14.285714, assigned s4/s5
- Signed-byte player copy: total 30, nocs 3, save 10, assigned s6
- Diagnostic priority-only oracle `PRIO=13:1.75`: 0/169, without source changes
- Actual panel-loop source read: total 192, nocs 11, save 17.454546; player s4,
  bounds s5/s6, signed copy s7; **canonical stock 0/169**
- Same read in the slot loop: canonical stock 7/169, a placement control
- Removing the submitted read: canonical stock 18/169

The read contributes one weight-100 use. The diagnostic compiler's complete
optimized intermediate and complete object equal stock with tracing on for
all three retained source controls. Its oracle output is diagnostic only.
`research.json` retains compiler decisions and scores, no native dumps.

The previous PTRACE_TRACEME denial was not retried. This research builds supported
static-recompilation source instrumentation and does not attach to or patch a
running process.

## Verification and scope

`verification.json` pins submitted source, group, own verifier/research scripts,
and complete native words by population, address, extent and SHA-256. Only
relevant extents/symbols manifest rows are retained; mutable global tools, locks
and assembly text are not receipt-equality inputs. Current scorer manifest
validation remains mandatory.

The independent ELF reader checks all five complete STT_FUNC extents
(676/528/480/444/136 bytes) and their exact next symbol boundaries. Independent GNU assembly/linking applies all relocations (56 in D05C) and
equals all native target words for every member. This re-sections the complete function;
it does not prove contiguous original-TU placement.

D05C has no arguments and intentionally clobbers s0-s8. It is not a standalone
O32 replacement. All four native and four generated A1EAC callsites are checked;
that caller saves/restores s0-s8. A1EAC remains nonmatching and is not executed
end-to-end. This complete body has no owned literal/storage references.

Optional behavior replay covers 512 native, GNU-linked and independent model
cases: all 169 instructions, 37 branch outcomes, 209 helper-driven player-count
mutations; full memory/read/write/call-trace agreement. The eight external
services are O32 clobbering hooks, not proof of their actual gameplay/lifetime
behavior. Count samples are -32768, -1, 0, 1, 2, 3, 4, with four-player storage;
there is no unrestricted arbitrary-memory claim or host-C execution proof.

Independent review reproduced the complete stock/GNU/body/ABI/model proof and
read-removal control against master `eae0c6466`. Accepted-image and cartridge
gain for this submission remain **unverified** (reported as zero), pending the
independent checker/coordinator's overlay landing and full-ROM gate.

## Cheap routine replay

```
python3 cloud/work/frontier/dot_cleanup_colortrace_20261011/verify.py
REQUIRE_TOOLCHAIN=1 python3 -m pytest tests/cloud/test_cleanup_colortrace_packet.py -q
```

Exactly one collected packet test. Measured 0.59 s with IDO; 0.48 s without IDO
(skipped after input validation). `REQUIRE_TOOLCHAIN=1` turns missing IDO into a
failure. Scoring failures never become missing-tool skips. No production ROM TU
is compiled by this packet; a future ROM-TU build must use its `tu_cc` pin.

Optional deep model replay (measured 3.44 s):

```
python3 cloud/work/frontier/dot_cleanup_colortrace_20261011/verify.py --deep
```

Optional compiler-cause replay, outside routine CI:

```
python3 cloud/work/frontier/dot_cleanup_colortrace_20261011/research.py --uopt /path/to/diagnostic/uopt
```

To build that diagnostic locally, use official `decompals/ido-static-recomp`
v1.2 commit `9c242adc890beef098020149d9554f48208f699d`. Run `make setup`, then
`make VERSION=5.3 build/5.3/uopt.c`. Copy `header.h`, `helpers.h`, `libc_impl.h`,
`libc_impl.c` and `version_info.c` into private build scratch. Apply repository
`cloud/work/frontier/tools/trace/patches/instrument_w5d.py` to the generated C,
then `w17f/tools/prio_patch.py` to the result and `tools/trace/patches/ecvt.patch.py`
to the private libc copy. Build libc/version with `gcc -std=c11 -Os
-fno-strict-aliasing -DIDO53 -I. -c`; link the traced C and those objects with
`-lm`. Generated uopt source in this build hashes to
`b0058f1559441c1a194d649271eb43b8637ec255682cfdd629031340b915b13f`.
Host GCC and the diagnostic binary are provenance, not replay-equality pins.
The replay checks diagnostic fidelity itself before accepting causal evidence.

Original review base: `eae0c6466`. Publication refresh base: `0b3daa0cb412638cda3bbf53963516fef7924ec3`;
no unmerged runtime dependencies. The refresh changes preservation metadata only: E200/E3BC
now appear in members because they have landed, with the same frozen C, keep list,
and D05C-only claim. The overlay installer selects claims for installation, so
existing locked E200/E3BC bodies remain with their original groups. No ROM bytes,
raw assembly dumps, compiler outputs, credentials, accepted-lock changes or
protected input changes are submitted. Leave image integration and merging to
the independent checker.
