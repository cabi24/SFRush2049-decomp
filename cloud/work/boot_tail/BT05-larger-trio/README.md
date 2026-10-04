# BT05 larger note and duration handlers

**One first-form strict O2 match, 292 B / 73 words; two complete NONMATCHs,
672 B.** Independent review and exact aggregate-head CI precede acceptance.
No cartridge-coverage or promotion claim.

- Exact activation `2dd2e2a3`: `22678+292`, `233B0+368`, `239A4+304`, each
  address prefixed `800`, totaling964 B.
- Branch `dot/boot-tail-bt05-larger-trio` explicitly stacks on frozen,
  peer-approved pair commit `f06d238e221eff9cd83f0e9783de0be01277775f`; underlying
  master is `301d9e7552ad4fd7f54a38796db84671e1000d35`.
- Only matching22678 and this packet differ from that stack base. The earlier
  pair and all previous sources remain byte-identical. Central ledger ownership
  and independent checker merging remain unchanged.

## Native operations and genuine ABI

`22678` receives state/command pointers. It adds a signed command byte to either
packed current note+0x50 or original note+0x4E, stores the u16 result, then clamps
its signed16 interpretation below0 and its unsigned value above127. It writes
detune+0xC0, queries real21764(state), conditionally forwards the real three byte
inputs to20F4C, changes command word0 to4 and returns2193C(state,command).
The two operands' sum is safe in signed32 before conversion to u16. Target
signed16/signed8 conversion conventions preserve the native low-bit behavior.
Both callee contracts were independently inspected for earlier matching225FC;
this complete body matches its first source at all73 words.

`233B0` has three genuine inputs: state, command and starting volume. Already
matched23520/23544 pass the current volume or zero as the third word. It writes
the command duration to packed state+0xB4, then calls1E930 on that meaningful
field address or1E940(field-address,state). Runtime voice records have the
ordinary aligned word address required by those helpers; packed field access
lowering does not invent a misaligned temporary.

It converts duration through1E9A0(u32), computes a capped unsigned-low-word volume
from state+0x30 and command bytes, calls2321C(u32,u16), then stores target+0x88,
signed delta+0x84, starting volume+0x30 and flag0x20000 at+0x24. Native1E9A0 returns
its input shifted right8, so its output is0..0xFFFFFF; replacing zero with1 gives
a positive signed divisor. The target-minus-start subtraction is unsigned low32,
then converted under N64 two's-complement conventions before division. Therefore
this division has neither zero nor INT_MIN/-1 overflow. The external tick-time
helper retains its established tempo/division contract; its body is not copied.

`239A4` receives two real pointers. It stores mode byte+0x98 and converts a real
address-taken u32 duration local through1E930 or1E940, then stores packed duration
+0x90. Selector0 clears0x800, sets0x1000 and calls20FDC(channel,set,0) unless
channel255. Selector1 calls19BE4(state) when0x800 was absent, then sets0x1800,
retaining helper mutations through native reloads. Other selector values only
perform the earlier mode/duration work. The two-mode switch has no local table.
20FDC has three true byte inputs;19BE4 consumes the state pointer.

All unknown arrays describe object storage gaps, not local padding. Local values
and formal arguments are genuinely used. Callees are declared only, and all
references resolve in the canonical population. No fake float parameter, callee
implementation, hidden argument, boundary change or caller-local rodata is needed.

## Bounded diagnosis

Every seed was scored at O2 first and O1 second. The unchanged workbench diagnosed
both residuals before refinement using fresh relocated candidates and canonical
target objects held only in temporary storage. Frames40/48 bytes are correct.

- `233B0`:27/92, with curve-byte extraction and scalar assignment/allocation
  differences. Splitting the meaningful product and fixed-point shift into two
  assignments leaves27/92 unchanged.
- `239A4`:21/76, exclusively register allocation in the diagnosed alignment.
  A full-word masked mode selector leaves21/76 unchanged.

Only one natural expression control per residual, then frozen. Previously
exhausted narrow-formal declaration/register/K&R/SDK-word controls were not
repeated. There are no artificial locals, keepers, volatile barriers, padding or
assembly. Initial and directed receipts record all forms without native dumps.

| Function | Final O2 | Final O1 |
|---|---:|---:|
| 22678 |MATCH|73/73 +20 extras|
| 233B0 |27/92|91/92 +17 extras|
| 239A4 |21/76|76/76 +11 extras|

All final O2 rows have zero extras/unresolved/unverified/errors. Only22678 has
complete relocated word equality. The other two live solely under `nonmatch/`.

The pinned CC0 [MusyX source](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/src/musyx/runtime/synthmacros.c)
provides AddKey and envelope-calculation family context, not authentic N64
layout/ABI authority. Revision `78d2e16e4905fc675952162d331c24d5198b2687`, source blob
`a3203f9e0032fa5e9d08fa5e51da80acf4cab0c0`, CC0 license blob
`0e259d42c996742e9e3cba14c677129b2c1b6311`. Native omitted stores,32-bit flags,
packed field locations and external calls remain authoritative.

## Tests and replay

Three strict-C89 ASan/UBSan tests compile the actual final sources:120 note-update
cases including wrap/clamp and real return forwarding;150 envelope cases with
unsigned overflow, genuine third input and positive-divisor extremes;48 two-mode
cases preserving live helper flag mutations and exact call contracts. Helpers
are synthetic contracts, not native implementations. LeakSanitizer alone is
disabled under ptrace; no heap is allocated.

```sh
python3 cloud/work/boot_tail/BT05-larger-trio/verify.py
python3 -m unittest discover -s cloud/work/boot_tail/BT05-larger-trio -p 'test_*.py' -v
```

Use the pinned compiler setup or `IDO_DIR` for an existing hash-verified copy.
`verification.json` binds six final rows/source hashes. Protected manifest/getter,
all439 extents/99,120B, all161 locks and whitespace checks pass. No shared target,
compiler/scorer, symbol, lock, layout, runtime image, farm, spec or production gate
changes. Accepted800D1248 and restricted helper work remain untouched. No ROM,
raw assembly, object or credentials are published. Central owns aggregate CI;
merging remains checker-owned.

Independent paired review PASS binds source commit
`f2d103aa18d9f3872e3e9f1ae3cd566ba58cc522`, tree
`d475b32bb1a72584af1699dbf4e00b9fcc8368f9`, inspecting only the delta from frozen
`f06d238e`. All six rows/hashes were reproduced from immutable Git copies and
all three sanitizer groups passed. The packed-note wrap, actual third input,
aligned duration contracts, positive divisor and post-helper flag reloads were
independently checked. `independent_review.json` records one292-byte match and
two honest nonmatches totaling672 B.
