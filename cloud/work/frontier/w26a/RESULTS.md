# w26a - inflate/lzss TU recipe, gViTimeAccumLo retirement, rename leads (worktree lane)

Branch `w26a` (worktree `.claude/worktrees/w26a`, from wave26 46780d3e). Not pushed. Static numbers only.

Status: STUB (work in progress). Start: static 617/667, 139,624 B (86.87%).

## Early finding (inflate block recipe)
The missing piece of w25a's "cc -O2 -S + as1 -O1" recipe is the `.loc` directives: as1 treats each `.loc` as a
scheduling barrier. With every `\t.loc\t` line removed from the `cc -S` output (plus `.option O2`), as1 -O1
reproduces retail's prologue/delay-slot schedule: inflate_loop and inflate_block (w25a's 21/23 and 99/105) are
byte-identical, inflate_read_bits too (non-volatile draft). Recipe implemented as a driver wrapper
`tools/ido_sas/cc` (compiler name `ido5.3s`, in progress).

## Progress (mid-session)
- Recipe final: `cc -S -g0 -O2`, drop `.loc`/`.option O<n>`/comment lines, assemble `cc -c -O1 -g` (the -g stops
  as1 -O1 moving a macro branch's `sltiu at` into the previous branch's delay slot: huft_build, inflate_dynamic).
- Promoted: inflate_block, inflate_loop, inflate_read_bits (832 B, 8dfc3680); huft_build, inflate_stored,
  inflate_fixed (2,740 B, 3c9774d2). Toolkits (unpinned): 4eb51387 (pre -g driver, superseded), 8bdf0cae (current).
- Merged wave26 eda352c5 (8d0f7923); rom_tu.h `gDisplayListSize` without volatile + owned pins rebound (4069372a).
