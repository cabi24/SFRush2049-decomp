# w26a - inflate/lzss TU recipe, gViTimeAccumLo retirement, rename leads (worktree lane)

Branch `w26a` (worktree `.claude/worktrees/w26a`, from wave26 46780d3e). Not pushed. Static numbers only.

Status: STUB (work in progress). Start: static 617/667, 139,624 B (86.87%).

## Early finding (inflate block recipe)
The missing piece of w25a's "cc -O2 -S + as1 -O1" recipe is the `.loc` directives: as1 treats each `.loc` as a
scheduling barrier. With every `\t.loc\t` line removed from the `cc -S` output (plus `.option O2`), as1 -O1
reproduces retail's prologue/delay-slot schedule: inflate_loop and inflate_block (w25a's 21/23 and 99/105) are
byte-identical, inflate_read_bits too (non-volatile draft). Recipe implemented as a driver wrapper
`tools/ido_sas/cc` (compiler name `ido5.3s`, in progress).
