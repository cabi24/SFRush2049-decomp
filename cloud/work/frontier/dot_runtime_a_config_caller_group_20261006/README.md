# Runtime A configuration flags: real-caller group candidate

**Local group MATCH: func_8039B120, 61/61 words / 244 bytes**, image A
`[0x8039B120,0x8039B214)`, at normal whole-program O3 plus mandatory
`r4300_mul`. Zero differing, unresolved, unverified or extra words for B120.
Fixed base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.

The existing natural standalone seed is five register words off. This group
includes complete reconstructions of both actual direct callers rather than
stand-in callers:

- `func_8039B214`, 836 bytes, calls B120 at +0x2E0: directional grid/code editing,
  deletion/insertion, validation outcome flags and configuration refresh.
- `func_8039BE48`, 760 bytes, calls B120 at +0x254: player synchronization,
  optional configuration-driven object setup, and screen/widget initialization.

With only the first caller, IDO inlined B120 and left an 8-byte stub. Adding the
second genuine caller produces B120's complete strict matching body. The two
callers remain unclaimed NONMATCH context: B214 is 125/209 words off; BE48 is
190/190 off plus eight extra words and an unpaired HI16 relocation in its own
unclaimed extent. Their enclosing/private ABI context is not
reconstructed here. This is not a claim that the whole module or closure is
matched or safely integrable.

## Genuine wrapper return correction

BE48 consumes the return register from both calls to 0x8008E398, storing the
resulting handles in D_803B3448 and D_803B344C. The production sign_extend_call
source currently declares void despite forwarding the real allocator's s32
result unchanged. The genuine returning implementation is supplied separately in
wrapper_return.c, not merely asserted by a forward declaration. It scores
standalone strict MATCH, 10/10 against the protected main-blob target. The
runtime-A group sees its external s32 contract only. This respects the genuine
image boundary and does not inline main-blob code into the runtime unit. No
duplicate byte claim or production source/lock change is made.

The source preserves the actual config selector snapshot across resource calls.
Record declarations express observed offsets/strides, not original type names.
BE48's unknown float data value at D_803B95B4 remains a named external native
storage anchor in unclaimed context; no literal value or owned-data proof is
invented. No exact arcade donor is established. No synthetic caller, forced
register, unused local, assembly, or compiler patch is used.

## Reproduce

From repository root with documented IDO:

```
python3 tools/cloud/score.py group cloud/work/frontier/dot_runtime_a_config_caller_group_20261006 --claims --targets asm/us/ovl_a
```

To inspect all three runtime scores omit --claims. group.json lists every
runtime body and declares only B120 as a claim; external_sources documents the
separate wrapper sidecar without compiling it into the unit. Compile that
sidecar with score.py fn and --targets asm/us/blob to reproduce its own score. Do not move B120 into
cloud/matches: it is not a standalone match.

Lean source/context/scoring handoff only. No full tests, semantic proof,
image/compression/ROM acceptance, or coverage gain is claimed. The independent
checker owns validation, integration and merging.

Correction after initial publication: the first draft compiled the returning
wrapper inside the runtime group. It is now separated at the actual main-blob
boundary. B120 remains strict MATCH; the local claim did not depend on that
cross-image inlining. Context scores above describe the corrected build.
