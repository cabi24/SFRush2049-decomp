import re
src=open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w12b/comp/d5e64.c').read()
m=re.search(r'^static s8 \*rdy.*?^\}\n', src, re.S|re.M)
b0=src[m.start():m.end()]
def R(s,a,b):
    assert a in s, a
    return s.replace(a,b)
V={}
V['if_after_def']=R(b0,' ready=rdy(player);\n',' ready=rdy(player);\n if(ready){}\n')
V['if_before_loop']=R(b0,' for(i=0;',' if(ready){}\n for(i=0;')
V['if_after_st']=R(b0,' state=&D_80140420[player];\n',' state=&D_80140420[player];\n if(ready){}\n')
V['if_in_loop']=R(b0,'  player_conditional_call(&state->pair[i]);\n','  if(ready){}\n  player_conditional_call(&state->pair[i]);\n')
V['if_end']=R(b0,' *ready=1;',' if(ready){}\n *ready=1;')
