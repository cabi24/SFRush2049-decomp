import sys,os,itertools
out=sys.argv[1]; os.makedirs(out,exist_ok=True)
src=open('prior.c').read().replace('    f32 weight,inverse;\n','DECL')
L1='    inverse=1.0f-(weight=D_80152708[slot]);\n'
line='        D_80150B70[slot].position[i]=D_80150B70[slot].position[i]*weight+inverse*(delta[i]+position[i]);'
P='D_80150B70[slot].position[i]'
W='D_80152708[slot]'
for (sk,S),(ok,one),(wk,mode),(pk,par) in itertools.product({'pd':'(position[i]+delta[i])','dp':'(delta[i]+position[i])'}.items(),{'f':'1.0f','i':'1'}.items(),{'g':0,'w':1,'x':2}.items(),{'p':1,'n':0}.items()):
    if mode==0: d='';p='';w=W
    elif mode==1: d='    f32 weight;\n';p='    weight=%s;\n'%W;w='weight'
    else: d='    f32 weight;\n';p='';w=None
    if w: body='%s=%s*%s+%s*(%s-%s);'%(P,P,w,S,one,w)
    else: body='%s=%s*(weight=%s)+%s*(%s-weight);'%(P,P,W,S,one)
    if par: body=body.replace('=','=(',1)[:-1]+');'
    open('%s/%s%s%s%s.c'%(out,sk,ok,wk,pk),'w').write(src.replace('DECL',d).replace(L1,p).replace(line,'        '+body))
