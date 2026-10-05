import sys,os
out=sys.argv[1]; os.makedirs(out,exist_ok=True)
src=open('prior.c').read().replace('    f32 weight,inverse;\n','DECL')
L1='    inverse=1.0f-(weight=D_80152708[slot]);\n'
line='        D_80150B70[slot].position[i]=D_80150B70[slot].position[i]*weight+inverse*(delta[i]+position[i]);'
P='D_80150B70[slot].position[i]'
W='D_80152708[slot]'
S='(delta[i]+position[i])'
vs={
 'd':('    f32 weight;\n','    weight=D_80152708[slot];\n', '%s=%s*weight+(1.0f-weight)*%s;'%(P,P,S)),
 'd2':('    f32 weight;\n','    weight=D_80152708[slot];\n', '%s=(1.0f-weight)*%s+%s*weight;'%(P,S,P)),
 'h':('    f32 weight;\n','', '%s=%s*(weight=%s)+(1.0f-weight)*%s;'%(P,P,W,S)),
 'i':('    f32 weight;\n','', '%s=%s*%s+(1.0f-%s)*%s;'%(P,P,W,W,S)),
 'j':('    f32 inverse;\n','    inverse=1.0f-D_80152708[slot];\n', '%s=%s*(1.0f-inverse)+inverse*%s;'%(P,P,S)),
 'k':('    f32 weight,inverse;\n','    inverse=1.0f-(weight=D_80152708[slot]);\n', '%s=%s*weight+(1.0f-weight)*%s;'%(P,P,S)),
}
for k,(d,a,b) in vs.items():
    open('%s/%s.c'%(out,k),'w').write(src.replace('DECL',d).replace(L1,a).replace(line,'        '+b))
