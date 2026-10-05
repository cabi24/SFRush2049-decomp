# func_800E3430 (arcade `controls()`), with its caller func_800E3724 (arcade `sym()`)

`best.c` is the two-member group source (same file as `../groups/sym_controls/group.c`).

- State: code identical in the real group, own float literals unverified (not strict).
- It cannot match alone: it uses `$f20`-`$f26` unsaved (`alone_control.c` scores 187/189 at `-O3`).
  `frontier show` does not flag this; its unsaved-register detector only looks at integer registers.
- Closes when `score.py` can verify a function's own `.rodata` (plan workstream B). The retail words at
  0x801243F0..0x80124404 are 0.7, 0.33, 0.99, 0.05, 9.5493, 0.9, in the order the group object emits them.
