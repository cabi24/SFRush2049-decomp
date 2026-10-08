"""Run the unchanged typed C under host pointers and normalize target storage.

Only pointer width and endian conversion live here. Every service boundary uses
exactly the fixture's model, and synchronizes external mutations in both ways.
"""
import ctypes as C
import struct

NODE_BASE, MATRIX_BASE, INPUT = 0x80600000, 0x80601000, 0x80603000
U8,U16,U32,F32,PTR = C.c_ubyte,C.c_ushort,C.c_uint,C.c_float,C.c_void_p
class Node(C.Structure):
    _fields_=[('next',PTR),('state',U16),('scene',U16),('index',U16),('unused',U16),
              ('data',U32),('timer',F32),('callback',PTR)]

# A field is target offset, host offset, width, kind (raw/int/pointer).
def raw(off,size): return (off,off,size,'raw')
def number(off,width=4): return (off,off,width,'int')
def floats(start,count): return [number(start+4*i) for i in range(count)]
TRANSFORM=floats(0,12)
PLAYER=[raw(0,8)]+floats(8,3)+[raw(20,952-20)]
EXTRA=[number(0)]+floats(4,12)+[raw(52,4),number(56)]
DEBRIS=[number(0)]+floats(4,18)+[raw(76,4),number(80)]
GROUP=[(n+84*i,h+84*i,w,k) for i in range(4) for n,h,w,k in DEBRIS]+[
       (n+336,h+336,w,k) for n,h,w,k in EXTRA]+[number(396)]
SCENE=[raw(0,12),number(12),number(16),raw(20,40),number(60),raw(64,4)]
NODE=[(n,getattr(Node,name).offset,w,k) for n,name,w,k in [
    (0,'next',4,'ptr'),(4,'state',2,'int'),(6,'scene',2,'int'),(8,'index',2,'int'),
    (10,'unused',2,'int'),(12,'data',4,'int'),(16,'timer',4,'int'),(20,'callback',4,'ptr')]]

