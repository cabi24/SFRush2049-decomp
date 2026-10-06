#!/usr/bin/env python3
"""Full extent, all relocations, GNU placement and four-route bounded behavior."""
import argparse, hashlib, importlib.util, json, os, struct, subprocess, sys
from pathlib import Path
from elf_support import elf
import semantics
HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[3]
NAME='func_803A4134';ENTRY=0x803A4134;SIZE=524
SOURCE=ROOT/'cloud/matches/ovl_a'/ (NAME+'.c')
FLAGS='-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul'
NATIVE='c79fb81fa3af695c6fa11fb403e7ddba62771bdd022442c8cc4410715c21f9eb'
ANCHORS={'D_8014A108':0x8014A108,'D_8014A110':0x8014A110,'D_803B85D8':0x803B85D8,'D_803BA028':0x803BA028,'D_803BA190':0x803BA190,'func_800EF5B0':0x800EF5B0,'input_new_data_wrapper':0x80094F88,'Input_ApplyPadConfig':0x80094EC8}
HELPERS={'func_800EF5B0':[0x800EF5B0,124,'b9b278845d966d684513a894e9b728164b3291016e67967a1a51fb395c30eadd'], 'input_new_data_wrapper':[0x80094F88,60,'51bff8e44879d93b2c44f11fea4c78e7a725283b496a06d94290f1793f4763b3'], 'Input_ApplyPadConfig':[0x80094EC8,192,'3282399bd7919e39047f452faa4070df2ef7b4b91d82318591c2592e7bfda590'], 'sound_control':[0x800B37E8,468,'56fb7406e5ae20cf6e85f574d6bc988e5e5d6dd694cfee5e7ef20c1ed73daad9'], 'state_update_global':[0x8010B560,112,'f6c6d560d9fec4655cd774bd5da62442f4893bbb0cfc07505a37372d6fd7c689']}
def sha(b):return hashlib.sha256(b).hexdigest()
def shell(*args):
    p=subprocess.run([str(x) for x in args],capture_output=True,text=True);assert p.returncode==0,(args,p.stdout,p.stderr);return p.stdout

