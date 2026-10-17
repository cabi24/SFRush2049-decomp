#!/usr/bin/env python3
"""Replay the research-only LoadCars group, preserving every locked context body."""
import argparse,contextlib,hashlib,io,json,re,shutil,struct,subprocess,sys,tempfile
from pathlib import Path
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[3]
sys.path.insert(0,str(ROOT/'tools/cloud'));import score
NAME='audio_interrupt_handler';UNMATCHED='audio_frame_sync'
PRESERVED=['func_8009768C','audio_output_setup','func_800BAF90','func_800BB02C','func_8008B2B4','func_8008B2E4','state_change_preprocess','resource_slot_clear','func_80097694','resource_slots_clear_multiple']
CONTEXT={'setup.c':'src/blob/func_800BB02C.c','rand.c':'src/blob/func_8008B2B4.c','random.c':'cloud/work/frontier/w8c/func_800B23E0/rand_arc_unit.c','heap_total.c':'src/blob/groups/frontier_heap_free_total/group.c','resource.c':'src/blob/groups/codex_state_audio_a146/group.c'}
PACKET=['README.md','PROVENANCE.md','replay.py','semantics.c','group/group.json']+['group/'+n for n in ['root.c','setup.c','rand.c','random.c','resource.c','heap_total.c']]
LAYOUT=[8,1,2,3,4,2056,1996,4,4,1,2,4]
def require(condition,message):
 if not condition:raise RuntimeError(message)
def sha(b):return hashlib.sha256(b).hexdigest()
def recipe():return json.loads((HERE/'group/group.json').read_text())
def source():return (HERE/'group/root.c').read_text()
def once(s,a,b):
 require(s.count(a)==1,'ambiguous source anchor: '+a);return s.replace(a,b,1)
def diagnostic(s):return re.sub(r'retail [0-9a-fA-F]+, got [0-9a-fA-F]+','retail [redacted], got [redacted]',s)
def verify_scope(spec):
 require(spec['members']==[NAME] and spec['context']==PRESERVED+[UNMATCHED] and spec['claims']==[],'research scope or complete context changed')
 require(spec['files']==['root.c','setup.c','rand.c','random.c','resource.c','heap_total.c'],'source order changed')
 require(spec['keep']==[NAME,'audio_output_setup','func_800BB02C','func_8008B2B4','func_8008B2E4','state_change_preprocess','resource_slots_clear_multiple',UNMATCHED],'kept roots changed')
 require(spec['flags']=='-g0 -O3 -mips2 -G 0 -non_shared' and not spec.get('allow_unverified'),'compiler contract changed')
def verify_context():
 for n,p in CONTEXT.items():
  expected=(ROOT/p).read_text()
  if n=='resource.c':expected=once(expected,'extern s32 D_8002E580[],D_801569AC;','extern u32 D_8002E580[];\nextern s32 D_801569AC;')
  require((HERE/'group'/n).read_text()==expected,'preserved context source changed: '+n)
def inputs():
 targets=score.targets();syms=score.image_symbols()
 return {'packet_sha256':{p:sha((HERE/p).read_bytes()) for p in PACKET},'context_sha256':{p:sha((ROOT/p).read_bytes()) for p in CONTEXT.values()},'targets':{n:{'address':hex(syms[n]),'bytes':4*len(targets[n]),'sha256':sha(struct.pack('>%dI'%len(targets[n]),*targets[n]))} for n in [NAME]+PRESERVED+[UNMATCHED]},'recipe':recipe(),'protocol_version':2,'toolchain_sha256':{n:sha(Path(score.ido(n)).read_bytes()) for n in ['cc','cfe','ujoin','uld','usplit','umerge','uopt','ugen','as1']},'verifier_sha256':{n:sha((ROOT/n).read_bytes()) for n in ['tools/cloud/score.py','tools/cloud/owndata.py']}}
