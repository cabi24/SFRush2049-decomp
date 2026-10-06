b0=open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w12b/d5e64/b16.c').read()
def R(s,a,b):
    assert a in s, a
    return s.replace(a,b)
V={}
OBJ='state->definition->item[i].object'
nob=R(R(R(R(b0,' s32 object;\n',''),'  object=state->definition->item[i].object;\n',''),'if(object==-1)','if('+OBJ+'==-1)'),'(object,','('+OBJ+',')
nob=R(nob,'0.0f,object,','0.0f,'+OBJ+',')
V['noobj']=nob
nrd=R(R(R(b0,' s8 *ready;\n',''),' ready=&D_8010FFC4[player];\n if(ready!=0);\n',' if(&D_8010FFC4[player]!=0);\n'),' *ready=1;',' D_8010FFC4[player]=1;')
V['noready']=nrd

V['noready_noobj']=R(R(R(nob,' s8 *ready;\n',''),' ready=&D_8010FFC4[player];\n if(ready!=0);\n',' if(&D_8010FFC4[player]!=0);\n'),' *ready=1;',' D_8010FFC4[player]=1;')
