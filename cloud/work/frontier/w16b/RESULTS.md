# w16b RESULTS: entity_spawn_init (0x8008EA10, 5544 B, 1386 words, -O3)

| function | bytes | state | flags | scorer output (Pi, `blob_unit --tag w16b --remote-dir rush2049/scratch/frontier/w16b score entity_spawn_init --with entity_spawn_init/best.c --neighbours`) |
|---|---:|---|---|---|
| entity_spawn_init | 5544 | NONMATCH, 61 words (unchanged from w15e); frame 728 = retail, size 1386 = retail | -g0 -O3 -mips2 -G 0 -non_shared | `FAIL entity_spawn_init: 61 of 1386 words differ` / `umerge inlined into it: func_8008B2E4` / `locked bodies that differ in this unit: 0` |

Nothing to integrate: no match, no group, no cloud/matches file, no unit_overrides, no data-symbol changes.
`entity_spawn_init/best.c` is w15e's best.c unchanged (no variant beat it). This lane traced the cause of the
function-top residual down to a specific uopt decision (below). The `probe_*.c` files are the diagnostic
sources the findings refer to; they are not candidates. Stop rule reached (about 3 h of trace work).

## The residual has two coupled causes (traced)

Retail top: the model address `&D_8014A250[slot]` is computed straight into **t1** (a coloured split piece that
lives until the loop preheader, where `sw t1,92(sp)` stores it to its spill home). `surface` has **no** register:
`sh t3,666(sp)` right after the load, then `lh 666(sp)` at both uses. Ours gives surface's top piece t1 and spills
the model address at its definition. Because t1 is one of ugen's temp-ring registers when it is not a colour,
fixing only one of the two shifts the ring by one register and costs ~600 words (probe_v7: 653). Both have to
change together.

1. **surface memory-resident.** `if (&surface) {}` (workbench L28, alias the memory half) reproduces retail's
   surface code exactly (probe_v7). This is a shaping device, not real structure; `volatile s16 surface` is
   wrong (687 words, changes other code). A real source reason for surface being aliased was not found.
2. **Model top piece coloured t1.** Traced with a split-logging uopt (`tools/splitpatch.py`, below):
   - The model web (`ixa(lda D_8014A250, mpy(slot,2056))`) has occurrences at block 0 (surface load) and three
     loop blocks (case 4 `.type`, case 5 `.type`, colour switch `.type`). That is the same set as retail,
     which reloads 92(sp) at exactly those three places.
   - p1 splits it (tot 32 / nocs 48 = save 0.6667 < bestcost 80). f_split starts the grown piece at block 0 and
     grows it through adjacent blocks while `colorsleft != 0` (`f_addadjacents`).
   - **In ours, the growth stops before blocks 43/45/49** (the paths into the loop preheader). Those blocks go to
     the rest piece, so the grown top piece has several exits and is never re-queued: no p1dec record, and the
     model address is spilled at its definition.
   - **With one fewer loop occurrence** (probe_p3, p1, p4: any one of the three removed), save drops to 0.458,
     the web is decided later and the growth takes 43/45/49. The top piece is then re-decided
     (`tot 1, best 0 -> t1`) and the top of the function becomes retail's (probe_p3/p1: rows before the loop
     identical). Split dumps from the two runs (st_s7b vs st_p3) differ only by {43,45,49} moving between pieces.
   - So the cause is **decision timing**. At save 0.6667 the model web ties with the constant-5 web (30/45) and
     wins on the lower web number. It is then decided before the constant 5/4/0 webs, the `i` piece (v0), `fast`
     (a2) and `first` have been processed, and at that point blocks 43/45/49 leave no colour.
     Probe p5 (`fast` raised above model) did not help. Probes p8/p9 (`i` raised) did not help.
