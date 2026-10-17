"""Replay source-bound StartEnvironment research; no accepted matching bytes."""
from pathlib import Path
import argparse,contextlib,hashlib,io,json,shutil,struct,sys
from dataclasses import asdict
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[3]
sys.path.insert(0,str(HERE))
from environment_behavior import score,require,verify as behavior_verify,candidate
from environment_layout import verify as layout_verify
from environment_negatives import verify as negative_verify
TARGET='music_control'
INPUTS=['verify.py','environment_behavior.py','environment_layout.py','environment_negatives.py',
        'trace_fidelity.py','group/group.c','group/group.json']
def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def compare(obj):
    with contextlib.redirect_stdout(io.StringIO()):row=score.compare(obj,TARGET,show=0)
    require(not any((row.extra_words,row.unresolved,row.unverified,row.errors)),'uncertain comparison')
    return dict(asdict(row),summary=row.summary())
def stack_only(w,g):
    return w>>16==g>>16 and (w>>21)&31==29 and w>>26 in (9,33,35,40,41,43,49,57)
def verify(out,record=False):
    out=Path(out);out.mkdir(parents=True,exist_ok=True)
    spec=json.loads((HERE/'group/group.json').read_text())
    require(spec['claims']==[] and spec['members']==[TARGET] and spec['context']==[],'research scope changed')
    require('__standin_' not in (HERE/'group/group.c').read_text(),'stand-in caller')
    inputs={p:digest(HERE/p) for p in INPUTS}
    target=score.targets()[TARGET];target_hash=hashlib.sha256(struct.pack('>%dI'%len(target),*target)).hexdigest()
    if not record:
        receipt=json.loads((HERE/'verification.json').read_text())
        require(receipt['inputs_sha256']==inputs,'source changed')
        require(receipt['target_words_sha256']==target_hash,'native target changed')
        trace=json.loads((HERE/'trace_fidelity.json').read_text())
        require(trace['inputs_sha256']==inputs,'trace input binding changed')
    obj=out/'candidate.o';score.compile_group(HERE/'group',obj);row=compare(obj)
    if not record:
        require(digest(obj)==trace['stock_object_sha256']==trace['trace_object_sha256'],'trace object binding changed')
    require(row['differing']==18 and row['total']==248,'research residual changed')
    data,sections=score._elf(obj);ti=score._text_index(sections)
    symbols=[s for i,sec in enumerate(sections) if sec['type']==2 for s in score._symbol_table(data,sections,i) if s['name']==TARGET and s['section']==ti and s['type']==2]
    require(len(symbols)==1 and symbols[0]['size']==992,'complete symbol extent')
    code=candidate(obj);compiled=[code[0x800B1B48+4*i] for i in range(len(target))]
    for word,frame in ((target[0],160),(compiled[0],112)):
        require(word>>26==9 and (word>>21)&31==29 and (word>>16)&31==29 and word&65535==65536-frame,'stack frame changed')
    bad=[(i,w,g) for i,(w,g) in enumerate(zip(target,compiled)) if w!=g]
    require(len(bad)==18 and all(stack_only(w,g) for _,w,g in bad),'non-stack residual')
    # Historical flag spelling is a semantics-preserving compiler control.
    old=out/'old_flag';old.mkdir(exist_ok=True);shutil.copyfile(HERE/'group/group.json',old/'group.json')
    source=(HERE/'group/group.c').read_text();spelling='0x8000 | (0x200 | flags)'
    require(source.count(spelling)==1,'ambiguous flag expression')
    (old/'group.c').write_text(source.replace(spelling,'flags|0x8200'))
    old_obj=out/'old_flag.o';score.compile_group(old,old_obj);old_row=compare(old_obj)
    require(old_row['differing']==172,'historical control changed')
    result=dict(status='VERIFIED_RESEARCH_PARTIAL',claims=[],new_matching_bytes=0,
                canonical_or_ROM_verification=False,inputs_sha256=inputs,target_words_sha256=target_hash,
                object_sha256=digest(obj),comparison=row,original_flag_control=old_row,
                complete_symbol_bytes=992,native_frame_bytes=160,candidate_frame_bytes=112,
                residual_kind='18 stack/frame immediates only; identical opcode and register fields',
                emitted_type_layout=layout_verify(out),behavior=behavior_verify(obj),negative_controls=negative_verify(out))
    if record:(HERE/'verification.json').write_text(json.dumps(result,indent=2)+'\n')
    return result
if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--record',action='store_true');p.add_argument('--out',type=Path,default=ROOT/'build/mat3_environment_20261017')
    args=p.parse_args();verify(args.out,args.record)
    print('PASS research: StartEnvironment 18/248 stack-only; original flag 172; 512 native/candidate fixtures; 3 semantic negatives; zero matching credit')
