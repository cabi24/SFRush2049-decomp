exec(open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w11f/d5e64/v8.py').read().split('V = {}')[0])
import itertools
b0 = R(B, '&D_8010FFC4[player]', 'rdy(player)')
decls = [' s8 *ready;\n',' Player84 *state;\n',' State20 *pair;\n',' int i;\n',' s32 object;\n']
blk = ''.join(decls)
assert blk in b0
V = {}
orders = [
 [3,0,1,2,4],[0,1,2,4,3],[3,4,0,1,2],[1,2,3,4,0],[0,3,1,2,4],[1,3,0,2,4],
]
for o in orders:
    d = ''.join(decls[k] for k in o)
    s = b0.replace(blk, d)
    # vec before ready
    s = s.replace(' s8 *ready;\n', ' f32 vec[3];\n s8 *ready;\n')
    V['o'+''.join(map(str,o))] = H1 + s
