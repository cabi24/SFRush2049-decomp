#!/usr/bin/env python3
"""Replay research-only Pak cleanup; excess words and full-context failures remain."""
import argparse,contextlib,hashlib,io,json,re,shutil,struct,subprocess,sys,tempfile
from pathlib import Path
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[3]
sys.path.insert(0,str(ROOT/'tools/cloud'));import score
NAME='func_800A3640';HEAP='src/blob/groups/codex_heap_release_a25'
PACKET=['candidate.c','group.json','semantics.c','replay.py','README.md']
LAYOUT=[104,4,12,32,14,40,4,8,772,5,12,132,76,16,17,64,68,72,4,16,8,12,4]
FAILURES={'car_lod_select':(16,81,324,64,0),'func_800A3640':(0,51,204,24,2),'func_800A43FC':(51,53,360,None,33),'no_catchup':(14,14,8,None,0),'PS_CreateFile':(348,438,1776,288,6),'track_render_process':(747,800,3080,416,0)}
def require(c,m):
 if not c:raise RuntimeError(m)
def sha(b):return hashlib.sha256(b).hexdigest()
def diagnostic(s):return re.sub(r'retail [0-9a-fA-F]+, got [0-9a-fA-F]+','retail [redacted], got [redacted]',s)
def recipe():return json.loads((HERE/'group.json').read_text())
def source():return (HERE/'candidate.c').read_text()
def heap_recipe():return json.loads((ROOT/HEAP/'group.json').read_text())
def names():return recipe()['members']+recipe()['context']
def strict_names():return [n for n in names() if n not in FAILURES]
def once(s,a,b):
 require(s.count(a)==1,'ambiguous source anchor: '+a);return s.replace(a,b,1)
def root_body(s):
 a=s.index('void func_800A3640(s32 port)\n{');b=s.index('\n/* Native takes',a);return s[a:b]
def replace_root(s,b):return once(s,root_body(s),b)
def verify_scope(sp):
 h=heap_recipe();require(sp['members']==[NAME] and sp['claims']==[],'research claims changed')
 require(sp['files']==['candidate.c']+['heap_'+n for n in h['files']],'complete heap files changed')
 require(len(sp['context'])==33,'complete context size changed')
 require(set(sp['context'])==set(h['members']+h['context']+['car_lod_select','func_800A1E94','func_800A43FC','no_catchup','PS_CreateFile','track_render_process','func_800A3724']),'complete context changed')
 require(sp['keep']==['func_800A1E94','func_800A43FC','car_lod_select','PS_CreateFile','track_render_process']+h['keep'],'native visibility changed')
 require(sp['flags']=='-g0 -O3 -mips2 -G 0 -non_shared' and not sp.get('allow_unverified'),'compiler contract changed')
def inputs():
 ts=score.targets();sy=score.image_symbols();h=heap_recipe()
 return {'packet_sha256':{n:sha((HERE/n).read_bytes()) for n in PACKET},'context_sha256':{HEAP+'/'+n:sha((ROOT/HEAP/n).read_bytes()) for n in h['files']+['group.json']},'targets':{n:{'address':hex(sy[n]),'bytes':len(ts[n])*4,'sha256':sha(struct.pack('>%dI'%len(ts[n]),*ts[n]))} for n in names()},'recipe':recipe(),'protocol_version':2,'toolchain_sha256':{n:sha(Path(score.ido(n)).read_bytes()) for n in ['cc','cfe','ujoin','uld','usplit','umerge','uopt','ugen','as1']},'verifier_sha256':{n:sha((ROOT/n).read_bytes()) for n in ['tools/cloud/score.py','tools/cloud/owndata.py']}}
def materialize(d,s=None,sp=None):
 (d/'candidate.c').write_text(s or source())
 for n in heap_recipe()['files']:shutil.copyfile(ROOT/HEAP/n,d/('heap_'+n))
 (d/'group.json').write_text(json.dumps(sp or recipe(),indent=2)+'\n')
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

