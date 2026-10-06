"""Rebuild the complete strict match, GNU link, real context and behavior proof."""
import argparse
from dataclasses import asdict
import hashlib
import importlib.util
import json
from pathlib import Path
import shutil
import struct
import subprocess
import sys
import tempfile

HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[3]

# Context groups superseded after this packet was frozen are kept byte-identical
# under cloud/work/frontier/superseded/; receipts keep the original paths.
SUPERSEDED={'src/blob/groups/frontier_list_alloc_sound':'cloud/work/frontier/superseded/frontier_list_alloc_sound'}
def context_path(p):
    p=str(p)
    for old,new in SUPERSEDED.items():
        if p.startswith(old) and not (ROOT/old).exists():return ROOT/(new+p[len(old):])
    return ROOT/p
sys.path.insert(0,str(ROOT))
from tools.cloud import score
spec=importlib.util.spec_from_file_location('direction_semantics',HERE/'semantics.py')
semantics=importlib.util.module_from_spec(spec);spec.loader.exec_module(semantics)
FN='stat_lap_split'
SOURCE=ROOT/'cloud/matches/stat_lap_split.c'
FLAGS='-g0 -O3 -mips2 -G 0 -non_shared'
CALLERS=['func_8010C7F4','func_8010D9CC','func_8010DAF8','func_8010DF90']
CONTEXT=['src/blob/func_800A61B0.c','src/blob/groups/frontier_list_alloc_sound/group.c']+['src/blob/'+n+'.c' for n in CALLERS]
CONTEXT_NAMES=['func_800A61B0','func_80092278','entity_flags_apply','high_scores_display',
               'func_8009211C','func_80091FBC']+CALLERS


def sha(data): return hashlib.sha256(data).hexdigest()


def inspect(obj,name):
    data,secs=score._elf(obj); ti=score._text_index(secs)
    syms=[s for i,sec in enumerate(secs) if sec['type']==2
          for s in score._symbol_table(data,secs,i)]
    fn,=[s for s in syms if s['name']==name and s['type']==2 and s['section']==ti]
    cmp=score.compare(obj,name,show=0)
    return dict(asdict(cmp),verdict=cmp.summary(),symbol_bytes=fn['size'],
                native_bytes=4*len(score.targets()[name]),own_data_notes=list(cmp.notes))


def complete(result):
    return (result['verdict']=='MATCH' and result['symbol_bytes']==result['native_bytes'] and
            not any(result[k] for k in ('differing','unresolved','unverified','errors','extra_words')))


def alias_diagnostic(work,late):
    """A compiler-metadata counterfactual, explicitly not a C submission."""
    folder=work/'alias_diagnostic';folder.mkdir()
    listings={}
    for name,path in [('late',late),('natural',SOURCE)]:
        sub=folder/name;sub.mkdir();shutil.copyfile(path,sub/'source.c')
        subprocess.run([score.ido('cc'),'-c','-K',*FLAGS.split(),score.R4300_CC,
                        '-o','source.o','source.c'],cwd=sub,check=True,capture_output=True)
        listings[name]=(sub/'u.out.s').read_text()
    marker='\t.noalias\t$2,$sp\n'
    assert listings['late'].count(marker)==1 and marker not in listings['natural']
    modified=listings['late'].replace(marker,'')
    (folder/'diagnostic.s').write_text(modified)
    subprocess.run([score.ido('as0'),'-G','0','-mips2','-EB','-g0','-O3','diagnostic.s',
                    '-o','diagnostic.G','-t','diagnostic.T'],cwd=folder,check=True,capture_output=True)
    subprocess.run([score.ido('as1'),'-elf','-G','0','-p0','-mips2','-EB','-g0','-O3',
                    '-r4300_mul','-Olimit','5000','diagnostic.G','-o','diagnostic.o',
                    '-t','diagnostic.T'],cwd=folder,check=True,capture_output=True)
    comparison=inspect(folder/'diagnostic.o',FN)
    assert complete(comparison)
    return {'diagnostic_only':True,'eligible_for_promotion':False,
            'change':'Remove one compiler-generated v0/stack no-alias directive from the late-pointer listing.',
            'natural_source_omits_that_directive':True,'comparison':comparison}


