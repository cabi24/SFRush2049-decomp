exec(open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w12b/d5e64/v20.py').read())
a=V['A']
OBJ='state->definition->item[i].object'
nob=R(R(R(R(a,' s32 object;\n',''),'  object=state->definition->item[i].object;\n',''),'if(object==-1)','if('+OBJ+'==-1)'),'(object,','('+OBJ+',')
nob=R(nob,'0.0f,object,','0.0f,'+OBJ+',')
V={}
V['A_noobj']=nob
V['A_s16i']=R(a,' int i;\n',' s16 i;\n')
V['A_u8i']=R(a,' int i;\n',' u8 i;\n')
V['A_noi_ptr']=None
del V['A_noi_ptr']
