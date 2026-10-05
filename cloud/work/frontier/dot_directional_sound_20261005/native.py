"""Bounded MIPS-II interpreter for this caller and its real matrix callee.

Fail closed on unknown instructions or accesses. Binary32 arithmetic uses host
round-to-nearest. FCSR flags, signaling NaNs and payload propagation are outside
the tested contract. No target words or game data are retained here.
"""
import struct

MASK = 0xffffffff
STACK, INPUT, STOP = 0x700000, 0x710000, 0xfffffffc


def signed(value, bits=32):
    value &= (1 << bits)-1
    return value-(1 << bits) if value & (1 << (bits-1)) else value


def bits(value):
    return struct.unpack('>I', struct.pack('>f', value))[0]


def floating(value):
    return struct.unpack('>f', struct.pack('>I', value))[0]


class Machine:
    def __init__(self, addresses, caller, matrix):
        self.addresses = addresses
        self.start = addresses['stat_lap_split']
        self.code = {self.start+4*i: w for i,w in enumerate(caller)}
        self.code.update({addresses['func_800A61B0']+4*i: w for i,w in enumerate(matrix)})
        self.coverage = set()

    def run(self, case, tables):
        enabled, state, sound, slot, mode, position, car_position, matrix, tag = case
        a = self.addresses
        regions = {STACK:bytearray(1024), INPUT:bytearray(12),
                   a['D_8010FFC0']:bytearray(1), a['D_80153E8F']:bytearray(64),
                   a['player_array']:bytearray(8*952),
                   a['D_8011F020']:bytearray(32), a['D_8011F040']:bytearray(32),
                   0x80124824:bytearray(struct.pack('>f', floating(bits(.924))*floating(bits(.924))))}
        reads, writes, calls = [], [], []
        matrix_calls = 0

        def mem(address, width, value=None):
            assert address % width == 0, ('unaligned', address, width)
            for base,data in regions.items():
                off = address-base
                if 0 <= off and off+width <= len(data):
                    if value is None:
                        reads.append((address,width))
                        return int.from_bytes(data[off:off+width],'big')
                    writes.append((address,width))
                    data[off:off+width] = (value & ((1 << (width*8))-1)).to_bytes(width,'big')
                    return
            raise AssertionError(('unmapped',hex(address),width))

        mem(a['D_8010FFC0'],1,enabled)
        mem(a['D_80153E8F']+slot*8,1,state)
        car = a['player_array']+slot*952
        for i,v in enumerate(position): mem(INPUT+4*i,4,bits(v))
        for i,v in enumerate(car_position): mem(car+8+4*i,4,bits(v))
        for i,v in enumerate(matrix): mem(car+44+4*i,4,bits(v))
        for table,name in zip(tables,('D_8011F020','D_8011F040')):
            for i,v in enumerate(table): mem(a[name]+i*2,2,v)
        before = {k:bytes(v) for k,v in regions.items() if k != STACK}
        reads.clear(); writes.clear()
        r = [0xA5000000+i for i in range(32)]
        f = [bits(123.5+i) for i in range(32)]
        r[0],r[29],r[31] = 0,STACK+512,STOP
        r[4:8] = [sound&MASK,slot&MASK,INPUT,mode&MASK]
        saved,saved_f = r[:],f[:]
        pc,pending,condition,steps = self.start,None,False,0
        while pc != STOP:
            if pc == a['func_800A61B0']:
                matrix_calls += 1
            if pc == a['high_scores_display']:
                assert pending is None
                calls.append(tuple(r[4:8])+tuple(mem(r[29]+off,4) for off in (16,20,24)))
                for reg in list(range(1,16))+[24,25]: r[reg]=0xBAD00000+reg
                for reg in range(20): f[reg]=bits(-321.25-reg)
                r[2]=tag&MASK
                pc=r[31]
                continue
            assert pc in self.code and steps < 1000, ('invalid control',hex(pc))
            if self.start <= pc < self.start+608: self.coverage.add(pc-self.start)
            w=self.code[pc]; op=w>>26
            rs,rt,rd,sh=(w>>21)&31,(w>>16)&31,(w>>11)&31,(w>>6)&31
            imm=w&65535; si=signed(imm,16); address=(r[rs]+si)&MASK
            old,pending,next_pc=pending,None,pc+4
            if w == 0: pass
            elif op == 0:
                fn=w&63
                if fn == 0: r[rd]=r[rt]<<sh
                elif fn == 3: r[rd]=signed(r[rt])>>sh
                elif fn == 8: pending=r[rs]
                elif fn == 33: r[rd]=r[rs]+r[rt]
                elif fn == 35: r[rd]=r[rs]-r[rt]
                elif fn == 37: r[rd]=r[rs]|r[rt]
                else: raise AssertionError(('SPECIAL',hex(w)))
            elif op in (2,3):
                if op == 3: r[31]=pc+8
                pending=((pc+4)&0xf0000000)|((w&0x3ffffff)<<2)
            elif op in (4,5,20,21):
                take=(r[rs]==r[rt]) if op in (4,20) else (r[rs]!=r[rt])
                if take: pending=pc+4+4*si
                elif op in (20,21): next_pc+=4
            elif op == 9: r[rt]=address
            elif op == 12: r[rt]=r[rs]&imm
            elif op == 13: r[rt]=r[rs]|imm
            elif op == 15: r[rt]=imm<<16
            elif op in (32,33,35,36):
                width={32:1,33:2,35:4,36:1}[op]
                value=mem(address,width)
                r[rt]=value if op==36 else signed(value,width*8)
            elif op in (40,43): mem(address,1 if op==40 else 4,r[rt])
            elif op == 49: f[rt]=mem(address,4)
            elif op == 57: mem(address,4,f[rt])
            elif op == 17:
                fn=w&63
                if rs == 4: f[rd]=r[rt]
                elif rs == 8:
                    assert rt in (0,1,2,3)
                    if condition == bool(rt&1): pending=pc+4+4*si
                    elif rt&2: next_pc+=4
                elif rs == 20 and fn == 32: f[sh]=bits(signed(f[rd]))
                elif rs == 16:
                    x,y=floating(f[rd]),floating(f[rt])
                    if fn == 0: f[sh]=bits(x+y)
                    elif fn == 1: f[sh]=bits(x-y)
                    elif fn == 2: f[sh]=bits(x*y)
                    elif fn == 6: f[sh]=f[rd]
                    elif fn == 7: f[sh]=f[rd]^0x80000000
                    elif fn == 60: condition=x<y
                    elif fn == 62: condition=x<=y
                    else: raise AssertionError(('COP1 operation',hex(w)))
                else: raise AssertionError(('COP1 format',hex(w)))
            else: raise AssertionError(('opcode',hex(w),hex(pc)))
            r=[v&MASK for v in r]; r[0]=0
            pc=old if old is not None else next_pc
            steps+=1
        assert r[29]==saved[29] and r[16:24]==saved[16:24] and r[30]==saved[30]
        assert r[28]==saved[28] and f[20:]==saved_f[20:]
        assert before=={k:bytes(v) for k,v in regions.items() if k!=STACK}
        return r[2],calls,reads,writes,matrix_calls
