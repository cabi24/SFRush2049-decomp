import itertools,sys
HEAD=open('/home/user/SFRush2049-decomp/cloud/work/r5_d/f7f3c.c').read()
HEAD=HEAD[:HEAD.index('void func_800F7F3C')]
def sortloop(key, opt):
    k=lambda x: key.replace('X',x)
    sty=opt.get('sty','named')
    if sty=='named':
        cmp="""        a = D_80143F54[j];
        b = D_80143F54[j + 1];
        if (%s < %s) {
          D_80143F54[j + 1] = a;
          D_80143F54[j] = b;
        }
""" % (k('a'),k('b'))
    elif sty=='idx':
        cmp="""        a = D_80143F54[j];
        b = D_80143F54[j + 1];
        ia = D_8014A250[a].idx;
        ib = D_8014A250[b].idx;
        if (%s < %s) {
          D_80143F54[j + 1] = a;
          D_80143F54[j] = b;
        }
""" % (key.replace('D_8014A250[X].idx','ia'),key.replace('D_8014A250[X].idx','ib'))
    else:
        cmp="""        if (%s < %s) {
          a = D_80143F54[j];
          b = D_80143F54[j + 1];
          D_80143F54[j + 1] = a;
          D_80143F54[j] = b;
        }
""" % (k('D_80143F54[j]'),k('D_80143F54[j + 1]'))
    return f"""    for (i = 0; i < last; i++) {{
      for (j = 0; j < last; j++) {{
{cmp}      }}
    }}
"""
LAB=[0]
def ties(key,opt):
    LAB[0]+=1; opt=dict(opt); opt['lab']=LAB[0]
    k=lambda x: key.replace('X',x)
    iv=opt['tiei']
    if opt.get('tform')=='idx0':
        k2=key.replace('D_8014A250[X].idx','IDX0') if 'D_8014A250' in key else key
        return f"""    f = D_8014A250[D_80143F54[0]].idx;
    for ({iv} = 1; {iv} < n; {iv}++) {{
      if ({k2.replace('IDX0','f').replace('X','')} == {k('D_80143F54[%s]'%iv)}) {{
        D_80150B68[{iv}] = 1;
        D_80150B60++;
      }}
    }}
"""
    if opt.get('tform')=='goto':
        return f"""    i = 1;
    if (n > 1) {{
    L{opt['lab']}:
      if ({k('D_80143F54[0]')} == {k('D_80143F54[i]')}) {{
        D_80150B68[i] = 1;
        D_80150B60++;
      }}
      i++;
      if (i < n) goto L{opt['lab']};
    }}
"""
    if opt.get('tform')=='dw':
        return f"""    i = 1;
    if (n > 1) {{
      do {{
        if ({k('D_80143F54[0]')} == {k('D_80143F54[i]')}) {{
          D_80150B68[i] = 1;
          D_80150B60++;
        }}
        i++;
      }} while (i < n);
    }}
"""
    if opt.get('tform')=='ptr':
        return f"""    q = D_80143F54 + 1;
    for ({iv} = 1; {iv} < n; {iv}++, q++) {{
      if ({k('D_80143F54[0]')} == {k('q[0]')}) {{
        D_80150B68[{iv}] = 1;
        D_80150B60++;
      }}
    }}
"""
    return f"""    for ({iv} = 1; {iv} < n; {iv}++) {{
      if ({k('D_80143F54[0]')} == {k('D_80143F54[%s]'%iv)}) {{
        D_80150B68[{iv}] = 1;
        D_80150B60++;
      }}
    }}
"""
K4='D_80152038[D_8014A250[X].idx].key'
K6='D_80152818[D_8014A250[X].idx].b931'
K6b='D_8015256C[D_8012E77C[D_8014A250[X].idx]]'
K0='D_80152818[D_8014A250[X].idx].b238'
def gen(opt):
    nt=opt['ntype']; lt=opt['ltype']
    pre=f"""void func_800F7F3C(void)
{{
  s32 i;
  s32 j;
  {nt} n;
  {lt} last;
  s8 *p;
  s8 *q;
  s16 ia;
  s16 ib;
  s16 f;
  s8 a;
  s8 b;
  for (i = 0; i < D_80151AD0; i++) {{
    D_80143F54[i] = i;
  }}
"""
    def blk(keys,extra=''):
        s="    n = D_8014A108;\n    last = n - 1;\n"
        for kk in keys[:-1] if False else []: pass
        return s
    body=pre
    body+="  if (D_8014A110 == 4) {\n"+blk(0)+sortloop(K4,opt)+ties(K4,opt)
    body+="  } else if (D_8014A110 == 6) {\n"+blk(0)+sortloop(K6,opt)+"    i = 0;\n"+sortloop(K6b,opt)+ties(K6,opt)
    body+="  } else {\n"+blk(0)+sortloop(K0,opt)+ties(K0,opt)+"  }\n}\n"
    return HEAD+body
def gen_gl(opt):
    t=gen(opt)
    t=t.replace("    n = D_8014A108;\n    last = n - 1;\n","")
    t=t.replace("< last","< D_8014A108 - 1").replace("< n;","< D_8014A108;").replace("n > 1","D_8014A108 > 1")
    return t
if __name__=='__main__':
    opt=dict(named=1,tiei='i',ntype='s16',ltype='s32')
    open(sys.argv[1],'w').write(gen(opt))
