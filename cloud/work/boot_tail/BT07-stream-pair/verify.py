#!/usr/bin/env python3
"""Read-only strict replay for two COMPLETE-NONMATCH stream bodies."""
from hashlib import sha256
from pathlib import Path
import json,struct,subprocess,sys,tempfile
ROOT=Path(__file__).resolve().parents[4]
PACKET=Path(__file__).resolve().parent
sys.path.insert(0,str(ROOT))
from tools.cloud import score
score.ASM_DIR=ROOT/'asm/us/boot_tail'
EXPECTED={'80025AB4':[(45,0),(94,11)],'800252AC':[(44,0),(136,28)]}
INITIAL={'80025AB4':[(99,3),(103,9)],'800252AC':[(126,7),(137,32)]}
def digest(path):return sha256(path.read_bytes()).hexdigest()
def trial(source,address,level,obj):
    name='func_'+address; flags='-g0 -O%d -mips2 -G 0 -non_shared'%level
    score.compile_single(source,flags,obj)
    result=score.compare(obj,name,show=0)
    native=score.targets()[name]; words=score.text_words(obj)
    relocated,masks,unresolved,unverified,errors=score.relocate(obj,words,0,len(native)*4,score.image_symbols())
    assert score.symbols(obj)=={name:0}
    assert not result.accepted()
    assert not masks and not unresolved and not unverified
    if level==2:assert not errors
    return {'flags':flags,'effective_flags':flags+' -Wab,-r4300_mul',
            'source_sha256':digest(source),'object_sha256':digest(obj),
            'differing_words':result.differing,'total_words':result.total,'extra_words':result.extra_words,
            'strict_match':result.accepted(),'unresolved':unresolved,'unverified':unverified,
            'relocation_errors':errors,'relocation_mask_count':len(masks),
            'full_body_equal':relocated[:len(native)]==native,'object_text_bytes':len(words)*4}
def run():
    manifest=subprocess.check_output(['sha256sum','-c','SHA256SUMS'],cwd=score.ASM_DIR,text=True).splitlines()
    inv=json.loads((ROOT/'specs/015-boot-tail-runtime/inventory.json').read_text())['functions']
    ext=json.loads((score.ASM_DIR/'extents.json').read_text())['functions']
    assert len(inv)==len(ext)==439
    assert {r['address']:r['size'] for r in inv}=={r['address']:r['size'] for r in ext}
    assert sum(r['size'] for r in inv)==99120
    selected=[r for r in inv if r['address'][2:] in EXPECTED]
    assert len(selected)==2 and sum(r['size'] for r in selected)==988
    assert all(r['scope']=='in_scope' for r in selected)
    pins=json.loads((PACKET/'input_pins.json').read_text())
    compiler={p.name:digest(p) for p in sorted(score.IDO.iterdir()) if p.is_file()}
    assert compiler==pins['compiler_files_sha256']
    for path,value in pins['source_hashes'].items():assert digest(ROOT/path)==value,path
    receipt={'packet':'BT07-stream-pair','base_commit':'301d9e7552ad4fd7f54a38796db84671e1000d35',
             'central_claim_commit':'b5723bfa','manifest':manifest,'compiler_files_sha256':compiler,
             'inventory_extents':{'count':439,'bytes':99120,'equal':True},'results':[],
             'packet_hashes':{str(p.relative_to(ROOT)):digest(p) for p in sorted(PACKET.rglob('*')) if p.is_file() and p.name not in ['verification.json','semantics.json']}}
    with tempfile.TemporaryDirectory(prefix='bt07-stream-verify-') as tmp:
        tmp=Path(tmp)
        getter=ROOT/'cloud/matches/boot_tail/func_80010A00.c'
        score.compile_single(getter,score.DEFAULT_FLAGS,tmp/'getter.o')
        assert score.compare(tmp/'getter.o',getter.stem,show=0).accepted()
        receipt['getter_strict_match']=True
        for address in EXPECTED:
            source=PACKET/('func_'+address+'.c')
            assert source.read_text().splitlines()[0]=='/* flags: '+score.DEFAULT_FLAGS+' */'
            native=score.targets()['func_'+address]
            row={'name':'func_'+address,'bytes':len(native)*4,'source_path':str(source.relative_to(ROOT)),
                 'source_sha256':digest(source),'target_body_sha256':sha256(struct.pack('>%dI'%len(native),*native)).hexdigest(),'trials':[],'initial_control':[]}
            for k,level in enumerate((2,1)):
                result=trial(source,address,level,tmp/(address+'-%d.o'%level))
                assert (result['differing_words'],result['extra_words'])==EXPECTED[address][k]
                row['trials'].append(result)
                control=trial(PACKET/'controls'/('initial_'+address+'.c'),address,level,tmp/(address+'-initial-%d.o'%level))
                assert (control['differing_words'],control['extra_words'])==INITIAL[address][k]
                row['initial_control'].append(control)
            receipt['results'].append(row)
    receipt.update(result='PASS',new_matching_bodies=0,new_matching_bytes=0,complete_nonmatch_bodies=2,complete_nonmatch_bytes=988)
    return receipt
if __name__=='__main__':print(json.dumps(run(),indent=2))
