s=open('mode_b.c').read()
body=s[s.index('void func_800E05F0(ModelView *model)'):]
tail_old=body[body.index('    if(MODE(model)!=2)return;'):]
tail_new=open('tailB.txt').read()
HEAD_OLD='''    if (!D_8010FFC0) return;
    if (D_8010FFCC[original_slot]) {
        D_8010FFCC[original_slot]=0;
        return;
    }
    D_8010FFCC[original_slot]=1;
    if (!D_8010FFC4[original_slot]) return;
'''
V={
 'tailB':[(tail_old,tail_new)],
 'gotoend':[(tail_old,tail_old.replace('    if(MODE(model)!=2)return;','    if(MODE(model)!=2)goto end;').replace('\n}\n','\nend:;\n}\n'))],
}
