import sys,os,itertools
out=sys.argv[1]; os.makedirs(out,exist_ok=True)
src=open('prior.c').read().replace('    f32 weight,inverse;\n','DECL')
L1='    inverse=1.0f-(weight=D_80152708[slot]);\n'
line='        D_80150B70[slot].position[i]=D_80150B70[slot].position[i]*weight+inverse*(delta[i]+position[i]);'
P='D_80150B70[slot].position[i]'
W='D_80152708[slot]'
S='(delta[i]+position[i])'
pre={'A':'    weight=%s;\n'%W,'B':'    inverse=1.0f-(weight=%s);\n'%W,'C':'    weight=%s; inverse=1.0f-weight;\n'%W,'D':'','E':'    inverse=1.0f-%s;\n'%W}
invf={'i':'inverse','e':'(1.0f-weight)','g':'(1.0f-%s)'%W}
wf={'w':'weight','g':W}
decl={'2':'    f32 weight,inverse;\n','i':'    f32 inverse;\n','w':'    f32 weight;\n','n':''}
for (pk,p),(ik,iv),(wk,w),(dk,d) in itertools.product(pre.items(),invf.items(),wf.items(),decl.items()):
    body='%s=%s*%s+%s*%s;'%(P,P,w,iv,S)
    text=p+body
    need=set()
    if 'weight' in text: need.add('weight')
    if 'inverse' in text: need.add('inverse')
    has={'2':{'weight','inverse'},'i':{'inverse'},'w':{'weight'},'n':set()}[dk]
    if not need<=has: continue
    open('%s/%s%s%s%s.c'%(out,pk,ik,wk,dk),'w').write(src.replace('DECL',d).replace(L1,p).replace(line,'        '+body))
