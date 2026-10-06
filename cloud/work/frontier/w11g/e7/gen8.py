import itertools
for basen in ['base','a_lo']:
    src=open('base.c' if basen=='base' else 'v2/a_lo.c').read()
    R={
     'lodef': ("                lo = curve->level;\n","                lo = (s32 *)((u32)curve->level ^ 0);\n"),
     'lodef2': ("                lo = curve->level;\n","                lo = (s32 *)((u32)curve ^ 0);\n"),
     'test': ("                    if (lo[1] >= sample) {\n","                    if (lo[1] >= (sample ^ 0)) {\n"),
     'test2': ("                    if (lo[1] >= sample) {\n","                    if ((lo[1] ^ 0) >= sample) {\n"),
     'segf': ("(sample - lo[0]) / (f32)(lo[1] - lo[0]) + segment)","(sample - lo[0]) / (f32)(lo[1] - lo[0]) + (segment ^ 0))"),
     'lo0': ("(sample - lo[0]) / (f32)(lo[1] - lo[0])","(sample - (lo[0] ^ 0)) / (f32)(lo[1] - lo[0])"),
     'lo1': ("(sample - lo[0]) / (f32)(lo[1] - lo[0])","(sample - lo[0]) / (f32)((lo[1] ^ 0) - lo[0])"),
     'hiuse': ("                if (hi < sample) {\n","                if ((hi ^ 0) < sample) {\n"),
     'hidef': ("                hi = lo[4];\n","                hi = lo[4] ^ 0;\n"),
     'samp': ("                    sample = hi;\n","                    sample = hi ^ 0;\n"),
     'carl': ("                car->level[i] = f;\n","                car->level[(i ^ 0)] = f;\n"),
    }
    for k,(a,b) in R.items():
        if a not in src: print('miss',basen,k); continue
        open('v8/%s_%s.c'%(basen,k),'w').write(src.replace(a,b,1))
