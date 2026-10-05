# A171 sqrtf intrinsic proof

Fresh native IDO compilation is strict zero and all 16 original slot bytes are identical, with no relocations or warnings. Publication source: `sqrtf.c`, SHA e4d770042b7fb90b905586ee2cab06543a746f7e33aa0d906a53cfdbbefef8ba. Literal flags: `-g0 -O2 -mips2 -G 0 -non_shared`. Claims remain empty until coordinator production gates.

The original routine performs hardware single-precision square root in the return delay slot. Its runtime extent is 8 bytes; the existing slot is 16 bytes with 8 bytes of zero alignment. Preserve the existing slot, and credit only the proven runtime extent if owned-text policy separates padding. `raw_proof.json` records full slot identity and actual native symbol metadata.

Source deliberately depends on IDO's `#pragma intrinsic(sqrtf)` after a genuine sqrtf declaration. The same-name C call denotes the hardware intrinsic under that pragma; compiling it with an unsupported or ignored pragma would recurse. It is not a portable implementation. No assembly, synthesized opcode, stand-in helper, source padding, alias or scoring exception is used. Hardware square-root behavior, including exceptional inputs, follows the original instruction.

Provenance: original `asm/us/EFC0.s`; SDK `reference/repos/ultralib/src/gu/sqrtf.s` identifies sqrtf as this hardware operation and explains the older compiler's inability to emit it. The initial diagnostic placed the pragma before declaration: warning773 and a recursive call, strict700/raw7. That rejected source and report remain immutable in `initial_undeclared*`; placing the required declaration before the pragma repairs the compiler contract.

No shared SDK source, accepted lock, layout, ROM gate or commit changed. Parent must independently replay and complete production acceptance.
