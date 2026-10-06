# Runtime-A joint transforms: 400-byte local candidate

Frozen base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Stock IDO 5.3 group pipeline; actual flags:
`-g0 -O3 -mips2 -G 0 -non_shared`.

`func_80390A28`, `[0x80390A28,0x80390BB8)`: **400 bytes, 100/100 words MATCH**.
The observed comparison reports zero differing words, unresolved symbols,
unverified relocations, relocation errors and nonzero extra words. The helper
owns no literal data. This is a local candidate awaiting the independent checker,
not accepted cartridge coverage.

The complete helper updates the four joint transforms for a model slot: obtains
the native matrix/position pointer, copies the model's corresponding joint
position with the native 1.25f Y offset, and copies or scales the identity matrix
using the front/rear scale tables. Both input values are real signed-halfword
arguments. Native receives the second in private register s3; the complete real
`func_80390DCC` caller supplies that context through two actual call sites.

The first natural reconstruction differed at nine words. Workbench diagnosed
only scheduling, with identical instruction count, frame, registers and FP work.
Putting the real description-pointer assignment and `for` header on the same
source line closes the nine words under stock IDO's line-aware scheduling.
That deliberate line grouping is part of the source recipe. It adds no operation,
local, dummy argument, pressure function, inline asm or retention barrier.

## Caller and external contracts

90DCC is a complete 1,700-byte reconstruction of the model loading/update path,
but remains unclaimed at **295/425 differing words plus one extra nonzero word**.
Its native frame is232 versus the current192. It receives nine observed inputs:
slot/player halfwords, three appearance bytes, a part-texture halfword,
position/matrix pointers and an alpha byte. The mask's expected player domain
is0–3 after normalization; the native default switch path leaves that stack
value unset, so this research does not assert arbitrary out-of-domain safety.

The existing BC0 resource-cache routine and accepted D2C release stay external;
no old bodies are repeated or credited. Other main-blob routines stay external.
Their native/proven source contracts include s16 handles, the pointer-word
return of B3A0, and the real signed-byte third formal of `sfx_volume_set`.
That explicit byte narrowing is retained even though it contributes to the
caller's remaining code mismatch; no false prototype is used to improve score.

Typed views describe accessed offsets and strides: model state64, load state24,
returned matrix/position48, model-description joint positions at112, and object
flag records68. They do not assert original type names or a general alias/bounds
proof. External model/scale/texture/global data ownership remains with its
respective native image context; none is passed off as function-owned constants.

## Reproduce

With stock IDO and GNU MIPS tools configured:

```
python tools/cloud/score.py --targets asm/us/ovl_a group \
  cloud/work/frontier/dot_runtime_a_joint_transforms_20261006 --claims
```

Only matching compilation/scoring and residual diagnosis were performed. No
acceptance suite, proof packet, CI wait, ROM gate, lock or promotion is included.
