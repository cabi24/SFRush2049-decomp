exec(open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w11f/d5e64/v28.py').read())
V2 = {}
for k in ['d_hs2','rdy2_hs2','d_hs','rdy2_hs']:
    V2[k+'_nv'] = V[k].replace(' f32 vec[3];\n','')
    V2[k+'_v1'] = V[k].replace(' f32 vec[3];\n',' f32 vec[1];\n')
V = V2
