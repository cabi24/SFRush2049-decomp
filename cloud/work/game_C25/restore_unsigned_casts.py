from pathlib import Path
import re
p=Path('cloud/work/game_C25');n='func_800AB18C';text=Path('build/codex-r4300-sweep/sources/'+n+'.c').read_text();body=text[text.rindex('void '+n+'('):]
for var,val in [('var_t5','temp_f6'),('var_t2','temp_f18_2')]:
 start=body.index('if (M2C_ERROR(/* cfc1 */)');brace=body.index('{',start);i=brace+1;depth=1
 while depth:
  if body[i]=='{':depth+=1
  elif body[i]=='}':depth-=1
  i+=1
 # Include paired else block.
 assert body[i:].lstrip().startswith('else');j=body.index('{',i);k=j+1;depth=1
 while depth:
  if body[k]=='{':depth+=1
  elif body[k]=='}':depth-=1
  k+=1
 body=body[:start]+var+' = (u32)'+val+';'+body[k:]
constants=set(re.findall(r'\bD_[0-9A-Fa-f]+\b',body))|{'active_player_count'};decls=[]
for name in sorted(constants):
 m=re.search(r'^extern .*?\b'+name+r'\b.*?;$',text,re.M);assert m,name;decls.append(m[0])
ctx='/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */\n#include "types.h"\n#define M2C_FIELD(p,t,o) (*(t)((u8*)(p)+(o)))\n'+'\n'.join(decls)+'\nextern f32 sqrtf(f32);\n#pragma intrinsic (sqrtf)\n'
p.joinpath(n+'.c').write_text(ctx+body)