3. The spill-home shift (retail model 92, type-copy 84; ours 96/88) means retail has **one more coloured
   expression web ahead of the model web in itable order** (homes run car -628, X -632, model -636, ...). The
   named area is the same (cfe area 624, 8-aligned; an extra unused local does nothing until it crosses 8).
   An extra early web would also change the interference at blocks 43/45/49 and the decision order, so it is
   probably the same missing structure as item 2.

## Other findings
- **umerge inline budget is one statement wide.** start.c plus one compiled-out `if (x) {}` still inlines all
  19 func_8008B2E4 sites. A second one, anywhere, drops the first site (`jal 0x8b2e4`, body 1380, frame 712).
  So retail's source is at most about one small statement larger than ours. Any structural hypothesis must not
  add code.
- Surface written through direct indexing (`D_8014A250[slot].surface[type]`) gives cfe's `ixa(lda, slot)` form,
  not the `mpy` form. It then CSEs with the suspension access and splits from the `.type` uses (probe_v20).
  That gets the top right but breaks the loop: suspension reloads the home, `.type` gets its own a3 web, +9 words.
  Retail shows that `.type` and the top are one web and suspension is separate. That is only consistent with
  the mpy form, i.e. a `Model *` (or equivalent) for the top access.
- `surface` as declaration initialiser vs statement: the initialiser gives retail's colour-load order (t4..t9
  after the surface load); best.c's statement form trades that for the mph/count gains (see w15e a36/a37).

## Best next hypothesis
Find the missing early coloured expression web (item 3). It should first appear before the model address
initialiser, be live through blocks 43/45/49 or change the p1 order there, and cost no more than one small
statement (budget). Candidates not yet tried:
- an address/pointer local for `D_801392D8[slot]` or `&D_8013C098[slot]` initialised at the top;
- the `car->mph` expression kept as a coloured web (e.g. `s16 mph = car->mph` as an initialiser before model);
- `type` converted once (`u32 t = type`) feeding the jump table.

Test each with the split logger. Success is st_*/split.log showing old=<model> with 43/45/49 in the grown piece,
plus spill homes 92/84. Then drop `if (&surface) {}` if surface goes to memory on its own; otherwise disclose it.

## Tool: split-logging uopt (builder, diagnostic only)
`tools/splitpatch.py` patches a copy of the toolkit's `uopt.trace.c` at f_split's growth exit (label L46fe70).
For the procedure whose globalcolor ordinal equals `$SPLITLOG`, it prints old/new web, start block, each piece's
livbb list (u/d counts, flags 19/20/23) and the block bit-vectors (+0xc, +0x14). Build and run:
```
cd ~/rush2049/scratch/frontier/w16b && mkdir -p src2 && cp ../wtk/src/uopt.trace.c ../wtk/src/libc_impl.o ../wtk/src/*.h src2/
python3 splitpatch.py src2/uopt.trace.c && (cd src2 && nice gcc -std=c11 -Os -fno-strict-aliasing -I. -o ../bin/uopt_split uopt.trace.c libc_impl.o -lm)
cd st_LABEL && cp st st.sp && SPLITLOG=94 ../bin/uopt_split -G 0 -Olimit 5000 -mips2 -EB -g0 -O3 merged opt.sp -t st.sp optlog 2>split.log >/dev/null
```
(94 = entity_spawn_init's ordinal in this unit; `ctrace.sh` prints it.) Decoded f_split/f_addadjacents:
- **Start block:** the first livbb with flag20 set and a free register in its block.
- **Growth:** BFS over adjacents. A block is accepted iff `colorsleft` stays non-zero (flag 0x1001eb10 clear).
- **Not expanded from:** blocks with flag23 set, or whose first statement is opcode 0x31.

## Notes
- Snapshot labels on the builder: s0 (start), s6, s7b (= probe_v7), s20, p1..p9. (Label `s7` is actually the
  v18 build. A mislabel; ignore it.)
- No permission denials. Nothing written outside the lane dir and the builder scratch.
