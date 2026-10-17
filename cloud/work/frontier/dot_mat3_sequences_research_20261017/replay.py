#!/usr/bin/env python3
"""Reproduce the one-word sequence research result without claiming a match."""
import argparse,contextlib,hashlib,io,json,re,shutil,struct,subprocess,sys,tempfile
from pathlib import Path
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[3]
sys.path.insert(0,str(ROOT))
from tools.cloud import score
NAME='func_800988D8';BASE='src/blob/groups/engine_sound_sync'
THREAD='cloud/work/lean_effect_tick_20261006/candidate.c'
CONTEXT={n:BASE+'/'+n for n in ['engine_sound_sync.c','audio_effect_setup_b132.c','codex_transform_b109.c']}
CONTEXT['timeout.c']='src/blob/func_80095198.c'
UNMATCHED=['audio_pitch_adjust','func_80098FB8','func_80091B00']
PACKET=['candidate.c','group.json','replay.py','semantics.c','README.md']
LAYOUT=[60,0,8,12,13,14,16,20,24,1,28,44,48,52,56,68,8,12,16,20,24,25,26,27,28,32,36,40,44,48,52,56,60,64,16,4,8,12,4]
def require(c,m):
 if not c:raise RuntimeError(m)
def sha(b):return hashlib.sha256(b).hexdigest()
def recipe():return json.loads((HERE/'group.json').read_text())
def source():return (HERE/'candidate.c').read_text()
def once(s,a,b):
 require(s.count(a)==1,'ambiguous source anchor: '+a);return s.replace(a,b,1)
def original_recipe():return json.loads((ROOT/BASE/'group.json').read_text())
def names():return recipe()['members']+recipe()['context']
def preserved():return [n for n in names() if n not in [NAME]+UNMATCHED]
def body(s):return s[s.index('void '+NAME+'(void) {'):]
def donor(s):return once(once(s,'    s32 state=t->state;\n',''),'if(state==2','if(t->state==2')
def verify_scope(sp):
 base=original_recipe();allnames=base['members']+base['context']+['func_80095198']
 require(sp['members']==[NAME] and sp['context']==[n for n in allnames if n!=NAME] and sp['claims']==[],'research scope changed')
 require(sp['files']==base['files']+['timeout.c'] and sp['keep']==base['keep']+['func_80095198'],'materialized context recipe changed')
 require(sp['flags']=='-g0 -O3 -mips2 -G 0 -non_shared' and not sp.get('allow_unverified'),'compiler contract changed')
def inputs():
 ts=score.targets();syms=score.image_symbols()
 return {'protocol_version':2,'recipe':recipe(),'packet_sha256':{n:sha((HERE/n).read_bytes()) for n in PACKET},'context_sha256':{n:sha((ROOT/n).read_bytes()) for n in list(CONTEXT.values())+[BASE+'/group.json',THREAD]},'targets':{n:{'address':hex(syms[n]),'bytes':len(ts[n])*4,'sha256':sha(struct.pack('>%dI'%len(ts[n]),*ts[n]))} for n in names()+['entity_ai_pathfind']},'toolchain_sha256':{n:sha((score.IDO/n).read_bytes()) for n in ['cc','cfe','uld','usplit','umerge','uopt','ugen','as1']},'verifier_sha256':{n:sha((ROOT/n).read_bytes()) for n in ['tools/cloud/score.py','tools/cloud/owndata.py']}}
def materialize(d,candidate=None,thread=False):
 for n,p in CONTEXT.items():shutil.copyfile(ROOT/p,d/n)
 if candidate is not None:
  p=d/'audio_effect_setup_b132.c';s=p.read_text();a=s.index('void func_800988D8(void) {');b=s.index('\n\nvoid func_80098FB8',a);p.write_text(s[:a]+candidate+s[b:])
 sp=recipe()
 if thread:
  s=(ROOT/THREAD).read_text();require(s.count('osJamMesg')==2,'thread queue boundary changed');(d/'thread.c').write_text(s.replace('osJamMesg','osSendMesg'));sp['files'].append('thread.c');sp['keep'].remove('func_80098FB8');sp['keep'].append('entity_ai_pathfind');sp['context'].append('entity_ai_pathfind')
 (d/'group.json').write_text(json.dumps(sp,indent=2)+'\n');return sp

