# Image A renderer root: 576 newly verified candidate bytes

## Deliverable first

`groups/dot_runtime_a_menu_renderers_20261011/` supersedes the existing group
of the **same name**, preserving both locked members unchanged and adding
`func_803A7918`, native extent `0x803A7918..0x803A7B58` (576 bytes).
All three members are claims so the existing members cannot be accidentally
unlocked by the installer's claims-to-members conversion. Only 576 bytes are
new; `803A6F2C` and `803A7620` get no duplicate credit.

The executable C is unchanged from the genuine previously accepted group's
context. This packet supplies the missing current-tree, authenticated owned-data
proof for 7918, not a new source reconstruction. The unchanged canonical scorer
accepts every member and verifies the whole five-entry, 20-byte switch table.
Default scoring without the authenticated image still reports two own-section
references unverified. Neither scorer logic nor acceptance requirements change.

Base: current master `c340dfcac`. The existing `assets/us/data.bin` is read
locally, checked by SHA-256 and loader pointer, decompressed in memory, and
checked against the complete image hash and size in protected overlay metadata.
No historical git commit or external checkout is a replay dependency. No native
bytes, extracted images, raw assembly or compiled object is published.

`func_803A7180` remains complete, unclaimed context: 2/296 words differ, frame
192 versus 168. Its text 100/suffix 16 buffers and every other live capacity are
unchanged. No accepted-image, recompression, full-ROM or gameplay claim is made.

## Reproduction

- `python3 cloud/work/frontier/dot_runtime_a_renderer_residual_20261011/verify.py --score`
  runs canonical `score.py group` logic without `--claims` or `--allow-unverified`,
  after supplying only the authenticated image through its existing data API.
- `python3 cloud/work/frontier/dot_runtime_a_renderer_residual_20261011/verify.py`
  replays the input-bound proof and negative table-data controls.
- `python3 cloud/work/frontier/dot_runtime_a_renderer_residual_20261011/verify.py --deep`
  also runs the actual group installer's complete relocation/data path, without
  modifying production sources, locks, layout, images or calling splice.
- `REQUIRE_TOOLCHAIN=1 python3 -m pytest tests/cloud/test_runtime_a_renderer_residual.py -o addopts='' -q`
  is the one bounded routine test, using shared missing-tool handling.

The independent checker owns final installation with the existing group name,
image/recompression gates, full-ROM SHA-1 and merging. This PR stays a draft.

## Trace-first 7180 result

Stock diagnostics reproduce `M=148`, `T=28`, `P=4`, and a 192-byte frame.
The native text base is `sp+68`: the frozen 100-byte object ends exactly at
native frame top 168. The candidate's 24 additional bytes are above this live
buffer. Registers and every used buffer/home address already agree.

Three bounded source probes test the stated carrier/lifetime hypothesis:
removing the named X carrier keeps 2/296 and frame 192; deriving row Y from
`80+row*12`, with and without its short carrier, gives 16/296 and frame 192.
The compiler replaces the removed source storage with temporary storage;
none closes the frame. These are diagnostics, not accepted edits. No capacity
reduction, dead-parameter signature, fake helper or modified compiler was tried.
The remaining issue needs evidence about original declarations/capacities or a
new source-local mechanism; this is not an impossibility claim.

## Source shaping and boundaries

Inherited shaping is disclosed in the C header: two unused s32 locals in 6F2C,
four in 7620 and three in 7918; 7918's real text-X return uses an s16 carrier.
The scalar slots reproduce measured frame residuals, not original declarations.
All live capacities and all helper signatures remain frozen. The whole genuine
four-function family is compiled, with 7918 the sole kept root. Context 7180's
body is not installed and its native address is retained for the root call.

## Recorded verification

Authenticated canonical member output (no `--claims`, no `--allow-unverified`):

```text
Members:
func_803A6F2C:
  MATCH
func_803A7620:
  MATCH
func_803A7918:
  MATCH
    own .rodata verified at 0x803B9834..0x803B9848
```

The context prints `2/296 words differ`. The ordinary unauthenticated command
against `asm/us/ovl_a` still refuses 7918 with two unverified relocations, as it
should. `verify.py --score` calls the same canonical command implementation after
authentication; it never changes comparison logic or enables an uncertainty
waiver. The verifier restores the original scorer input state on exit.

Stock IDO replay: **1 passed in 0.15 s**. Missing-IDO control: **1 skipped in
0.07 s**, using the shared conftest policy. Optimized-Python replay passes too.
The regular test checks complete function extents, every claimed code word,
all five relocated table entries, all twelve zero alignment bytes, mutations
of each entry, truncated table data and absent data. No ROM is needed.

`--deep` additionally exercises `ovl_splice.packet_spec` and
`blob_group.member_slices`/`relocate`, the actual installer conversion and
relocation path. All three output bodies equal their authenticated native
extents; a corrupt table is refused. It does not perform an installation,
image build, recompression or cartridge check. The superseding packet keeps
the existing group name and both old members, avoiding the installer's
silent-unlock guard. No installer incompatibility was observed.

The optional `trace_probes.py` reproduces only the four stated probes and
retains no raw listing. Baseline memory areas are M148/T28/P4; removing X
makes M152/T28/P4 while the rounded frame remains 192. Both row-expression
probes retain M148/T28/P4. Diagnostic and ordinary complete objects are
byte-identical for each probe. These source edits were not retained.

Receipts bind this packet's source/verifiers, image-qualified target words,
relevant protected metadata and authenticated asset/image hashes. They do
not pin tools, locks, the Makefile, assembly text or non-ancestor commits.
