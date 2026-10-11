p="uopt.trace.c"; s=open(p).read()
old='''    fprintf(stderr, "[CUP] web=%d color=%d site=%d bbidx=%d bbno=%d w=%d inc=%.3f\\n", (int)MEM_U32(lr+4), (int)col, site, (int)bbi, (int)MEM_U16(bb+8), (int)MEM_U32(bb+44), (double)inc);
'''
assert old in s
new=old+'''    { uint32_t pr=MEM_U32(MEM_U32(bb+20)+0); uint32_t stt=MEM_U32(pr+32); uint32_t proc=MEM_U32(stt+20);
      fprintf(stderr, "[CUP]   predbb=%d op=%d f26=0x%x proc=0x%x flag12=%d arr=%d\\n", (int)MEM_U16(pr+8), (int)MEM_U8(stt), (int)MEM_U16(stt+26), proc, proc?(int)MEM_U8(proc+12):-1,
        (proc && MEM_U8(proc+12)) ? (int)MEM_U8(MEM_U32(proc+36)+col-1) : -1); }
'''
s=s.replace(old,new,1); open(p,"w").write(s); print("ok")
