#!/usr/bin/env python3
"""Protocol-v2 research replay. An exact loader is not dependency-closure credit."""
from pathlib import Path
from contextlib import redirect_stdout
import dataclasses, hashlib, importlib.util, io, json, shutil, struct, sys, tempfile
HERE=Path(__file__).resolve().parent

def require(ok,msg):
    if not ok:raise RuntimeError(msg)
def digest(b):return hashlib.sha256(b).hexdigest()
def wordhash(ws):return digest(struct.pack('>'+str(len(ws))+'I',*ws))
def module(name):
    spec=importlib.util.spec_from_file_location('readpath_'+name,HERE/(name+'.py'))
    m=importlib.util.module_from_spec(spec);spec.loader.exec_module(m);return m

def function(score,obj,name,resolve=False):
    data,secs=score._elf(obj)
    entries=[s for i,sec in enumerate(secs) if sec['type']==2 for s in score._symbol_table(data,secs,i)
             if s['type']==2 and s['section']==score._text_index(secs) and s['name']==name]
    require(len(entries)==1,name+' function extent');e=entries[0];a=e['value'];b=a+e['size'];ws=score.text_words(obj)
    if resolve:
        ws,m,u,v,err=score.relocate(obj,ws,a,b,score.image_symbols())
        require(not(m or u or v or err),name+' uncertain relocation')
    return ws[a//4:b//4]

def extent(words):
    return {'bytes':len(words)*4,'frame':next((65536-(w&65535) for w in words[:40] if w>>16==0x27bd and w&32768),0)}

def sections(score,obj):
    data,secs=score._elf(obj)
    return {s['name']:digest(data[s['off']:s['off']+s['size']]) for s in secs if s['type'] in (1,4,9) and s['size']}

LAYOUT=[('vertex_size','sizeof(Vertex)',6),('alternate_size','sizeof(Alternate)',16),
        ('alternate_count','OFF(Alternate,numPts)',10),('alternate_points','OFF(Alternate,pts)',12),
        ('respath_size','sizeof(ResPath)',16),('respath_points','OFF(ResPath,pts)',4),
        ('respath_altcount','OFF(ResPath,numAlt)',8),('respath_altwidth','sizeof(((ResPath*)0)->numAlt)',1),
        ('respath_alts','OFF(ResPath,alt)',12),('drone_size','sizeof(DronePath)',8),
        ('drone_points','OFF(DronePath,pts)',4),('point_size','sizeof(DronePoint)',8),
        ('point_mph','OFF(DronePoint,mph)',6),('point_mphwidth','sizeof(((DronePoint*)0)->mph)',1),
        ('signed_counter','(s16)-1',0xffffffff),('unsigned_count','(u16)-1',65535)]

def layout(score,src,d):
    head=src[src.index('typedef signed char'):src.index('extern s8')]
    code=head+'\n#define OFF(t,m) ((u32)&((t*)0)->m)\nu32 layout_values[]={'+','.join(x[1] for x in LAYOUT)+'};\n'
    d.mkdir();(d/'probe.c').write_text(code);score.compile_single(d/'probe.c','-g0 -O3 -mips2 -G 0 -non_shared',d/'probe.o')
    data,secs=score._elf(d/'probe.o')
    es=[s for i,sec in enumerate(secs) if sec['type']==2 for s in score._symbol_table(data,secs,i) if s['name']=='layout_values']
    require(len(es)==1 and es[0]['size']==4*len(LAYOUT),'layout extent');e=es[0];sec=secs[e['section']]
    got=struct.unpack('>'+str(len(LAYOUT))+'I',data[sec['off']+e['value']:sec['off']+e['value']+e['size']])
    return {'values':dict(zip((x[0] for x in LAYOUT),got)), 'mismatches':[n for (n,_,v),g in zip(LAYOUT,got) if v!=g]}

def produce(score):
    g=json.loads((HERE/'group/group.json').read_text());source=(HERE/'group/loader.c').read_text();behavior=module('behavior')
    variants={'natural':source,'repeat':source,
              'truthy_alternate':source.replace('numAlt > 0','numAlt'),
              'wrong_header':source.replace('D_80153F20+812','D_80153F20+816'),
              'wrong_drone_count':source.replace('p<4','p<3')}
    require(len(set(variants.values()))==4,'control mutation failed')
    results={}
    with tempfile.TemporaryDirectory(prefix='readpath-research-') as t:
        tmp=Path(t)
        for tag,src in variants.items():
            d=tmp/tag;shutil.copytree(HERE/'group',d);(d/'loader.c').write_text(src);obj=d/'result.o';score.compile_group(d,obj)
            rows={}
            for name in g['members']+g['context']:
                with redirect_stdout(io.StringIO()):r=score.compare(obj,name,show=0)
                require(not(r.unresolved or r.unverified or r.errors or r.extra_words),tag+': '+name+' incomplete proof')
                if name in g['members']:require(r.accepted(),tag+': lost '+name)
                rows[name]=dataclasses.asdict(r)
            words=function(score,obj,'audio_effect_apply',True)
            if tag in ('natural','repeat'):require(rows['audio_effect_apply']['differing']==0 and extent(words)=={'bytes':696,'frame':80},'loader match')
            else:require(rows['audio_effect_apply']['differing']>0,'negative matched')
            require(rows['physics_response']['differing']==211,'natural timing residual changed')
            result={'scores':rows,'loader':dict(extent(words),body_sha256=wordhash(words)),
                    'timing':extent(function(score,obj,'physics_response')),'sections':sections(score,obj)}
            if tag in ('natural','repeat','truthy_alternate'):
                result['behavior']=behavior.verify(score.targets()['audio_effect_apply'],words)
            else:
                rejected=False
                try:behavior.verify(score.targets()['audio_effect_apply'],words)
                except RuntimeError:rejected=True
                require(rejected,tag+' semantic negative accepted');result['semantic_negative_rejected']=True
            results[tag]=result
        require(results['natural']==results['repeat'],'complete replay changed')
        lm={'native':source,'vertex_stride':source.replace('Vertex[3]','Vertex[4]'),
            'wide_alt_count':source.replace('u8 numAlt;u8 pad9[3]','u16 numAlt;u8 pad9[3]'),
            'wide_speed':source.replace('u8 mph,pad7','u16 mph,pad7')}
        layouts={}
        for tag,src in lm.items():
            require(tag=='native' or src!=source,'layout mutation failed');v=layout(score,src,tmp/('layout_'+tag))
            require(bool(v['mismatches'])==(tag!='native'),tag+' layout result');layouts[tag]=v
    return {'claims':[],'preserved_locked_matches':len(g['members']),'new_accepted_bytes':0,
            'natural_dependency_closure':False,'results':results,'layouts':layouts}

def verify(repo=None,compile=True):
    repo=Path(repo).resolve() if repo else HERE.parents[3];r=json.loads((HERE/'receipt.json').read_text())
    require(r['protocol_version']==2 and r['claims']==[] and r['new_accepted_bytes']==0,'false claims')
    require(digest(Path(__file__).read_bytes())==r['verify_sha256'],'verifier mutation')
    for n,h in r['source_sha256'].items():require(digest((HERE/n).read_bytes())==h,n+' source mutation')
    sys.path.insert(0,str(repo/'tools/cloud'));import score
    for n,h in r['target_words_sha256'].items():require(wordhash(score.targets()[n])==h,n+' target mutation')
    g=json.loads((HERE/'group/group.json').read_text())
    require(g['claims']==[] and g['members']==r['preserved_locked_members'] and g['context']==['audio_effect_apply','physics_response'],'group roles')
    require(set(g['members']+g['context'])==set(r['target_words_sha256']),'target set')
    require('do {} while (0)' not in (HERE/'group/physics_response.c').read_text(),'artificial timing blocks')
    if not compile:return {'input_integrity':True,'compiler_run':False,'claims':[]}
    result=produce(score);require(result==r['expected'],'replay differs from sealed receipt')
    return dict(result,input_integrity=True,compiler_run=True)

if __name__=='__main__':print(json.dumps(verify(sys.argv[1] if len(sys.argv)>1 else None),indent=2))
