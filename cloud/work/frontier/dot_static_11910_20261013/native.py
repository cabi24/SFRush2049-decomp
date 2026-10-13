"""Small fail-closed big-endian o32 replay; obtains instructions from its caller.

This models the synchronous callback contract, not an audio/RSP implementation.
"""
U32 = 0xffffffff
ADDRESS = 0x80011910
SP = 0x70001000
RETURN = 0x60000000
TASK = 0x800382f8
FLAG = 0x800382cd
HOOK = 0x80038000
CALLBACK = 0x60100000
CACHE = 0x80007ca0


def need(ok, reason):
    if not ok:
        raise ValueError(reason)


def signed(n, bits=32):
    n &= (1 << bits) - 1
    return n - (1 << bits) if n >> (bits - 1) else n


class Memory:
    def __init__(self, seed):
        self.regions = {TASK - 16: bytearray(((seed + i*13) & 255) for i in range(96)),
                        FLAG: bytearray([seed & 255]), HOOK: bytearray(CALLBACK.to_bytes(4,'big')),
                        SP - 128: bytearray([0xa5]*256)}

    def access(self, address, size):
        need(address % size == 0, 'unaligned access')
        for base, data in self.regions.items():
            if base <= address and address+size <= base+len(data):
                return data, address-base
        raise ValueError(('unmapped access', hex(address), size))

    def get(self, address, size):
        data, at = self.access(address, size)
        return int.from_bytes(data[at:at+size], 'big')

    def put(self, address, value, size):
        data, at = self.access(address, size)
        data[at:at+size] = (value & ((1 << (8*size))-1)).to_bytes(size,'big')

    def snapshot(self):
        return tuple((a,bytes(d)) for a,d in sorted(self.regions.items()) if a != SP-128)


def event(memory, address, args, events, mutation):
    if address == CACHE:
        events.append(('cache',tuple(args[:2]),memory.snapshot()))
        if mutation:
            memory.put(FLAG, 0xd3, 1)
        return
    need(address == CALLBACK, ('unknown callback',hex(address)))
    events.append(('submit',args[0],memory.snapshot()))
    if mutation:
        # The submit hook can mutate task and completion flag before return.
        memory.put(TASK+4, 0x76543210, 4)
        memory.put(FLAG, 0xb7, 1)


def oracle(count, pointer, seed, mutation):
    memory, events = Memory(seed), []
    count &= 65535
    if count:
        for offset,value in ((0,2),(4,0),(8,0x80027dd0),(12,0xd0),
                             (16,0x8002a050),(20,4096),(24,0x8002e210),(28,2048),
                             (48,pointer),(52,count)):
            memory.put(TASK+offset,value,4)
        event(memory,CACHE,[pointer,count*2576],events,mutation)
        event(memory,CALLBACK,[TASK],events,mutation)
        memory.put(FLAG,1,1)
    return events,memory.snapshot()


def execute(words,count,pointer,seed,upper,mutation):
    memory, events, covered = Memory(seed), [], set()
    regs = [((i+1)*0x1234567+seed)&U32 for i in range(32)]
    regs[0],regs[4],regs[5],regs[29],regs[31] = 0,pointer,(upper<<16)|(count&65535),SP,RETURN
    saved = regs[:]
    pc,pending = 0,None
    for step in range(400):
        if pc == RETURN:
            need(pending is None,'pending return')
            need(all(regs[r]==saved[r] for r in list(range(16,24))+[28,29,30,31]),'o32 preserved-register corruption')
            return (events,memory.snapshot()),covered
        need(pc%4==0 and 0 <= pc < len(words)*4,('PC out of full function',pc))
        covered.add(pc)
        w=words[pc//4]
        op,rs,rt,rd,sh,fn = w>>26,w>>21&31,w>>16&31,w>>11&31,w>>6&31,w&63
        imm=signed(w,16); address=(regs[rs]+imm)&U32
        current,pending,nextpc = pending,None,pc+4
        if op==0:
            if fn==0: regs[rd]=regs[rt]<<sh&U32
            elif fn==0x21: regs[rd]=(regs[rs]+regs[rt])&U32
            elif fn==0x23: regs[rd]=(regs[rs]-regs[rt])&U32
            elif fn==0x25: regs[rd]=regs[rs]|regs[rt]
            elif fn==8:
                need(current is None and rs==31,'unsupported jump')
                pending=('jump',regs[rs])
            elif fn==9:
                need(current is None and rd==31,'unsupported indirect call')
                target=regs[rs];regs[31]=ADDRESS+pc+8;pending=('call',target)
            else: raise ValueError(('unknown SPECIAL',fn))
        elif op==9: regs[rt]=(regs[rs]+imm)&U32
        elif op==12: regs[rt]=regs[rs]&(w&65535)
        elif op==13: regs[rt]=regs[rs]|(w&65535)
        elif op==15: regs[rt]=(w&65535)<<16
        elif op==0x23: regs[rt]=memory.get(address,4)
        elif op in (0x28,0x2b):
            n=1 if op==0x28 else 4
            if SP-128<=address<SP+128:
                need(n==4 and address in (SP-4,SP+4),'stack write outside contract')
            memory.put(address,regs[rt],n)
        elif op in (4,5):
            need(current is None,'branch in delay slot')
            taken=(regs[rs]==regs[rt]) if op==4 else (regs[rs]!=regs[rt])
            pending=('jump',pc+4+imm*4 if taken else pc+8)
        elif op==3:
            need(current is None,'call in delay slot')
            regs[31]=ADDRESS+pc+8;pending=('call',0x80000000|((w&0x3ffffff)<<2))
        else: raise ValueError(('unknown opcode',op))
        regs[0]=0
        if current:
            if current[0]=='jump': nextpc=current[1]
            else:
                event(memory,current[1],regs[4:8],events,mutation)
                # Every o32 caller-saved register is poisoned; no retained a0/a1.
                for r in list(range(1,16))+[24,25]: regs[r]=(0xc0000000+step*31+r)&U32
        pc=nextpc
    raise ValueError('instruction limit')


def prove(bodies):
    result={}
    for label,words in bodies.items():
        covered=set();cases=0
        for count in (0,1,2,255,256,32767,32768,65535):
            for pointer in (0,0x10000000,0x80041000,0xfffffff0):
                for upper in (0,0xffff,0xa53c):
                    for mutation in (False,True):
                        seed=(count^pointer^upper)&255
                        actual,visits=execute(words,count,pointer,seed,upper,mutation)
                        need(actual==oracle(count,pointer,seed,mutation),('behavior/ABI mismatch',label,count,pointer,upper,mutation))
                        covered.update(visits);cases+=1
        need(covered==set(range(0,len(words)*4,4)),('uncovered instruction',label))
        result[label]={'cases':cases,'instructions_covered':len(covered),'full_instruction_coverage':True,'frame_restored':True}
    return result
