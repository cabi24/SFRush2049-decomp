# patch_add.py IN OUT: oracle on top of w16d's uopt.spl.c. Env ADD_SYM=s ADD_BB=b ADD_AT=c (W5D_PROC procedure only):
# when formlivbb is first called for symbol s in block c, first create a livbb for s in block b (captured from any
# earlier formlivbb call in block b). Diagnostic only.
import sys
s = open(sys.argv[1]).read()
anchor = 'L4641cc:\n'
assert s.count(anchor) == 1
hook = r'''{ static uint32_t add_bbp = 0; static int add_done = 0; const char *e_ = getenv("ADD_SYM");
  if (e_ && w5d_selected(mem)) {
    unsigned want_bb = (unsigned)atoi(getenv("ADD_BB")), at_bb = (unsigned)atoi(getenv("ADD_AT"));
    if (MEM_U16(a1 + 8) == want_bb) add_bbp = a1;
    if (!add_done && add_bbp && MEM_U16(a0 + 2) == (unsigned)atoi(e_) && MEM_U16(a1 + 8) == at_bb) {
      add_done = 1;
      fprintf(stderr, "[ADD] livbb sym=%u bb=%u before bb=%u\n", MEM_U16(a0 + 2), want_bb, at_bb);
      f_formlivbb(mem, sp - 0x200, a0, add_bbp, sp - 0x100);
    }
  } }
'''
s = s.replace(anchor, anchor + hook)
open(sys.argv[2], 'w').write(s)
