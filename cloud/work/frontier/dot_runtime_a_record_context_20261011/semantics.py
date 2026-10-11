"""Bounded record-navigation model, fail-closed MIPS execution and C89 checks.

Instruction streams are supplied by verified target/ELF readers, never embedded.
Only the real external menu-record predicate is stubbed. Its O32 volatile
register clobbers and optional handle rewrite test the native reload boundary.
"""
import random
import re
import struct
import subprocess

ENTRY=0x8039E200
EXIT=0x81234560
SP=0x81010000
HEAD=0x80152028
TAIL=0x8015202C
CACHE=0x803BA010
SELECTED=0x80152698
TRACK=0x8014978C
MODE=0x80152570
HANDLES=0x81100000
OBJECTS=0x81101000
COUNT=9
LINKED=6
CALL=0x800CC804

def require(condition,message):
    if not condition:raise ValueError(message)

def signed(value,bits=32):
    value&=(1<<bits)-1
    return value-(1<<bits) if value&(1<<(bits-1)) else value

def read(memory,address,size):
    require(address%size==0 and all(address+j in memory for j in range(size)),('unmapped read',hex(address),size))
    return int.from_bytes(bytes(memory[address+j] for j in range(size)),'big')

def write(memory,address,size,value):
    require(address%size==0 and all(address+j in memory for j in range(size)),('unmapped write',hex(address),size))
    for j,b in enumerate((value&((1<<(8*size))-1)).to_bytes(size,'big')):memory[address+j]=b

def handle(index):return 0 if index<0 else HANDLES+index*4

def cases():
    rng=random.Random(0xE200)
    # Every start/direction/list-length, accepted position and rejection kind.
    for length in range(LINKED+1):
        starts=[-1]+list(range(length))+list(range(LINKED,COUNT))
        for direction in (-128,-1,0,1,127):
            for start in starts:
                for accepted in range(-1,COUNT):
                    records=[]
                    for i in range(COUNT):records.append((3,2,1,int(i==accepted),1,0))
                    yield (length,start,direction,3,2,(-1,-1,-1),records)
    # All signed directions plus randomized short-circuit gates and callback results.
    for k in range(4096):
        length=rng.randrange(LINKED+1);start=rng.choice([-1]+list(range(length))+list(range(LINKED,COUNT)))
        track=rng.choice((-128,-1,0,3,6,127));mode=rng.choice((-128,-1,0,2,127))
        records=[]
        for i in range(COUNT):
            records.append((track if rng.randrange(3) else rng.choice((-128,-1,0,3,6,127)),
                (mode&255) if rng.randrange(3) else rng.randrange(256),rng.randrange(2),
                rng.choice((0,1,0xffffffff)),rng.randrange(2),rng.randrange(4)))
        yield (length,start,signed(k,8),track,mode,tuple(rng.choice([-1]+list(range(COUNT))) for _ in range(3)),records)

def state(case):
    length,start,direction,track,mode,selected,records=case
    spans=((SP-32,SP+16),(HEAD-8,TAIL+12),(CACHE-8,CACHE+12),(SELECTED-8,SELECTED+24),
        (TRACK,TRACK+1),(MODE,MODE+1),(HANDLES-8,HANDLES+COUNT*4+8),(OBJECTS-8,OBJECTS+COUNT*128+8))
    memory={a:0xa5 for lo,hi in spans for a in range(lo,hi)}
    for address,value in ((HEAD,handle(0) if length else 0),(TAIL,handle(length-1)),(CACHE,handle(LINKED))):write(memory,address,4,value)
    write(memory,TRACK,1,track);write(memory,MODE,1,mode)
    for i,value in enumerate(selected):write(memory,SELECTED+4*(i+1),4,handle(value))
    for i,record in enumerate(records):
        base=OBJECTS+i*128
        write(memory,handle(i),4,base)
        for delta in (0,64):
            write(memory,base+delta,4,handle(i+1) if i+1<length else 0)
            write(memory,base+delta+4,4,record[2])
            write(memory,base+delta+8,1,record[0]);write(memory,base+delta+9,1,record[1])
            write(memory,base+delta+36,4,record[3] if delta==0 else int(record[5]==2))
    return memory

