#!/usr/bin/env python3
"""Independent review: original full-function extents, GNU relocation, and paired semantics.

No native bytes or dumps are persisted. Requires the protected overlay-A target
manifest, IDO toolchain and GNU MIPS assembler/linker. Temporary .word assembly
contains newly compiled candidate bytes only and is deleted at exit.
"""
import contextlib
import hashlib
import io
import json
from pathlib import Path
import random
import re
import struct
import subprocess
import sys
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
BOUNDARY = ROOT / 'cloud/work/frontier/dot_runtime_a_availability_boundary_20261006'
sys.path[:0] = [str(ROOT / 'tools/cloud'), str(BOUNDARY)]
import score
import native
from elf_support import elf

NAMES = {'func_8039D494': 528, 'func_8039E3BC': 136, 'func_8039ED20': 480}

def sha(b): return hashlib.sha256(b).hexdigest()
def packed(w): return struct.pack('>%dI' % len(w), *w)
def shell(*args, **kw):
    p = subprocess.run(list(map(str, args)), capture_output=True, text=True, **kw)
    assert p.returncode == 0, (args, p.stdout, p.stderr)
    assert 'runtime error:' not in p.stderr
    return p.stdout

def function(text, name):
    start = re.search(r'^(?:s32|void) ' + name + r'\([^;\n]*\)\n\{', text, re.M).start()
    brace = text.index('{', start); depth = 1; end = brace + 1
    while depth:
        depth += (text[end] == '{') - (text[end] == '}'); end += 1
    return text[start:end]