class Host:
    def __init__(self,path):
        self.lib=C.CDLL(str(path))
        self.lib.host_layout.restype=C.c_ulong
        expected=[C.sizeof(Node),48,952,60,84,400,68,Node.callback.offset,56,77,336,60]
        assert [self.lib.host_layout(i) for i in range(len(expected))]==expected
        self.regions=[]
        def add(name,address,size,fields,count=1,host_size=None):
            host_size=host_size or size
            ptr=C.addressof(U8.in_dll(self.lib,name))
            fs=[(n+size*i,h+host_size*i,w,k) for i in range(count) for n,h,w,k in fields]
            self.regions.append((address,size*count,ptr,host_size*count,fs,name))
        for address,width in [(0x80156994,1),(0x8014978C,1),(0x8010FFC0,1),
            (0x8014A108,2),(0x8013C094,2),(0x8012E66C,2),(0x8012E678,2),
            (0x8011735C,4),(0x8011B554,4),(0x8011B550,4),(0x801543CC,4),
            *[(a,4) for a in (0x801239A8,0x801239AC,0x801239B0,0x801239B4,
              0x801239B8,0x801239BC,0x801239C0,0x801239C4,0x80123C00,0x80123C04)],
            (0x80142904,2),(0x80142906,2),(0x80142908,2)]:
            add('D_'+format(address,'08X'),address,width,[number(0,width)])
        add('D_8011418C',0x8011418C,36,floats(0,9))
        add('D_801141B0',0x801141B0,12,floats(0,3))
        add('D_801428FC',0x801428FC,8,[number(i,2) for i in range(0,8,2)])
        for name,address,size,fields,count in [
            ('D_80152818',0x80152818,952,PLAYER,6),
            ('D_80154660',0x80154660,400,GROUP,6),
            ('D_80154FD8',0x80154FD8,60,EXTRA,6),
            ('D_8012E700',0x8012E700,68,SCENE,128),
            ('host_transforms',MATRIX_BASE,48,TRANSFORM,50)]:
            add(name,address,size,fields,count)
        add('host_nodes',NODE_BASE,24,NODE,8,C.sizeof(Node))
        add('D_8013C238',0x8013C238,4,[(0,0,4,'ptr')],50,C.sizeof(PTR))
        for address in (0x801391F0,0x801392C8):
            add('D_'+format(address,'08X'),address,4,[(0,0,4,'ptr')],host_size=C.sizeof(PTR))
        add('host_input',INPUT,12,floats(0,3))
        self.callbacks={C.cast(getattr(self.lib,name),PTR).value:address for name,address in [
            ('entity_collision_detect',0x80090B68),('entity_physics_update',0x800908A0),
            ('entity_update_callback',0x80090FEC)]}
        self.reverse_callbacks={v:k for k,v in self.callbacks.items()}
        self.calltype=C.CFUNCTYPE(C.c_ulonglong,U32,*([C.c_ulonglong]*10))
        self.lib.set_host_call.argtypes=[self.calltype]
        self.lib.save_write_data.argtypes=[PTR,C.c_int,F32,C.c_int]

    def to_host_pointer(self,address):
        if not address:return 0
        if address in self.reverse_callbacks:return self.reverse_callbacks[address]
        for start,size,ptr,host_size,fields,name in self.regions:
            if start<=address<start+size:
                if size==host_size:return ptr+address-start
                if name=='host_nodes':
                    i,offset=divmod(address-start,24)
                    for n,h,w,k in NODE:
                        if n<=offset<n+w:return ptr+i*C.sizeof(Node)+h+offset-n
                if name=='D_8013C238':
                    i,offset=divmod(address-start,4)
                    return ptr+i*C.sizeof(PTR)+offset
                assert address==start
                return ptr
        return address  # Preserve unread/uninitialized pointer fields as opaque bits.

    def to_native_pointer(self,address):
        if not address:return 0
        if address in self.callbacks:return self.callbacks[address]
        for start,size,ptr,host_size,fields,name in self.regions:
            if ptr<=address<ptr+host_size:
                if size==host_size:return start+address-ptr
                if name=='host_nodes':
                    i,offset=divmod(address-ptr,C.sizeof(Node))
                    for n,h,w,k in NODE:
                        if h<=offset<h+(C.sizeof(PTR) if k=='ptr' else w):return start+24*i+n+offset-h
                if name=='D_8013C238':
                    i,offset=divmod(address-ptr,C.sizeof(PTR))
                    return start+4*i+offset
                assert address==ptr
                return start
        if address <= 0xFFFFFFFF:return address
        raise AssertionError(('unmapped host pointer',hex(address)))

    def import_state(self,memory):
        for start,size,ptr,host_size,fields,name in self.regions:
            for n,h,w,kind in fields:
                data,off=memory.region(start+n,1)
                value=int.from_bytes(data[off:off+w],'big')
                if kind=='ptr':
                    raw=self.to_host_pointer(value).to_bytes(C.sizeof(PTR),'little')
                else:raw=value.to_bytes(w,'big' if kind=='raw' else 'little')
                C.memmove(ptr+h,raw,len(raw))

    def export_state(self,memory):
        for start,size,ptr,host_size,fields,name in self.regions:
            for n,h,w,kind in fields:
                raw=C.string_at(ptr+h,C.sizeof(PTR) if kind=='ptr' else w)
                value=int.from_bytes(raw,'big' if kind=='raw' else 'little')
                if kind=='ptr':value=self.to_native_pointer(value)
                data,off=memory.region(start+n,1)
                data[off:off+w]=value.to_bytes(w,'big')

    def run(self,fixture):
        # A tagged input is either a short or three floats, never both at once.
        short_input=fixture.mode!=0 or (fixture.memory.get(0x80156994,1)==0 and
                                      fixture.memory.get(0x8014978C,1)<6)
        fields=[number(0,2),raw(2,10)] if short_input else floats(0,3)
        r=self.regions[-1];assert r[-1]=='host_input'
        self.regions[-1]=(*r[:4],fields,r[-1])
        errors=[]
        def call(address,*args):
            try:
                self.export_state(fixture.memory)
                argv=list(args)
                if address==0x8008D6B0:
                    argv=argv[:2]
                    argv[:]=[self.to_native_pointer(x) for x in argv]
                elif address==0x8008E26C:
                    argv=argv[:4];argv[1]=self.to_native_pointer(argv[1])
                elif address==0x80090088:argv=argv[:3]
                elif address==0x8008EA10:argv=argv[:4]
                elif address==0x800AED64:
                    argv[0]=self.to_native_pointer(argv[0]);argv[1]=self.to_native_pointer(argv[1])
                elif address==0x80090284:argv=[]
                else:raise AssertionError(('unexpected host boundary',hex(address)))
                result=fixture.service(address,*argv)
                self.import_state(fixture.memory)
                return self.to_host_pointer(result) if address==0x80090284 else (result or 0)
            except BaseException as error:
                errors.append(error)
                return 0
        callback=self.calltype(call)
        self.lib.set_host_call(callback)
        self.import_state(fixture.memory)
        self.lib.save_write_data(self.to_host_pointer(INPUT),fixture.mode,
                                 fixture.scale,fixture.sound)
        if errors:raise errors[0]
        self.export_state(fixture.memory)
