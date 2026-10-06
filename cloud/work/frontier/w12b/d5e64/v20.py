exec(open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w12b/d5e64/v16.py').read())
b0=open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w12b/d5e64/b16.c').read()
# b16: player, ready, state, i, object; state pre-check; ready=&..; if(ready!=0); post-check
ST=' state=&D_80140420[player];\n'
CHK=' if(!D_8010FFC0 || vehicle->disabled)return;\n'
RD=' ready=&D_8010FFC4[player];\n'
DIS=' if(ready!=0);\n'
nb=R(R(R(b0,ST,''),RD,''),DIS,'')
V={}
V['A']=R(nb,CHK,RD+ST+CHK+DIS)
V['A2']=R(nb,CHK,RD+ST+CHK+' if(*ready);\n')
V['B']=R(nb,CHK,ST+RD+CHK+DIS)
V['C']=R(nb,CHK,RD+CHK+ST+DIS)
V['D']=R(nb,CHK,RD+ST+CHK+' if(ready);\n')
