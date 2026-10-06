import re
src=open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w11f/dfba0/h2.c').read()
m=re.search(r'^void func_800DFBA0\(.*?^\}\n', src, re.S|re.M)
b0=src[m.start():m.end()]
def R(s, a, b):
    assert a in s, a
    return s.replace(a, b)
D0='''    s32 i,count0=0,count1=0,count2=0;
    f32 sum0=0.0f,sum1=0.0f,sum2=0.0f;
    f32 fraction,scale;
    s16 slot=model->index;
    LayerSet *set;
'''
V={}
V['base']=b0
V['sum1first']=R(b0,D0,D0.replace('f32 sum0=0.0f,sum1=0.0f,sum2=0.0f;','f32 sum1=0.0f,sum0=0.0f,sum2=0.0f;'))
V['ilast']=R(b0,D0,D0.replace('s32 i,count0=0,count1=0,count2=0;','s32 count0=0,count1=0,count2=0,i;'))
V['sepinit']=R(b0,D0,'''    s32 i,count0,count1,count2;
    f32 sum0,sum1,sum2;
    f32 fraction,scale;
    s16 slot=model->index;
    LayerSet *set;
    count0=count1=count2=0;
    sum0=sum1=sum2=0.0f;
''')
V['sepinit2']=R(b0,D0,'''    s32 i,count0,count1,count2;
    f32 sum0,sum1,sum2;
    f32 fraction,scale;
    s16 slot;
    LayerSet *set;
    slot=model->index;
    count0=0; count1=0; count2=0;
    sum1=0.0f; sum0=0.0f; sum2=0.0f;
''')
V['slotlate']=R(R(b0,D0,D0.replace('    s16 slot=model->index;\n','    s16 slot;\n')),'    if(sum0<sum1','    slot=model->index;\n    if(sum0<sum1')
V['sw']=b0
L=b0
L=R(L,'        if (model->contact[i]==0) {','        switch (model->contact[i]) {\n        case 0:')
L=R(L,'''            sum0+=fraction;
        } else if(model->contact[i]==1) {''','''            sum0+=fraction;
            break;
        case 1:''')
L=R(L,'''            sum1+=fraction;
        } else if(model->contact[i]==2 || model->contact[i]==3) {''','''            sum1+=fraction;
            break;
        case 2: case 3:''')
L=R(L,'''            sum2+=fraction;
        }''','''            sum2+=fraction;
            break;
        }''')
V['sw']=L
