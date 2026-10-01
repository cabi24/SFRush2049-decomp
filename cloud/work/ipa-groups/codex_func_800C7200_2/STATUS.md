# Codex real pool-allocation group, 2026-10-01

Strict `score.py group cloud/work/ipa-groups/codex_func_800C7200_2 --claims` reports **MATCH** for func_800C7200: 64 words, 256 bytes. Flags: `-g0 -O3 -mips2 -G 0 -non_shared`. No locked claim overlap. The sole claim is the pool allocator; both real callers are unclaimed context.

## Mechanism, including source quirk

Repaired and used real func_800C813C and func_800CC50C bodies, replacing the pilot's synthetic callers. The allocator remains internal; both real callers are kept. This restores the target four-wide temp ring and reproduces the pilot's four-word residual.

The target keeps loop limit 64 in v0. On pool exhaustion its final instructions are `jr ra; nop`, returning whatever remains in v0 (64). An explicit `return (void **)0x40` in C creates an extra `li v0,64`. The matched source falls off the nonvoid function on this exhausted-pool path: that removes the rematerialization and changes 4 differing words to 2. Joining `var_v1=0` and pointer initialization onto the same physical line fixes the remaining scheduled swap: 2 -> strict MATCH.

**This source has a nonvoid fall-through quirk / undefined C return on exhaustion.** It is not proven original source and must be called out in the commit. The current exact compiler emits the same deterministic return register and every target word; do not silently replace the exhaustion path with NULL or explicit 64, or change its line layout. The ordinary successful allocations return their real slot pointer. No unreachable switch, stand-in or fabricated caller was used.

## Real context repairs and audit limits

Previous five-member generated group did not compile. This bounded group retains the two real callers and leaves real helpers external instead of adding broken seed bodies. Repairs, checked against full target assembly:

- C7200 returns a slot pointer (`void **`), not inferred s32; its four branches map empty pointer slots to 44-byte objects and return the slot address. Pointer arithmetic remains byte-scaled where required.
- C813C loads a pointer from D_80144D68[index*16], not a single byte. Its store to D_80117430 uses the low byte of the real index rather than the generator's uninitialized unksp9F. Its call to C7EC8 receives the actual current object pointer; the target conveys that via s0.
- CC50C's second parameter is a signed-byte output pointer, not s32. It reads a float from D_80111754[index*4], not a byte. Its two audio_reverb_update calls convey the actual input/output buffer pointers in the second parameter; the old generated seed passed zero. Target audio_reverb_update overwrites a0 from a1 on entry.

The bodies still use historical header/prototype and raw-field reconstructions. They are real logic, not empty/generated fallback stubs, and have no active M2C_ERROR placeholders. Their helper-call register conventions and frames are not fully reconstructed: C813C is 222/225 words different (target frame160 vs compiled88), and CC50C is 162/190 (target104 vs compiled136). These context bodies must remain unclaimed and retail assembly must stay linked. This is a verified allocator claim, not verified whole-UI-module C.

The apparent 22-byte-vs-88-byte memcpy destination offset in CC50C is correct: the seed expression uses s32* +0x16 (88 bytes), as in target +0x58. It was retained after audit rather than blindly cast to a byte pointer.

## Verification sequence

Initial repaired real-context build: 32/64 due init-order pool coloring. Restoring index-first init: 4/64. New fall-through return: 2/64. Same-line initializers: MATCH. After additional real caller semantic repairs, exact final files were copied to Rocky A and score.py --claims again returned MATCH; context remained unclaimed. Root must independently score, run image gate, and verify full ROM SHA-1.

No splice, lock/layout change, commit, or coordinator-state mutation was performed by this worker.
