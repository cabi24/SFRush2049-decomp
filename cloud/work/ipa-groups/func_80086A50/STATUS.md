# func_80086A50 -> 1/1 MATCH (real function, 387 words)

Strict (`python3 cloud/work/tools/extscore.py cloud/work/ipa-groups/func_80086A50`, and
`python3 tools/cloud/score.py group cloud/work/ipa-groups/func_80086A50 --allow-unverified`):
387/387 words. Only caveat: the 2 words of the switch jump-table address (`lui`/`lw` into `.rodata`)
are section-relative relocations the scorer cannot verify (hence `--allow-unverified`).

**It is IPA, not ABI.** Reads only `a0`, but it is a leaf compiled with -O3 interprocedural register
assignment: it uses `v0,v1,a1,t6-t9` only. Standalone `-O1/-O2/-O3` give 251/387 words differing
(shape 93%, registers t0-t5 used). As a group with stand-in callers (`callers.c`, two call sites,
callers in `keep`) it matches exactly. Real callers: func_8008705C, func_800878E0, object_render,
func_8008A46C, sound_init.

What it does: `switch (mode)` (0..4) emits RDP set-other-mode / set-combine words into the display list
`D_80149438` (`g = D_80149438; D_80149438 = g + 1;`), selected by `D_8012E608 & 0x10/0x20` and
`D_8014A248`; finally `D_8014A248 = mode`.

Tricks that mattered (found by `cloud/work/r5_a/gen.py` + `run.py` search):
- push as `g = D_80149438; D_80149438 = g + 1;` (not `g = D_80149438++`): fixes the `lui/addiu v1` placement.
- each push written `{ u32 x = w1; g->w0 = w0; g->w1 = x; }` (w1 held in a temp, stored second): reproduces
  the `ori` order and t-register assignment in every push (plain `w0=;w1=` or `w1=;w0=` leave 90-180 words off).
  These temps are a compile-affecting quirk and may not be the original source form.

Open: with this real callee, `func_8008705C` (3/45 differ) and `func_800878E0` (19/74 differ) choose
`a2`/`a3` for the mask/pointer where retail uses `t0`/`t1` (see cloud/work/r5_a/gall). Retail callers
behave as if the callee clobbered a2/a3; nothing in the callee's words explains it.
