"""Source receipts, controlled variants and fixed-width layout checks only.

No compiler, scorer, target execution, image or ROM verification is performed.
Uses explicit failures so the same checks remain active under python -O.
"""
import ctypes as C
import hashlib
import json
from pathlib import Path
import re
import struct

HERE = Path(__file__).resolve().parent


def need(ok, message):
    if not ok:
        raise ValueError(message)


def body(source, signature):
    start = source.index(signature)
    brace = source.index('{', start)
    depth = 1
    i = brace + 1
    while depth:
        if source[i] == '{':
            depth += 1
        elif source[i] == '}':
            depth -= 1
        i += 1
    return source[start:i]


def check(packet, blobs):
    need(packet['claims']==[] and packet['fresh_score'] is None, 'false match claim')
    need(packet['new_matching_bytes']==0 and not packet['compiler']['executed'], 'false compile/coverage claim')
    targets = packet['targets']
    need(len(targets)==14 and len({t['name'] for t in targets})==14, 'fourteen distinct targets')
    need(sum(t['bytes'] for t in targets if t['population']=='blob')==12996, 'game subtotal')
    need(sum(t['bytes'] for t in targets if t['population']=='ovl_b')==8392, 'overlay subtotal')
    for path, expected in packet['published_files'].items():
        need(hashlib.sha256(blobs[path]).hexdigest()==expected, 'source drift: '+path)
    baseline = blobs['routes/baseline_w6b.c'].decode('utf-8')
    signature = 's32 audio_priority_find(s16 who, s16 route)'
    helper = body(baseline, signature)
    types = baseline[baseline.index('typedef signed char s8;'):baseline.index('extern RouteSet D_801407F0;')]
    for name, row in packet['route_groups'].items():
        src = blobs[row['source']].decode('utf-8')
        spec = row['spec']
        need(spec['claims']==[] and set(spec['members'])=={'audio_priority_find','audio_mixer_main'}, 'route manifest')
        need(body(src, signature)==helper and types in src, 'helper/types drift')
        need('func_800B930C(' not in src, 'locked successor redefined')
        if name!='baseline_w6b':
            need(not re.search(r'^\s*s32 pad\[2\];',src,re.M), 'new padding device')
        axes = row['axes']
        if 'successor_placement' in axes:
            ctype = axes['successor_type']
            definition = src.index('static %s gc_lane_GetNextCheckpoint(%s cp) {' % (ctype,ctype))
            caller = src.index('static void gc_lane_FindPathDistances(void) {')
            need((definition<caller)==(axes['successor_placement']=='above'), 'successor placement')
            need('return cp + %s;' % ('1U' if ctype=='unsigned int' else '1') in src, 'successor arithmetic')
    need('audio_priority_find' in packet['route_groups']['negative_kept_helper']['spec']['keep'], 'ABI negative absent')
    blast=packet['blast_group']
    need(blast['claims']==[] and len(blast['members'])==3, 'blast claim/member drift')
    need('func_80090308' not in blast['keep'], 'internal blast accidentally kept')
    need(set(blast['context'])=={r['symbol'] for r in packet['blast_context'].values()}, 'genuine blast context')
    need('f32 mat[12];' in blobs['blast/baseline_start_blast.c'].decode('utf-8'), 'baseline evidence missing')
    need('f32 mat[12];' not in blobs['blast/start_blast.c'].decode('utf-8'), 'unsupported new array')
    need(packet['c910_owner']['owner']=='Astra' and 'handoff' in packet['c910_owner']['purpose'], 'ownership lost')
    candidate=blobs['entity/candidate.c'].decode('utf-8')
    need('Surface *surfaceptr;' in candidate and 'temp[0] * 0.25f' in candidate, 'entity source contract')


