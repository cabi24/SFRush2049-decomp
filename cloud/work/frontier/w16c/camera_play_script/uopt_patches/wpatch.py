p="uopt.trace.c"; s=open(p).read()
old='''    fprintf(stderr, "[SAVE] web=%d units=%d bvectcard=%d total=%.3f flag34=%d\\n", (int)MEM_U32(lr+4), (int)MEM_U32(lr+28), (int)card, (double)tot, (int)MEM_U8(lr+34));
'''
assert old in s
new=old+'''    { static int dumped=0; if (!dumped && getenv("CUPDUMP")) { int i, n=(int)MEM_U32(0x1001c904); dumped=1;
      for (i=0;i<n;i++) { uint32_t bb=MEM_U32(MEM_U32(0x100226a4)+i*4); if (bb) fprintf(stderr,"[BBW] %d w=%d\\n",(int)MEM_U16(bb+8),(int)MEM_U32(bb+44)); } } }
'''
s=s.replace(old,new,1); open(p,"w").write(s); print("ok")