def mutants(s):
 b=root_body(s)
 return {n:replace_root(s,c) for n,c in {
 'skip_callback':once(b,'if (request->done != 0) request->done(node);','if (0) request->done(node);'),
 'missing_buffer_clear':once(b,'            request->buffer = 0;','            /* negative: omitted clear */'),
 'wrong_release_tag':once(b,'audio_reverb_update((u32)buffer, 0);','audio_reverb_update((u32)buffer, 1);'),
 'wrong_remove_port':once(b,'&D_80144D60[node->request->port]','&D_80144D60[port]'),
 'append_free_list':once(b,'node, D_801460E0.head','node, 0'),
 'lost_successor':once(b,'        node = next;','        node = node->request->next;')}.items()}
def outer_buffer(s):
 b=root_body(s);b=once(b,'    PakRequest *request;','    PakRequest *request;\n    u8 **buffer;');b=once(b,'        if (request->buffer != 0) {\n            u8 **buffer = request->buffer;','        buffer = request->buffer;\n        if (buffer != 0) {');return replace_root(s,b)
def host(d,s,negative=False):
 (d/'candidate_body.inc').write_text(root_body(s));shutil.copyfile(HERE/'semantics.c',d/'semantics.c');exe=d/'host';subprocess.run(['cc','-std=c11','-O2','-fno-strict-aliasing','-fsanitize=undefined','-fno-sanitize-recover=undefined',str(d/'semantics.c'),'-o',str(exe)],check=True,capture_output=True,text=True);r=subprocess.run([str(exe)],capture_output=True,text=True,timeout=15)
 if negative:
  require(r.returncode==3 and r.stderr=='pak cleanup differential mismatch\n' and not r.stdout,'semantic negative failed for an unexpected reason: '+repr((r.returncode,r.stdout,r.stderr)));return 'rejected with exact differential exit 3 and message'
 require(r.returncode==0 and r.stdout=='65536 finite cleanup state-and-service-trace fixtures\n' and not r.stderr,'host differential failed: '+repr((r.returncode,r.stdout,r.stderr)));return r.stdout.strip()
def layout(d,s):
 prefix=s[:s.index('/* Exact accepted status-conversion body,')]
 text=prefix+"""
#define OFF(T,f) ((u32)&(((T *)0)->f))
u32 layout_values[]={sizeof(OSPfs),OFF(OSPfs,queue),OFF(OSPfs,id),sizeof(OSPfsState),OFF(OSPfsState,game_name),sizeof(PakFile40),OFF(PakFile40,bitmap_bytes),OFF(PakFile40,state),sizeof(Pak772),OFF(Pak772,blocked),OFF(Pak772,pfs),OFF(Pak772,files),sizeof(PakRequest),OFF(PakRequest,port),OFF(PakRequest,file),OFF(PakRequest,file_size),OFF(PakRequest,pages),OFF(PakRequest,buffer),sizeof(PakNode),sizeof(PakList),OFF(PakList,head),OFF(PakList,tail),sizeof(void *)};
"""
 p=d/'layout.c';p.write_text(text);o=d/'layout.o';score.compile_single(p,score.DEFAULT_FLAGS,o);data,secs=score._elf(o);z=next(s for i,sec in enumerate(secs) if sec['type']==2 for s in score._symbol_table(data,secs,i) if s['name']=='layout_values');sec=secs[z['section']];return list(struct.unpack('>%dI'%len(LAYOUT),data[sec['off']+z['value']:sec['off']+z['value']+4*len(LAYOUT)]))
def verify_strict(r,targets,available=None):
 for n in available or strict_names():
  z=r['rows'][n];require(z['strict'] and z['differing']==0 and z['total']*4==targets[n]['bytes'] and z['bytes']==targets[n]['bytes'] and not any(z[k] for k in ['extra_words','unresolved','unverified','errors']),'strict body/extent failed: '+n)
