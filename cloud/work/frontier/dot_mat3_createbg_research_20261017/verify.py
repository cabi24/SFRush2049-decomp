"""Bounded createBG source/data-ownership research. No accepted matching credit."""
from pathlib import Path
import argparse, contextlib, dataclasses, hashlib, io, json, os, shutil, struct, sys, tempfile
HERE=Path(__file__).resolve().parent
ROOT=Path(os.environ.get('N64_RESEARCH_REPO',HERE.parents[3] if len(HERE.parents)>3 else Path.cwd()))
ROOT_NAME='particle_lifetime_set'
PRESERVED=['func_800A78BC','func_8008C074','func_800FC9F8']
NAMES=[ROOT_NAME]+PRESERVED
FLAGS='-g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul'

def require(ok,message):
    if not ok: raise RuntimeError(message)
def digest(path): return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def whash(words): return hashlib.sha256(struct.pack('>%dI'%len(words),*words)).hexdigest()
def inputs():
    return {str(p.relative_to(HERE)):digest(p) for p in sorted(HERE.rglob('*'))
            if p.is_file() and p.suffix in ('.c','.py','.json') and '__pycache__' not in p.parts
            and p.name not in ('receipt.json','trace_fidelity.json','independent_review.json')}
def sections(score,obj):
    data,secs=score._elf(obj)
    return data,secs,{s['name']:data[s['off']:s['off']+s['size']] for s in secs if s['type']!=8}
def symbols(score,obj):
    data,secs=score._elf(obj)
    return [s for i,sec in enumerate(secs) if sec['type']==2 for s in score._symbol_table(data,secs,i)]
