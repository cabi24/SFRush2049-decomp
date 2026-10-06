K=('    f32 k;\n','')
U=('    f32 unused[2];\n','')
LAD='        if (rpm < t->b0) {'
V={
 'base':[],
 'nok':[K],
 'nok_nounused':[K,U],
 'ifb1':[K,(LAD,'        if (t->b1) {}\n'+LAD)],
 'ifb0':[K,(LAD,'        if (t->b0) {}\n'+LAD)],
 'ifb1b0':[K,(LAD,'        if (t->b1) {}\n        if (t->b0) {}\n'+LAD)],
 'seg1diff':[K,('            pitch = (rpm - t->b0) / (t->b1 - t->b0);','            pitch = (t->b1 - t->b0);\n            pitch = (rpm - t->b0) / pitch;')],
 'seg1swap':[K,('            pitch = (rpm - t->b0) / (t->b1 - t->b0);','            pitch = (-(t->b0 - rpm)) / (t->b1 - t->b0);')],
 'cmp_ge':[K,('        } else if (rpm < t->b1) {','        } else if (!(t->b1 <= rpm)) {')],
}
