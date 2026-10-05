import itertools,re
src=open('prior.c').read()
line='        D_80150B70[slot].position[i]=D_80150B70[slot].position[i]*weight+inverse*(delta[i]+position[i]);'
assert line in src
P='D_80150B70[slot].position[i]'
n=0
for oa,m1,m2,ia,inv in itertools.product([0,1],[0,1],[0,1],[0,1],['inverse','(1.0f-weight)']):
    s=('delta[i]+position[i]','position[i]+delta[i]')[ia]
    a=(P+'*weight','weight*'+P)[m1]
    b=(inv+'*('+s+')','('+s+')*'+inv)[m2]
    e=(a+'+'+b, b+'+'+a)[oa]
    out=src.replace(line,'        %s=%s;'%(P,e))
    n+=1; open('v/v%02d.c'%n,'w').write(out)