def diagnostic(s):return re.sub(r'retail [0-9a-fA-F]+, got [0-9a-fA-F]+','retail [redacted], got [redacted]',s)
def object_result(obj,sp):
 data,secs=score._elf(obj);ti=score._text_index(secs)
 fs=[s for i,sec in enumerate(secs) if sec['type']==2 for s in score._symbol_table(data,secs,i) if s['type']==2 and s['section']==ti];words=score.text_words(obj);rows={}
 for n in sp['members']+sp['context']:
  with contextlib.redirect_stdout(io.StringIO()):r=score.compare(obj,n,show=0)
  f=[s for s in fs if s['name']==n];require(len(f)==1,'missing or duplicate emitted function: '+n);f=f[0]
  frames=[65536-(w&65535) for w in words[f['value']//4:(f['value']+f['size'])//4] if w>>16==0x27bd and w&0x8000]
  rows[n]={'differing':r.differing,'total':r.total,'extra_words':r.extra_words,'unresolved':r.unresolved,'unverified':r.unverified,'errors':[diagnostic(e) for e in r.errors],'bytes':f['size'],'frame':frames[0] if frames else None,'strict':r.accepted()}
 return {'rows':rows,'emitted_functions':sorted(s['name'] for s in fs),'sections':{s['name']:{'bytes':s['size'],'sha256':sha(data[s['off']:s['off']+s['size']])} for s in secs if s['name'] in {'.text','.rel.text','.rodata','.rel.rodata','.data','.rel.data'}}}
def residual(obj):
 start=score.symbols(obj)[NAME];want=score.targets()[NAME];w,m,u,v,e=score.relocate(obj,score.text_words(obj),start,start+len(want)*4,score.image_symbols());got=w[start//4:start//4+len(want)]
 require(not any([m,u,v,e]),'residual has unresolved relocation uncertainty');bad=[i for i,(a,b) in enumerate(zip(want,got)) if a!=b]
 require(len(got)==len(want) and bad==[44],'residual location changed');a,b=want[44],got[44]
 require(a>>26==5 and b>>26==5 and a&0xffff==b&0xffff and (a>>21)&31==(b>>16)&31 and (a>>16)&31==(b>>21)&31,'residual is not solely a BNE operand reversal')
 return {'offset':176,'relation':'BNE source operands reversed; opcode and branch displacement identical','accepted':False}
def host(d,s,negative=False):
 shutil.copyfile(HERE/'semantics.c',d/'semantics.c');(d/'candidate_body.inc').write_text(body(s));(d/'donor_body.inc').write_text(body(donor(source())).replace(NAME,'donor_root'));exe=d/'host'
 subprocess.run(['cc','-std=c11','-O2','-fno-strict-aliasing','-fsanitize=undefined','-fno-sanitize-recover=undefined',str(d/'semantics.c'),'-o',str(exe)],check=True,capture_output=True,text=True);r=subprocess.run([str(exe)],capture_output=True,text=True)
 if negative:
  require(r.returncode==3 and r.stderr=='sequence differential mismatch\n' and not r.stdout,'semantic negative failed for an unexpected reason: '+repr((r.returncode,r.stdout,r.stderr)));return 'rejected with exact differential exit 3 and message'
 require(r.returncode==0 and r.stdout=='32768 finite sequence-list fixtures\n' and not r.stderr,'finite host differential failed: '+repr((r.returncode,r.stdout,r.stderr)));return r.stdout.strip()
def controls(s):
 return {'wrong_state':once(s,'if(state==2','if(state==1'),'wrong_id_bound':s.replace('s->next>=4','s->next>=3'),'lost_successor':once(s,'s;s=next','s;s=(Sequence *)s->ll.next')}
def layout(d,s):
 prefix='typedef signed char s8;typedef unsigned char u8;typedef int s32;typedef unsigned int u32;typedef float f32;typedef struct Link{struct Link *next,*prev;} Link;typedef struct List{u8 indirect,doubly,pad[2];u32 count;Link *head,*tail;}List;\n'+s[s.index('typedef struct Sequence'):s.index('s32 func_80095198')]
 text=prefix+'''#define OFF(T,f) ((u32)&(((T *)0)->f))
u32 layout_values[]={sizeof(Sequence),OFF(Sequence,ll),OFF(Sequence,id),OFF(Sequence,free),OFF(Sequence,active),OFF(Sequence,stop),OFF(Sequence,startTime),OFF(Sequence,timeout),OFF(Sequence,next),sizeof(((Sequence *)0)->next),OFF(Sequence,ids),OFF(Sequence,subId),OFF(Sequence,replace),OFF(Sequence,priority),OFF(Sequence,sndTag),sizeof(FxTag),OFF(FxTag,list),OFF(FxTag,id),OFF(FxTag,state),OFF(FxTag,type),OFF(FxTag,paused),OFF(FxTag,loop),OFF(FxTag,numCmds),OFF(FxTag,priority),OFF(FxTag,wpriority),OFF(FxTag,startTime),OFF(FxTag,vol),OFF(FxTag,lrpan),OFF(FxTag,fbpan),OFF(FxTag,pitch),OFF(FxTag,fxid),OFF(FxTag,subId),OFF(FxTag,sysId),OFF(FxTag,data),sizeof(List),OFF(List,count),OFF(List,head),OFF(List,tail),sizeof(void *)};\n'''
 p=d/'layout.c';p.write_text(text);o=d/'layout.o';score.compile_single(p,score.DEFAULT_FLAGS,o);data,secs=score._elf(o);z=next(s for i,sec in enumerate(secs) if sec['type']==2 for s in score._symbol_table(data,secs,i) if s['name']=='layout_values');sec=secs[z['section']];return list(struct.unpack('>%dI'%len(LAYOUT),data[sec['off']+z['value']:sec['off']+z['value']+4*len(LAYOUT)]))
def verify_preserved(r,targets):
 for n in preserved():
  z=r['rows'][n];require(z['strict'] and z['differing']==0 and z['total']*4==targets[n]['bytes'] and z['bytes']==targets[n]['bytes'] and not any(z[k] for k in ['extra_words','unresolved','unverified','errors']),'strict context/extent changed: '+n)
def verify_report(r):
 verify_scope(r['inputs']['recipe']);require(r['claims']==[] and r['new_candidate_bytes']==r['accepted_image_bytes']==0,'false matching credit')
 require(r['positive']['first']==r['positive']['repeat'],'repeat executable receipt differs')
 for g in r['positive'].values():
  require(g['emitted_functions']==sorted(names()),'incomplete function inventory');verify_preserved(g,r['inputs']['targets']);z=g['rows'][NAME]
  require(z['differing']==1 and z['total']==95 and z['bytes']==380 and z['frame']==24 and not z['strict'],'one-word root boundary changed')
  require(not any(z[k] for k in ['extra_words','unresolved','unverified','errors']),'root uncertainty changed')
  require([(g['rows'][n]['differing'],g['rows'][n]['extra_words']) for n in UNMATCHED]==[(18,0),(189,25),(11,0)],'inherited failures changed')
 require(r['residual']=={'offset':176,'relation':'BNE source operands reversed; opcode and branch displacement identical','accepted':False},'residual relation changed')
 require(len(preserved())==16,'preserved context count changed')
 require(set(r['controls'])=={'historical','donor_only','real_thread'}|set(controls(source())),'control set changed')
 for name,g in r['controls'].items():
  require(g['emitted_functions']==sorted(names()+(['entity_ai_pathfind'] if name=='real_thread' else [])),'control inventory changed: '+name);verify_preserved(g,r['inputs']['targets']);require(not g['rows'][NAME]['strict'],'control falsely accepted: '+name)
 require(r['controls']['historical']['rows'][NAME]['differing']==80 and r['controls']['historical']['rows'][NAME]['extra_words']==1,'historical control changed')
 require(r['controls']['donor_only']['rows'][NAME]['differing']==21,'donor causal control changed')
 t=r['controls']['real_thread']['rows'];require(t[NAME]['differing']==1 and t['audio_pitch_adjust']['strict'] and t['func_80098FB8']['differing']==166 and t['entity_ai_pathfind']['errors'] and not t['entity_ai_pathfind']['strict'],'real thread boundary changed')
 require(r['layout']['emitted']==LAYOUT and r['layout']['wrong_width_emitted']!=LAYOUT,'layout control failed')
 require(r['host_semantics']=='32768 finite sequence-list fixtures' and set(r['semantic_negatives'])==set(controls(source())),'host semantic scope changed')
 require(all(x=='rejected with exact differential exit 3 and message' for x in r['semantic_negatives'].values()),'semantic negative receipt changed')
def verify(check=True):
 verify_scope(recipe());actual=inputs()
 if check:
  saved=json.loads((HERE/'proof.json').read_text());require(saved['inputs']==actual,'source, context, toolchain, verifier or target receipt changed');verify_report(saved)
 r={'inputs':actual,'claims':[],'new_candidate_bytes':0,'accepted_image_bytes':0,'positive':{},'controls':{},'semantic_negatives':{}}
 with tempfile.TemporaryDirectory(prefix='rush-sequences-') as td:
  d=Path(td);s=source()
  def build(label,candidate=s,thread=False):
   sp=materialize(d,candidate,thread);o=d/(label+'.o');score.compile_group(d,o);return object_result(o,sp),o
  for name in ['first','repeat']:
   g,o=build(name);r['positive'][name]=g
   if name=='first':r['residual']=residual(o)
  for name,c in {'historical':None,'donor_only':donor(s),**controls(s)}.items():r['controls'][name]=build(name,c)[0]
  r['controls']['real_thread']=build('real_thread',thread=True)[0]
  r['host_semantics']=host(d,s)
  for name,c in controls(s).items():r['semantic_negatives'][name]=host(d,c,True)
  r['layout']={'emitted':layout(d,s),'wrong_width_emitted':layout(d,once(s,'u8 next;','unsigned short next;'))}
 verify_report(r)
 if check:require(r==saved,'fresh proof differs')
 return r
if __name__=='__main__':
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--record',action='store_true');ap.add_argument('--materialize',type=Path);a=ap.parse_args()
 if a.materialize:a.materialize.mkdir(parents=True,exist_ok=True);materialize(a.materialize,source());sys.exit(0)
 r=verify(not a.record)
 if a.record:(HERE/'proof.json').write_text(json.dumps(r,indent=2,sort_keys=True)+'\n')
 print('PASS research: sequence 1/95, extent380/frame24, 16 strict context bodies; empty claims; 32768 finite fixtures and three precise semantic negatives')
