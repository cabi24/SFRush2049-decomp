exec(open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w12b/d5e64/v16.py').read())
b=V['noready']
ns=R(R(R(b,' Player84 *state;\n',''),' state=&D_80140420[player];\n',''),'state->','D_80140420[player].')
V={}
V['nostate']=ns
V['nostate_dis']=R(ns,' if(!D_8010FFC0',' if(&D_80140420[player]!=0);\n if(!D_8010FFC0')
V['nostate_dis2']=R(ns,' if(&D_8010FFC4[player]!=0);\n',' if(&D_8010FFC4[player]!=0);\n if(&D_80140420[player]!=0);\n')