def verify_report(r):
 verify_scope(r['inputs']['recipe']);require(r['claims']==[] and r['new_candidate_bytes']==r['accepted_image_bytes']==0,'false matching credit');require(r['positive']['first']==r['positive']['repeat'],'repeat object changed')
 for g in r['positive'].values():
  require(g['emitted_functions']==sorted(names()+['heap_free_locked','mem_free','zcfree','func_80095CF4','func_80095CFC','func_800960CC','post_type1','post_type7']) and set(g['rows'])==set(names()),'emitted/scored inventory changed');verify_strict(g,r['inputs']['targets'])
  for n,want in FAILURES.items():
   z=g['rows'][n];require(tuple(z[k] for k in ['differing','total','bytes','frame','extra_words'])==want and not z['strict'] and not any(z[k] for k in ['unresolved','unverified','errors']),'complete failure boundary changed: '+n)
 require(len(strict_names())==28 and r['existing_heap_functions']==26,'context count changed')
 require(set(r['controls'])==set(mutants(source()))|{'outer_buffer','external_heap'},'control set changed')
 for label,g in r['controls'].items():
  require(not g['rows'][NAME]['strict'],'negative unexpectedly strict: '+label)
  if label!='external_heap':verify_strict(g,r['inputs']['targets'])
 require(r['controls']['outer_buffer']['rows'][NAME]['differing']==5,'buffer-lifetime control changed')
 require(r['controls']['external_heap']['rows'][NAME]['differing']>0,'genuine heap-context dependency changed')
 require(r['host_semantics']=='65536 finite cleanup state-and-service-trace fixtures' and set(r['semantic_negatives'])==set(mutants(source())),'semantic scope changed')
 require(all(x=='rejected with exact differential exit 3 and message' for x in r['semantic_negatives'].values()),'semantic rejection changed')
 require(r['layout']['emitted']==LAYOUT and r['layout']['wrong_stride_emitted']!=LAYOUT,'layout proof failed')
def verify(check=True):
 verify_scope(recipe());actual=inputs()
 if check:
  saved=json.loads((HERE/'proof.json').read_text());require(saved['inputs']==actual,'source, context, toolchain, verifier or target receipt changed');verify_report(saved)
 r={'inputs':actual,'claims':[],'new_candidate_bytes':0,'accepted_image_bytes':0,'existing_heap_functions':26,'positive':{},'controls':{},'semantic_negatives':{}}
 with tempfile.TemporaryDirectory(prefix='rush-pak-cleanup-') as td:
  d=Path(td);s=source()
  def build(label,candidate=s,sp=None):
   sp=sp or recipe();materialize(d,candidate,sp);o=d/(label+'.o');score.compile_group(d,o);return object_result(o,sp)
  r['positive']['first']=build('first');r['positive']['repeat']=build('repeat')
  r['controls']['outer_buffer']=build('outer_buffer',outer_buffer(s));sp=recipe();h=heap_recipe();sp['files']=['candidate.c'];sp['context']=[n for n in sp['context'] if n not in h['members']+h['context']];sp['keep']=[n for n in sp['keep'] if n not in h['members']+h['context']];r['controls']['external_heap']=build('external_heap',sp=sp)
  for label,c in mutants(s).items():r['controls'][label]=build(label,c);r['semantic_negatives'][label]=host(d,c,True)
  r['host_semantics']=host(d,s);r['layout']={'emitted':layout(d,s),'wrong_stride_emitted':layout(d,once(s,'u8 name[35], extension[11];','u8 name[35], extension[15];'))}
 verify_report(r)
 if check:require(r==saved,'fresh proof differs')
 return r
if __name__=='__main__':
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--record',action='store_true');a=ap.parse_args();r=verify(not a.record)
 if a.record:(HERE/'proof.json').write_text(json.dumps(r,indent=2,sort_keys=True)+'\n')
 print('PASS research: cleanup0/51 plus2 excess words, extent204/frame24; 26 strict heap functions; empty claims; 65536 finite fixtures and six precise semantic negatives')
