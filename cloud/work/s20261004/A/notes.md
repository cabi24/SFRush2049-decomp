# s20261004 group A

All four targets sit in one IPA (-O3 whole-program) menu/controller-pak cluster.
None can be matched one function at a time: static callees take their parameters in
callee-saved registers (s1/s5/s8, or a stack home), and they do not save the registers
they clobber. Builds use `tools/cloud/score.py group` (cc -j / uld -kp / -O3), run on
watchman2 from a private copy (`~/rush2049/scratch/s20261004_A`).
Helper scripts: `rs.sh` runs score.py remotely, `rd.sh` + `diff.py` show the want/got
disassembly side by side, and `runfinal.sh` runs score.py and prints its exit code.

## MATCHED (true score 0, exit 0): func_800CD058, func_800CCEFC
Group `menu_cc/` (copied to `func_800CD058.c` and `func_800CCEFC.c`). Flags: `-g0 -O3 -mips2 -G 0 -non_shared`.
- The real closure is CD104, CD058, CCEFC (kept), CCCCC (s5 param), draw_number (s1/s2 params,
  f20/f22 constants) and draw_speedometer (s8 + stack-slot param). There are no synthetic functions.
- func_800CCCCC and draw_number also MATCH as context. func_800CCCCC was the blocker named in
  game_C53.md.
- Key lever for CCEFC: declare the byte flags `s8`, not `u8`. With signed bytes IDO shares the
  constant-1 register across the sb, the empty `for(i=1;i<21;i++){}` and the three s16 stores.

## func_800CC040: best 29/224 (stack offsets only)
Group `cc040/` (`group.c` = `group.best.c`). Members are func_800CC040 and menu_transition
(4/40). menu_item_value_get also MATCHES there (bonus, not a target). Context: func_800CBF2C,
menu_back, and audio_reverb_update (copied from codex_heap_release_a25; it MATCHES, IPA a1/a2 params).
- Every instruction and register is identical to the target. The only difference is the frame:
  184 in the target against 136 here. Locals get slots top-down in declaration order (verified).
  The target layout is node@176, buf[32]@144, size@132 and old@80. Ordering m,node,buf,obj,created,size
  reproduces node, buf and size relative to the top. The target still has 8 more words between
  size and the free-param spill, and 4 more below it, for 12 words in total. Those slots look like
  they come from inlined helpers. Single-call static helpers do inline and add bottom slots, but no
  helper structure I tried reproduced the layout without perturbing codegen. Adding unused locals
  would be padding, so I did not try it.
- func_800CBF2C needs draw_ui_element (t0-t2 IPA params) in the unit to match itself. That does not
  affect func_800CC040.

## func_800CD104: best ~253/272 (frame + register allocation)
Draft is in `menu_cc/group.c` and `cd104/group.c`, with logic fully decoded.
- Frame 224 against 192. The target has many more local slots (entry@220, data@208, created@204,
  file[17]@184, digits@136, list@88), which again suggests inlined helpers.
- Register assignment also differs. The target has name in s7, attempt in s2, i in s0, and hoisted
  s1=10, s3=4, s4='0', s5=&file[9], s6=&D_801211DC. In the sanitizer loop it hoists a1='=' and
  a2='*', and copies a3=attempt.
- Levers that already got closer: a `while(k--)` power loop (no unroll), `*(Name9*)D_801211F8`
  (unaligned lwl/lwr copy), indexing `name[i]`, and storing re-read `name[i]` (v1/v0 copy).