def detail(score,obj,name):
    with contextlib.redirect_stdout(io.StringIO()): r=score.compare(obj,name,show=0)
    entries=[s for s in symbols(score,obj) if s['name']==name and s['type']==2]
    require(len(entries)==1,'function definition missing or ambiguous: '+name)
    e=entries[0];words=score.text_words(obj)[e['value']//4:(e['value']+e['size'])//4]
    frame=next((65536-(x&65535) for x in words[:24] if x>>26==9 and x>>21&31==29 and x>>16&31==29 and x&32768),0)
    row=dataclasses.asdict(r)
    row['errors']=[x.split(' expected ')[0].split(' (+0x')[0] for x in row['errors']]
    return {'comparison':row,'symbol_bytes':e['size'],'frame_bytes':frame}
def exact(row,name):
    require(not any(row['comparison'][k] for k in ('differing','unresolved','unverified','errors','extra_words')),'not strict: '+name)
def table_proof(score,obj):
    raw=sections(score,obj)[2].get('.data',b'')
    require(len(raw)==960,'owned table extent mismatch')
    native=score.own_data();require(native is not None,'protected data unavailable')
    require(raw==native.read(0x80114264,960),'table-content mismatch')
    return {'whole_bytes':960,'rectangles':{'address':'0x80114264','bytes':896,'sha256':hashlib.sha256(raw[:896]).hexdigest()},
            'cameras':{'address':'0x801145E4','bytes':64,'sha256':hashlib.sha256(raw[896:]).hexdigest()}}
def compile_variant(score,directory,body):
    shutil.copytree(HERE/'group',directory,dirs_exist_ok=True)
    (directory/'createbg.c').write_text(body)
    obj=directory/'candidate.o';score.compile_group(directory,obj);return obj

def layout_proof(score,out,body):
    facts=['sizeof(Color4)','sizeof(Vertex20)','sizeof(Mesh)','sizeof(Texture36)','sizeof(BackRect56)','sizeof(MultiBlit)',
           '(u32)&((Mesh*)0)->vertices','(u32)&((Mesh*)0)->flags','(u32)&((Vertex20*)0)->s','(u32)&((Vertex20*)0)->t',
           '(u32)&((Vertex20*)0)->color','(u32)&((Texture36*)0)->width','(u32)&((Texture36*)0)->height',
           '(u32)&((BackRect56*)0)->offX','(u32)&((BackRect56*)0)->offY','sizeof(D_80114264)','sizeof(D_801145E4)']
    expected=[4,20,88,36,56,36,8,2,12,14,16,16,18,24,40,896,64]
    output={}
    for tag,source in [('native',body),('wrong_vertex_capacity',body.replace('Vertex20 vertices[4]','Vertex20 vertices[5]',1))]:
        directory=out/tag;directory.mkdir(parents=True,exist_ok=True);probe=directory/'layout.c'
        probe.write_text(source[:source.index('void native_ShowPoly')]+'\nconst u32 mat3_layout_facts[] = {'+','.join(facts)+'};\n')
        obj=directory/'layout.o';score.compile_single(probe,'-g0 -O2 -mips2 -G 0 -non_shared',obj)
        data,secs,_=sections(score,obj);entries=[e for e in symbols(score,obj) if e['name']=='mat3_layout_facts']
        require(len(entries)==1,'layout fact symbol missing');e=entries[0];sec=secs[e['section']]
        raw=data[sec['off']+e['value']:sec['off']+e['value']+len(expected)*4];actual=list(struct.unpack('>17I',raw))
        if tag=='native':require(actual==expected,'native layout mismatch')
        else:require(actual!=expected and actual[2]==108,'wrong capacity not rejected')
        output[tag]=actual
    return {'expressions':facts,'expected':expected,'observed':output}

def verify(repo=ROOT,out=None,record=False,compile=True):
    repo=Path(repo).resolve();sys.path.insert(0,str(repo/'tools/cloud'));import score
    g=json.loads((HERE/'group/group.json').read_text())
    require(g.get('claims')==[],'false group claims')
    require(g['members']==NAMES and g['context']==[] and g['keep']==NAMES,'group boundary changed')
    require(g['files']==['createbg.c','newpoly.c','updatepoly.c'] and g['flags']==FLAGS,'recipe changed')
    source=inputs();targets={n:whash(score.targets()[n]) for n in NAMES}
    baseline={n:digest(repo/n) for n in ('tools/cloud/score.py','tools/cloud/owndata.py','blob_matched.lock.json','asm/us/blob/SHA256SUMS','asm/us/blob_data/SHA256SUMS')}
    tools={n:digest(Path(score.ido(n))) for n in ('cc','acpp','cfe','ujoin','uld','usplit','umerge','uopt','ugen','as1')}
    saved=None
    if not record:
        saved=json.loads((HERE/'receipt.json').read_text())
        require(saved['protocol_version']==2 and saved['claims']==[] and saved['new_accepted_bytes']==0,'false credit')
        require(saved['source_sha256']==source,'source or proof changed')
        require(saved['target_sha256']==targets and saved['baseline_sha256']==baseline,'target or baseline changed')
        require(saved['toolchain_sha256']==tools,'toolchain changed')
    if not record:
        trace=json.loads((HERE/'trace_fidelity.json').read_text())
        require(trace['inputs_sha256']==source and trace['observation_only'] and not trace['forced_colors'] and trace['complete_objects_identical'],'trace scope or inputs changed')
        require(trace['object_sha256']==saved['results']['natural']['object_sha256'],'trace object changed')
    if not compile: return {'integrity':'PASS'}
    out=Path(out) if out else Path(tempfile.mkdtemp(prefix='createbg-proof-'));out.mkdir(parents=True,exist_ok=True)
    body=(HERE/'group/createbg.c').read_text();variants={'natural':body,'repeat':body,
       'ordinary_models':body.replace('extern volatile u8 D_80140BDC;','extern u8 D_80140BDC;'),
       'wrong_rectangle':body.replace('{ -1.0f, 0.0f, -1.0f, 0.0f, 0.2f, 0.2f,','{ -0.5f, 0.0f, -1.0f, 0.0f, 0.2f, 0.2f,',1),
       'wrong_camera':body.replace('{1, 1, 1, 1}','{2, 1, 1, 1}',1)}
    results={};objects={}
    for tag,text in variants.items():
        require(tag in ('natural','repeat') or text!=body,'negative anchor missing')
        obj=compile_variant(score,out/tag,text);objects[tag]=obj
        rows={n:detail(score,obj,n) for n in NAMES}
        for n in PRESERVED:
            if tag!='wrong_rectangle' or n!='func_800FC9F8': exact(rows[n],n)
            require(rows[n]['symbol_bytes']==len(score.targets()[n])*4,'preserved extent: '+n)
        root=rows[ROOT_NAME];require(root['symbol_bytes']==(728 if tag=='ordinary_models' else 732) and root['frame_bytes']==184,'root extent/frame changed')
        if tag in ('natural','repeat'):
            require(root['comparison']['differing']==59 and not any(root['comparison'][k] for k in ('unresolved','unverified','errors','extra_words')),'natural residual changed')
            table=table_proof(score,obj)
        elif tag=='ordinary_models':
            require(root['comparison']['differing']==153,'ordinary-count control changed');table=table_proof(score,obj)
        else:
            try: table_proof(score,obj)
            except RuntimeError as e: require(str(e)=='table-content mismatch','wrong negative failure')
            else: raise RuntimeError('bad table accepted')
            table={'rejected':'table-content mismatch'}
        _,_,secs=sections(score,obj)
        results[tag]={'object_sha256':digest(obj),'section_sha256':{n:hashlib.sha256(d).hexdigest() for n,d in secs.items()},'functions':rows,'tables':table}
    require(objects['natural'].read_bytes()==objects['repeat'].read_bytes(),'repeat changed object')
    layout=layout_proof(score,out/'layout',body)
    hd=out/'historical';hd.mkdir();shutil.copy2(HERE/'controls/historical.c',hd/'historical.c')
    hg=dict(g,files=['historical.c'],members=[ROOT_NAME],keep=[ROOT_NAME]);(hd/'group.json').write_text(json.dumps(hg))
    ho=hd/'candidate.o';score.compile_group(hd,ho);hrow=detail(score,ho,ROOT_NAME)
    require(hrow['comparison']['differing']==59 and hrow['symbol_bytes']==732 and hrow['frame_bytes']==184,'historical control changed')
    historical={'object_sha256':digest(ho),'function':hrow,'source_limits':'shifted table base and abbreviated polygon view; no semantic claim'}
    # The complete four-body graph and an explicitly unclaimed deleted helper are inventoried.
    functions=[s for s in symbols(score,objects['natural']) if s['type']==2 and s['size']]
    require({s['name'] for s in functions}==set(NAMES+['native_ShowPoly']),'unaccounted emitted function')
    residue=next(s for s in functions if s['name']=='native_ShowPoly');require(residue['size']==8,'helper residue changed')
    result={'protocol_version':2,'claims':[],'new_accepted_bytes':0,'source_sha256':source,'target_sha256':targets,
            'baseline_sha256':baseline,'toolchain_sha256':tools,'results':results,'preserved':PRESERVED,
            'nonnative_residues':{'native_ShowPoly':8},'layout':layout,'historical':historical,'semantic_execution_claim':False,'canonical_or_rom_verification':False}
    if record: (HERE/'receipt.json').write_text(json.dumps(result,indent=2)+'\n')
    else: require(result==saved,'sealed result changed')
    return result
if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--repo',type=Path,default=ROOT);p.add_argument('--out',type=Path);p.add_argument('--record',action='store_true');a=p.parse_args()
    verify(a.repo,a.out,a.record);print('PASS: createBG59/183; three strict context bodies;960 owned bytes; no credit')
