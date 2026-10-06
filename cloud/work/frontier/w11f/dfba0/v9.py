import re
src=open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w11f/dfba0/h4.c').read()
m=re.search(r'^void func_800DFBA0\(.*?^\}\n', src, re.S|re.M)
b0=src[m.start():m.end()]
def R(s, a, b):
    assert a in s, a
    return s.replace(a, b)
INIT='''    slot=model->index;
    count0=0; count1=0; count2=0;
    sum1=0.0f; sum0=0.0f; sum2=0.0f;
    for (i=0;i<4;i++) {
'''
V={}
V['forinit']=R(b0,INIT,'''    slot=model->index;
    sum1=0.0f; sum0=0.0f; sum2=0.0f;
    for (count0=count1=count2=i=0;i<4;i++) {
''')
V['dowhile']=R(R(b0,INIT,'''    slot=model->index;
    count0=0; count1=0; count2=0;
    sum1=0.0f; sum0=0.0f; sum2=0.0f;
    i=0;
    do {
'''),'''        }
    }
    if(sum0<sum1''','''        }
    } while(++i<4);
    if(sum0<sum1''')
V['dowhile2']=R(R(b0,INIT,'''    slot=model->index;
    count0=0; count1=0; count2=0;
    sum1=0.0f; sum0=0.0f; sum2=0.0f;
    i=0;
    do {
'''),'''        }
    }
    if(sum0<sum1''','''        }
        i++;
    } while(i<4);
    if(sum0<sum1''')
V['while']=R(R(b0,INIT,'''    slot=model->index;
    count0=0; count1=0; count2=0;
    sum1=0.0f; sum0=0.0f; sum2=0.0f;
    i=0;
    while(i<4) {
'''),'''        }
    }
    if(sum0<sum1''','''        }
        i++;
    }
    if(sum0<sum1''')