def layouts():
    class Visual(C.Structure):
        _fields_=[('next',C.c_uint32),('index',C.c_int16),('handle',C.c_int16),('slot',C.c_int16),
                  ('data',C.c_uint32),('time',C.c_float),('callback',C.c_uint32)]
    class Blast(C.Structure):
        _fields_=[('handle',C.c_int32),('matrix',C.c_float*12),('motion',C.c_float*6),
                  ('alpha',C.c_uint8*2),('unused',C.c_int16),('height',C.c_float)]
    class Aggregate(C.Structure):
        _fields_=[('objects',Blast*4),('extra',C.c_int32),('matrix',C.c_float*12),
                  ('alpha',C.c_uint8*4),('life',C.c_float),('increment',C.c_float)]
    class Defense(C.Structure):
        _fields_=[('active',C.c_uint32),('invisibility',C.c_float),('time',C.c_float),
                  ('scale',C.c_float),('phase',C.c_float)]
    class Battle(C.Structure):
        _fields_=[('weapon',C.c_uint8*8),('defense',Defense),('shoot',C.c_int8),
                  ('fade',C.c_uint8),('state',C.c_int8),('points',C.c_int8),
                  ('loaded',C.c_int32),('times_angles',C.c_float*4)]
    need(C.sizeof(Visual)==24 and Visual.slot.offset==8 and Visual.callback.offset==20, 'visual layout')
    need(C.sizeof(Blast)==84 and Blast.height.offset==80, 'blast layout')
    need(C.sizeof(Aggregate)==400 and Aggregate.matrix.offset==340, 'aggregate layout')
    need(C.sizeof(Defense)==0x14 and C.sizeof(Battle)==0x34, 'N64 shield phase')
    need(0x384+Battle.defense.offset+Defense.phase.offset==0x39c, 'shield phase offset')
    need(0x384+Battle.shoot.offset==0x3a0, 'shoot offset')
    need(0xf8+21*C.sizeof(Visual)==0x2f0, 'embedded visual')
    memory=bytearray(0x32c)
    for cp in range(10):
        for path in range(20):
            a=0xc+cp*0x50+0x22+path*2
            b=cp*0x50+0x2e+path*2
            need(a==b,'checkpoint header skew')
            struct.pack_into('>h',memory,a,cp*20+path)
            need(struct.unpack_from('>h',memory,b)[0]==cp*20+path,'checkpoint source model')
    cases=0
    wrong=False
    for count in range(1,11):
        for loop in range(count):
            for cp in range(count):
                signed=loop if cp==count-1 else cp+1
                unsigned=loop if cp==count-1 else (cp+1)&0xffffffff
                need(signed==unsigned,'valid-domain successor')
                wrong |= (loop if cp==count-1 else cp+2)!=signed
                cases+=1
    need(wrong,'semantic negative not detected')
    return {'checkpoint_slots':200,'successor_model_cases':cases,
            'visual':24,'blast':84,'aggregate':400,'defense':20,'battle':52}


def main():
    packet=json.loads((HERE/'packet.json').read_text(encoding='utf-8'))
    blobs={p:(HERE/p).read_bytes() for p in packet['published_files']}
    expected=set(blobs)|{'packet.json','CHECKS.json'}
    actual={p.relative_to(HERE).as_posix() for p in HERE.rglob('*') if p.is_file() and '__pycache__' not in p.parts}
    need(actual==expected,'unexpected or missing published file: '+str(actual^expected))
    check(packet,blobs)
    forbidden={'.zip','.o','.obj','.elf','.dol','.bin','.rom','.z64','.n64','.v64','.exe','.dll','.hex','.s'}
    for path,data in blobs.items():
        need(Path(path).suffix.lower() not in forbidden,'forbidden artifact '+path)
        text=data.decode('utf-8')
        need('\x00' not in text,'binary payload')
        need(not re.search(r'(?m)^\s*\.word\s+0x',text),'raw target words')
        need(not re.search(r'(?i)(?:[A-Z]:\\|/[h]ome/|/[U]sers/|[g]ithub_pat_|[g]hp_[A-Za-z0-9]{20}|-----[B]EGIN .*PRIVATE KEY)',text), 'private path/credential '+path)
    rejected=[]
    changed=dict(blobs)
    changed['entity/candidate.c']=changed['entity/candidate.c'].replace(b'temp[0] * 0.25f',b'temp[0] * 0.5f')
    for name,p,b in [('source mutation',packet,changed),
                     ('false claim',dict(packet,claims=['entity_iterate']),blobs),
                     ('kept internal blast',dict(packet,blast_group=dict(packet['blast_group'],keep=packet['blast_group']['keep']+['func_80090308'])),blobs)]:
        try:
            check(p,b)
        except ValueError:
            rejected.append(name)
        else:
            raise ValueError('negative control accepted '+name)
    report={'state':'SOURCE_INTEGRITY_AND_LAYOUT_ONLY','files_checked':len(blobs),
            'targets':14,'population_bytes':packet['population_bytes'],'claims':[],
            'compiler_run':False,'scorer_run':False,'fresh_score':None,
            'negative_controls_rejected':rejected,'layout_models':layouts()}
    print(json.dumps(report,indent=2,sort_keys=True))


if __name__=='__main__':
    main()
