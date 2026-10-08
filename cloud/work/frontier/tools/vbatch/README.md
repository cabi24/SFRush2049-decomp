# vbatch: generate and score many variants per function

Hand-writing a handful of variants leaves most of the search space untried. This kit makes a sweep of
hundreds of variants a two-command job.

1. **Template.** Copy your best source to `<lane>/<fn>/t1.c` and mark choice points in it:
   ```c
   x = /*@{*/a + b/*@| b + a @| (a + b) @}*/;          /* 3 options; option 0 is what the file compiles as */
   /*@{*/s32 i; s32 n;/*@| s32 n; s32 i; @}*/            /* declaration order */
   /*@{*//*@| if (p) {} @}*/                           /* add a compiled-out read (option 0 = nothing) */
   /*@{st*/D = -1;/*@|@}*/ call(); /*@{st*//*@| D = -1; @}*/   /* linked by name: move a statement */
   ```
   Options are inserted verbatim. An option may span lines or be empty, but may not contain `*/`.
2. **Expand:** `python3 cloud/work/frontier/tools/vbatch/vgen.py t1.c <lane>/<fn>/b1 --max 300`
   This writes `v0000.c` (all option 0) and onward, plus `manifest.tsv` mapping each file to its option
   indices. Above `--max` it samples randomly; vary `--seed` for another sample.
3. **Score on the builder:** `sh cloud/work/frontier/tools/vbatch/vbatch.sh <lane> <lane>/<fn>/b1 <fn> [--flags "-g0 -O3 -mips2 -G 0 -non_shared"] [--keep a,b] [--top 20] [--jobs 2] [--sort strict|mnem]`
   This prints the best first (sorted by strict; `--sort mnem` ranks by structure instead) and saves the
   ranking to `b1/score.txt`. The columns are:
   - strict differing words;
   - mnemonic-aligned missing words, i.e. structure;
   - word-aligned missing words;
   - size delta.

   Your lane scratch must exist with `tools/cloud` synced. `--keep` compiles each file as a one-file -O3
   group with that keep list, for IPA-shaped functions.
4. **Iterate:** read which options the top files share in `manifest.tsv`. Fix those into a new template, add
   new choice points, and run b2, b3, and so on.
5. **Confirm:** bscore compiles **standalone**. A strict 0 is a lead only. Confirm with `score.py fn`
   (`MATCH`), then in the whole-program unit (`blob_unit --tag <lane> score … --neighbours`, 0 locked bodies
   differ). For unit-shaped functions, the standalone ranking is still a guide, but re-score the top 3–5 in
   the unit. Unit runs on the Pi are slow and shared, so only run the top few.

**What to put in choice points** (handoff §7 items 5–7):
- declaration order and which variables are declared;
- operand order of `==`, `+`, `*`, `&` and `|`;
- `t = a; t -= b;` versus `t = a - b`;
- the same value through a temp versus inline;
- loop form (`for`, `while`, `do`, with a down-counter or pointer walk);
- signed versus unsigned types and casts;
- `volatile` on a global;
- an empty `if (x) {}` with different `x`;
- early `return` versus `else`;
- array index versus pointer;
- one shared loop variable versus several;
- statement order where semantics allow;
- line splitting (as1 scheduling).

Many small axes multiply quickly: 8 axes × 2–3 options gives 256–6,561 combinations.
