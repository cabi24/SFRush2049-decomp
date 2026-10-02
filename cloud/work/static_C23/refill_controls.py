exec(open('/home/cburnes/agents/C/scratch/static-C23/check.py').read().split('results=[]')[0])
rr=[]
for n in ['inflate_block','inflate_dynamic']:
 base=(p/(n+'.c')).read_text()
 variants={'volatile_ptr':base.replace('extern u8 *gInflateInPtr,*gInflateInEnd;','extern u8 *volatile gInflateInPtr;extern u8 *gInflateInEnd;')}
 for name,s in list(variants.items()):
  variants[name+'_ternary']=s.replace('u32 word; if(gInflateInPtr<gInflateInEnd) {gInflateInPtr+=2;word=(gInflateInPtr[-1]<<8)|gInflateInPtr[-2];} else word=inflate_read_bits();b|=word<<k;', 'b |= (gInflateInPtr<gInflateInEnd ? (gInflateInPtr+=2,(gInflateInPtr[-1]<<8)|gInflateInPtr[-2]) : inflate_read_bits()) << k;')
 variants['ternary']=variants['volatile_ptr_ternary'].replace('extern u8 *volatile gInflateInPtr;extern u8 *gInflateInEnd;','extern u8 *gInflateInPtr,*gInflateInEnd;')
 for label,s in variants.items():
  src=p/(n+'.'+label+'.c');src.write_text(s);obj=src.with_suffix('.o');cp=subprocess.run([str(tk/'ido/cc'),'-c','-g0','-O2','-mips2','-G','0','-non_shared','-I'+str(p/'include'),'-I'+str(p),str(src),'-o',str(obj)],capture_output=True,text=True)
  if cp.returncode:r={'function':n,'variant':label,'error':cp.stderr}
  else:
   a=cloudscore.text_words(p/(n+'.target.o'));b=cloudscore.text_words(obj);r={'function':n,'variant':label,'strict_score':scoring.score(p/(n+'.target.o'),obj,stack_differences=True),'raw_word_diff':sum(x!=y for x,y in zip(a,b))+abs(len(a)-len(b)),'words':len(b)}
  print(json.dumps(r),flush=True);rr.append(r)
(p/'refill_controls.json').write_text(json.dumps(rr,indent=2)+'\n')
