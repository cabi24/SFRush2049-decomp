"""Strict source-bound music root, full genuine group, three locked callable leaves."""
from pathlib import Path
import argparse,contextlib,hashlib,importlib.util,io,json,re,shlex,shutil,struct,sys
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[3];sys.path.insert(0,str(ROOT))
from tools.cloud import score
ROOT_NAME='func_800D6160'
def require(value,message):
    if not value:raise RuntimeError(message)
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def load(name):
    s=importlib.util.spec_from_file_location('music_'+name,HERE/(name+'.py'));m=importlib.util.module_from_spec(s);s.loader.exec_module(m);return m
def report(obj,name):
    data,sections=score._elf(obj);ti=score._text_index(sections)
    syms=[s for i,sec in enumerate(sections) if sec['type']==2 for s in score._symbol_table(data,sections,i) if s['type']==2 and s['section']==ti]
    selected=[s for s in syms if s['name']==name];require(len(selected)==1,'missing symbol '+name);s=selected[0]
    with contextlib.redirect_stdout(io.StringIO()):r=score.compare(obj,name,show=0)
    d=json.loads(json.dumps(vars(r)));d['errors']=[re.sub(r'retail [0-9a-fA-F]+, got [0-9a-fA-F]+','retail [redacted], got [redacted]',x) for x in d['errors']]
    words=score.text_words(obj)[s['value']//4:(s['value']+s['size'])//4]
    frames=[65536-(w&65535) for w in words if w>>26==9 and (w>>21)&31==29 and (w>>16)&31==29 and w&32768]
    return {'comparison':d,'symbol_bytes':s['size'],'frame':frames[0] if frames else 0}
def exact(row,name):
    r=row['comparison'];require(r['differing']==0 and not any(r[k] for k in ['extra_words','unresolved','unverified','errors']),'non-strict '+name)
    require(row['symbol_bytes']==r['total']*4==len(score.targets()[name])*4,'incomplete extent '+name)
def inventory(obj,names,private_prefix=0):
    data,sections=score._elf(obj);ti=score._text_index(sections)
    symbols=[s for i,sec in enumerate(sections) if sec['type']==2 for s in score._symbol_table(data,sections,i) if s['type']==2 and s['section']==ti]
    require(sorted(s['name'] for s in symbols)==sorted(names),'emitted function inventory changed')
    cursor=private_prefix
    for s in sorted(symbols,key=lambda s:s['value']):
        require(s['value']==cursor,'unaccounted function gap');cursor+=s['size']
    words=score.text_words(obj)
    require(all(w==0 for w in words[cursor//4:]),'nonzero trailing residue')
    for at in range(0,private_prefix//4,2):
        w=words[at];require(w>>26==0 and (w>>21)&31==31 and w&63==8 and words[at+1]==0,'unexpected private prefix')
    return {'functions':{s['name']:{'offset':s['value'],'bytes':s['size']} for s in sorted(symbols,key=lambda s:s['value'])},'private_preentry_stub_bytes':private_prefix,'zero_alignment_bytes':len(words)*4-cursor}

def compile_leaf(source,flags,out):
    args=shlex.split(flags)
    if score.R4300_CC not in args:args.append(score.R4300_CC)
    proc=score._run([score.ido('cc'),'-c',*args,'-o',str(out.resolve()),source.name],cwd=source.parent)
    require(proc.returncode==0 and out.exists(),'leaf compile failed')
def graph(names):
    syms=score.image_symbols();inv={a:n for n,a in syms.items()};targets=score.targets();result={}
    for n in names:
        dst=set()
        for w in targets[n]:
            if w>>26 in (2,3):
                a=(syms[n]&0xf0000000)|((w&0x3ffffff)<<2)
                if not syms[n]<=a<syms[n]+4*len(targets[n]):dst.add(inv.get(a,hex(a)))
            require(not(w>>26==0 and (w&63==9 or (w&63==8 and (w>>21)&31!=31))),'unreviewed indirect call '+n)
        result[n]=sorted(dst)
    return result
LAYOUT=[41,12,14,76,19,4,24,0,2,3,4,4]
def layout(out):
    out.mkdir(exist_ok=True)
    root=(HERE/'group/func_800D6160.c').read_text();prefix=root[:root.index('void func_800D6160(s32 resume)')]
    slot=(HERE/'group/func_80091B00.c').read_text();decl=re.search(r'typedef struct Slot18 .*? Slot18;',slot).group()
    source=prefix+'\ntypedef signed short s16;\n'+decl+'''\n#define OFF(T,m) ((unsigned int)&(((T*)0)->m))
unsigned int facts[]={sizeof(AudioCfg),OFF(AudioCfg,music),OFF(AudioCfg,track),sizeof(D_8010FFD4),sizeof(D_8010FFD4)/sizeof(D_8010FFD4[0]),sizeof(s32),sizeof(Slot18),OFF(Slot18,f0),OFF(Slot18,f2),OFF(Slot18,f3),OFF(Slot18,pad),sizeof(void*)};
'''
    result={}
    for tag,s in [('positive',source),('short_options',source.replace('remaining[25]','remaining[24]')),('short_table',source.replace('D_8010FFD4[19]','D_8010FFD4[18]')),('wrong_slot',source.replace('pad[0x14]','pad[0x18]'))]:
        d=out/tag;d.mkdir(exist_ok=True);p=d/'source.c';p.write_text(s);o=d/'layout.o';compile_leaf(p,score.DEFAULT_FLAGS,o)
        data,sections=score._elf(o);symbol=next(s for i,sec in enumerate(sections) if sec['type']==2 for s in score._symbol_table(data,sections,i) if s['name']=='facts');sec=sections[symbol['section']];values=list(struct.unpack('>%dI'%len(LAYOUT),data[sec['off']+symbol['value']:sec['off']+symbol['value']+4*len(LAYOUT)]));result[tag]=values
        require((values==LAYOUT)==(tag=='positive'),'layout control '+tag)
    return result
def inputs(spec,closure):
    paths=['verify.py','behavior.py','semantics.c','trace_fidelity.py','closure.json','group/group.json']+['group/'+f for f in spec['files']]+[r['source'] for r in closure['leaves'].values()]
    names=spec['members']+list(closure['leaves'])
    return {'files':{f:digest(HERE/f) for f in paths},'targets':{n:hashlib.sha256(struct.pack('>%dI'%len(score.targets()[n]),*score.targets()[n])).hexdigest() for n in names},'tools':{n:digest(ROOT/n) for n in ['tools/cloud/score.py','tools/cloud/owndata.py','blob_matched.lock.json']},'compiler':{n:digest(score.IDO/n) for n in ['cc','cfe','uld','usplit','umerge','uopt','ugen','as1']}}
def verify_report(r):
    require(r['new_root']==ROOT_NAME and r['new_candidate_bytes']==304 and r['accepted_image_bytes']==0,'false credit')
    require(len(r['group_claims'])==13 and len(r['leaf_claims'])==3 and set(r['positive'])==set(r['group_claims']) and set(r['leaves'])==set(r['leaf_claims']),'claim inventory changed')
    for n,row in {**r['positive'],**{n:x['row'] for n,x in r['leaves'].items()}}.items():exact(row,n)
    require(set(r['inventory']['functions'])==set(r['group_claims']) and r['inventory']['private_preentry_stub_bytes']==0 and r['inventory']['zero_alignment_bytes']==0,'emitted group inventory changed')
    for n,row in r['positive'].items():require(r['inventory']['functions'][n]['bytes']==row['symbol_bytes'],'emitted group extent changed')
    for n,row in r['leaves'].items():
        inv=row['inventory'];require(set(inv['functions'])=={n} and inv['functions'][n]['bytes']==row['row']['symbol_bytes'] and inv['private_preentry_stub_bytes']==(32 if n=='func_800D63EC' else 0),'leaf inventory changed')
    require(r['positive'][ROOT_NAME]['frame']==24,'root frame changed')
    require(r['behavior']['fixtures']==86184,'semantic fixture scope changed')
    require(r['layout']['positive']==LAYOUT and all(v!=LAYOUT for k,v in r['layout'].items() if k!='positive'),'layout control changed')
    require(set(r['negatives'])=={'wrong_exclusion','wrong_random_range','wrong_resume_time','wrong_song_loop'},'negative scope changed')
    for tag,x in r['negatives'].items():require(x['row']['comparison']['differing']>0 and x['behavior']=={'status':'rejected','reason':'music differential mismatch','exit':3},'semantic refusal changed '+tag)
    private=r['causal_private_rng']['func_800D6530'];require(private['comparison']['differing']==0 and private['comparison']['extra_words']==1,'private RNG boundary control changed')
    require(r['causal_assign_first']['row']['comparison']['differing']>0 and r['causal_assign_first']['behavior']==r['behavior'],'causal source control changed')
def verify(out,record=False):
    out=out.resolve();out.mkdir(parents=True,exist_ok=True)
    spec=json.loads((HERE/'group/group.json').read_text());closure=json.loads((HERE/'closure.json').read_text())
    require(spec['claims']==spec['members']==closure['group_claims'] and spec['context']==[] and len(spec['members'])==13,'complete group claims changed')
    require(closure['new_root']==ROOT_NAME and closure['new_bytes']==304 and closure['SDK_boundaries']==['osRecvMesg','osSendMesg'],'root scope changed')
    names=spec['members']+list(closure['leaves']);require(graph(names)==closure['direct_call_graph'],'native call graph changed')
    require(all(c in names+closure['SDK_boundaries'] for ds in closure['direct_call_graph'].values() for c in ds),'unmatched callable boundary')
    binding=inputs(spec,closure)
    if not record:
        saved=json.loads((HERE/'verification.json').read_text());require(saved['inputs']==binding,'source or target binding changed');verify_report(saved)
        trace=json.loads((HERE/'trace_fidelity.json').read_text());require(trace['inputs']==binding,'trace input binding changed');require(trace['complete_objects_identical'] and trace['stock_object_sha256']==trace['trace_object_sha256'],'trace pair changed')
    obj=out/'candidate.o';score.compile_group(HERE/'group',obj);positive={n:report(obj,n) for n in spec['members']};emitted=inventory(obj,spec['members'])
    for n,row in positive.items():exact(row,n)
    if not record:require(digest(obj)==trace['stock_object_sha256'],'trace object changed')
    leaves={}
    for n,row in closure['leaves'].items():
        require(digest(ROOT/row['original_source'])==row['original_sha256'],'locked leaf provenance changed')
        o=out/(n+'.o');compile_leaf(HERE/row['source'],row['flags'],o);leaves[n]={'row':report(o,n),'object_sha256':digest(o),'inventory':inventory(o,[n],32 if n=='func_800D63EC' else 0)};exact(leaves[n]['row'],n)
    behavior=load('behavior');sem=behavior.run(HERE/'group',out/'host')
    source=(HERE/'group/func_800D6160.c').read_text();mutations={'wrong_exclusion':('while(song == 5)','while(song == 4)'), 'wrong_random_range':('func_8008B2E4(12.0f)','func_8008B2E4(11.0f)'), 'wrong_resume_time':('resume ? 1.0f : 0.05f','resume ? 0.5f : 0.05f'),'wrong_song_loop':('sync_entry_register(song,1)','sync_entry_register(song,0)')};negatives={}
    for tag,(old,new) in mutations.items():
        require(source.count(old)==1,'ambiguous mutation '+tag);d=out/tag;shutil.copytree(HERE/'group',d,dirs_exist_ok=True);(d/'func_800D6160.c').write_text(source.replace(old,new));o=out/(tag+'.o');score.compile_group(d,o);negatives[tag]={'row':report(o,ROOT_NAME),'behavior':behavior.run(d,out/(tag+'_host'),True)}
        for n in spec['members']:
            if n!=ROOT_NAME:exact(report(o,n),n)
    old='''  if(D_8010FFD4[D_8014978C] == -1) {
   do { song=(s32)func_8008B2E4(12.0f); } while(song == 5);
  } else song=D_8010FFD4[D_8014978C];'''
    new='''  song=D_8010FFD4[D_8014978C];
  if(song == -1) {
   do { song=(s32)func_8008B2E4(12.0f); } while(song == 5);
  }'''
    require(source.count(old)==1,'causal source anchor changed');d=out/'assign_first';shutil.copytree(HERE/'group',d,dirs_exist_ok=True);(d/'func_800D6160.c').write_text(source.replace(old,new));o=out/'assign_first.o';score.compile_group(d,o);causal={'row':report(o,ROOT_NAME),'behavior':behavior.run(d,out/'assign_first_host')}
    # Causal boundary control: preserve the same RNG body but make it a private
    # copy again. Its deleted stub must remain visible beside D6530.
    d=out/'private_rand';shutil.copytree(HERE/'group',d,dirs_exist_ok=True)
    private_spec=json.loads(json.dumps(spec))
    for key in ['members','claims','keep']:private_spec[key].remove('func_8008B2B4')
    private_spec['files'].remove('rand.c');(d/'group.json').write_text(json.dumps(private_spec))
    rng=behavior.body((d/'rand.c').read_text(),'int func_8008B2B4(void)').replace('int func_8008B2B4(void)','static int rand(void)',1)
    random=(d/'random.c').read_text();require(random.count('extern int func_8008B2B4(void);')==1,'random boundary changed')
    random=random.replace('extern int func_8008B2B4(void);','extern int D_8011735C;\n'+rng).replace('func_8008B2B4()','rand()')
    (d/'random.c').write_text(random);o=out/'private_rand.o';score.compile_group(d,o)
    private_rows={n:report(o,n) for n in private_spec['members']}
    for n,row in private_rows.items():
        if n!='func_800D6530':exact(row,n)
    private=private_rows['func_800D6530'];require(private['comparison']['differing']==0 and private['comparison']['extra_words']==1 and private['symbol_bytes']==352,'private RNG residue control changed')
    result={'new_root':ROOT_NAME,'new_candidate_bytes':304,'accepted_image_bytes':0,'group_claims':spec['members'],'leaf_claims':list(closure['leaves']),'inputs':binding,'positive':positive,'inventory':emitted,'leaves':leaves,'object_sha256':digest(obj),'behavior':sem,'layout':layout(out/'layout'),'negatives':negatives,'causal_assign_first':causal,'causal_private_rng':private_rows,'canonical_or_ROM_verification':False}
    verify_report(result)
    if record:(HERE/'verification.json').write_text(json.dumps(result,indent=2)+'\n')
    else:require(result==saved,'fresh sealed receipt changed')
    return result
if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--record',action='store_true');p.add_argument('--out',type=Path,default=ROOT/'build/mat3_game_music_exact_20261017');a=p.parse_args();verify(a.out,a.record);print('PASS: music 304 bytes; 13 group plus 3 leaf bodies strict; 86184 fixtures; four semantic negatives')
