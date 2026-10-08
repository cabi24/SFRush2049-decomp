"""Inspect complete ELF/ECOFF extents and validate all GNU relocations."""
import hashlib,struct
def sha(x):return hashlib.sha256(x).hexdigest()
def signed16(x):return x-65536 if x&32768 else x

def sections(score,path):
    raw,ss=score._elf(path)
    h=struct.unpack_from('>16sHHIIIIIHHHHHH',raw)
    for i,s in enumerate(ss):
        sh=struct.unpack_from('>10I',raw,h[6]+i*h[11])
        s.update(index=i,address=sh[3],flags=sh[2],alignment=sh[8])
    return raw,ss

def procedures(score,path):
    raw,ss=sections(score,path)
    debug=next(s for s in ss if s['name']=='.mdebug')
    header=struct.unpack_from('>HH23I',raw,debug['off'])
    assert header[0]==0x7009
    names='ilineMax cbLine cbLineOffset idnMax cbDnOffset ipdMax cbPdOffset isymMax cbSymOffset ioptMax cbOptOffset iauxMax cbAuxOffset issMax cbSsOffset issExtMax cbSsExtOffset ifdMax cbFdOffset crfd cbRfdOffset iextMax cbExtOffset'.split()
    h=dict(zip(names,header[2:]));out={}
    for f in range(h['ifdMax']):
        fd=struct.unpack_from('>10I2H7I',raw,h['cbFdOffset']+72*f)
        string_base=h['cbSsOffset']+fd[2];syms=[]
        for i in range(fd[5]):
            iss,value,info=struct.unpack_from('>III',raw,h['cbSymOffset']+12*(fd[4]+i))
            name=raw[string_base+iss:].split(b'\0',1)[0].decode()
            syms.append(dict(name=name,value=value,st=info>>26,sc=(info>>21)&31,index=info&0xfffff))
        pd={}
        for i in range(fd[11]):
            p=struct.unpack_from('>9I2H3I',raw,h['cbPdOffset']+52*(fd[10]+i))
            pd[p[1]]=p
        for i,s in enumerate(syms):
            if s['st'] not in (6,14) or s['sc']!=1:continue
            end=[x for x in syms if x['st']==8 and x['sc']==1 and x['index']==i and x['name']==s['name']]
            assert len(end)==1 and end[0]['value']%4==0 and end[0]['value']>0
            p=pd[i];assert p[0]==s['value']
            out[s['name']]=dict(offset=s['value'],size=end[0]['value'],
                kind='stStaticProc' if s['st']==14 else 'stProc',frame_bytes=p[8],
                integer_save_mask=hex(p[3]),float_save_mask=hex(p[6]),
                frame_register=p[9],return_register=p[10],source_line_start=p[11],source_line_end=p[12],
                evidence='ECOFF procedure + matching stEnd length + PDR address/frame cross-check')
    text=next(s for s in ss if s['name']=='.text');owned=set()
    for n,s in out.items():
        assert s['offset']+s['size']<=text['size']
        span=set(range(s['offset'],s['offset']+s['size']))
        assert not owned&span,('overlap',n)
        owned|=span
    unowned=set(range(text['size']))-owned
    assert not any(raw[text['off']+i] for i in unowned),'nonzero text outside compiler procedure extents'
    return out,dict(text_size=text['size'],procedure_bytes=len(owned),zero_alignment_bytes=len(unowned),
                    zero_alignment_offsets=[hex(x) for x in sorted(unowned)])

def validate_link(score,obj,elf):
    a,ss=sections(score,obj);b,ls=sections(score,elf);by_name={s['name']:s for s in ls}
    syms={s['name']:s for i,sec in enumerate(ls) if sec['type']==2 for s in score._symbol_table(b,ls,i)}
    checked=[];changed={}
    for rel in ss:
        if rel['type']!=9:continue
        target=ss[rel['info']];linked=by_name[target['name']]
        table=score._symbol_table(a,ss,rel['link']);pending=[]
        assert target['size']==linked['size']
        def base(s):
            if s['section']==0:
                assert s['name'] in syms and syms[s['name']]['section']!=0
                return syms[s['name']]['value']
            if s['section']==0xfff1:return s['value']
            return by_name[ss[s['section']]['name']]['address']+s['value']
        def check(off,want,typ,name):
            got=struct.unpack_from('>I',b,linked['off']+off)[0]
            assert got==(want&0xffffffff),('GNU relocation mismatch',target['name'],hex(off),typ,name)
            changed.setdefault(target['name'],set()).update(range(off,off+4))
            checked.append(dict(section=target['name'],offset=hex(off),type=typ,symbol=name))
        for off,info in struct.iter_unpack('>II',a[rel['off']:rel['off']+rel['size']]):
            ix,typ=info>>8,info&255;s=table[ix];word=struct.unpack_from('>I',a,target['off']+off)[0]
            if typ==2:check(off,word+base(s),typ,s['name'])
            elif typ==4:
                value=base(s)+((word&0x3ffffff)<<2)
                assert value%4==0
                check(off,(word&0xfc000000)|((value>>2)&0x3ffffff),typ,s['name'])
            elif typ==5:pending.append((off,ix,word))
            elif typ==6:
                matching=[p for p in pending if p[1]==ix];lo=signed16(word&0xffff)
                for hi_off,_,hi_word in matching:
                    value=base(s)+((hi_word&0xffff)<<16)+lo
                    check(hi_off,(hi_word&0xffff0000)|(((value+0x8000)>>16)&0xffff),5,s['name'])
                value=base(s)+(((matching[0][2]&0xffff)<<16) if matching else 0)+lo
                check(off,(word&0xffff0000)|(value&0xffff),typ,s['name'])
                pending=[p for p in pending if p[1]!=ix]
            else:raise AssertionError(('unsupported relocation',typ))
        assert not pending,'unpaired HI16'
    unchanged=0
    for name in [s['name'] for s in ss if s['name'] in ('.text','.rodata','.rdata','.data','.sdata','.lit4','.lit8') and s['size']]:
        s=next(s for s in ss if s['name']==name);t=by_name[name]
        assert s['size']==t['size']
        for off in set(range(s['size']))-changed.get(name,set()):
            assert a[s['off']+off]==b[t['off']+off],('changed unrelocated byte',name,off)
            unchanged+=1
    return dict(status='PASS: every object relocation reproduces GNU-linked words; other text/rodata bytes unchanged',
                relocation_count=len(checked),unrelocated_bytes_checked=unchanged,relocations=checked)
