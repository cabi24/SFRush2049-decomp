p="uopt.trace.c"; s=open(p).read()
old='''    fprintf(stderr, "[SAVE] web=%d units=%d bvectcard=%d total=%.3f flag34=%d\\n", (int)MEM_U32(lr+4), (int)MEM_U32(lr+28), (int)card, (double)tot, (int)MEM_U8(lr+34));
'''
assert old in s
new=old+'''    { int i; fprintf(stderr,"[SAVE]   set0c:"); for (i=0;i<200;i++) if (f_bvectin(mem,0,i,lr+0xc)) fprintf(stderr," %d",i);
      fprintf(stderr,"\\n[SAVE]   set14:"); for (i=0;i<200;i++) if (f_bvectin(mem,0,i,lr+0x14)) fprintf(stderr," %d",i); fprintf(stderr,"\\n"); }
'''
s=s.replace(old,new,1); open(p,"w").write(s); print("ok")