def gnu_resolve(obj, temp):
    """Re-section complete ELF function bodies; GNU independently applies relocations.

    Local .text addends are translated to their ELF STT_FUNC owner and offset.
    This is not a claim that this extracted three-function ELF is a full image.
    """
    secs, syms, rels = elf(obj)
    ti, _, raw = secs['.text']; lines = ['.set noreorder', '.set noat']; externs = set(); counts = {}
    for name, size in NAMES.items():
        start, actual, kind, section = syms[name]
        assert (actual, kind, section) == (size, 2, ti)
        following = min(v[0] for v in syms.values() if v[2] == 2 and v[3] == ti and v[0] > start)
        assert following == start + size, (name, actual, following)
        words = list(struct.unpack('>%dI' % (size // 4), raw[start:start+size])); entries = []
        for loc, typ, symbol, dest in rels:
            if dest != ti or not start <= loc < start + size: continue
            assert typ in (4, 5, 6) and loc % 4 == 0
            off = loc - start
            if symbol == '.text':
                assert typ == 4
                add = (words[off//4] & 0x3ffffff) * 4
                owners = [(n,v) for n,v in syms.items() if v[2] == 2 and v[3] == ti and v[0] <= add < v[0]+v[1]]
                assert len(owners) == 1
                symbol, owner = owners[0]
                words[off//4] = (words[off//4] & 0xfc000000) | ((add-owner[0])//4)
            assert not symbol.startswith('.')
            entries.append((off, typ, symbol)); externs.add(symbol)
        lines += ['.section .review_' + name + ',"ax",@progbits', '.balign 4', '.globl ' + name, '.type '+name+',@function', name+':']
        lines += ['.word 0x%08x' % w for w in words]
        lines += ['.size %s, .-%s' % (name, name)]
        lines += ['.reloc %s+%d,%s,%s' % (name, off, {4:'R_MIPS_26',5:'R_MIPS_HI16',6:'R_MIPS_LO16'}[typ], sym) for off,typ,sym in entries]
        counts[name] = len(entries)
    assembly = temp/'review.s'; assembly.write_text('\n'.join(lines)+'\n')
    reloc = temp/'review.o'; linked = temp/'review.elf'; script = temp/'review.ld'
    script.write_text('SECTIONS {\n'+''.join('.review_%s 0x%s : SUBALIGN(4) { *(.review_%s) }\n' % (n,n[-8:],n) for n in NAMES)+' /DISCARD/ : { *(.reginfo) *(.MIPS.abiflags) *(.pdr) *(.gnu.attributes) }\n}\n'+''.join('%s = 0x%X;\n' % (n, score.image_symbols().get(n,score.address_named(n))) for n in sorted(externs-set(NAMES))))
    shell('mips-linux-gnu-as','-EB','-mips2','-o',reloc,assembly)
    shell('mips-linux-gnu-ld','-EB','-T',script,'-o',linked,reloc)
    ss, sy, rr = elf(linked); assert not rr
    result = {}
    for n,size in NAMES.items():
        index, row, body = ss['.review_'+n]
        assert row[3] == int(n[-8:],16) and len(body) == size
        assert sy[n] == (int(n[-8:],16),size,2,index)
        result[n] = list(struct.unpack('>%dI' % (size//4), body))
    return result, counts

def host_pair(source, cases, temp):
    declarations = '''#include <stdio.h>
typedef signed char s8; typedef signed short s16; typedef signed int s32;
s8 D_801407D0,D_80156994,D_8014978C,D_803B9FD0[4],D_803B3020[128];
s32 D_803AF980,D_8014A110,D_803BA050[4],D_803BA060[4][19];
s16 D_801164BE;
'''
    driver = (BOUNDARY/'host.c').read_text()
    body = driver[driver.index('int main(void)'):]
    body = body.replace('printf("%d\\n", func_8039D494(item, player));', '''for(p=0;p<4;p++) { int j; D_803BA050[p]=-91; for(j=0;j<19;j++)D_803BA060[p][j]=-91; }
        func_8039E3BC(player);
        printf("%d",D_803BA050[player]);
        for(p=0;p<19;p++)printf(" %d",D_803BA060[player][p]);
        printf("\\n");
        for(p=0;p<4;p++)if(p!=player) { int j; if(D_803BA050[p]!=-91)return 3; for(j=0;j<19;j++)if(D_803BA060[p][j]!=-91)return 4; }''')
    path = temp/'host.c'; path.write_text(declarations+function(source,'func_8039D494')+'\n'+function(source,'func_8039E3BC')+'\n'+body)
    binary = temp/'host'; shell('gcc','-std=c89','-pedantic-errors','-O2','-fsanitize=undefined,bounds','-fno-sanitize-recover=all',path,'-o',binary)
    output = shell(binary, input=''.join(' '.join(map(str,c))+'\n' for c in cases))
    rows = [list(map(int,line.split())) for line in output.splitlines()]
    assert len(rows)==len(cases)
    return rows

def main():
    if not __debug__:
        raise SystemExit('Run without Python -O')
    score.ASM_DIR = ROOT/'asm/us/ovl_a'
    manifest = score.target_manifest(); targets = score.targets()
    meta = json.loads(score.verified_bytes(score.ASM_DIR/'extents.json',manifest))
    assert (meta['image'],meta['base'],meta['rom_offset'])==('A','0x8038A400','0xB5C534')
    source = (HERE/'group.c').read_text(); baseline = ROOT/'cloud/work/lean/runtime_a_car_select_20261006'
    old = (baseline/'group.c').read_text()
    expected = old.replace('func_8039D494(s32 item, s32 player)','func_8039D494(s32 player, s32 item)').replace('func_8039D494(item,player)','func_8039D494(player,item)').replace('D_803BA050[player]=0;\n    for(item=0;','D_803BA050[player]=0; for(item=0;')
    assert source == expected
    assert source.count('func_8039D494(player,item)')==3
    assert json.loads((HERE/'group.json').read_text())['claims']==['func_8039E3BC']
    with tempfile.TemporaryDirectory(prefix='available-list-review-') as td:
        temp=Path(td); obj=temp/'candidate.o'; prior=temp/'prior.o'
        score.compile_group(HERE,obj); score.compile_group(baseline,prior)
        resolved,relocations = gnu_resolve(obj,temp)
        # Audit every generated local call to both pair members, including the
        # real E3BC upstream callers. Register moves are decoded, not dumped.
        secs, syms, rels = elf(obj); words = score.text_words(obj)
        by_offset = {syms[n][0]:n for n in ('func_8039D494','func_8039E3BC')}
        callsites = []
        move = lambda source, dest: (source << 21) | (dest << 11) | 37
        for loc,typ,symbol,dest in rels:
            if typ != 4 or symbol != '.text': continue
            callee = by_offset.get((words[loc//4] & 0x3ffffff)*4)
            if not callee: continue
            owner = next(n for n,v in syms.items() if v[2]==2 and v[3]==secs['.text'][0] and v[0]<=loc<v[0]+v[1])
            offset = loc-syms[owner][0]
            if callee=='func_8039D494':
                assert words[loc//4+1]==move(10 if owner=='func_8039E3BC' else 19,4)
                if owner=='func_8039E3BC': assert words[loc//4-1]==move(12,5)
                elif offset==0x8f0: assert words[loc//4-1]==move(22,5)
                else:
                    assert owner=='func_8039D6A4' and offset==0x8b4
                    assert words[loc//4-0x194//4]==move(22,5)
                    assert not any(w>>26==3 for w in words[loc//4-0x194//4:loc//4])
            elif owner=='func_803A0FD8': assert words[loc//4-2]==move(20,12)
            else: assert owner=='func_803A1EAC' and words[loc//4+1]==move(30,12)
            callsites.append({'caller':owner,'offset':hex(offset),'callee':callee})
        assert len(callsites)==5
        assert source.count('func_8039E3BC(player)')==2
        comparisons={}; bindings={}
        for n,size in NAMES.items():
            extent=next(f for f in meta['functions'] if f['name']==n)
            assert int(extent['address'],16)==int(n[-8:],16) and extent['size']==size
            assert len(targets[n])*4==size and resolved[n]==targets[n]
            with contextlib.redirect_stdout(io.StringIO()): comp=score.compare(obj,n,show=0)
            assert comp.accepted() and comp.differing==0 and comp.extra_words==0
            comparisons[n]=comp.__dict__
            bindings[n]={'address':extent['address'],'bytes':size,'sha256':sha(packed(targets[n])),'entry_evidence':extent['evidence'],'relocations':relocations[n]}
        for n in ('func_8039D494','func_8039ED20'):
            words=score.text_words(prior);start=score.symbols(prior)[n]
            rr,m,u,v,e=score.relocate(prior,words,start,start+NAMES[n],score.image_symbols())
            assert not any((m,u,v,e)) and rr[start//4:(start+NAMES[n])//4]==resolved[n]
        leafcases=list(native.cases()); leafcoverage=set()
        for i,c in enumerate(leafcases):
            m=native.state(c)
            for code in (targets['func_8039D494'],resolved['func_8039D494']):
                result=native.run({native.ENTRY:code},m,c[0],c[1],salt=i)
                assert result['return']==native.oracle(c) and result['memory']==m and not result['writes']
                assert all(result['registers'][k]==result['initial_registers'][k] for k in range(10,14))
                leafcoverage.update(result['coverage'])
        cases=[]
        for p in range(4):
            for mask in range(32):
                for bits in range(8):
                    cases.append((0,p,mask&1,(mask>>1)&1,3 if mask&4 else -1,(mask>>3)&1,2 if mask&16 else 6,mask&4,(mask*3+bits)%128,bits))
        cases += [c for c in leafcases[-1024:]]
        hostrows=host_pair(source,cases,temp)
        signedcases=[(0,p,1,1,7,1,2,1,s,(s*31)&255) for p in range(4) for s in range(-128,0)]
        callercoverage=set(); branch=set()
        nativepair={native.ENTRY:targets['func_8039D494'],native.CALLER:targets['func_8039E3BC']}
        gnupair={native.ENTRY:resolved['func_8039D494'],native.CALLER:resolved['func_8039E3BC']}
        for i,c in enumerate(cases+signedcases):
            wanted=[k for k in range(19) if native.oracle((k,)+c[1:])]; m=native.state(c);p=c[1]
            rr=native.run(nativepair,m,player=p,caller=True,salt=i)
            gg=native.run(gnupair,m,player=p,caller=True,salt=i)
            assert rr==gg
            assert native.read(rr['memory'],native.COUNTS+p*4,4)==len(wanted)
            assert [native.read(rr['memory'],native.OUTPUT+p*76+j*4,4) for j in range(len(wanted))]==wanted
            allowed=set(range(native.COUNTS+p*4,native.COUNTS+p*4+4))|set(range(native.OUTPUT+p*76,native.OUTPUT+p*76+4*len(wanted)))|set(range(native.SP-24,native.SP))
            assert all(rr['memory'][a]==b for a,b in m.items() if a not in allowed)
            assert all(all(a+j in allowed for j in range(4)) for a,_ in rr['writes'])
            assert rr['registers'][16]==19 and rr['registers'][12]==p
            callercoverage.update(rr['coverage']);branch.update(rr['branches'])
            if i<len(cases):assert hostrows[i]==[len(wanted)]+wanted+[-91]*(19-len(wanted))
        assert len([a for a in callercoverage if native.CALLER<=a<native.CALLER+136])==34
        negative=[]
        for label,body in [('unknown-opcode',[0xfc000000]),('escaped-code',[0])]:
            try:native.run({native.ENTRY:body},native.state(leafcases[0]))
            except AssertionError:negative.append(label)
        assert len(negative)==2
        receipt={'status':'PASS','claimed_new_match':'func_8039E3BC','source_sha256':sha(source.encode()),'source_diff_exactly_expected':True,'comparisons':comparisons,'full_extent_bindings':bindings,'previous_D494_ED20_complete_bytes_unchanged':True,'gnu_route':'fresh complete STT_FUNC bodies re-sectioned from fresh IDO object; original REL records retained; .text R26 addends translated by ELF function ownership; GNU as/ld relocation at native VAs','source_calls':{'D494_formal_order':['player','item'],'all_three_real_calls_consistent':True,'generated_D494_input_order':{'a0':'item','a1':'player'},'E3BC_private_input':'t4=player','E3BC_private_clobber':'s0=19','all_generated_pair_callsites':callsites,'D494_preserves':['t2','t3','t4','t5']},'semantics':{'leaf_cases_per_route':len(leafcases),'pair_cases_per_route':len(cases)+len(signedcases),'host_C89_UBSan_bounds_pair_cases':len(cases),'signed_native_pair_cases':len(signedcases),'pair_routes':['protected native','fresh GNU linked candidate'],'complete_memory_register_read_write_trace_equality':True,'guard_sibling_tail_integrity':True,'E3BC_instructions_covered':34,'D494_instructions_covered':len(leafcoverage),'branch_outcomes':len(branch),'decoder_negative_checks':negative},'protected_manifest':manifest,'support_sha256':{p.name:sha(p.read_bytes()) for p in (BOUNDARY/'native.py',BOUNDARY/'elf_support.py',ROOT/'tools/cloud/score.py')},'limits':['No full-ROM, full-image, splice, compressed stream or hardware claim.','D6A4 and E3BC upstream callers are source-audited, not executed end-to-end.','E3BC uses a compiler-private t4 input and s0 clobber; it is not a standalone O32 function.','Host tests use selectors 0..127; signed negative selector checks are mapped native-memory tests only.','GNU independently links extracted complete function bodies, not the whole group image.']}
        (HERE/'independent_review.json').write_text(json.dumps(receipt,indent=2)+'\n')
        print(json.dumps({'status':'PASS','semantics':receipt['semantics'],'bindings':bindings},indent=2))

if __name__=='__main__': main()
