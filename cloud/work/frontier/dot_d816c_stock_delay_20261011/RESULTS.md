# D816C: stock-as1 branch-delay residual closed

## Deliverable and boundary

`group/target.c` now compiles to **strict MATCH: 699 words / 2,796 bytes** for
`func_800D816C` with unmodified IDO 5.3. The group is self-contained and based on
current master `c340dfcacc1a3051930f984c9c05ad16548c01f2`. It carries the genuine
D91A0 caller and necessary private-ABI context from the PR #317 reconstruction.
No unmerged commit or external worktree is required to replay it.

This remains an **empty-claims research packet**. D816C uses the genuine group's
private argument convention and callee-save assumptions; it cannot be installed
as a standalone single. D91A0 improves from 239/965 differing words plus one nonzero extra word to
166/965 differing words at the exact 965-word native extent. Therefore this round adds 2,796 strictly matched source bytes, but
**zero installable bytes, accepted-image bytes, or ROM coverage**.

Eight preservation members remain strict matches at their complete native
extents. The existing `drone_throttle_calc` context is code-identical but still
fails its split-owned-rodata proof (20 unverified references); the failure is
preserved and is not overridden.

## Source-side cause and fix

The previous canonical D816C reconstruction differed only at offsets 0x88 and
0x8C. Both first-loop branches targeted the same nonempty loop-increment block.
Stock as1's `do_branch_opt` copied that increment into both delay slots and
changed both branches to branch-likely forms. Retail only uses the first copy.

Move the independent `count++` after the conditional `before++`:

```c
if (j < D_8014A0F8[player]) before++;
count++;
if (count) {}
```

Stock as1 schedules `count++` back before the comparison. Its distinct target
block consequently has zero instructions. The second branch still identifies
that empty block, so `do_branch_opt` declines its cross-block copy; the first
branch still identifies the actual nonempty loop-increment block. The two
source blocks share an emitted address but remain different compiler CFG nodes.
No forced instruction, assembler directive, patched compiler, or scorer change
is used in the result.

Reordering alone exchanges the `count` and `before` registers, leaving ten word
differences. The final compiled-out `if (count) {}` is a disclosed source-shaping
lever. Observation-only uopt tracing gives:

| Source state | count priority / live blocks | before priority / live blocks | Strict residual |
|---|---:|---:|---:|
| Original increment order | 5.5 / 4 | 4.6 / 5 | 2/699 |
| Reordered, without count read | 4.4 / 5 | 4.6 / 5 | 10/699 |
| Reordered, with count read | 6.4 / 5 | 4.6 / 5 | MATCH |

The two-register forced-color diagnostic on the reordered source reached exact
D816C before this final source lever was written. It is only causal evidence;
all recorded acceptance results below use stock IDO. Traced optimized streams
and symbol tables were byte-identical to stock. The observation-only as1 tracer
also produced a byte-identical whole object and showed the second branch's
zero-instruction target. Stock `as1 -R` was byte-identical as well.

The two increments affect separate nonvolatile local counters. There is no
observable effect between them other than the selected-index load. For the
native 12-item loop, both counters remain in 0..11; the move does not introduce
signed overflow or alter the predicate, selected-index read count, or final
counter values. The empty read has no runtime effect. Original source spelling
is not claimed.

## Tracing and rejected alternatives

Workbench diagnose was run before changes. Its direct relocatable-object report
includes expected symbol-name warnings against a raw-word target, so canonical
`score.py` relocation and owned-data verification determined the actual gap.
The branch-copy observation motivated narrow control-flow, counter-placement,
and selected-index spelling probes. Continue/goto forms, duplicated increment
paths, post/pre-tested loops, empty blocks, integer-width changes and volatile
selected-index views did not close the stock residual. Moving the count update
was the first source form to produce the required ordinary second branch. Only
three count-read placements were then needed after measuring the coloring cause.

The useful general lesson is that an emitted empty CFG block can prevent a
cross-block delay-slot copy even when its address equals the following block.
Inspect as1's block identity and post-scheduling instruction count, rather than
assuming equal addresses imply equal optimization targets.

