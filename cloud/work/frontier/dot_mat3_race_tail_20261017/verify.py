#!/usr/bin/env python3
"""Protocol-v2 full-group strict replay and bounded negative controls."""
from contextlib import redirect_stdout
from pathlib import Path
import hashlib,io,json,shutil,struct,sys,tempfile
HERE=Path(__file__).resolve().parent
GROUP=HERE/'groups/frontier_car_checkpoints'

def require(ok, message):
    if not ok:
        raise RuntimeError(message)

def verify(repo=None,compile=True):
    repo=Path(repo).resolve() if repo else HERE.parents[3]
    receipt=json.loads((HERE/'receipt.json').read_text())
    require(receipt['protocol_version']==2 and receipt['native_bytes']==924 and receipt['new_claims']==['render_viewport_init'], 'false credit or unsupported protocol')
    require(hashlib.sha256(Path(__file__).read_bytes()).hexdigest()==receipt['verify_sha256'], 'verify mutation')
    for rel,digest in receipt['source_sha256'].items():
        require(hashlib.sha256((HERE/rel).read_bytes()).hexdigest()==digest, rel+' mutation')
    sys.path.insert(0,str(repo/'tools/cloud'))
    import score
    group=json.loads((GROUP/'group.json').read_text())
    require(len(group['members'])==25 and not group['context'], 'proof condition failed')
    require(group['claims']==group['members']==receipt['claims'], 'proof condition failed')
    for name,digest in receipt['target_words_sha256'].items():
        words=score.targets()[name]
        require(hashlib.sha256(struct.pack('>'+str(len(words))+'I',*words)).hexdigest()==digest, name+' target changed')
    if not compile:return {'input_integrity':True,'compiler_run':False}
    code=(GROUP/'race_tail.c').read_text()
    variants={
        'candidate':code,
        'direct_index_no_car':code.replace('    Car952 *car;\n','').replace('        car = &player_array[i];\n','').replace('car->','player_array[i].'),
        'flatten_smoke_reset':code.replace('            func_800CB718(slot);','            D_801392D8[slot] &= ~0x20;'),
        'plain_restart_reads':code.replace('*(volatile s8 *)&car->restart','car->restart'),
        'camera_stride_156':code.replace('u8 pad90[8]','u8 pad90[12]'),
        'slot_arg_s8':code.replace('void cpak_read(s16);','void cpak_read(s8);'),
        'merged_one_literal':code.replace('save_write_data(&slot, 1, 1.f, 0)','save_write_data(&slot, 1, 1.0f, 0)'),
        'gc_sound_id':code.replace('func_800B61A8(45,','func_800B61A8(9,').replace('0.0f, 45,','0.0f, 9,'),
    }
    results={}
    with tempfile.TemporaryDirectory(prefix='mat3-race-tail-') as tmp:
        tmp=Path(tmp)
        for variant,source in variants.items():
            require(variant=='candidate' or source!=code, 'unchanged control '+variant)
            path=tmp/variant;shutil.copytree(GROUP,path)
            (path/'race_tail.c').write_text(source)
            obj=path/'result.o';score.compile_group(path,obj)
            rows={}
            for name in group['members']:
                with redirect_stdout(io.StringIO()):comparison=score.compare(obj,name,show=0)
                if name=='render_viewport_init':
                    require(comparison.accepted()==(variant=='candidate'), variant+': '+comparison.summary())
                else:require(comparison.accepted(), variant+' '+name+': '+comparison.summary())
                rows[name]=comparison.summary()
            results[variant]=rows
    return {'input_integrity':True,'compiler_run':True,'results':results}

if __name__=='__main__':
    print(json.dumps(verify(sys.argv[1] if len(sys.argv)>1 else None),indent=2))
