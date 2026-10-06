import re
src=open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w11f/dfba0/h3.c').read()
m=re.search(r'^void func_800DFBA0\(.*?^\}\n', src, re.S|re.M)
b0=src[m.start():m.end()]
def R(s, a, b):
    assert a in s, a
    return s.replace(a, b)
A='''            fraction=(f32)(model->steering>>2)/80.0f;
            count1++;
            if(fraction!=0.0f) {
                if(fraction<0.0f) fraction=-fraction;
                fraction=fraction<0.0f ? 0.0f : fraction>1 ? 1 : fraction;
                fraction=1-(1-fraction)*(1-fraction);
            }
            if(fraction<model->power[i])fraction=model->power[i];
'''
V={}
V['before']=R(b0,A,A.replace('            count1++;\n','').replace('            fraction=(f32)(model->steering>>2)/80.0f;\n','            count1++;\n            fraction=(f32)(model->steering>>2)/80.0f;\n'))
V['afterif']=R(b0,A,A.replace('            count1++;\n','').replace('            if(fraction<model->power','            count1++;\n            if(fraction<model->power'))
V['afterpow']=R(b0,A,A.replace('            count1++;\n','')+'            count1++;\n')
V['powlocal']=R(R(b0,A,A.replace('            if(fraction<model->power[i])fraction=model->power[i];\n','            if(fraction<*pw)fraction=*pw;\n').replace('            fraction=(f32)(model->steering>>2)/80.0f;\n','            pw=&model->power[i];\n            fraction=(f32)(model->steering>>2)/80.0f;\n')),'    LayerSet *set;\n','    LayerSet *set;\n    f32 *pw;\n')