def verify(work,behavior=True):
    obj=work/'candidate.o';score.compile_single(SOURCE,FLAGS,obj)
    result={'base_revision':'cc4d5fdd','function':FN,'range':['0x800FEA00','0x800FEC60'],
            'status':'STRICT_MATCH_PENDING_INDEPENDENT_REVIEW','new_candidate_bytes':608,
            'accepted_byte_gain':0,'flags':FLAGS,'assembler_erratum_flag':score.R4300_CC,
            'source_sha256':sha(SOURCE.read_bytes()),'object':inspect(obj,FN)}
    assert complete(result['object'])
    data,secs=score._elf(obj);ti=score._text_index(secs);txt=secs[ti]
    assert txt['size']==608
    ro,=[s for s in secs if s['name']=='.rodata']
    own=data[ro['off']:ro['off']+ro['size']]
    literal=struct.pack('>f',semantics.f(.924)*semantics.f(.924))
    assert own[:4]==literal==score.own_data().read(0x80124824,4)
    assert not any(own[4:]) and not any(s['size'] for s in secs if s['name'] in ('.data','.bss'))
    result['own_literal']={'address':'0x80124824','expression':'.924f * .924f',
                           'bytes_verified':4,'value':struct.unpack('>f',literal)[0],
                           'zero_section_alignment_bytes':len(own)-4}
    result['text_alignment_bytes_outside_symbol']=0
    relocs=[]
    for sec in secs:
        if sec['type']==9 and sec['info']==ti:
            symbols=score._symbol_table(data,secs,sec['link'])
            for off in range(sec['off'],sec['off']+sec['size'],8):
                at,info=struct.unpack_from('>II',data,off)
                relocs.append({'offset':at,'type':info&255,'symbol':symbols[info>>8]['name']})
    assert len(relocs)==15 and sum(r['type']==4 for r in relocs)==3
    result['relocations']=relocs
    addresses=score.image_symbols()
    names={r['symbol'] for r in relocs if r['symbol']!='.rodata'}
    script=work/'link.ld'
    script.write_text('SECTIONS { .text 0x800FEA00 : SUBALIGN(4) { *(.text) }\n'
                      '.rodata 0x80124824 : SUBALIGN(4) { *(.rodata) } }\n'+
                      ''.join('%s = 0x%08X;\n'%(n,addresses[n]) for n in sorted(names)))
    elf=work/'candidate.elf'
    subprocess.run(['mips-linux-gnu-ld','-EB','-T',str(script),'-o',str(elf),str(obj)],
                   check=True,capture_output=True)
    linked,sections=score._elf(elf);text=sections[score._text_index(sections)]
    words=list(struct.unpack('>152I',linked[text['off']:text['off']+608]))
    assert words==score.targets()[FN]
    linked_ro,=[s for s in sections if s['name']=='.rodata']
    shoff=struct.unpack_from('>I',linked,0x20)[0]
    shentsize=struct.unpack_from('>H',linked,0x2e)[0]
    assert struct.unpack_from('>I',linked,shoff+sections.index(linked_ro)*shentsize+12)[0]==0x80124824
    assert linked[linked_ro['off']:linked_ro['off']+4]==literal
    syms=[s for i,sec in enumerate(sections) if sec['type']==2 for s in score._symbol_table(linked,sections,i)]
    linked_fn,=[s for s in syms if s['name']==FN and s['type']==2]
    assert linked_fn['value']==0x800fea00 and linked_fn['size']==608
    result['gnu_link']={'all_152_words_equal':True,'symbol_bytes':608,
                        'literal_bytes_equal':True,'relocation_count':len(relocs)}
    controls={}
    source_text=SOURCE.read_text()
    variants={
        'late_pointer':source_text.replace('GameCar *car = &player_array[slot];','GameCar *car;').replace(
            '    vecsub(position, car->position, delta);','    car = &player_array[slot];\n    vecsub(position, car->position, delta);'),
        'expanded_product':source_text.replace('side_squared *= (f32)', 'side_squared = side_squared * (f32)'),
    }
    for name,text in variants.items():
        src=work/(name+'.c');src.write_text(text);o=work/(name+'.o')
        score.compile_single(src,FLAGS,o);controls[name]=inspect(o,FN)
    assert controls['late_pointer']['differing']==5
    assert controls['expanded_product']['differing']==1
    result['alias_causality']=alias_diagnostic(work,work/'late_pointer.c')
    for name,src,flags in [('archived_b8',ROOT/'cloud/work/near_miss_B8/stat_lap_split_B8_pointerfirst.c',FLAGS),
                           ('o2',SOURCE,FLAGS.replace('-O3','-O2'))]:
        out=work/(name+'.o');score.compile_single(src,flags,out);controls[name]=inspect(out,FN)
    assert controls['archived_b8']['differing']==6
    result['controls']=controls
    # Existing accepted exported roots are unchanged; no keep/root experiment.
    old=json.loads(context_path('src/blob/groups/frontier_list_alloc_sound/group.json').read_text())
    group=work/'context';group.mkdir()
    files=[SOURCE]+[context_path(p) for p in CONTEXT]
    for i,src in enumerate(files): shutil.copyfile(src,group/('c%d.c'%i))
    (group/'group.json').write_text(json.dumps({'files':['c%d.c'%i for i in range(len(files))],
        'members':[FN],'context':CONTEXT_NAMES,'keep':[FN,'func_800A61B0']+old['keep']+CALLERS,'flags':FLAGS}))
    out=group/'group.o';score.compile_group(group,out)
    result['genuine_context']={n:inspect(out,n) for n in [FN]+CONTEXT_NAMES}
    assert all(complete(r) for r in result['genuine_context'].values())
    result['context_sources']={p:sha(context_path(p).read_bytes()) for p in CONTEXT}
    if behavior: result['behavior']=semantics.verify(work,words,SOURCE)
    result['target_manifest_sha256']=sha((score.ASM_DIR/'SHA256SUMS').read_bytes())
    result['compiler_sha256']={n:sha(Path(score.ido(n)).read_bytes()) for n in
                              ['cc','cfe','uld','usplit','umerge','uopt','ugen','as1']}
    result['packet_sha256']={n:sha((HERE/n).read_bytes()) for n in ['verify.py','semantics.py','native.py','host.c']}
    result['limitations']=['No full-game shadow unit, splice, image, compression or ROM gate.',
                          'Source contract requires a valid player slot even on early-return paths.',
                          'Sound API uses a synthetic callback; actual sound subsystem behavior is not emulated.',
                          'Finite binary32 host round-to-nearest tests exclude FCSR effects and signaling NaNs.',
                          'Claim checks cannot reveal unpublished work.']
    return result


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--write',action='store_true')
    args=parser.parse_args()
    with tempfile.TemporaryDirectory(prefix='directional-sound-proof-') as temp:
        result=verify(Path(temp))
    if args.write: (HERE/'verification.json').write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps({k:result[k] for k in ('status','object','gnu_link','controls','behavior')},indent=2))
