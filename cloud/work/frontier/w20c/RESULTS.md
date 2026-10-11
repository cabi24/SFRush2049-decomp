# w20c - static boot tail: code-identical, not-lockable set (tooling/data lane, worktree)

Branch `w20c` (worktree `.claude/worktrees/w20c`, from master 81d91115). Not pushed. Static numbers only.

STATUS: in progress (stub written early).

| function | B | cause | fix | LOCK | TU | promoted |
|---|---:|---|---|---|---|---|
| func_80021548 | 96 | own jtbl jtbl_8002D930 (global .rodata) | owned .rodata slot | pending | pending | no |
| func_80023BDC | 404 | own jtbl jtbl_8002D950 | owned .rodata slot | pending | pending | no |
| func_8001BE14 | 904 | D_8004F800 label x3 + jtbl_8002D8E8 | reloc_addrs + owned slot | pending | pending | no |
| func_8001B9F8 | 960 | D_8004F800 label x3 + jtbl_8002D8D0 | reloc_addrs + owned slot | pending | pending | no |
| func_800114C0 | 904 | lui 0x8000 / addiu 63 paired as D_8000003F | reloc_addrs constant override | pending | pending | no |
| func_80011104 | 840 | same x2 | same | pending | pending | no |
| func_80013DEC | 780 | unpaired lui/lhu D_80038292 | reloc_addrs HI16/LO16 | pending | pending | no |