def oracle(case):
    length,start,direction,track,mode,selected,records=case
    ordered=list(range(length))+list(range(LINKED,COUNT))
    current=(0 if length else -1) if start<0 and direction>0 else (COUNT-1 if start<0 else start)
    if current<0:return -1,[],[]
    at=ordered.index(current)
    path=ordered[at:] if direction>0 else ordered[at::-1]
    if direction==0 and current==COUNT-1:path=[current]
    calls=[];rewrites=[]
    for i in path:
        rtrack,rmode,file,active,callback,mutation=records[i]
        if rtrack!=track or rmode!=mode or i in selected:continue
        if file:
            calls.append(i)
            if mutation:rewrites.append(i)
            if not callback:continue
            if mutation:active=int(mutation==2)
        if active:return i,calls,rewrites
    return -1,calls,rewrites

def run(words,case,salt):
    rng=random.Random(salt^0x89abcdef);registers=[rng.getrandbits(32) for _ in range(32)]
    registers[0]=0;registers[17]=handle(case[1]);registers[18]=(rng.getrandbits(24)<<8)|(case[2]&255)
    registers[29]=SP;registers[31]=EXIT;initial=list(registers);memory=state(case)
    pc=ENTRY;pending=None;coverage=set();branches=set();reads=[];writes=[];calls=[]
    def store(address,size,value):
        write(memory,address,size,value);writes.append((address,size,value&((1<<(size*8))-1)))
    for step in range(20000):
        if pc==EXIT:
            require(pending is None,'unfinished delay slot')
            require(all(registers[i]==initial[i] for i in (23,28,29,30,31)),'private ABI preserved-register corruption')
            return {'return':registers[2],'registers':registers,'initial_registers':initial,'memory':memory,
                'coverage':coverage,'branches':branches,'reads':reads,'writes':writes,'calls':calls}
        if pc==CALL:
            require(pending is None,'call hook entered in delay slot')
            index=(registers[4]-HANDLES)//4
            require(registers[4]==handle(index) and 0<=index<COUNT,'bad external-call argument')
            calls.append(index);record=case[6][index];return_pc=registers[31]
            if record[5]:store(handle(index),4,OBJECTS+index*128+64)
            for reg in (2,3,4,5,6,7,8,9,10,11,12,13,14,15,24,25):registers[reg]=rng.getrandbits(32)
            registers[2]=record[4];pc=return_pc;continue
        require(ENTRY<=pc<ENTRY+4*len(words) and pc%4==0,('escaped code',hex(pc)))
        coverage.add(pc);word=words[(pc-ENTRY)//4];op=word>>26;rs=word>>21&31;rt=word>>16&31;rd=word>>11&31;fn=word&63;imm=signed(word,16)
        delayed=pending;pending=None;transfer=None;annul=False;r=registers
        if word==0:pass
        elif op==0:
            if fn==0:r[rd]=(r[rt]<<((word>>6)&31))&0xffffffff
            elif fn==3:r[rd]=(signed(r[rt])>>((word>>6)&31))&0xffffffff
            elif fn==33:r[rd]=(r[rs]+r[rt])&0xffffffff
            elif fn==37:r[rd]=r[rs]|r[rt]
            elif fn==8:require(rs==31 and word&0x1fffff==8,'unsupported jr');transfer=r[rs]
            else:raise ValueError(('unknown special',hex(pc),fn))
        elif op==15:require(rs==0,'bad lui');r[rt]=(word&65535)<<16
        elif op==9:r[rt]=(r[rs]+imm)&0xffffffff
        elif op in (32,35,36):
            size=4 if op==35 else 1;address=(r[rs]+imm)&0xffffffff;value=read(memory,address,size);reads.append((address,size))
            r[rt]=(signed(value,8) if op==32 else value)&0xffffffff
        elif op==43:store((r[rs]+imm)&0xffffffff,4,r[rt])
        elif op in (1,4,5,6,20,21):
            if op==1:
                require(rt in (0,1),'unknown REGIMM');taken=(signed(r[rs])>=0) if rt else signed(r[rs])<0
            elif op==6:require(rt==0,'bad blez');taken=signed(r[rs])<=0
            else:taken=(r[rs]==r[rt]) if op in (4,20) else r[rs]!=r[rt]
            branches.add((pc,taken));annul=op in (20,21) and not taken;transfer=pc+4+imm*4 if taken else pc+8
        elif op==3:
            transfer=((pc+4)&0xf0000000)|((word&0x3ffffff)<<2);require(transfer==CALL,'unexpected external call');r[31]=pc+8
        else:raise ValueError(('unknown opcode',hex(pc),op))
        r[0]=0
        if delayed is not None:require(transfer is None,'branch in delay slot');pc=delayed
        elif annul:pc+=8
        else:pc,pending=pc+4,transfer
    raise ValueError('execution limit')

def extract(source,name):
    start=source.index('Record **'+name+'(');start=source.index('Record **'+name+'(',start+1) if source[start:source.index('\n',start)].endswith(';') else start
    brace=source.index('{',start);depth=1;end=brace+1
    while depth:depth+=(source[end]=='{')-(source[end]=='}');end+=1
    return source[start:end]

def host(source,rows,temp):
    declarations='''#include <stdio.h>
#include <stddef.h>
typedef signed char s8;typedef unsigned char u8;typedef signed int s32;typedef unsigned int u32;
typedef struct Record Record;
struct Record {Record **next;void *file;s8 track;u8 mode;u32 unknown24;};
typedef struct RecordList {u8 indirect,doubly,unknown02[2];s32 count;Record **first,**last;} RecordList;
RecordList D_80152020;Record **D_803BA010,**D_80152698[4];s8 D_8014978C,D_80152570;
Record objects[9],replacement[9];Record *handles[9];int call_count,calls[32],callback[9],mutation[9];
s32 menu_item_value_get(Record **node) {int i=(int)(node-handles);calls[call_count++]=i;if(mutation[i])*node=&replacement[i];return callback[i];}
'''
    driver='''
int main(void) {
 int length,start,direction,track,mode,selected[3],i,j,rt,rm,file,active,result,mut; unsigned int value;
 Record **answer;
 while(scanf("%d%d%d%d%d",&length,&start,&direction,&track,&mode)==5) {
  for(i=0;i<3;i++)if(scanf("%d",&selected[i])!=1)return 2;
  for(i=0;i<9;i++) {
   if(scanf("%d%d%d%u%d%d",&rt,&rm,&file,&value,&result,&mut)!=6)return 3;
   objects[i].next=i+1<length?&handles[i+1]:0;objects[i].file=file?&objects[i]:0;
   objects[i].track=(s8)rt;objects[i].mode=(u8)rm;objects[i].unknown24=value;
   replacement[i]=objects[i];replacement[i].unknown24=mut==2;handles[i]=&objects[i];callback[i]=result;mutation[i]=mut;
  }
  D_80152020.first=length?handles:0;D_80152020.last=length?&handles[length-1]:0;D_803BA010=&handles[6];
  for(i=0;i<3;i++)D_80152698[i+1]=selected[i]<0?0:&handles[selected[i]];
  D_8014978C=(s8)track;D_80152570=(s8)mode;call_count=0;
  answer=func_8039E200(start<0?0:&handles[start],(s8)direction);
  printf("%d %d",answer?(int)(answer-handles):-1,call_count);
  for(i=0;i<call_count;i++)printf(" %d",calls[i]);
  for(i=0;i<9;i++)printf(" %d",handles[i]==&replacement[i]);
  printf("\\n");
 }
 return 0;
}
'''
    path=temp/'host.c';path.write_text(declarations+extract(source,'func_8039E200')+'\n'+driver);binary=temp/'host'
    result=subprocess.run(['gcc','-std=c89','-pedantic-errors','-O2','-fsanitize=undefined,bounds','-fno-sanitize-recover=all',str(path),'-o',str(binary)],capture_output=True,text=True)
    require(result.returncode==0,result.stderr)
    text=''.join(' '.join(map(str,(*r[:5],*r[5],*(v for record in r[6] for v in record))))+'\n' for r in rows)
    result=subprocess.run([str(binary)],input=text,capture_output=True,text=True)
    require(result.returncode==0 and 'runtime error:' not in result.stderr,result.stderr)
    return [list(map(int,line.split())) for line in result.stdout.splitlines()]

def audit_calls(obj,score,elf,targets):
    sections,symbols,relocs=elf(obj);words=score.text_words(obj);entry=symbols['func_8039E200'][0];result=[]
    for offset,kind,symbol,section in relocs:
        if kind!=4 or symbol!='.text' or (words[offset//4]&0x3ffffff)*4!=entry:continue
        owner=next(name for name,v in symbols.items() if v[2]==2 and v[3]==sections['.text'][0] and v[0]<=offset<v[0]+v[1])
        require(owner=='func_803A1EAC','unexpected E200 source caller')
        move=words[offset//4-2]
        require(move>>26==0 and move&63==37 and (move>>11&31)==18 and (move>>16&31)==0,'direction input is not s2')
        # Each path loads its own selected-record handle directly into private s1.
        before=words[offset//4-3];require(before>>26==35 and (before>>16&31)==17,'record input is not s1')
        after=words[offset//4+3];require(after>>26==43 and (after>>16&31)==2,'return not stored from v0')
        for delta,reg in ((-1,21),(1,22)):
            instruction=words[offset//4+delta]
            require(instruction>>26==43 and (instruction>>21&31)==29 and (instruction>>16&31)==reg,'caller clobber save differs')
            restore=(instruction&0x03ffffff)|(35<<26)
            require(words[offset//4+4:offset//4+7].count(restore)==1,'caller clobber restore differs')
        result.append({'caller':owner,'offset':hex(offset-symbols[owner][0]),'input_record':'s1','input_direction':'s2','return':'v0'})
    require(len(result)==3,'expected three genuine record-navigation callsites')
    # Audit protected native caller sites independently by image-qualified destination.
    native=[]
    for name,body in targets.items():
        for i,word in enumerate(body):
            if word>>26==3 and ((word&0x3ffffff)<<2)==(ENTRY&0x0fffffff):
                native.append((name,i*4))
                require(body[i-3]>>26==35 and (body[i-3]>>16&31)==17,'native s1 input differs')
                move=body[i-2];require(move>>26==0 and move&63==37 and (move>>11&31)==18 and (move>>16&31)==0,'native s2 input differs')
                require(body[i+3]>>26==43 and (body[i+3]>>16&31)==2,'native v0 return differs')
                for delta,reg in ((-1,21),(1,22)):
                    instruction=body[i+delta];require(instruction>>26==43 and (instruction>>21&31)==29 and (instruction>>16&31)==reg,'native caller save differs')
                    restore=(instruction&0x03ffffff)|(35<<26)
                    require(body[i+4:i+7].count(restore)==1,'native caller restore differs')
    require(len(native)==3 and all(n=='func_803A1EAC' for n,_ in native),'native caller population differs')
    return {'generated':result,'native':[{'caller':n,'offset':hex(off)} for n,off in native]}

def verify(source,native,linked,temp):
    rows=list(cases());hostrows=host(source,rows,temp);require(len(rows)==len(hostrows),'host output count')
    coverage=set();branches=set();mutating=0
    for i,(case,hrow) in enumerate(zip(rows,hostrows)):
        expected,calls,rewrites=oracle(case);a=run(native,case,i);b=run(linked,case,i)
        require(a==b,('native/GNU trace mismatch',i))
        require(a['return']==handle(expected) and a['calls']==calls,('oracle mismatch',i,expected,calls,a['calls'],hex(a['return'])))
        require(hrow==[expected,len(calls)]+calls+[int(j in rewrites) for j in range(COUNT)],('host mismatch',i))
        initial=state(case);allowed=set(range(SP-24,SP))|set(range(SP+4,SP+8))|{handle(j)+k for j in rewrites for k in range(4)}
        require(all(a['memory'][address]==value for address,value in initial.items() if address not in allowed),('guard mutation',i))
        require(all(all(address+k in allowed for k in range(size)) for address,size,value in a['writes']),('unexpected write',i))
        coverage.update(a['coverage']);branches.update(a['branches']);mutating+=bool(rewrites)
    require(len(coverage)==111,('instruction coverage',len(coverage)))
    negatives=[]
    for label,words in [('unknown-opcode',[0xfc000000]),('escaped-code',[0])]:
        try:run(words,rows[0],0)
        except ValueError:negatives.append(label)
    require(len(negatives)==2,'decoder must fail closed')
    return {'cases_per_native_and_GNU_route':len(rows),'host_C89_UBSan_bounds_cases':len(rows),
        'instructions_covered':len(coverage),'branch_outcomes':len(branches),'callback_mutation_cases':mutating,
        'complete_memory_register_read_write_trace_equality':True,'all_signed_byte_directions':True,
        'private_preserved_registers':['s7','s8','gp','sp','ra'],'native_outgoing_argument_home_write':'entry sp+4',
        'decoder_negative_checks':negatives,'external_service':'menu_item_value_get is stubbed with O32 volatile-register clobbers and optional handle mutation'}
