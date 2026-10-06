exec(open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w11f/d5e64/v10.py').read().split('V = {}')[0])
V = {}
V['d_after'] = H1 + R(b1, ' s32 object;\n', ' s32 object;\n volatile s32 dz;\n')
V['d_after_use'] = H1 + R(R(b1, ' s32 object;\n', ' s32 object;\n volatile s32 dz;\n'), ' *ready=1;', ' *ready=1;\n dz=0;')
V['d_before_use'] = H1 + R(R(b1, ' s8 *ready;\n', ' volatile s32 da,db,dc;\n s8 *ready;\n'), ' *ready=1;', ' *ready=1;\n da=db=dc=0;')
