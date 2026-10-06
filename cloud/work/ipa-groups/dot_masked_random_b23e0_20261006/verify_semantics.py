"""Bounded exact interpreter for the complete native selector and source oracle.

Unsupported instructions/memory/control flow fail closed. The unsigned-convert
fallback is outside this function's reachable range: Random(32) lies in [0,32).
"""
import hashlib
import math
from pathlib import Path
import random
import struct
import subprocess

HERE=Path(__file__).resolve().parent
MASK=0xffffffff
SEED=0x8011735c
TABLE=0x80123418
STACK=0x80408000
RETURN=0x81234560

def signed(x): return x if x<0x80000000 else x-(1<<32)
def bits(x): return struct.unpack('>I',struct.pack('>f',x))[0]
def real(x): return struct.unpack('>f',struct.pack('>I',x))[0]

def oracle(seed, argument, mask, limit=4096):
    """Integer expression independently uses the exact scale's sample bins."""
    trace=[]
    for _ in range(limit):
        seed=(1103515245*seed+12345)%(1<<32)
        trace.append(seed)
        choice=((seed//65536)%32768)//1024
        if mask & (2**choice): return seed,choice,trace
    raise RuntimeError('iteration bound exceeded')

class Native:
    def __init__(self,words):
        assert len(words)==67, 'complete 268-byte native body required'
        self.words=words
        self.coverage=set()
        self.branches=set()

    def run(self,seed,argument,mask,limit=4096,fcsr=0,stop_after=None):
        assert fcsr & ~0x7c == 0, 'only default rounding and sticky flags modeled'
        initial_seed=seed
        r=[(0x13570000+i*0x10203)&MASK for i in range(32)]
        r[0]=0; r[4]=argument; r[29]=STACK; r[31]=RETURN
        f=[0x3e800000+i for i in range(32)]
        original_r=list(r);original_f=list(f);original_fcsr=fcsr
        memory={TABLE+4*i:(0xa5f00000+i) for i in range(256)}
        memory[TABLE+4*(argument&255)]=mask
        memory[SEED]=seed
        for a in range(STACK-32,STACK+36,4):memory[a]=0xca000000+(a-STACK+32)
        initial=dict(memory);writes=[];seed_trace=[];reads=[]
        lo=None;pc=0;pending=None;returned=False;steps=0
        while not returned:
            assert 0<=pc<len(self.words), 'execution left complete function'
            assert steps<limit*67, 'instruction bound exceeded'
            steps+=1;self.coverage.add(pc*4);w=self.words[pc]
            op=w>>26;rs=(w>>21)&31;rt=(w>>16)&31;rd=(w>>11)&31;sh=(w>>6)&31;fn=w&63
            imm=w&65535;simm=imm if imm<32768 else imm-65536
            finish=pending;pending=None;next_pc=pc+1
            if op==15:r[rt]=imm<<16
            elif op==9:r[rt]=(r[rs]+simm)&MASK
            elif op==13:r[rt]=r[rs]|imm
            elif op==12:r[rt]=r[rs]&imm
            elif op==35:
                addr=(r[rs]+simm)&MASK
                assert addr in (SEED,TABLE+4*(argument&255)), 'unexpected read'
                r[rt]=memory[addr];reads.append(addr)
            elif op==43:
                addr=(r[rs]+simm)&MASK
                assert addr in (SEED,STACK), 'unexpected write'
                memory[addr]=r[rt];writes.append((addr,r[rt]))
                if addr==SEED:
                    seed_trace.append(r[rt])
                    if stop_after is not None and len(seed_trace)==stop_after:break
            elif op==0 and fn==25:lo=(r[rs]*r[rt])&MASK
            elif op==0 and fn==18:
                assert lo is not None;r[rd]=lo
            elif op==0 and fn==0:r[rd]=(r[rt]<<sh)&MASK
            elif op==0 and fn==3:r[rd]=(signed(r[rt])>>sh)&MASK
            elif op==0 and fn==4:r[rd]=(r[rt]<<(r[rs]&31))&MASK
            elif op==0 and fn==33:r[rd]=(r[rs]+r[rt])&MASK
            elif op==0 and fn==36:r[rd]=r[rs]&r[rt]
            elif op==0 and fn==37:r[rd]=r[rs]|r[rt]
            elif op==0 and fn==8:
                assert rs==31 and r[31]==RETURN and finish is None
                pending='return'
            elif op in (4,5,20) or op==1:
                assert finish is None, 'branch in delay slot'
                if op==1:
                    assert rt==0;take=signed(r[rs])<0
                else:take=(r[rs]==r[rt]) if op in (4,20) else (r[rs]!=r[rt])
                self.branches.add((pc*4,bool(take)))
                if op==20 and not take:next_pc=pc+2
                else:pending=pc+1+simm if take else pc+2
            elif op==17 and rs==4:f[rd]=r[rt]
            elif op==17 and rs==0:r[rt]=f[rd]
            elif op==17 and rs==2:
                assert rd==31;r[rt]=fcsr
            elif op==17 and rs==6:
                assert rd==31;fcsr=r[rt]
            elif op==17 and rs==20 and fn==32:f[sh]=bits(float(signed(f[rd])))
            elif op==17 and rs==16 and fn==2:f[sh]=bits(real(f[rd])*real(f[rt]))
            elif op==17 and rs==16 and fn==3:f[sh]=bits(real(f[rd])/real(f[rt]))
            elif op==17 and rs==16 and fn==36:
                assert fcsr==1,'conversion must select truncation with exceptions disabled'
                value=real(f[rd]);assert math.isfinite(value) and 0<=value<32,'native range contract violated'
                result=math.trunc(value);f[sh]=result&MASK
                if value!=result:fcsr|=(1<<2)|(1<<12)
            else:raise AssertionError('unsupported instruction at +0x%x'% (pc*4))
            r[0]=0
            if finish=='return':returned=True
            elif finish is not None:next_pc=finish
            pc=next_pc
        assert all(r[i]==original_r[i] for i in list(range(16,24))+[28,29,30,31])
        assert f[20:]==original_f[20:]
        assert writes[0]==(STACK,argument)
        assert all(addr==SEED for addr,value in writes[1:])
        assert memory[STACK]==argument
        assert {a:v for a,v in memory.items() if a not in (SEED,STACK)}=={a:v for a,v in initial.items() if a not in (SEED,STACK)}
        assert reads[:2]==[SEED,TABLE+4*(argument&255)] and len(reads)==2, 'seed and invariant table load order changed'
        if stop_after is not None:
            assert not returned and len(seed_trace)==stop_after
            return seed_trace
        assert returned and fcsr==original_fcsr
        expected=oracle(initial_seed,argument,mask,limit)
        assert (memory[SEED],r[2],seed_trace)==expected
        assert 0<=r[2]<32 and mask&(1<<r[2])
        return memory[SEED],r[2],seed_trace

def cases(retail_masks):
    inverse=pow(1103515245,-1,1<<32)
    for sample in range(32768):
        next_seed=(sample<<16)|((sample*4051)&65535)|((sample&1)<<31)
        seed=((next_seed-12345)*inverse)&MASK
        yield seed,((sample*0x1020301)&0xffffff00)|(sample&255),MASK
    rng=random.Random(0xb23e0)
    for i in range(4096):
        yield rng.randrange(1<<32),rng.randrange(1<<32),1<<(i%32)
    for index,mask in enumerate(retail_masks):
        assert mask
        for i in range(32):yield rng.randrange(1<<32),index,mask
    for mask in [MASK,0x80000000,1,0x55555555,0xaaaaaaaa,0x80000001]:
        for seed in [0,1,MASK,0x80000000,0x7fffffff,12345]:
            for index in [0,31,127,255]:yield seed,index,mask

def host(directory,payload,source=None,range_source=None):
    source=source or HERE/'selector.c';range_source=range_source or HERE/'range.c'
    exe=directory/('host-'+hashlib.sha256(source.read_bytes()+range_source.read_bytes()).hexdigest()[:12])
    subprocess.run(['cc','-std=c89','-O2','-fwrapv','-ffp-contract=off','-fsanitize=undefined','-fno-sanitize-recover=all',str(HERE/'rand.c'),str(range_source),str(source),str(HERE/'semantic_test.c'),'-o',str(exe)],check=True,capture_output=True)
    result=subprocess.run([str(exe)],input=payload,capture_output=True,check=True,timeout=90)
    assert result.stderr==b''
    return result.stdout

def verify(directory,native_words,linked_words,retail_masks):
    inputs=list(cases(retail_masks));native=Native(native_words);linked=Native(linked_words)
    payload=b''.join(struct.pack('=III',*x) for x in inputs);expected=[];max_iterations=0;retry_cases=0
    digest=hashlib.sha256()
    for i,x in enumerate(inputs):
        value=oracle(*x);max_iterations=max(max_iterations,len(value[2]));retry_cases+=len(value[2])>1
        assert native.run(*x,fcsr=(i%32)<<2)==value
        assert linked.run(*x,fcsr=(i%32)<<2)==value
        digest.update(struct.pack('>%dI'%len(value[2]),*value[2]))
        expected.append(value[:2])
    expected_bytes=b''.join(struct.pack('=II',*x) for x in expected)
    assert host(directory,payload)==expected_bytes
    source=(HERE/'selector.c').read_text();range_source=(HERE/'range.c').read_text();rejected={}
    variants={'ignore_mask':source.replace('D_80123418[index] & (1U << choice)','1'),
              'wrong_table_index':source.replace('D_80123418[index]','D_80123418[(index+1)&255]'),
              'skip_high_bit':source.replace('choice = (u32)func_8008B2E4(32.0f);','choice = (u32)func_8008B2E4(31.0f);')}
    # Mutant tests use all-bit masks to guarantee termination even for wrong range.
    # Mask omission is checked on the full corpus; high-bit exclusion is exercised
    # on first-draw all-bit masks, avoiding deliberately nonterminating mutants.
    for name,text in variants.items():
        path=directory/(name+'.c');path.write_text(text)
        chosen=inputs if name=='ignore_mask' else inputs[:32768]
        exp=expected_bytes if name=='ignore_mask' else expected_bytes[:32768*8]
        out=host(directory,b''.join(struct.pack('=III',*x) for x in chosen),path)
        assert len(out)==len(exp)
        bad=next(i for i in range(len(chosen)) if out[i*8:i*8+8]!=exp[i*8:i*8+8])
        rejected[name]={'first_case':bad}
    changed=range_source.replace('32768.0f','32767.0f');path=directory/'wrong_denominator.c';path.write_text(changed)
    try:
        out=host(directory,payload[:32768*12],range_source=path)
        bad=next(i for i in range(32768) if out[i*8:i*8+8]!=expected_bytes[i*8:i*8+8])
        rejected['wrong_denominator']={'first_case':bad}
    except subprocess.CalledProcessError as error:
        assert b'shift exponent 32' in error.stderr
        rejected['wrong_denominator']={'rejection':'UBSan rejects out-of-range shift from produced choice 32'}
    zero=Native(native_words).run(12345,7,0,stop_after=128)
    z=12345;wanted=[]
    for _ in range(128):z=(z*1103515245+12345)&MASK;wanted.append(z)
    assert zero==wanted
    assert native.coverage==linked.coverage
    assert native.branches==linked.branches
    return {'cases':len(inputs),'native_executions':2*len(inputs),'host_c89_ubsan':True,'host_signed_wrap':'-fwrapv',
            'all_32768_first_random_outputs':True,'retail_table_window_rows':len(retail_masks),'synthetic_index_domain':'all 256 low-byte values; varied full upper argument bits',
            'retry_cases':retry_cases,'maximum_iterations':max_iterations,'seed_trace_sha256':digest.hexdigest(),'result_sha256':hashlib.sha256(expected_bytes).hexdigest(),
            'executed_instruction_offsets':sorted(native.coverage),'conditional_outcomes':[list(x) for x in sorted(native.branches)],
            'zero_mask_bounded_prefix':128,'compiled_mutants_rejected':rejected,
            'limitations':'finite mapped inputs and nonzero masks; conversion fallback unreachable under [0,32); default rounding with sticky flags, no trap enables/FCSR hardware exception claim; zero mask bounded native prefix only'}