def inspect(path,linked=False):
    sections,symbols,relocs=elf(path);index,row,raw=sections['.text']
    funcs={n:s for n,s in symbols.items() if s[2]==2 and s[3] not in (0,0xfff1)}
    assert set(funcs)=={NAME};v,size,typ,i=funcs[NAME]
    assert (v,size,i)==(ENTRY if linked else 0,SIZE,index)
    assert row[3]==(ENTRY if linked else 0) and len(raw)==528 and raw[SIZE:]==bytes(4)
    for name,(_,r,b) in sections.items():
        if r[2]&2 and name not in ('.text','.reginfo','.options'):assert r[5]==0,name
    if linked:
        assert not relocs
        for n,a in ANCHORS.items():assert symbols[n][0]==a and symbols[n][3]==0xfff1
    else:
        assert len(relocs)==16
        assert all(off<SIZE and off%4==0 and kind in (4,5,6) and symbol in ANCHORS and target==index for off,kind,symbol,target in relocs)
    return raw,relocs

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--repo',type=Path,default=ROOT);ap.add_argument('--tools-repo',type=Path,default=ROOT);ap.add_argument('--out',type=Path,default=HERE/'verification.json');ap.add_argument('--check',action='store_true');a=ap.parse_args()
    repo=a.repo.resolve();tr=a.tools_repo.resolve();build=ROOT/'build/stat_bar';build.mkdir(parents=True,exist_ok=True)
    tmp=build/'tmp';tmp.mkdir(exist_ok=True);os.environ['TMPDIR']=str(tmp)
    import tempfile;tempfile.tempdir=str(tmp)
    sys.path.insert(0,str(tr/'tools/cloud'));spec=importlib.util.spec_from_file_location('score',tr/'tools/cloud/score.py');score=importlib.util.module_from_spec(spec);sys.modules['score']=score;spec.loader.exec_module(score)
    score.ASM_DIR=repo/'asm/us/ovl_a';manifest=score.target_manifest();words=score.targets()[NAME]
    native=struct.pack('>%dI'%len(words),*words);assert len(native)==SIZE and sha(native)==NATIVE
    meta=json.loads(score.verified_bytes(score.ASM_DIR/'extents.json',manifest));row=next(x for x in meta['functions'] if x['name']==NAME)
    assert meta['image']=='A' and meta['base']=='0x8038A400'
    assert row==dict(name=NAME,address='0x803A4134',size=SIZE,evidence=['data_ref','prologue'])
    addresses=score.image_symbols();assert addresses[NAME]==ENTRY
    for n,v in ANCHORS.items():assert addresses.get(n,score.address_named(n))==v,n
    score.ASM_DIR=repo/'asm/us/blob';helper_manifest=score.target_manifest();hw=score.targets();hs=score.image_symbols()
    for n,(entry,size,digest) in HELPERS.items():
        body=struct.pack('>%dI'%len(hw[n]),*hw[n]);assert hs[n]==entry and len(body)==size and sha(body)==digest,n
    # Bind the distinct complete float-product producer without claiming its C.
    score.ASM_DIR=repo/'asm/us/ovl_a';producer=score.targets()['func_8039D300'];pb=struct.pack('>%dI'%len(producer),*producer)
    assert len(pb)==404 and sha(pb)=='e0bf3d0731f618da18552bced27d621bec55be5f5cc274d1554c3a9656b046bc'
    obj=build/'candidate.o';score.compile_single(SOURCE,FLAGS,obj);comparison=score.compare(obj,NAME,show=0);assert comparison.accepted(),comparison.summary()
    raw,relocs=inspect(obj);full=score.text_words(obj);resolved,masks,unresolved,unverified,errors=score.relocate(obj,full,0,len(full)*4,addresses)
    assert not any((masks,unresolved,unverified,errors));assert struct.pack('>%dI'%len(resolved),*resolved)==native+bytes(4)
    script=build/'whole.ld';script.write_text('SECTIONS { .text 0x803A4134 : SUBALIGN(4) { *(.text) } /DISCARD/ : { *(.options) *(.reginfo) *(.mdebug) *(.pdr) *(.gnu.attributes) } }\n'+''.join('%s = 0x%X;\n'%(n,v) for n,v in ANCHORS.items()))
    linked=build/'candidate.elf';shell('mips-linux-gnu-ld','-EB','-T',script,'-o',linked,obj);linkedraw,_=inspect(linked,True);assert linkedraw==native+bytes(4)
    layout=build/'layout.c';fields={'X':14,'Y':16,'Width':20,'Height':22,'Hide':26,'Top':28,'Bot':30,'Left':32,'Right':34,'AnimFunc':40,'AnimID':44}
    layout.write_text('#include "'+str(SOURCE)+'"\ntypedef char ptr[sizeof(void*)==4?1:-1];\n'+''.join('typedef char f_%s[__builtin_offsetof(Blit,%s)==%d?1:-1];\n'%(n,n,v) for n,v in fields.items()));shell('gcc','-m32','-std=c89','-fsyntax-only',layout)
    behavior,samples=semantics.verify(build,words,list(struct.unpack('>131I',linkedraw[:SIZE])))
    negatives={};text=SOURCE.read_text()
    changes={'wrong_division':('blt->Height / 8','blt->Height >> 3'),'wrong_player':('>> 8','>> 9'),'wrong_visibility':('D_803BA028[player] == 1','D_803BA028[player] != 0'),'wrong_minimum':('blt->Right < 2 ? 2','blt->Right < 3 ? 3'),'missing_disable':('blt->AnimFunc = 0;',';'),'missing_update':('    Input_ApplyPadConfig(blt);','    ;'),'wrong_bar_index':('D_803BA190[player][bar]','D_803BA190[player][bar ^ 1]'),'wrong_stat_player':('D_803BA190[player][bar]','D_803BA190[player ^ 1][bar]'),'wrong_hide_player':('D_803BA028[player]','D_803BA028[player ^ 1]')}
    for label,(old,new) in changes.items():
        assert text.count(old)==1,(label,text.count(old));p=build/(label+'.c');p.write_text(text.replace(old,new));changed=semantics.host(build,p,label)
        failed=next((list(c) for c in samples if changed(c)!=semantics.oracle(c)),None);assert failed is not None,label
        mo=build/(label+'.o');score.compile_single(p,FLAGS,mo);result=score.compare(mo,NAME,show=0);assert not result.accepted()
        negatives[label]={'case':failed,'host_rejected':True,'strict_match':False,'comparison':result.__dict__}
    receipt={'status':'MATCH','image':'A','address':hex(ENTRY),'end':hex(ENTRY+SIZE),'bytes':SIZE,'words':len(words),'flags':FLAGS,'source_sha256':sha(SOURCE.read_bytes()),'native_sha256':sha(native),'gnu_linked_body_sha256':sha(linkedraw[:SIZE]),'comparison':comparison.__dict__,'function_bytes':SIZE,'alignment_bytes':4,'owned_data_bytes':0,'relocations':len(relocs),'anchors':{n:hex(v) for n,v in ANCHORS.items()},'behavior':behavior,'layout_checks':fields,'negative_controls':negatives,'protected_targets':manifest,'helper_native_bindings':HELPERS,'helper_protected_targets':helper_manifest,'producer_native_sha256':sha(pb),'support_sha256':{p.name:sha(p.read_bytes()) for p in (HERE/'host.c',HERE/'semantics.py',HERE/'elf_support.py')},'scorer_sha256':sha((tr/'tools/cloud/score.py').read_bytes()),'tools':{p:sha((score.IDO/p).read_bytes()) for p in ('cc','cfe','uopt','ugen','as1')},'limits':['Table-read player/bar 0..3; rejected uninitialized players through15 tested','Finite signed32-convertible arithmetic only; host float-to-s16 tests restrict truncated result to signed16','Wider signed16 wrapping is native-only evidence, not portable C behavior','Helpers are side-effecting O32 contract hooks; real renderer and texture data do not execute','Actual descriptor table and asset float ranges not available; no reachability or full resource-range proof','No full image/compression/ROM/hardware gate, no coverage acceptance or original-source claim']}
    receipt=json.loads(json.dumps(receipt))
    if a.check:assert json.loads(a.out.read_text())==receipt,'portable receipt differs'
    else:a.out.write_text(json.dumps(receipt,indent=2)+'\n')
    (build/'local_provenance.json').write_text(json.dumps({'object_sha256':sha(obj.read_bytes()),'gcc':shell('gcc','--version').splitlines()[0],'gnu_ld':shell('mips-linux-gnu-ld','--version').splitlines()[0]},indent=2)+'\n')
    print(json.dumps({k:receipt[k] for k in ('status','bytes','relocations','comparison','behavior')},indent=2))
if __name__=='__main__':
    assert __debug__,'Python optimization is unsupported'
    main()
