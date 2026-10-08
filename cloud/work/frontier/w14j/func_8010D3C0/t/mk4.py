s=open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w14j/func_8010D3C0/t/t3.c').read()
a=s.index("        switch(effect->effect) {")
b=s.index("        }\n", s.index("default:", a))+len("        }\n")
sw=s[a:b]
ifc='''        if(effect->effect==350) { kind=0; amount=D_80121D60; }
        else if(effect->effect==351) { kind=1; amount=D_80121D64; }
        else if(effect->effect==352) { kind=2; amount=D_80121D68; }
        else if(effect->effect==353) { car->timer=800; return; }
        else if(effect->effect==354) {
            car->extra_flags|=1;
            car->state=1;
            car->boost=30.0f;
            car->field936=0.666667f;
            car->color=255;
            return;
        }
        else if(effect->effect==355) { kind=3; amount=D_80121D6C; }
        else if(effect->effect==356) { kind=4; amount=D_80121D70; }
        else if(effect->effect==357) { kind=5; amount=D_80121D74; }
        else if(effect->effect==358) { kind=6; amount=D_80121D78; }
        else if(effect->effect==359) {
            car->extra_flags|=10;
            car->field916=0.0333333f;
            car->field920=0.05f;
            car->field924=0.0f;
            return;
        }
        else if(effect->effect==360) { kind=7; amount=D_80121D7C; }
        else { kind=1; amount=D_80121D64; }
'''
new='        /*@{sw*/'+sw+'/*@|'+ifc+'@}*/'
s=s[:a]+new+s[b:]
open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w14j/func_8010D3C0/t/t4.c','w').write(s)
