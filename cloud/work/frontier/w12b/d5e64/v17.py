exec(open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w12b/d5e64/v16.py').read())
b=V['noready']
ST=' state=&D_80140420[player];\n'
CHK=' if(!D_8010FFC0 || vehicle->disabled)return;\n'
DIS=' if(&D_8010FFC4[player]!=0);\n'
nb=R(R(b,ST,''),DIS,'')
V={}
V['dis_st_chk']=R(nb,CHK,DIS+ST+CHK)
V['st_chk_dis']=R(nb,CHK,ST+CHK+DIS)   # = noready
V['chk_dis_st']=R(nb,CHK,CHK+DIS+ST)
V['dis_chk_st']=R(nb,CHK,DIS+CHK+ST)
V['mid']=R(nb,CHK,' if(!D_8010FFC0)return;\n'+ST+' if(vehicle->disabled)return;\n'+DIS)
V['mid2']=R(nb,CHK,' if(!D_8010FFC0)return;\n'+DIS+ST+' if(vehicle->disabled)return;\n')
