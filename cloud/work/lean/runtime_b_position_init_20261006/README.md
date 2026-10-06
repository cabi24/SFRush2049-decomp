# Image B four-position initializer: lean match candidate

Target: `B:func_8038D054`, `[0x8038D054,0x8038D1A8)`, **340 bytes**.
Frozen base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.

## Observed result

**MATCH: all 85/85 words**, no extra words, unresolved symbols, unverified
relocations or errors. Actual standalone IDO 5.3 flags:
`-g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul`.
Three own-data float literals also compare against authenticated image B.

The initial complete source differed in 51/85 words and was one instruction
short. Assigning each local offset before its corresponding mode flag follows
the native case ordering and produces the full match. No extra locals,
artificial reads, volatile, fake helpers/callers or assembly were added.

## Reproduce

```
python3 cloud/work/lean/runtime_b_position_init_20261006/reproduce.py
```

The minimal helper reads the frozen native asset, authenticates the complete
image B in memory, and compiles/scores using the unchanged canonical tool.
The frozen base lacks an image-B own-data artifact, so the helper supplies the
required data context without publishing raw bytes. Literals at B:0x80394DAC,
0x80394DB0 and 0x80394DB4 decode to binary32 0.1, 0.35 and 0.025. Image SHA-256:
`b55fc2d1b22eb1ebdf01286a69a181b496da7b45ff7aec888ff74b7db748e7cd`.

## Source and assumptions

Allocates from the external pool at B:0x80395EE8, returns null on failure, sets
mode flags/lifetime, copies the supplied position into four triples, then
separates the last two along their second coordinate. Mode 0/1/2/3 selects
flags 1/2/4/8 and offsets 0.75/0.5/0.35/0.025; mode 3 also clears lifetime.
The native third incoming argument is a second position pointer, genuinely
homed but otherwise unused; the real FCE0 caller supplies its position twice.
The pool allocator remains external and unmodified.

Field names and the four-position interpretation are descriptive native
reconstruction; original names and whole-function arcade ancestry are unknown.
Valid C inputs require mode 0..3, three accessible source floats, and an
allocator result with the accessed 64-byte prefix. Other modes read an
uninitialized local on the native path and are not claimed valid C inputs.
Aliasing, pool lifetime and full-game reachability are not established here.

This is observed full compile/score evidence, not accepted coverage or a ROM
integration claim. No extra tests, independent acceptance checks or CI wait
were run. The independent checker owns acceptance and integration.