def object_result(obj,spec):
 data,sections=score._elf(obj);ti=score._text_index(sections)
 symbols=[s for i,sec in enumerate(sections) if sec['type']==2 for s in score._symbol_table(data,sections,i) if s['type']==2 and s['section']==ti]
 rows={}
 for name in spec['members']+spec['context']:
  with contextlib.redirect_stdout(io.StringIO()):r=score.compare(obj,name,show=0)
  fs=[s for s in symbols if s['name']==name];require(len(fs)==1,'missing or ambiguous function: '+name);f=fs[0]
  words=score.text_words(obj)[f['value']//4:(f['value']+f['size'])//4]
  frames=[65536-(w&65535) for w in words if w>>16==0x27bd and w&0x8000]
  rows[name]={'differing':r.differing,'total':r.total,'extra_words':r.extra_words,'unresolved':r.unresolved,'unverified':r.unverified,'errors':[diagnostic(e) for e in r.errors],'bytes':f['size'],'frame':frames[0] if frames else None,'strict':r.accepted()}
 return {'rows':rows,'object_sha256':sha(data),'sections':{s['name']:{'bytes':s['size'],'sha256':sha(data[s['off']:s['off']+s['size']])} for s in sections if s['name'] in {'.text','.rel.text','.rodata','.rel.rodata','.data','.rel.data'}},'emitted_functions':sorted(s['name'] for s in symbols)}
def verify_positive(result,targets):
 require(result['emitted_functions']==sorted([NAME]+PRESERVED+[UNMATCHED]),'unexpected emitted function set')
 for n,r in result['rows'].items():
  require(not any(r[k] for k in ['extra_words','unresolved','unverified','errors']),'uncertain bytes: '+n)
  if n==UNMATCHED:
   require(r['differing']==130 and r['total']==130 and r['bytes']==432 and r['frame']==48 and not r['strict'],'LoadWave research boundary changed')
  else:require(r['differing']==0 and r['strict'] and r['bytes']==targets[n]['bytes'],'strict preservation or extent changed: '+n)
 require(result['rows'][NAME]['frame']==168,'LoadCars native frame changed')
def host(d,s,negative=False):
 s=once(s,'void *car;void *parameters;','u32 car;u32 parameters;')
 (d/'host_candidate.inc').write_text(s);shutil.copyfile(HERE/'semantics.c',d/'semantics.c')
 exe=d/'host';subprocess.run(['cc','-std=c11','-O2','-fno-strict-aliasing','-fsanitize=undefined','-fno-sanitize-recover=undefined',str(d/'semantics.c'),'-o',str(exe)],check=True,capture_output=True,text=True)
 r=subprocess.run([str(exe)],capture_output=True,text=True)
 if negative:
  require(r.returncode==3 and r.stderr=='loadcars differential mismatch\n' and not r.stdout,'semantic negative failed for an unexpected reason: '+repr((r.returncode,r.stdout,r.stderr)))
  return 'rejected with exact differential exit 3 and message'
 require(r.returncode==0 and not r.stderr,'finite host comparison failed: '+repr((r.returncode,r.stdout,r.stderr)))
 require(r.stdout=='4224 finite candidate/model state-and-service-trace cases; all 8 branch classes covered\n','host fixture coverage changed')
 return r.stdout.strip()
def type_layout(d,s):
 prefix=s[:s.index('void audio_interrupt_handler(')]
 text=prefix+'''\n#define OFF(T,f) ((u32)&(((T *)0)->f))
struct Alignment {u8 a;Vehicle2056 v;};
u32 layout_values[]={sizeof(Selection8),OFF(Selection8,model),OFF(Selection8,type),OFF(Selection8,tune),OFF(Selection8,variant),sizeof(Vehicle2056),OFF(Vehicle2056,mode),OFF(struct Alignment,v),sizeof(void *),sizeof(s8),sizeof(s16),sizeof(u32)};\n'''
 p=d/'layout.c';p.write_text(text);obj=d/'layout.o';score.compile_single(p,score.DEFAULT_FLAGS,obj)
 data,sections=score._elf(obj);sym=next(x for i,sec in enumerate(sections) if sec['type']==2 for x in score._symbol_table(data,sections,i) if x['name']=='layout_values')
 sec=sections[sym['section']];return list(struct.unpack('>12I',data[sec['off']+sym['value']:sec['off']+sym['value']+48]))
def controls(s):
 fallback='    D_80142B08[i]=D_80142B08[0];\n    func_800BB02C(i,D_80153E88[i].model,0);\n   } else {\nload2:'
 return {'wrong_wave_base':s.replace('.model+88]','.model+87]').replace('.model+88,','.model+87,').replace('.model+88)','.model+87)'),
 'wrong_margin':s.replace('+512','+511'),
 'missing_mode2_setup':once(s,fallback,fallback.replace('    func_800BB02C(i,D_80153E88[i].model,0);\n','')),
 'wrong_active_bound':s.replace('i<6','i<5'),
 'wrong_metadata':once(s,'.type=D_80111604[','.type=D_80111648['),
 'wrong_find_handle':once(s,'return func_80097694(waveNum,-1);','return func_80097694(waveNum,0);')}
def verify_report(report):
 verify_scope(report['inputs']['recipe']);verify_positive(report['positive']['first'],report['inputs']['targets'])
 require(report['positive']['first']==report['positive']['repeat'],'repeat object differs')
 require(report['claims']==[] and report['new_candidate_bytes']==0 and report['accepted_image_bytes']==0,'false byte credit')
 require(report['locally_exact_root_bytes']==1372 and report['existing_context_functions']==10,'root/context accounting changed')
 require(report['layout']['emitted']==LAYOUT and report['layout']['wrong_stride_rejected'],'layout proof changed')
 require(set(report['semantic_negatives'])==set(controls(source())),'semantic control set changed')
 require(set(report['negative_controls'])==set(controls(source()))|{'without_resource_context','flattened_find_call','artificial_wave_record'},'structural control set changed')
 for label,result in report['negative_controls'].items():
  require(not result['rows'][NAME]['strict'],'structural negative accepted: '+label)
  for n in set(result['rows'])&set(PRESERVED):
   r=result['rows'][n];require(r['strict'] and r['bytes']==report['inputs']['targets'][n]['bytes'],'negative damaged a locked helper: '+label+'/'+n)
 require(report['negative_controls']['without_resource_context']['rows'][NAME]['differing']==119,'context dependency changed')
 flat=report['negative_controls']['flattened_find_call']['rows'][NAME]
 require(flat['differing']==2 and flat['bytes']==1372 and flat['frame']==160,'genuine wrapper frame control changed')
 require(report['host_semantics']=='4224 finite candidate/model state-and-service-trace cases; all 8 branch classes covered','semantic receipt changed')
 for result in report['semantic_negatives'].values():require(result=='rejected with exact differential exit 3 and message','semantic negative receipt changed')
def verify(check=True):
 spec=recipe();verify_scope(spec);verify_context();actual_inputs=inputs()
 if check:
  saved=json.loads((HERE/'proof.json').read_text());require(saved['inputs']==actual_inputs,'source, verifier, recipe, context or protected target receipt changed');verify_report(saved)
 report={'inputs':actual_inputs,'claims':[],'new_candidate_bytes':0,'accepted_image_bytes':0,'locally_exact_root_bytes':1372,'existing_context_functions':10,'promotion_boundary':'Research only: full context contains unmatched LoadWave; without it LoadCars differs by 119 words. The 8-byte FindWaveInstance wrapper is already locked and receives no new credit.','positive':{},'negative_controls':{},'semantic_negatives':{}}
 with tempfile.TemporaryDirectory(prefix='rush-loadcars-') as tmp:
  d=Path(tmp);s=source()
  def build(label,candidate=s,manifest=None,resource=None):
   for n in spec['files']:shutil.copyfile(HERE/'group'/n,d/n)
   rs=manifest or spec;(d/'group.json').write_text(json.dumps(rs));(d/'root.c').write_text(candidate)
   if resource is not None:(d/'resource.c').write_text(resource)
   obj=d/(label+'.o');score.compile_group(d,obj);return object_result(obj,rs)
  report['positive']['first']=build('first');report['positive']['repeat']=build('repeat')
  for label,mutant in controls(s).items():
   report['negative_controls'][label]=build(label,mutant);report['semantic_negatives'][label]=host(d,mutant,True)
  nores=json.loads(json.dumps(spec));removed={'state_change_preprocess','resource_slot_clear','func_80097694','resource_slots_clear_multiple',UNMATCHED}
  nores['context']=[x for x in nores['context'] if x not in removed];nores['keep']=[x for x in nores['keep'] if x not in removed];nores['files'].remove('resource.c')
  report['negative_controls']['without_resource_context']=build('no_resource',manifest=nores)
  flattened=s.replace('func_8009768C(D_80153E88[i].model+88)','func_80097694(D_80153E88[i].model+88,-1)')
  # Retain the actual wrapper function as an existing claim even in this
  # source-control experiment. It is unused here and naturally deletes.
  report['negative_controls']['flattened_find_call']=build('flat_find',flattened)
  archive=once(s,'extern u32 D_8002E580[];','typedef struct Archive404 {u8 prefix[352];u32 bytes[13];} Archive404;\nextern Archive404 D_8002E580;')
  archive=archive.replace('D_8002E580[D_80153E88[i].model+88]','D_8002E580.bytes[D_80153E88[i].model]')
  report['negative_controls']['artificial_wave_record']=build('wave_record',archive)
  report['host_semantics']=host(d,s)
  layout=type_layout(d,s);require(layout==LAYOUT,'native type layout changed')
  bad=type_layout(d,once(s,'opaque[1988]','opaque[1984]'));require(bad!=LAYOUT,'wrong-stride layout negative accepted')
  report['layout']={'emitted':layout,'wrong_stride_emitted':bad,'wrong_stride_rejected':True}
 verify_report(report)
 if check:require(report==saved,'fresh proof differs from saved receipt')
 return report
if __name__=='__main__':
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--record',action='store_true');a=ap.parse_args();r=verify(not a.record)
 if a.record:(HERE/'proof.json').write_text(json.dumps(r,indent=2,sort_keys=True)+'\n')
 print('PASS research: LoadCars exact 1372 B/frame168; ten existing bodies preserved; unmatched LoadWave; claims empty; 4224 finite host cases and six semantic negatives')