## Bounded caller improvement

D91A0's final button read previously stayed in a1 and needed an additional copy
into the resource_type_select argument register. The native code keeps that
word in the argument register through the duplicate scan. Reusing the now-dead
inner-loop local j for that word removes the extra instruction and narrows the
strict gap from239/965 +1 to166/965, at the exact native extent. It adds no local
home, parameter, helper, or compiled-out read. The next outer iteration resets
j before its next inner-loop use.

On the path reaching the call, the intervening duplicate scan and slot read
make no calls and do not write the nonvolatile button field. The u32-to-s32
conversion moves from call argument conversion to the assignment; IDO's target
representation preserves the same bits and low-bit test. This carries the
existing single-threaded/nonvolatile-source contract, not an assertion about
concurrent input writers. Independent source/semantic review approved the edit.
The verifier reverses these two source expressions and checks that239/965 +1
returns while D816C stays exact.

A forced-color diagnostic still leaves119/965 after this improvement, so the
remaining problem is not just the four known saved-register choices. Narrow
attempts to boost the21 constant changed live ranges and worsened code/extents;
no such shaping was retained. The caller needs further source work and remains
a blocking research member.

## Retained reconstruction limits

The predecessor's four unused scalar homes remain disclosed in `target.c`:
`span`, `distance`, `current_y`, and `frame_residual3`, 16 bytes total. They are
exact frame residuals, not pad arrays. Removing them now produces frame216 and
20/699 differences; the submitted frame is232. The four earlier compiled-out
reads remain, for five total including the new count read. All20 bytes of the
D816C natural literal block are verified at0x801241E8..0x801241FC.

The genuine six-word D8154 body retains its leading player argument and the
existing unreachable source-size lever controlling umerge's legitimate inline
threshold. The private `t2` argument and D91A0 save-set relationship remain
unchanged. All supplied members, context, keep entries, and owned extents remain
explicit in the manifest.

Legacy context limitations are unchanged: D91A0's `func_800A1910` declaration
uses unsigned size against the locked signed-size declaration, with all supplied
calls passing10. Native argument bits agree, but that type view must be settled
before integration. The existing volatile global views follow sibling source
interfaces and repeated native reads, without claiming asynchronous writers.
The supported semantic domain remains valid player/handle values and finite
animation values with representable alpha conversions. See PR #317 for the
prior reconstruction review; this packet does not upgrade its context to a
whole-cluster match.

## Verification

```sh
python3 tools/cloud/score.py group cloud/work/frontier/dot_d816c_stock_delay_20261011/group
python3 cloud/work/frontier/dot_d816c_stock_delay_20261011/verify.py
REQUIRE_TOOLCHAIN=1 python3 -m pytest -q tests/cloud/test_d816c_stock_delay.py
```

Canonical output (the ordinary group command exits nonzero for D91A0):

```text
func_800D816C:
  MATCH
    own .rodata verified at 0x801241E8..0x801241FC
func_800D91A0:
  166/965 words differ
    own .rodata verified at 0x80124218..0x80124224
render_replay_ui: MATCH
render_results_screen: MATCH
func_800D8078: MATCH
func_800D8154: MATCH
func_800DA0B4: MATCH
func_8008B32C: MATCH
func_800B5898: MATCH (owned rodata verified)
func_800B5940: MATCH (owned rodata verified)
```

The one collected test verifies submitted sources, manifest, verifier and test,
parsed target-word hashes and extents, referenced symbol/address rows, and the
D816C literal hash. It replays the full group with stock IDO, all next-function
boundaries, both D816C causal source controls, the caller button-carrier control, and the
four-home frame control. The
normal and optimized-Python replays pass. Missing IDO uses the shared conftest
skip/fail policy. Compiler hashes are metadata only; no mutable tool, lock,
Makefile, or raw assembly-text hash is pinned. No static ROM TU is rebuilt, so
no per-TU compiler selection or production compiler pin changes.

Protected paths, raw target data, compiled objects, and compiler instrumentation
are excluded from the published packet. Publication stays draft and merging is
left to the independent checker.
