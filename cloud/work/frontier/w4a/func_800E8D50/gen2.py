src=open('prior.c').read()
L1='    inverse=1.0f-(weight=D_80152708[slot]);\n'
line='        D_80150B70[slot].position[i]=D_80150B70[slot].position[i]*weight+inverse*(delta[i]+position[i]);'
P='D_80150B70[slot].position[i]'
W='D_80152708[slot]'
vs={
 'a':('', '%s=%s*%s+(1.0f-%s)*(delta[i]+position[i]);'%(P,P,W,W)),
 'b':('    weight=D_80152708[slot];\n', '%s=%s*weight+(1.0f-%s)*(delta[i]+position[i]);'%(P,P,W)),
 'c':('    inverse=1.0f-D_80152708[slot];\n', '%s=%s*%s+inverse*(delta[i]+position[i]);'%(P,P,W)),
 'd':('    weight=D_80152708[slot];\n', '%s=%s*weight+(1.0f-weight)*(delta[i]+position[i]);'%(P,P)),
 'e':('    weight=D_80152708[slot]; inverse=1.0f-weight;\n', '%s=%s*weight+inverse*(delta[i]+position[i]);'%(P,P)),
 'f':('    inverse=1.0f-(weight=D_80152708[slot]);\n', '%s=weight*%s+inverse*(delta[i]+position[i]);'%(P,P)),
 'g':('    inverse=1.0f-(weight=D_80152708[slot]);\n', '%s=%s*weight+(delta[i]+position[i])*inverse;'%(P,P)),
}
for k,(a,b) in vs.items():
    open('v/%s.c'%k,'w').write(src.replace(L1,a).replace(line,'        '+b))
