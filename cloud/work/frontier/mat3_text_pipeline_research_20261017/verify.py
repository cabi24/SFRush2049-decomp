"""Sealed partial text-pipeline research; no accepted or new candidate byte credit."""
from pathlib import Path
import argparse,contextlib,hashlib,importlib.util,io,json,re,shutil,struct,sys
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[3];sys.path.insert(0,str(ROOT));sys.path.insert(0,str(HERE))
from tools.cloud import score
import renderer_behavior,renderer_negatives,storage_semantics,layout
INPUTS=['README.md','verify.py','scope.json','storage_handoff.json','renderer_behavior.py','renderer_negatives.py','storage_semantics.py','storage_semantics.c','layout.py','trace_fidelity.py','group/group.json']
NEGATIVE_FIXTURES={'wrong_zero_kerning':291,'wrong_draw_endpoint':1,'wrong_newline_leading':6,'wrong_advance_spacing':2,'wrong_initial_page':0,'wrong_alpha_flag':36}
STORAGE_FIXTURES={'wrong_capacity':5769,'wrong_spacing_scale':1,'wrong_coordinate_shift':1,'wrong_depth_boundary':1921,'lost_full_unlock':5769}
def require(v,msg):
 if not v:raise RuntimeError(msg)
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def scope():return json.loads((HERE/'scope.json').read_text())
def report(obj,name):
 data,sections=score._elf(obj);ti=score._text_index(sections);selected=[s for i,sec in enumerate(sections) if sec['type']==2 for s in score._symbol_table(data,sections,i) if s['name']==name and s['type']==2 and s['section']==ti]
 require(len(selected)==1,'missing symbol '+name);s=selected[0]
 with contextlib.redirect_stdout(io.StringIO()):r=score.compare(obj,name,show=0)
 row=json.loads(json.dumps(vars(r)));row['errors']=[re.sub(r'retail [0-9a-fA-F]+, got [0-9a-fA-F]+','retail [redacted], got [redacted]',x) for x in row['errors']]
 words=score.text_words(obj)[s['value']//4:(s['value']+s['size'])//4];frames=[65536-(w&65535) for w in words if w>>26==9 and (w>>21)&31==29 and (w>>16)&31==29 and w&32768]
 return {'comparison':row,'symbol_bytes':s['size'],'frame':frames[0] if frames else 0}
def inventory(obj):
 data,secs=score._elf(obj);ti=score._text_index(secs);symbols=sorted([s for i,se in enumerate(secs) if se['type']==2 for s in score._symbol_table(data,secs,i) if s['type']==2 and s['section']==ti],key=lambda s:s['value']);words=score.text_words(obj);cursor=0
 for s in symbols:
  require(s['value']==cursor,'unaccounted emitted function gap');cursor+=s['size']
  if s['name'].startswith('du_gfx_'):
   require(s['size']==8,'deleted semaphore extent');w,z=words[s['value']//4:s['value']//4+2];require(w>>26==0 and (w>>21)&31==31 and w&63==8 and z==0,'unexpected semaphore residue')
 require(all(w==0 for w in words[cursor//4:]),'nonzero trailing residue')
 return {'functions':{s['name']:{'offset':s['value'],'bytes':s['size']} for s in symbols},'zero_alignment_bytes':len(words)*4-cursor}
def graph(names):
 syms=score.image_symbols();inv={a:n for n,a in syms.items()};inv.update({syms[n]:n for n in score.targets() if n in syms});inv.update({syms[n]:n for n in names});result={};indirect={}
 for name in names:
  calls=set();jumps=[];words=score.targets()[name]
  for i,w in enumerate(words):
   if w>>26 in (2,3):
    dest=(syms[name]&0xf0000000)|((w&0x3ffffff)<<2)
    if not syms[name]<=dest<syms[name]+len(words)*4:calls.add(inv.get(dest,hex(dest)))
   if w>>26==0 and (w&63==9 or w&63==8 and (w>>21)&31!=31):jumps.append({'offset':i*4,'register':w>>21&31,'link':w&63==9})
  result[name]=sorted(calls)
  if jumps:indirect[name]=jumps
 return {'direct':result,'external':sorted({c for v in result.values() for c in v if c not in names}),'nonreturn_register_jumps':indirect}
def inputs(spec):
 targets=score.targets();addresses=score.image_symbols();names=spec['members']
 return {'files':{n:digest(HERE/n) for n in INPUTS+['group/'+n for n in spec['files']]},'focused_test_sha256':digest(ROOT/'tests/cloud/test_mat3_text_pipeline_research.py'),'tools':{n:digest(ROOT/n) for n in ['tools/cloud/score.py','tools/cloud/owndata.py','blob_matched.lock.json']},'compiler':{n:digest(Path(score.ido(n))) for n in ['cc','cfe','ujoin','uld','usplit','umerge','uopt','ugen','as1']},'protected_manifest_sha256':digest(score.ASM_DIR/'SHA256SUMS'),'protected_symbols_sha256':digest(score.ASM_DIR/'symbols.json'),'protected_own_data_manifest_sha256':digest(score.owndata.artifact_dir(score.ASM_DIR)/'SHA256SUMS'),'targets':{n:{'address':hex(addresses[n]),'bytes':len(targets[n])*4,'sha256':hashlib.sha256(struct.pack('>%dI'%len(targets[n]),*targets[n])).hexdigest()} for n in names}}
def verify_scope(spec):
 s=scope();require(spec['claims']==[] and spec['context']==[] and spec['members']==s['native_members'] and spec['files']==s['files'] and spec['keep']==s['keep'],'research group scope changed')
 require(len(spec['members'])==39 and spec['flags']=='-g0 -O3 -mips2 -G 0 -non_shared' and not spec.get('allow_unverified') and not spec.get('unprototyped'),'compiler or group contract changed')
 require(s['claims']==[] and s['new_candidate_bytes']==s['accepted_image_bytes']==0 and s['installable'] is False,'scope false credit')
def verify_report(r):
 s=scope();require(r['status']=='VERIFIED_PARTIAL_RESEARCH' and r['protocol_version']==1 and r['claims']==[] and r['new_candidate_bytes']==r['accepted_image_bytes']==0 and r['canonical_or_ROM_verification'] is False and r['installable'] is False,'false research credit')
 require(set(r['rows'])==set(s['native_members']) and len(s['strict_observed_extents'])==34 and len(s['partial_observed_extents'])==5,'native inventory changed')
 for n,size in s['strict_observed_extents'].items():
  row=r['rows'][n];c=row['comparison'];require(c['differing']==0 and not any(c[k] for k in ['extra_words','unresolved','unverified','errors']) and row['symbol_bytes']==size==c['total']*4==r['inputs']['targets'][n]['bytes'],'strict observation changed '+n)
 for n,x in s['partial_observed_extents'].items():
  row=r['rows'][n];c=row['comparison'];require(row['symbol_bytes']==x['bytes'] and row['frame']==x['frame'] and all(c[k]==x[k] for k in ['differing','total','extra_words']) and len(c['unverified'])==x['unverified_count'] and not c['unresolved'] and not c['errors'],'partial boundary changed '+n)
  require(c['total']*4==r['inputs']['targets'][n]['bytes'],'protected partial extent changed '+n)
 inv=r['inventory'];require(set(inv['functions'])==set(s['native_members'])|set(s['nonnative_residues']) and inv['zero_alignment_bytes']==12,'complete emitted inventory changed')
 for n,row in r['rows'].items():require(inv['functions'][n]['bytes']==row['symbol_bytes'],'inventory extent changed '+n)
 for n,size in s['nonnative_residues'].items():require(inv['functions'][n]['bytes']==size,'nonnative residue changed')
 b=r['renderer'];require(b['status']=='PASS_BOUNDED' and b['fixtures']==2560 and b['executed_words']==[259,259] and b['branches']==[44,44] and b['claims']==[],'renderer behavior scope changed')
 require(b['cfg_unreachable']==['0xf0','0x21c','0x25c','0x3cc','0x41c'],'renderer CFG scope changed')
 require(len(b['native_missing'])==22,'renderer coverage changed')
 require(set(r['renderer_negatives'])==set(NEGATIVE_FIXTURES),'renderer negative inventory')
 for n,i in NEGATIVE_FIXTURES.items():require(r['renderer_negatives'][n]=={'status':'REJECTED_MODEL_DIFFERENCE','fixture':i,'state_differs':n=='wrong_alpha_flag','events_differ':True},'renderer semantic negative changed '+n)
 require(r['storage']['positive']=={'returncode':0,'stdout':'storage semantics passed: 737280 fixtures\n','stderr':''},'storage positive changed')
 require(set(r['storage']['negative'])==set(STORAGE_FIXTURES),'storage negative inventory')
 for n,i in STORAGE_FIXTURES.items():require(r['storage']['negative'][n]=={'returncode':3,'stdout':'','stderr':f'storage differential failure at fixture {i}\n'},'storage semantic negative changed '+n)
 require(r['layout']['positive']==layout.EXPECTED and set(r['layout'])=={'positive','short_color','wide_nonprop','wide_glyph','wide_loaded'} and all(v!=layout.EXPECTED for k,v in r['layout'].items() if k!='positive'),'native layout controls changed')
 require(r['one_caller_control']['audio_doppler_calc']['symbol_bytes']==8 and r['one_caller_control']['audio_doppler_calc']['comparison']['differing']==281 and r['one_caller_control']['world_velocity_integrate']['symbol_bytes']>r['rows']['world_velocity_integrate']['symbol_bytes'],'genuine caller causal control changed')
def verify_seal(actual,saved):
 verify_report(actual);verify_report(saved);require(actual==saved,'fresh sealed receipt changed')
def verify(out,record=False):
 out=Path(out).resolve();out.mkdir(parents=True,exist_ok=True);spec=json.loads((HERE/'group/group.json').read_text());verify_scope(spec);binding=inputs(spec)
 if not record:
  saved=json.loads((HERE/'verification.json').read_text());verify_report(saved);require(saved['inputs']==binding,'source/target/tool binding changed')
  trace=json.loads((HERE/'trace_fidelity.json').read_text());require(trace['inputs']==binding and trace['complete_objects_identical'] and trace['stock_object_sha256']==trace['trace_object_sha256']==saved['object_sha256'],'trace binding changed')
 obj=out/'candidate.o';score.compile_group(HERE/'group',obj);rows={n:report(obj,n) for n in spec['members']}
 if not record:require(digest(obj)==saved['object_sha256'],'complete object changed')
 # Remove the second genuine caller without inserting an artificial keeper.
 d=out/'one_caller';shutil.copytree(HERE/'group',d,dirs_exist_ok=True);one=json.loads(json.dumps(spec));one['files'].remove('text_storage.c');one['members'].remove('menu_input_process');one['keep'].remove('menu_input_process');(d/'group.json').write_text(json.dumps(one,indent=2)+'\n');other=out/'one_caller.o';score.compile_group(d,other)
 r={'status':'VERIFIED_PARTIAL_RESEARCH','protocol_version':1,'claims':[],'new_candidate_bytes':0,'accepted_image_bytes':0,'canonical_or_ROM_verification':False,'installable':False,'inputs':binding,'object_sha256':digest(obj),'rows':rows,'inventory':inventory(obj),'call_graph':graph(spec['members']),'renderer':renderer_behavior.verify(obj),'renderer_negatives':renderer_negatives.run(HERE/'group',out/'renderer_negatives'),'storage':{'positive':storage_semantics.run(HERE/'group'),'negative':{n:storage_semantics.run(HERE/'group',n) for n in STORAGE_FIXTURES}},'layout':layout.run(HERE/'group',out/'layout',score),'one_caller_control':{n:report(other,n) for n in ['world_velocity_integrate','audio_doppler_calc']}}
 verify_report(r)
 if record:(HERE/'verification.json').write_text(json.dumps(r,indent=2)+'\n')
 else:verify_seal(r,saved)
 return r
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--record',action='store_true');p.add_argument('--out',type=Path,default=ROOT/'build/mat3_text_pipeline_research_20261017/verify');a=p.parse_args();verify(a.out,a.record);print('PASS: partial text research; 34 strict observations, five failures, empty claims; bounded renderer/storage semantics')
