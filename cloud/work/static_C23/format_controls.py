exec(open('/home/cburnes/agents/C/scratch/static-C23/check.py').read().split('results=[]')[0])
base=(p/'inflate_fixed.reuse_i.c').read_text()
variants={}
for name,s in [('volatile',base),('cast',base.replace('gDisplayListSize=0;','*(s32*)&gDisplayListSize=0;'))]:
 s=s.replace(';',';\n').replace('{','{\n').replace('}','\n}\n')
 variants[name+'_lines']=s
 variants[name+'_looplines']=s.replace('for(i=0;\ni<144;\ni++)','for(i=0;i<144;i++)').replace('for(;\ni<','for(;i<').replace(';\ni++)',';i++)')
rr=[]
for label,s in variants.items():
 src=p/('inflate_fixed.'+label+'.c');src.write_text(s);obj=src.with_suffix('.o');cp=subprocess.run([str(tk/'ido/cc'),'-c','-g0','-O2','-mips2','-G','0','-non_shared','-I'+str(p/'include'),'-I'+str(p),str(src),'-o',str(obj)],capture_output=True,text=True)
 if cp.returncode:r={'variant':label,'error':cp.stderr}
 else:
  a=cloudscore.text_words(p/'inflate_fixed.target.o');b=cloudscore.text_words(obj);r={'variant':label,'strict_score':scoring.score(p/'inflate_fixed.target.o',obj,stack_differences=True),'raw_word_diff':sum(x!=y for x,y in zip(a,b))+abs(len(a)-len(b)),'words':len(b)}
 print(json.dumps(r),flush=True);rr.append(r)
(p/'fixed_format.json').write_text(json.dumps(rr,indent=2)+'\n')
