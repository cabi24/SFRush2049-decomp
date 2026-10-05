# Independent behavior-first reassessment of 80087110

## Result

A new single-emitter C formulation matches the protected function's packet
behavior on **6,926 cases**, checked independently against native execution,
the existing scalar oracle, host C with UBSan, and a fully GNU-linked stock IDO
object. Every one of the target's 445 instructions is exercised, including all
16 combinations of zero/nonzero mode, axis flips, and stretch flag.

This is **semantic research, not a matching candidate or a ROM claim**. The new
source deliberately factors the operation into one SDK command site. Stock
IDO 5.3 `-g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul` emits 135 instructions
(540 bytes), against 445 (1,780 bytes) in the protected target. This proves that
behavior does not require the inherited eight-emitter layout. It does not prove
that a single emitter could reproduce the target under the original unknown
source context.

The original four-word near-match was preserved. A separate source-backed test
replaced its simplified SDK context with authentic pinned SDK macros and then
the complete original SDK Gfx union, leaving the function body unchanged.
**All three contexts generate identical linked bytes**, still differing at
0x4c8, 0x4cc, 0x4d0 and 0x4d4. Thus the simplified union/alignment member and
unsigned-literal macro spelling do not explain this residual for this body.

## What the step-back changed

`DESIGN.md` was recorded before opening the old candidate C. It separates
machine facts from source hypotheses, including unknown parameter source types,
caller names, macro revision, block scopes, alias context, and optimization
settings. Actual native calls carry six O32 words, but a source declaration
with narrower parameters cannot be disproved merely by a missing callee-side
narrowing operation: it might describe a restricted caller domain.

The clean source uses unsigned arithmetic for modular operations and shifts,
without signed-overflow assumptions. Unlike the older source, all 3,000
full-word random stress cases are also checked as defined host C. No unsigned
to signed reinterpretation helper was needed in the final formulation: signed
comparisons use the original coordinate arguments and clipped signed globals;
command coordinates and texture arithmetic remain unsigned.

The strongest additional result is a **discriminating test for narrowing**.
The existing 3,828 host-domain cases do not reject an intentionally narrowed-y
mutant. Large y values in the older masking tests can lose their high bits at
packet encoding regardless of whether y was incorrectly narrowed earlier.
Two new cases make narrowing change clipping/rejection and reject the mutant:

- y=65536 with bottom=100 and wide clip bounds: the native function rejects;
  y narrowed to signed short becomes zero and emits.
- y=32768 with clip_top=32700: the native y remains above the clip boundary;
  narrowed y becomes negative and is clipped, changing the encoded coordinate.

These establish sensitivity on the broad native-word domain. They do not prove
that gameplay produces either coordinate combination.

The 98 new cases also reject five other plausible incorrect reconstructions:
strict-empty changes to `<=`, applying stretch in mode zero, computing the
texture origin from post-stretch height, applying the half-texel phase without
vertical flip, and clipping the command bottom a second time. Each mutation is
a deliberate wrong-contract control, never a candidate selected by score.

## SDK provenance and scope

The prior cloud report referred to a maintainer-only SDK header that was not
present here. This reassessment independently fetched these primary SDK-source
mirrors and verified their Git blob identities:

- [libreultra 2.0I gbi.h](https://github.com/n64decomp/libreultra/blob/master/include/2.0I/PR/gbi.h),
  revision 1.128, 1997-11-26; Git blob b418e0321f48acd86187a066a873211f0e0ff9cb
- [libreultra 2.0I mbi.h](https://github.com/n64decomp/libreultra/blob/master/include/2.0I/PR/mbi.h),
  revision 1.135, 1997-11-26; Git blob 9956ef20eeb3090533be9d0f513ada78e208041c

The F3DEX_GBI_2 opcode branch, gSPTextureRectangle, gImmp1, and _SHIFTL are used
unaltered. The full Gfx union has unsigned-int command words and a long-long
alignment member plus other SDK command views. This demonstrates an authentic
SDK context, **not proof that this was the game's exact original SDK revision**.
The SDK headers themselves are not redistributed in this research packet.

Host LP64 expands unused unsigned-long-bitfield views, giving sizeof(Gfx)=16;
IDO O32 uses the native size 8. The host wrapper counts Gfx objects and reads
only the two command words, so it checks packet values and packet-count advance
without pretending the host union's physical layout is the N64 layout. The
compiled MIPS replay checks actual eight-byte strides and guarded storage.

## Diagnostic interpretation

The clean source compiles to a much smaller CFG, with a 56-byte frame versus
104. Workbench diagnosis correctly identifies a structural mismatch; its
positional/aligned counts and generic stack-home suggestions are not a recipe
for this deliberately different source topology. Direct full linking agrees
with the project relocator, and rejects the shorter extent. No score or gate
was changed.

The original near-match's four-word residual survives authentic SDK context,
while the clean source independently corroborates the packet contract. This
supports keeping the proven behavior fixed and investigating real original
source/compiler context, rather than treating local score improvement as
additional behavioral evidence. It does not establish a compiler bug or a
unique original C spelling.

## Reproduce

1. Use the pinned IDO/binutils environment documented by this checkout.
2. Place the two SDK files above under ignored build/87110_fresh_behavior/,
   naming gbi.h `libreultra_gbi.h` and retaining `mbi.h`. The script validates
   exact Git blob identities before generating sdk_context.h.
3. Run the behavior check first (no IDO compile):

       python cloud/work/texture_rect_fresh_behavior/verify_behavior.py

4. Then add `--compile` to run the stock compile, full linker verification,
   and compiled-behavior replay. If this worktree predates the independent
   verifier, pass `--verifier-dir /path/to/cloud/work/texture_rect_verification`.
5. Run the unchanged-body context test:

       python cloud/work/texture_rect_fresh_behavior/context_probe.py \
         --baseline cloud/work/frontier/w4a/func_80087110/best.c \
         --verifier-dir cloud/work/texture_rect_verification

`fresh_verification.json` and `context_verification.json` contain scalar/hash
receipts. Source and Python syntax checks pass. Host C89 compilation succeeds;
pedantic warnings are from original SDK long-long alignment and one bitfield
extension, supported by the successful stock IDO compile.

No existing source, locked match, native target, gate, or prior candidate was
modified. No protected instruction streams, objects, ROM data, raw disassembly,
or downloaded SDK headers are included in this packet. No publish action was
performed by this worker.
