import sys
p=sys.argv[1]
s=open(p).read()
anchor="L46fe70:\nt6 = MEM_U32(t0 + 0);\nt4 = 0x100226b0;\nt5 = t6 + 0x1;\nMEM_U32(t0 + 0) = t5;\nt4 = MEM_U32(t4 + 0);\nv1 = t5;\nat = t4 < t5;\nif (at == 0) {\n//nop;\ngoto L46fd98;}\n"
assert s.count(anchor)==1
hook=r'''
if (getenv("SPLITLOG") && dkwb_cdx_globalcolor_ordinal == atoi(getenv("SPLITLOG"))) {
  uint32_t lrs[2]; int k; lrs[0]=MEM_U32(s2+0); lrs[1]=MEM_U32(s1+0);
  uint32_t st=MEM_U32(sp+100);
  fprintf(stderr,"[SPLIT] old=%u new=%u start_bb=%u\n", MEM_U32(lrs[0]+4), MEM_U32(lrs[1]+4), st?MEM_U16(MEM_U32(st+0)+8):9999);
  for (k=0;k<2;k++) { uint32_t n=MEM_U32(lrs[k]+8); fprintf(stderr,"[SPLIT]  %s:", k?"rest ":"grown");
    while (n) { uint32_t bb=MEM_U32(n+0); fprintf(stderr," %u(u%u d%u f19=%u f20=%u f23=%u)", MEM_U16(bb+8), MEM_U16(n+16), MEM_U8(n+18), MEM_U8(n+19), MEM_U8(n+20), MEM_U8(n+23)); n=MEM_U32(n+4); }
    fprintf(stderr,"\n");
    { int b; uint32_t off; for (off=0xc; off<=0x14; off+=8) { fprintf(stderr,"[SPLIT]   %s vec+%x:", k?"rest ":"grown", off);
      for (b=0;b<260;b++) if (f_bvectin(mem, sp, b, lrs[k]+off)) fprintf(stderr," %d", b); fprintf(stderr,"\n"); } }
  }
}
'''
s=s.replace(anchor, anchor+hook)
open(p,'w').write(s)
